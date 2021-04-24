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
            oscs[i].setWaveform(OSC_WAVEFORM_VALUES[oscParams[i].Waveform->getIndex()]);
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
        
        while (--numSamples >= 0)
        {
#if JUCE_DEBUG
            perf.start();
#endif
            smoothVelocity.step();
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].step(sampleRate);
            }
            
            double angleShift[NUM_OSC] {0, 0, 0};
            double octShift[NUM_OSC] {0, 0, 0};
            double detuneRatio[NUM_OSC] {1.0, 1.0, 1.0};
            double spreadRatio[NUM_OSC] {1.0, 1.0, 1.0};
            double gain[NUM_OSC] {1.0, 1.0, 1.0};
            double filterOctShift[NUM_FILTER] {0, 0};
            double filterQExp[NUM_FILTER] {1.0, 1.0};
            double lfoOctShift[NUM_LFO] {0, 0, 0};
            double lfoAmountGain[NUM_LFO] {1.0, 1.0, 1.0};
            
            // ---------------- MODENV ----------------
            for(int i = 0; i < NUM_MODENV; i++) {
                auto params = &modEnvParams[i];
                if(!params->Enabled->get()) {
                    continue;
                }
                modEnvs[i].step(sampleRate);
                auto modEnvValue = modEnvs[i].getValue();
                auto targetType = MODENV_TARGET_TYPE_VALUES[params->TargetType->getIndex()];
                if(targetType == MODENV_TARGET_TYPE::OSC) {
                    for(int oscIndex = 0; oscIndex < NUM_OSC; oscIndex++) {
                        int targetIndex = params->TargetOsc->getIndex();
                        if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                            auto targetParam = MODENV_TARGET_OSC_PARAM_VALUES[params->TargetOscParam->getIndex()];
                            if(targetParam == MODENV_TARGET_OSC_PARAM::Freq) {
                                octShift[oscIndex] += params->PeakFreq->get() * modEnvValue;
                            } else if(targetParam == MODENV_TARGET_OSC_PARAM::Detune) {
                                if(MODENV_FADE_VALUES[params->Fade->getIndex()] == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;
                                }
                                detuneRatio[oscIndex] *= modEnvValue;
                            } else if(targetParam == MODENV_TARGET_OSC_PARAM::Spread) {
                                if(MODENV_FADE_VALUES[params->Fade->getIndex()] == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;
                                }
                                spreadRatio[oscIndex] *= modEnvValue;
                            }
                        }
                    }
                }
                else if(targetType == MODENV_TARGET_TYPE::Filter) {
                    for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                        int targetIndex = params->TargetFilter->getIndex();
                        if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                            auto targetParam = MODENV_TARGET_FILTER_PARAM_VALUES[params->TargetFilterParam->getIndex()];
                            if(targetParam == MODENV_TARGET_FILTER_PARAM::Freq) {
                                filterOctShift[filterIndex] += params->PeakFreq->get() * modEnvValue;
                            } else if(targetParam == MODENV_TARGET_FILTER_PARAM::Q) {
                                if(MODENV_FADE_VALUES[params->Fade->getIndex()] == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;
                                }
                                filterQExp[filterIndex] *= modEnvValue;
                            }
                        }
                    }
                }
                else if(targetType == MODENV_TARGET_TYPE::LFO) {
                    for(int lfoIndex = 0; lfoIndex < NUM_MODENV; lfoIndex++) {
                        int targetIndex = params->TargetLfo->getIndex();
                        if(targetIndex == lfoIndex || targetIndex == NUM_LFO) {
                            auto targetParam = MODENV_TARGET_LFO_PARAM_VALUES[params->TargetLfoParam->getIndex()];
                            if(targetParam == MODENV_TARGET_LFO_PARAM::Freq) {
                                lfoOctShift[lfoIndex] += params->PeakFreq->get() * modEnvValue;
                            } else if(targetParam == MODENV_TARGET_LFO_PARAM::Amount) {
                                if(MODENV_FADE_VALUES[params->Fade->getIndex()] == MODENV_FADE::In) {
                                    modEnvValue = 1 - modEnvValue;
                                }
                                lfoAmountGain[lfoIndex] *= modEnvValue;
                            }
                        }
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
                    freq *= std::pow(2.0, lfoOctShift[i]);
                }
                lfoValue = lfos[i].step(freq, 0.0);
                auto lfoAmount = params->Amount->get() * lfoAmountGain[i];
                auto targetType = LFO_TARGET_TYPE_VALUES[params->TargetType->getIndex()];
                if(targetType == LFO_TARGET_TYPE::OSC) {
                    for(int oscIndex = 0; oscIndex < NUM_OSC; oscIndex++) {
                        int targetIndex = params->TargetOsc->getIndex();
                        if(targetIndex == oscIndex || targetIndex == NUM_OSC) {
                            auto param = LFO_TARGET_OSC_PARAM_VALUES[params->TargetOscParam->getIndex()];
                            if(param == LFO_TARGET_OSC_PARAM::Vibrato) {
                                octShift[oscIndex] += lfoValue * lfoAmount;
                            } else if(param == LFO_TARGET_OSC_PARAM::Tremolo) {
                                gain[oscIndex] *= 1 - ((lfoValue + 1) / 2 * lfoAmount);
                            } else if(param == LFO_TARGET_OSC_PARAM::FM) {
                                angleShift[oscIndex] += lfoValue * lfoAmount * juce::MathConstants<double>::twoPi;
                            } else if(param == LFO_TARGET_OSC_PARAM::AM) {
                                gain[oscIndex] *= 1 - ((lfoValue + 1) / 2 * lfoAmount);
                            }
                        }
                    }
                }
                else if(targetType == LFO_TARGET_TYPE::Filter) {
                    for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                        int targetIndex = params->TargetFilter->getIndex();
                        if(targetIndex == filterIndex || targetIndex == NUM_FILTER) {
                            auto param = LFO_TARGET_FILTER_PARAM_VALUES[params->TargetFilterParam->getIndex()];
                            if(param == LFO_TARGET_FILTER_PARAM::Freq) {
                                filterOctShift[filterIndex] += lfoValue * lfoAmount;
                            } else if(param == LFO_TARGET_FILTER_PARAM::Q) {
                                filterQExp[filterIndex] *= 1 - ((lfoValue + 1) / 2 * lfoAmount);
                            }
                        }
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
                double adsrValue = 0;
                auto targetEnvelope = OSC_ENV_VALUES[oscParams[oscIndex].Envelope->getIndex()];
                for(int envelopeIndex = 0; envelopeIndex < NUM_ENVELOPE; envelopeIndex++) {
                    if(targetEnvelope & (0x1 << envelopeIndex)) {
                        adsrValue += adsr[envelopeIndex].getValue();
                        if(adsr[envelopeIndex].isActive()) {
                            active = true;
//                            sparseLog.log("active", "osc:" + std::to_string(oscIndex) + ", envelope:" + std::to_string(envelopeIndex));
                        }
                    }
                }
                for (auto ch = 0; ch < numChannels; ch++) {
                    o[ch] *= 0.30 * adsrValue * gain[oscIndex] * oscParams[oscIndex].Gain->get() * smoothVelocity.value;
                    
                    for(int filterIndex = 0; filterIndex < NUM_FILTER; filterIndex++) {
                        if(!filterParams[filterIndex].Enabled->get()) {
                            continue;
                        }
                        if(filterParams[filterIndex].Target->getIndex() == oscIndex) {
                            auto filterType = FILTER_TYPE_VALUES[filterParams[filterIndex].Type->getIndex()];
                            double shiftedNoteNumber = shiftedNoteNumbers[oscIndex];
                            shiftedNoteNumber += filterParams[filterIndex].Octave->get() * 12;
                            shiftedNoteNumber += filterOctShift[filterIndex] * 12;
                            auto freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                            auto q = filterParams[filterIndex].Q->get();
                            q = std::pow(q, filterQExp[filterIndex]);
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
                    auto filterType = FILTER_TYPE_VALUES[filterParams[filterIndex].Type->getIndex()];
                    double shiftedNoteNumber = midiNoteNumber;
                    shiftedNoteNumber += filterParams[filterIndex].Octave->get() * 12;
                    shiftedNoteNumber += filterOctShift[filterIndex] * 12;
                    auto freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                    auto q = filterParams[filterIndex].Q->get();
                    q = std::pow(q, filterQExp[filterIndex]);
                    for (auto ch = 0; ch < numChannels; ch++) {
                        out[ch] = filters[filterIndex].step(filterType, freq, q, ch, out[ch]);
                    }
                }
            }
            for (auto ch = 0; ch < numChannels; ch++) {
                outputBuffer.addSample (ch, startSample, out[ch]);
            }
#if JUCE_DEBUG
            for (auto ch = 0; ch < numChannels; ch++) {
                auto value = outputBuffer.getSample(ch, startSample);
                jassert(value >= -1);
                jassert(value <= 1);
            }
            perf.stop();
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
