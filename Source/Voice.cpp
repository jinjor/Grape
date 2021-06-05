#include "Voice.h"
#include "Params.h"

//==============================================================================
GrapeSound::GrapeSound() {}
GrapeSound::~GrapeSound() {}
bool GrapeSound::appliesToNote(int) { return true; }
bool GrapeSound::appliesToChannel(int) { return true; }

//==============================================================================
Modifiers::Modifiers(VoiceParams* voiceParams, ControlItemParams* controlItemParams)
: voiceParams(voiceParams)
, controlItemParams(controlItemParams)
{
    std::fill_n(angleShift, NUM_OSC, 0);
    std::fill_n(octShift, NUM_OSC, 0);
    std::fill_n(edgeRatio, NUM_OSC, 1.0);
    std::fill_n(panBase, NUM_OSC, 0.0);
    std::fill_n(panMod, NUM_OSC, 0.0);
    std::fill_n(detuneRatio, NUM_OSC, 1.0);
    std::fill_n(spreadRatio, NUM_OSC, 1.0);
    std::fill_n(gain, NUM_OSC, 1.0);
    
    std::fill_n(filterOctShift, NUM_FILTER, 0);
    std::fill_n(filterQExp, NUM_FILTER, 1.0);
    
    std::fill_n(lfoOctShift, NUM_LFO, 0);
    std::fill_n(lfoAmountGain, NUM_LFO, 1.0);
    
    portamentoAmount = 1.0;
    delayAmount = 1.0;
    masterVolume = 1.0;
}
void Modifiers::pitchWheelMoved(int value) {
    if(pitch == value) {
        return;
    }
    pitch = value;
    for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
        octShift[oscIndex] = (value == 0 ? -8192.0 : value - 8192.0) / 8191.0 * (voiceParams->PitchBendRange->get() / 12.0);
    }
}
void Modifiers::controllerMoved(int number, int value) {
    if(cc[number] == value) {
        return;
    }
    cc[number] = value;
    auto normalizedValue = value / 127.0;
    
    // ---------------- CONTROL ----------------
    for(int i = 0; i < NUM_CONTROL; ++i) {
        auto params = &controlItemParams[i];
        if(number != CONTROL_NUMBER_VALUES[params->Number->getIndex()]) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(params->TargetType->getIndex());
        switch(targetType) {
            case CONTROL_TARGET_TYPE::OSC: {
                int targetIndex = params->TargetOsc->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_OSC_PARAM>(params->TargetOscParam->getIndex());
                for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                    if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                        switch(targetParam) {
                            case CONTROL_TARGET_OSC_PARAM::Freq: {
                                octShift[oscIndex] = 4.0 * normalizedValue;
                                break;
                            }
                            case CONTROL_TARGET_OSC_PARAM::Edge: {
                                edgeRatio[oscIndex] = normalizedValue;
                                break;
                            }
                            case CONTROL_TARGET_OSC_PARAM::Detune: {
                                detuneRatio[oscIndex] = normalizedValue;
                                break;
                            }
                            case CONTROL_TARGET_OSC_PARAM::Spread: {
                                spreadRatio[oscIndex] = normalizedValue;
                                break;
                            }
                            case CONTROL_TARGET_OSC_PARAM::Pan: {
                                panBase[oscIndex] = normalizedValue * 2 - 1.0;
                                break;
                            }
                            case CONTROL_TARGET_OSC_PARAM::Gain: {
                                gain[oscIndex] = normalizedValue;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case CONTROL_TARGET_TYPE::Filter: {
                int targetIndex = params->TargetFilter->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_FILTER_PARAM>(params->TargetFilterParam->getIndex());
                for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                    if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                        switch (targetParam) {
                            case CONTROL_TARGET_FILTER_PARAM::Freq: {
                                filterOctShift[filterIndex] = 4.0 * normalizedValue;
                                break;
                            }
                            case CONTROL_TARGET_FILTER_PARAM::Q: {
                                filterQExp[filterIndex] = normalizedValue;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case CONTROL_TARGET_TYPE::LFO: {
                int targetIndex = params->TargetLfo->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_LFO_PARAM>(params->TargetLfoParam->getIndex());
                for(int lfoIndex = 0; lfoIndex < NUM_MODENV; ++lfoIndex) {
                    if(targetIndex == lfoIndex || targetIndex == NUM_LFO) {
                        switch(targetParam) {
                            case CONTROL_TARGET_LFO_PARAM::Freq: {
                                lfoOctShift[lfoIndex] = 4.0 * normalizedValue;
                                break;
                            }
                            case CONTROL_TARGET_LFO_PARAM::Amount: {
                                lfoAmountGain[lfoIndex] = normalizedValue;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case CONTROL_TARGET_TYPE::Master: {
                auto targetParam = static_cast<CONTROL_TARGET_MISC_PARAM>(params->TargetMiscParam->getIndex());
                switch(targetParam) {
                    case CONTROL_TARGET_MISC_PARAM::PortamentoAmount: {
                        portamentoAmount = normalizedValue;
                        break;
                    }
                    case CONTROL_TARGET_MISC_PARAM::DelayAmount: {
                        delayAmount = normalizedValue;
                        break;
                    }
                    case CONTROL_TARGET_MISC_PARAM::MasterVolume: {
                        masterVolume = normalizedValue;
                        break;
                    }
                }
                break;
            }
        }
    }
}

//==============================================================================
GrapeVoice::GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo,
                       VoiceParams* voiceParams,
                       OscParams* oscParams,
                       EnvelopeParams* envelopeParams,
                       FilterParams* filterParams,
                       LfoParams* lfoParams,
                       ModEnvParams* modEnvParams,
                       Modifiers* modifiers)
: perf(juce::PerformanceCounter("voice cycle", 100000))
, currentPositionInfo(currentPositionInfo)
, voiceParams(voiceParams)
, oscParams(oscParams)
, envelopeParams(envelopeParams)
, filterParams(filterParams)
, lfoParams(lfoParams)
, modEnvParams(modEnvParams)
, modifiers(modifiers)
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
        auto midiNoteNumber = getCurrentlyPlayingNote();// TODO: まだ必要？
        if(getCurrentlyPlayingNote() == 0) {
            return;
        }
        auto sampleRate = getSampleRate();
        
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
        
        int numChannels = outputBuffer.getNumChannels();
        jassert(numChannels <= 2);
        
        while (--numSamples >= 0)
        {
#if JUCE_DEBUG
//            perf.start();
#endif
            smoothNote.step();
            smoothVelocity.step();
            
            double shiftedNoteNumbers[NUM_OSC] {smoothNote.value, smoothNote.value, smoothNote.value};
            for(int i = 0; i < NUM_OSC; ++i) {
                shiftedNoteNumbers[i] += oscParams[i].Octave->get() * 12;
                shiftedNoteNumbers[i] += oscParams[i].Coarse->get();
            }
            
            auto modifiers = *(this->modifiers);// clone -- includes controled values
            
            for(int i = 0; i < NUM_ENVELOPE; ++i) {
                adsr[i].step(sampleRate);
            }
            
            // ---------------- MODENV ----------------
            for(int i = 0; i < NUM_MODENV; ++i) {
                auto params = &modEnvParams[i];
                if(!params->Enabled->get()) {
                    continue;
                }
                modEnvs[i].step(sampleRate);
                auto modEnvValue = modEnvs[i].getValue();
                auto targetType = static_cast<MODENV_TARGET_TYPE>(params->TargetType->getIndex());
                switch(targetType) {
                    case MODENV_TARGET_TYPE::OSC: {
                        int targetIndex = params->TargetOsc->getIndex();
                        auto targetParam = static_cast<MODENV_TARGET_OSC_PARAM>(params->TargetOscParam->getIndex());
                        for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                            if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                                switch(targetParam) {
                                    case MODENV_TARGET_OSC_PARAM::Freq: {
                                        modifiers.octShift[oscIndex] += params->PeakFreq->get() * modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_OSC_PARAM::Edge: {
                                        modifiers.edgeRatio[oscIndex] *= modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_OSC_PARAM::Detune: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
                                        }
                                        modifiers.detuneRatio[oscIndex] *= modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_OSC_PARAM::Spread: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
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
                        int targetIndex = params->TargetFilter->getIndex();
                        auto targetParam = static_cast<MODENV_TARGET_FILTER_PARAM>(params->TargetFilterParam->getIndex());
                        for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                            if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                                switch (targetParam) {
                                    case MODENV_TARGET_FILTER_PARAM::Freq: {
                                        modifiers.filterOctShift[filterIndex] += params->PeakFreq->get() * modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_FILTER_PARAM::Q: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
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
                        int targetIndex = params->TargetLfo->getIndex();
                        auto targetParam = static_cast<MODENV_TARGET_LFO_PARAM>(params->TargetLfoParam->getIndex());
                        for(int lfoIndex = 0; lfoIndex < NUM_LFO; ++lfoIndex) {
                            if(targetIndex == lfoIndex || targetIndex == NUM_LFO) {
                                switch(targetParam) {
                                    case MODENV_TARGET_LFO_PARAM::Freq: {
                                        modifiers.lfoOctShift[lfoIndex] += params->PeakFreq->get() * modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_LFO_PARAM::Amount: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
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
                auto params = &lfoParams[i];
                if(!params->Enabled->get()) {
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
                auto lfoAmount = params->Amount->get() * modifiers.lfoAmountGain[i];
                auto targetType = static_cast<LFO_TARGET_TYPE>(params->TargetType->getIndex());
                switch(targetType) {
                    case LFO_TARGET_TYPE::OSC: {
                        int targetIndex = params->TargetOsc->getIndex();
                        auto param = static_cast<LFO_TARGET_OSC_PARAM>(params->TargetOscParam->getIndex());
                        for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                            if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                                switch(param) {
                                    case LFO_TARGET_OSC_PARAM::Vibrato: {
                                        modifiers.octShift[oscIndex] += lfoValue * lfoAmount;
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
                        int targetIndex = params->TargetFilter->getIndex();
                        auto param = static_cast<LFO_TARGET_FILTER_PARAM>(params->TargetFilterParam->getIndex());
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
            double out[2] {0, 0};
            // ---------------- OSC with Envelope and Filter ----------------
            for(int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                if(!oscParams[oscIndex].Enabled->get()) {
                    continue;
                }
                auto freq = getMidiNoteInHertzDouble(shiftedNoteNumbers[oscIndex] + modifiers.octShift[oscIndex] * 12);
                auto edge = oscParams[oscIndex].Edge->get() * modifiers.edgeRatio[oscIndex];
                auto pan = modifiers.panMod[oscIndex] == 0
                    ? modifiers.panBase[oscIndex]
                    : juce::jlimit(-1.0, 1.0, modifiers.panBase[oscIndex] + (std::min(1.0 - modifiers.panBase[oscIndex], 1.0 + modifiers.panBase[oscIndex]) * modifiers.panMod[oscIndex]));// TODO: 計算減らす
                auto detune = oscParams[oscIndex].Detune->get() * modifiers.detuneRatio[oscIndex];
                auto spread = oscParams[oscIndex].Spread->get() * modifiers.spreadRatio[oscIndex];
                
                double o[2] {0, 0};
                oscs[oscIndex].step(oscParams[oscIndex].Unison->get(),
                                    pan,
                                    detune,
                                    spread,
                                    freq,
                                    modifiers.angleShift[oscIndex],
                                    edge,
                                    o);
                int envelopeIndex = oscParams[oscIndex].Envelope->getIndex();
                if(adsr[envelopeIndex].isActive()) {
                    active = true;
//                            sparseLog.log("active", "osc:" + std::to_string(oscIndex) + ", envelope:" + std::to_string(envelopeIndex));
                }
                auto oscGain = adsr[envelopeIndex].getValue() * modifiers.gain[oscIndex] * oscParams[oscIndex].Gain->get();
                for (auto ch = 0; ch < numChannels; ++ch) {
                    o[ch] *= oscGain;
                }
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
                                shiftedNoteNumber += filterParams[filterIndex].Octave->get() * 12;
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
                            shiftedNoteNumber += filterParams[filterIndex].Octave->get() * 12;
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
            auto finalGain = 0.30 * smoothVelocity.value;
            for (auto ch = 0; ch < numChannels; ++ch) {
                out[ch] *= finalGain;
            }
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
