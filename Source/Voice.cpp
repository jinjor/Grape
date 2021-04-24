#include "Voice.h"
#include "Params.h"

//==============================================================================
GrapeSound::GrapeSound() {}
GrapeSound::~GrapeSound() {}
bool GrapeSound::appliesToNote(int) { return true; }
bool GrapeSound::appliesToChannel(int) { return true; }

//==============================================================================
GrapeVoice::GrapeVoice(OscParams* oscParams, EnvelopeParams* envelopeParams, FilterParams* filterParams, LfoParams* lfoParams, ModEnvParams* modEnvParams)
: perf(juce::PerformanceCounter("voice cycle", 100000))
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
{
//    for(int i = 0; i < NUM_ENVELOPE; i++) {
//        adsr[i].setParams(envelopeParams[i].Attack->get(),
//                          0.0,
//                          envelopeParams[i].Decay->get(),
//                          envelopeParams[i].Sustain->get(),
//                          envelopeParams[i].Release->get());
//    }
}
GrapeVoice::~GrapeVoice() {
    DBG("GrapeVoice's destructor called.");
}
bool GrapeVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<GrapeSound*> (sound) != nullptr;
}
void GrapeVoice::startNote (int midiNoteNumber, float velocity,
                juce::SynthesiserSound*, int currentPitchWheelPosition)
{
    DBG("startNote() midiNoteNumber:" << midiNoteNumber);
    if(GrapeSound* playingSound = dynamic_cast<GrapeSound*>(getCurrentlyPlayingSound().get()))
    {
        this->midiNoteNumber = midiNoteNumber;
        auto sampleRate = getSampleRate();
        if(stolen) {
            smoothVelocity.exponential(0.01, velocity, sampleRate);
        } else {
            smoothVelocity.init(velocity);
        }
        stolen = false;
        
        for(int i = 0; i < NUM_OSC; i++) {
            if(!stolen) {
//                oscs[i].setAngle(0.0);
            }
        }
        for(int i = 0; i < NUM_ENVELOPE; i++) {
            adsr[i].setParams(envelopeParams[i].Attack->get(),
                              0.0,
                              envelopeParams[i].Decay->get(),
                              envelopeParams[i].Sustain->get(),
                              envelopeParams[i].Release->get());
            adsr[i].doAttack(sampleRate);
        }
        for(int i = 0; i < NUM_FILTER; i++) {
            filters[i].initializePastData();
        }
        for(int i = 0; i < NUM_LFO; i++) {
            // noop
        }
        for(int i = 0; i < NUM_MODENV; i++) {
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
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].doRelease(sampleRate);
            }
        } else {
            stolen = true;
            for(int i = 0; i < NUM_OSC; i++) {
                oscs[i].setSampleRate(0.0);// stop
            }
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].forceStop();
            }
            midiNoteNumber = 0;
            clearCurrentNote();
        }
    }
}
void GrapeVoice::renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if(GrapeSound* playingSound = dynamic_cast<GrapeSound*>(getCurrentlyPlayingSound().get()))
    {
        if(midiNoteNumber == 0) {
            return;
        }
        auto sampleRate = getSampleRate();
        
        int shiftedNoteNumbers[NUM_OSC] {midiNoteNumber, midiNoteNumber, midiNoteNumber};
        for(int i = 0; i < NUM_OSC; i++) {
            shiftedNoteNumbers[i] += oscParams[i].Octave->get() * 12;
            shiftedNoteNumbers[i] += oscParams[i].Coarse->get();
        }
        for(int i = 0; i < NUM_OSC; i++) {
            oscs[i].setSampleRate(sampleRate);
            oscs[i].setWaveform(static_cast<OSC_WAVEFORM>(oscParams[i].Waveform->getIndex()));
        }
        for(int i = 0; i < NUM_ENVELOPE; i++) {
            adsr[i].setParams(envelopeParams[i].Attack->get(),
                              0.0,
                              envelopeParams[i].Decay->get(),
                              envelopeParams[i].Sustain->get(),
                              envelopeParams[i].Release->get());
        }
        for(int i = 0; i < NUM_FILTER; i++) {
            filters[i].setSampleRate(sampleRate);
        }
        for(int i = 0; i < NUM_LFO; i++) {
            lfos[i].setSampleRate(sampleRate);
            // waveform?
        }
        for(int i = 0; i < NUM_MODENV; i++) {
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
        
        double angleShift[NUM_OSC] {};
        double octShift[NUM_OSC] {};
        double detuneRatio[NUM_OSC] {};
        double spreadRatio[NUM_OSC] {};
        double gain[NUM_OSC] {};
        double filterOctShift[NUM_FILTER] {};
        double filterQExp[NUM_FILTER] {};
        double lfoOctShift[NUM_LFO] {};
        double lfoAmountGain[NUM_LFO] {};
        
        while (--numSamples >= 0)
        {
#if JUCE_DEBUG
//            perf.start();
#endif
            smoothVelocity.step();
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].step(sampleRate);
            }
            
            for(int i = 0; i < NUM_OSC; i++) {
                angleShift[i] = 0;
                octShift[i] = 0;
                detuneRatio[i] = 1.0;
                spreadRatio[i] = 1.0;
                gain[i] = 1.0;
            }
            for(int i = 0; i < NUM_FILTER; i++) {
                filterOctShift[i] = 0;
                filterQExp[i] = 1.0;
            }
            for(int i = 0; i < NUM_LFO; i++) {
                lfoOctShift[i] = 0;
                lfoAmountGain[i] = 1.0;
            }
            
            // ---------------- MODENV ----------------
            for(int i = 0; i < NUM_MODENV; i++) {
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
                        for(int oscIndex = 0; oscIndex < NUM_OSC; oscIndex++) {
                            if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                                switch(targetParam) {
                                    case MODENV_TARGET_OSC_PARAM::Freq: {
                                        octShift[oscIndex] += params->PeakFreq->get() * modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_OSC_PARAM::Detune: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
                                        }
                                        detuneRatio[oscIndex] *= modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_OSC_PARAM::Spread: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
                                        }
                                        spreadRatio[oscIndex] *= modEnvValue;
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
                        for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                            if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                                switch (targetParam) {
                                    case MODENV_TARGET_FILTER_PARAM::Freq: {
                                        filterOctShift[filterIndex] += params->PeakFreq->get() * modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_FILTER_PARAM::Q: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
                                        }
                                        filterQExp[filterIndex] *= modEnvValue;
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
                        for(int lfoIndex = 0; lfoIndex < NUM_MODENV; lfoIndex++) {
                            if(targetIndex == lfoIndex || targetIndex == NUM_LFO) {
                                switch(targetParam) {
                                    case MODENV_TARGET_LFO_PARAM::Freq: {
                                        lfoOctShift[lfoIndex] += params->PeakFreq->get() * modEnvValue;
                                        break;
                                    }
                                    case MODENV_TARGET_LFO_PARAM::Amount: {
                                        if(static_cast<MODENV_FADE>(params->Fade->getIndex()) == MODENV_FADE::In) {
                                            modEnvValue = 1 - modEnvValue;
                                        }
                                        lfoAmountGain[lfoIndex] *= modEnvValue;
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
            for(int i = 0; i < NUM_LFO; i++) {
                auto params = &lfoParams[i];
                if(!params->Enabled->get()) {
                    continue;
                }
                double lfoValue;
                double freq;
                if(lfoParams[i].shouldUseFastFreq()) {
                    int target = lfoParams[i].TargetOsc->getIndex();
                    auto shiftedNoteNumber = target == NUM_OSC ? midiNoteNumber : shiftedNoteNumbers[target];
                    shiftedNoteNumber += lfoOctShift[i];
                    freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                    freq *= lfoParams[i].FastFreq->get();
                } else {
                    freq = lfoParams[i].SlowFreq->get();
                    if(lfoOctShift[i] != 0.0) {
                        freq *= std::pow(2.0, lfoOctShift[i]);
                    }
                }
                lfoValue = lfos[i].step(freq, 0.0);
                auto lfoAmount = params->Amount->get() * lfoAmountGain[i];
                auto targetType = static_cast<LFO_TARGET_TYPE>(params->TargetType->getIndex());
                switch(targetType) {
                    case LFO_TARGET_TYPE::OSC: {
                        int targetIndex = params->TargetOsc->getIndex();
                        auto param = static_cast<LFO_TARGET_OSC_PARAM>(params->TargetOscParam->getIndex());
                        for(int oscIndex = 0; oscIndex < NUM_OSC; oscIndex++) {
                            if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                                switch(param) {
                                    case LFO_TARGET_OSC_PARAM::Vibrato: {
                                        octShift[oscIndex] += lfoValue * lfoAmount;
                                        break;
                                    }
                                    case LFO_TARGET_OSC_PARAM::Tremolo: {
                                        gain[oscIndex] *= 1 - ((lfoValue + 1) / 2 * lfoAmount);
                                        break;
                                    }
                                    case LFO_TARGET_OSC_PARAM::FM: {
                                        angleShift[oscIndex] += lfoValue * lfoAmount * juce::MathConstants<double>::twoPi;
                                        break;
                                    }
                                    case LFO_TARGET_OSC_PARAM::AM: {
                                        gain[oscIndex] *= 1 - ((lfoValue + 1) / 2 * lfoAmount);
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
                        for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                            if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                                switch (param) {
                                    case LFO_TARGET_FILTER_PARAM::Freq: {
                                        filterOctShift[filterIndex] += lfoValue * lfoAmount;
                                        break;
                                    }
                                    case LFO_TARGET_FILTER_PARAM::Q: {
                                        filterQExp[filterIndex] *= 1 - ((lfoValue + 1) / 2 * lfoAmount);
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
            for(int oscIndex = 0; oscIndex < NUM_OSC; oscIndex++) {
                if(!oscParams[oscIndex].Enabled->get()) {
                    continue;
                }
                auto freq = getMidiNoteInHertzDouble(shiftedNoteNumbers[oscIndex] + octShift[oscIndex] * 12);
                auto detune = oscParams[oscIndex].Detune->get() * detuneRatio[oscIndex];
                auto spread = oscParams[oscIndex].Spread->get() * spreadRatio[oscIndex];
                double o[2] {0, 0};
                oscs[oscIndex].step(oscParams[oscIndex].Unison->get(),
                                    detune,
                                    spread,
                                    freq,
                                    angleShift[oscIndex],
                                    o);
                int envelopeIndex = oscParams[oscIndex].Envelope->getIndex();
                if(adsr[envelopeIndex].isActive()) {
                    active = true;
//                            sparseLog.log("active", "osc:" + std::to_string(oscIndex) + ", envelope:" + std::to_string(envelopeIndex));
                }
                auto oscGain = adsr[envelopeIndex].getValue() * gain[oscIndex] * oscParams[oscIndex].Gain->get();
                for (auto ch = 0; ch < numChannels; ch++) {
                    o[ch] *= oscGain;
                }
                for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                    if(!filterParams[filterIndex].Enabled->get()) {
                        continue;
                    }
                    if(filterParams[filterIndex].Target->getIndex() == oscIndex) {
                        auto filterType = static_cast<FILTER_TYPE>(filterParams[filterIndex].Type->getIndex());
                        double shiftedNoteNumber = shiftedNoteNumbers[oscIndex];
                        shiftedNoteNumber += filterParams[filterIndex].Octave->get() * 12;
                        shiftedNoteNumber += filterOctShift[filterIndex] * 12;
                        auto freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                        auto q = filterParams[filterIndex].Q->get();
                        if(filterQExp[filterIndex] != 1.0) {
                            q = std::pow(q, filterQExp[filterIndex]);
                        }
                        for (auto ch = 0; ch < numChannels; ch++) {
                            o[ch] = filters[filterIndex].step(filterType, freq, q, ch, o[ch]);
                        }
                    }
                }
                out[0] += o[0];
                out[1] += o[1];
            }
            for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                if(!filterParams[filterIndex].Enabled->get()) {
                    continue;
                }
                if(filterParams[filterIndex].Target->getIndex() == NUM_OSC) {// All
                    auto filterType = static_cast<FILTER_TYPE>(filterParams[filterIndex].Type->getIndex());
                    double shiftedNoteNumber = midiNoteNumber;
                    shiftedNoteNumber += filterParams[filterIndex].Octave->get() * 12;
                    shiftedNoteNumber += filterOctShift[filterIndex] * 12;
                    auto freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                    auto q = filterParams[filterIndex].Q->get();
                    if(filterQExp[filterIndex] != 1.0) {
                        q = std::pow(q, filterQExp[filterIndex]);
                    }
                    for (auto ch = 0; ch < numChannels; ch++) {
                        out[ch] = filters[filterIndex].step(filterType, freq, q, ch, out[ch]);
                    }
                }
            }
            auto finalGain = 0.30 * smoothVelocity.value;
            for (auto ch = 0; ch < numChannels; ch++) {
                out[ch] *= finalGain;
            }
            for (auto ch = 0; ch < numChannels; ch++) {
                outputBuffer.addSample (ch, startSample, out[ch]);
            }
#if JUCE_DEBUG
//            for (auto ch = 0; ch < numChannels; ch++) {
//                auto value = outputBuffer.getSample(ch, startSample);
//                jassert(value >= -1);
//                jassert(value <= 1);
//            }
//            perf.stop();
#endif
            ++startSample;
            if(!active) {
                midiNoteNumber = 0;
                clearCurrentNote();
                break;
            }
        }
    }
}
