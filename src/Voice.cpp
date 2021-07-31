#include "Voice.h"
#include "Params.h"

//==============================================================================
GrapeSound::GrapeSound() {}
GrapeSound::~GrapeSound() {}
bool GrapeSound::appliesToNote(int) { return true; }
bool GrapeSound::appliesToChannel(int) { return true; }

//==============================================================================
GrapeVoice::GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo,
                       GlobalParams* globalParams,
                       VoiceParams* voiceParams,
                       OscParams* oscParams,
                       EnvelopeParams* envelopeParams,
                       FilterParams* filterParams,
                       LfoParams* lfoParams,
                       ModEnvParams* modEnvParams)
: perf(juce::PerformanceCounter("voice cycle", 100000))
, currentPositionInfo(currentPositionInfo)
, globalParams(globalParams)
, voiceParams(voiceParams)
, oscParams(oscParams)
, envelopeParams(envelopeParams)
, filterParams(filterParams)
, lfoParams(lfoParams)
, modEnvParams(modEnvParams)
, oscs { MultiOsc(), MultiOsc(), MultiOsc() }
, adsr { Adsr(), Adsr() }
, filters { Filter(), Filter() }
, lfos { Osc(), Osc(), Osc() }
, modEnvs { Adsr(), Adsr(), Adsr() }
{}
GrapeVoice::~GrapeVoice() {
    DBG("GrapeVoice's destructor called.");
}
bool GrapeVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    if(dynamic_cast<GrapeSound*>(sound) != nullptr) {
        return true;
    }
    return false;
}
void GrapeVoice::startNote (int midiNoteNumber, float velocity,
                juce::SynthesiserSound*, int currentPitchWheelPosition)
{
    DBG("startNote() midiNoteNumber:" << midiNoteNumber);
    if(GrapeSound* playingSound = dynamic_cast<GrapeSound*>(getCurrentlyPlayingSound().get()))
    {
        auto sampleRate = getSampleRate();
        smoothNote.init(midiNoteNumber);
        if(stolen) {
            smoothVelocity.exponential(0.01, velocity, sampleRate);
        } else {
            smoothVelocity.init(velocity);
        }
        stolen = false;
        
        for(int i = 0; i < NUM_OSC; ++i) {
            if(!stolen) {
//                oscs[i].setAngle(0.0);
            }
        }
        for(int i = 0; i < NUM_ENVELOPE; ++i) {
            adsr[i].setParams(envelopeParams[i].Attack->get(),
                              0.0,
                              envelopeParams[i].Decay->get(),
                              envelopeParams[i].Sustain->get(),
                              envelopeParams[i].Release->get());
            adsr[i].doAttack(sampleRate);
        }
        for(int i = 0; i < NUM_FILTER; ++i) {
            filters[i].initializePastData();
        }
        for(int i = 0; i < NUM_LFO; ++i) {
            lfos[i].setAngle(0.0);
        }
        for(int i = 0; i < NUM_MODENV; ++i) {
            if(modEnvParams[i].shouldUseHold()) {
                modEnvs[i].setParams(0.0,
                                     modEnvParams[i].Wait->get(),
                                     modEnvParams[i].Decay->get(),
                                     0.0,
                                     0.0);
            } else {
                modEnvs[i].setParams(modEnvParams[i].Attack->get(),
                                     0.0,
                                     modEnvParams[i].Decay->get(),
                                     0.0,
                                     0.0);
            }
            modEnvs[i].doAttack(sampleRate);
        }
    }
}
void GrapeVoice::stopNote (float velocity, bool allowTailOff)
{
    DBG("stopNote() allowTailOff:" << std::to_string(allowTailOff));
    if(GrapeSound* playingSound = dynamic_cast<GrapeSound*>(getCurrentlyPlayingSound().get()))
    {
        if(allowTailOff) {
            auto sampleRate = getSampleRate();
            for(int i = 0; i < NUM_ENVELOPE; ++i) {
                adsr[i].doRelease(sampleRate);
            }
        } else {
            stolen = true;
            for(int i = 0; i < NUM_OSC; ++i) {
                oscs[i].setSampleRate(0.0);// stop
            }
            for(int i = 0; i < NUM_ENVELOPE; ++i) {
                adsr[i].forceStop();
            }
            clearCurrentNote();
        }
    }
}
void GrapeVoice::glide(int midiNoteNumber, float velocity)
{
    auto sampleRate = getSampleRate();
    auto portamentTime = voiceParams->PortamentoTime->get();
    smoothNote.exponential(portamentTime, midiNoteNumber, sampleRate);
    smoothVelocity.exponential(portamentTime, velocity, sampleRate);
}
void GrapeVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if(GrapeSound* playingSound = dynamic_cast<GrapeSound*>(getCurrentlyPlayingSound().get()))
    {
        if(getCurrentlyPlayingNote() == 0) {
            return;
        }
        auto sampleRate = getSampleRate();
        
        applyParamsBeforeLoop (sampleRate);

        int numChannels = outputBuffer.getNumChannels();
        jassert(numChannels <= 2);
        
        while (--numSamples >= 0)
        {
            double out[2] {0, 0};
            auto active = step(out, sampleRate, numChannels);
            for (auto ch = 0; ch < numChannels; ++ch) {
                outputBuffer.addSample (ch, startSample, out[ch]);
            }
            ++startSample;
            if(!active) {
                clearCurrentNote();
                break;
            }
        }
    }
}
void GrapeVoice::applyParamsBeforeLoop (double sampleRate)
{
    for(int i = 0; i < NUM_OSC; ++i) {
        oscs[i].setSampleRate(sampleRate);
        oscs[i].setWaveform(OSC_WAVEFORM_VALUES[oscParams[i].Waveform->getIndex()]);
    }
    for(int i = 0; i < NUM_ENVELOPE; ++i) {
        adsr[i].setParams(envelopeParams[i].Attack->get(),
                            0.0,
                            envelopeParams[i].Decay->get(),
                            envelopeParams[i].Sustain->get(),
                            envelopeParams[i].Release->get());
    }
    for(int i = 0; i < NUM_FILTER; ++i) {
        filters[i].setSampleRate(sampleRate);
    }
    for(int i = 0; i < NUM_LFO; ++i) {
        lfos[i].setSampleRate(sampleRate);
        lfos[i].setWaveform(LFO_WAVEFORM_VALUES[lfoParams[i].Waveform->getIndex()]);
    }
    for(int i = 0; i < NUM_MODENV; ++i) {
        if(modEnvParams[i].shouldUseHold()) {
            modEnvs[i].setParams(0.0,
                                    modEnvParams[i].Wait->get(),
                                    modEnvParams[i].Decay->get(),
                                    0.0,
                                    0.0);
        } else {
            modEnvs[i].setParams(envelopeParams[i].Attack->get(),
                                    0.0,
                                    modEnvParams[i].Decay->get(),
                                    0.0,
                                    0.0);
        }
    }
}
bool GrapeVoice::step (double* out, double sampleRate, int numChannels)
{        
    smoothNote.step();
    smoothVelocity.step();
    
    double midiNoteNumber = smoothNote.value + globalParams->Pitch->get() * voiceParams->PitchBendRange->get();
    
    double shiftedNoteNumbers[NUM_OSC] {midiNoteNumber, midiNoteNumber, midiNoteNumber};
    for (int i = 0; i < NUM_OSC; ++i) {
        if(!oscParams[i].Enabled->get()) {
            continue;
        }
        shiftedNoteNumbers[i] += oscParams[i].Octave->get() * 12 + oscParams[i].Coarse->get();
    }
    
    auto modifiers = Modifiers();
    
    for(int i = 0; i < NUM_ENVELOPE; ++i) {
        adsr[i].step(sampleRate);
    }
    
    // ---------------- MODENV ----------------
    for(int i = 0; i < NUM_MODENV; ++i) {
        auto& params = modEnvParams[i];
        if(!params.Enabled->get()) {
            continue;
        }
        modEnvs[i].step(sampleRate);
        auto modEnvValue = modEnvs[i].getValue();
        auto targetType = static_cast<MODENV_TARGET_TYPE>(params.TargetType->getIndex());
        switch(targetType) {
            case MODENV_TARGET_TYPE::OSC: {
                int targetIndex = params.TargetOsc->getIndex();
                auto targetParam = static_cast<MODENV_TARGET_OSC_PARAM>(params.TargetOscParam->getIndex());
                for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                    if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                        switch(targetParam) {
                            case MODENV_TARGET_OSC_PARAM::Freq: {
                                modifiers.octShift[oscIndex] += params.PeakFreq->get() * modEnvValue;
                                break;
                            }
                            case MODENV_TARGET_OSC_PARAM::Edge: {
                                modifiers.edgeRatio[oscIndex] *= modEnvValue;
                                break;
                            }
                            case MODENV_TARGET_OSC_PARAM::Detune: {
                                if(static_cast<MODENV_FADE>(params.Fade->getIndex()) == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;// TODO: cause a bug when targetIndex == NUM_OSC
                                }
                                modifiers.detuneRatio[oscIndex] *= modEnvValue;
                                break;
                            }
                            case MODENV_TARGET_OSC_PARAM::Spread: {
                                if(static_cast<MODENV_FADE>(params.Fade->getIndex()) == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;// TODO: cause a bug when targetIndex == NUM_OSC
                                }
                                modifiers.spreadRatio[oscIndex] *= modEnvValue;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case MODENV_TARGET_TYPE::Filter: {
                int targetIndex = params.TargetFilter->getIndex();
                auto targetParam = static_cast<MODENV_TARGET_FILTER_PARAM>(params.TargetFilterParam->getIndex());
                for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                    if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                        switch (targetParam) {
                            case MODENV_TARGET_FILTER_PARAM::Freq: {
                                modifiers.filterOctShift[filterIndex] += params.PeakFreq->get() * modEnvValue;
                                break;
                            }
                            case MODENV_TARGET_FILTER_PARAM::Q: {
                                if(static_cast<MODENV_FADE>(params.Fade->getIndex()) == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;// TODO: cause a bug when targetIndex == NUM_FILTER
                                }
                                modifiers.filterQExp[filterIndex] *= modEnvValue;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case MODENV_TARGET_TYPE::LFO: {
                int targetIndex = params.TargetLfo->getIndex();
                auto targetParam = static_cast<MODENV_TARGET_LFO_PARAM>(params.TargetLfoParam->getIndex());
                for(int lfoIndex = 0; lfoIndex < NUM_LFO; ++lfoIndex) {
                    if(targetIndex == lfoIndex || targetIndex == NUM_LFO) {
                        switch(targetParam) {
                            case MODENV_TARGET_LFO_PARAM::Freq: {
                                modifiers.lfoOctShift[lfoIndex] += params.PeakFreq->get() * modEnvValue;
                                break;
                            }
                            case MODENV_TARGET_LFO_PARAM::Amount: {
                                if(static_cast<MODENV_FADE>(params.Fade->getIndex()) == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;// TODO: cause a bug when targetIndex == NUM_LFO
                                }
                                modifiers.lfoAmountGain[lfoIndex] *= modEnvValue;
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    // ---------------- LFO ----------------
    for(int i = 0; i < NUM_LFO; ++i) {
        auto& params = lfoParams[i];
        if(!params.Enabled->get()) {
            continue;
        }
        double lfoValue;
        double freq;
        if(lfoParams[i].shouldUseFastFreq()) {
            int target = lfoParams[i].TargetOsc->getIndex();
            auto shiftedNoteNumber = target == NUM_OSC ? midiNoteNumber : shiftedNoteNumbers[target];
            shiftedNoteNumber += modifiers.lfoOctShift[i];
            freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
            freq *= lfoParams[i].FastFreq->get();
        } else {
            freq = lfoParams[i].SlowFreq->get();
            if(modifiers.lfoOctShift[i] != 0.0) {
                freq *= std::pow(2.0, modifiers.lfoOctShift[i]);
            }
        }
        lfoValue = lfos[i].step(freq, 0.0, 0.0);
        auto lfoAmount = params.Amount->get() * modifiers.lfoAmountGain[i];
        auto targetType = static_cast<LFO_TARGET_TYPE>(params.TargetType->getIndex());
        switch(targetType) {
            case LFO_TARGET_TYPE::OSC: {
                int targetIndex = params.TargetOsc->getIndex();
                auto param = static_cast<LFO_TARGET_OSC_PARAM>(params.TargetOscParam->getIndex());
                for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                    if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                        switch(param) {
                            case LFO_TARGET_OSC_PARAM::Vibrato: {
                                jassert(lfoValue <= 1.1);
                                constexpr auto by12 = 1 / 12.0;
                                modifiers.octShift[oscIndex] += lfoValue * lfoAmount * by12;
                                break;
                            }
                            case LFO_TARGET_OSC_PARAM::Tremolo: {
                                modifiers.gain[oscIndex] *= 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                                break;
                            }
                            case LFO_TARGET_OSC_PARAM::Edge: {
                                modifiers.edgeRatio[oscIndex] *= 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                                break;
                            }
                            case LFO_TARGET_OSC_PARAM::FM: {
                                modifiers.angleShift[oscIndex] += lfoValue * lfoAmount * juce::MathConstants<double>::twoPi;
                                break;
                            }
                            case LFO_TARGET_OSC_PARAM::AM: {
                                modifiers.gain[oscIndex] *= 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                                break;
                            }
                            case LFO_TARGET_OSC_PARAM::Pan: {
                                modifiers.panMod[oscIndex] += lfoValue * lfoAmount;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case LFO_TARGET_TYPE::Filter: {
                int targetIndex = params.TargetFilter->getIndex();
                auto param = static_cast<LFO_TARGET_FILTER_PARAM>(params.TargetFilterParam->getIndex());
                for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                    if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                        switch (param) {
                            case LFO_TARGET_FILTER_PARAM::Freq: {
                                modifiers.filterOctShift[filterIndex] += lfoValue * lfoAmount;
                                break;
                            }
                            case LFO_TARGET_FILTER_PARAM::Q: {
                                modifiers.filterQExp[filterIndex] *= 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    bool active = false;
    // ---------------- OSC with Envelope and Filter ----------------
    for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
        auto& p = oscParams[oscIndex];
        if(!p.Enabled->get()) {
            continue;
        }
        int envelopeIndex = p.Envelope->getIndex();
        if(!adsr[envelopeIndex].isActive()) {
            continue;
        }
        active = true;
        
        auto freq = getMidiNoteInHertzDouble(shiftedNoteNumbers[oscIndex] + modifiers.octShift[oscIndex] * 12);
        auto edge = p.Edge->get() * modifiers.edgeRatio[oscIndex];
        auto panBase = globalParams->Pan->get();
        auto pan = modifiers.panMod[oscIndex] == 0
            ? panBase
            : juce::jlimit(-1.0, 1.0, panBase + (std::min(1.0 - panBase, 1.0 + panBase) * modifiers.panMod[oscIndex]));// TODO: 計算減らす
        auto detune = p.Detune->get() * modifiers.detuneRatio[oscIndex];
        auto spread = p.Spread->get() * modifiers.spreadRatio[oscIndex];
        
        double o[2] {0, 0};
        oscs[oscIndex].step(p.Unison->get(),
                            pan,
                            detune,
                            spread,
                            freq,
                            modifiers.angleShift[oscIndex],
                            edge,
                            o);
        auto oscGain = adsr[envelopeIndex].getValue() * modifiers.gain[oscIndex] * p.Gain->get();
        o[0] *= oscGain;
        o[1] *= oscGain;

        for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
            if(!filterParams[filterIndex].Enabled->get()) {
                continue;
            }
            if(filterParams[filterIndex].Target->getIndex() == oscIndex) {
                auto filterType = static_cast<FILTER_TYPE>(filterParams[filterIndex].Type->getIndex());
                double freq;
                switch(static_cast<FILTER_FREQ_TYPE>(filterParams[filterIndex].FreqType->getIndex())) {
                    case FILTER_FREQ_TYPE::Absolute: {
                        double noteShift = modifiers.filterOctShift[filterIndex] * 12;
                        freq = shiftHertsByNotes(filterParams[filterIndex].Hz->get(), noteShift);
                        break;
                    }
                    case FILTER_FREQ_TYPE::Relative: {
                        double shiftedNoteNumber = shiftedNoteNumbers[oscIndex];
                        shiftedNoteNumber += filterParams[filterIndex].Semitone->get();
                        shiftedNoteNumber += modifiers.filterOctShift[filterIndex] * 12;
                        freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                        break;
                    }
                }
                auto q = filterParams[filterIndex].Q->get();
                if(modifiers.filterQExp[filterIndex] != 1.0) {
                    q = std::pow(q, modifiers.filterQExp[filterIndex]);
                }
                auto gain = filterParams[filterIndex].Gain->get();
                for (auto ch = 0; ch < numChannels; ++ch) {
                    o[ch] = filters[filterIndex].step(filterType, freq, q, gain, ch, o[ch]);
                }
            }
        }
        out[0] += o[0];
        out[1] += o[1];
    }
    for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
        if(!filterParams[filterIndex].Enabled->get()) {
            continue;
        }
        if(filterParams[filterIndex].Target->getIndex() == NUM_OSC) {// All
            auto filterType = static_cast<FILTER_TYPE>(filterParams[filterIndex].Type->getIndex());
            double freq;
            switch(static_cast<FILTER_FREQ_TYPE>(filterParams[filterIndex].FreqType->getIndex())) {
                case FILTER_FREQ_TYPE::Absolute: {
                    double noteShift = modifiers.filterOctShift[filterIndex] * 12;
                    freq = shiftHertsByNotes(filterParams[filterIndex].Hz->get(), noteShift);
                    break;
                }
                case FILTER_FREQ_TYPE::Relative: {
                    double shiftedNoteNumber = midiNoteNumber;
                    shiftedNoteNumber += filterParams[filterIndex].Semitone->get();
                    shiftedNoteNumber += modifiers.filterOctShift[filterIndex] * 12;
                    freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                    break;
                }
            }
            auto q = filterParams[filterIndex].Q->get();
            if(modifiers.filterQExp[filterIndex] != 1.0) {
                q = std::pow(q, modifiers.filterQExp[filterIndex]);
            }
            auto gain = filterParams[filterIndex].Gain->get();
            for (auto ch = 0; ch < numChannels; ++ch) {
                out[ch] = filters[filterIndex].step(filterType, freq, q, gain, ch, out[ch]);
            }
        }
    }
    auto finalGain = 0.3 * smoothVelocity.value;
    for (auto ch = 0; ch < numChannels; ++ch) {
        out[ch] *= finalGain;
    }
    return active;
    // for (auto ch = 0; ch < numChannels; ++ch) {
    //     outputBuffer.addSample (ch, startSample, out[ch]);
    // }
    // ++startSample;
    // if(!active) {
    //     clearCurrentNote();
    //     break;
    // }
}


