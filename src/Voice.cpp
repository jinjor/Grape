#include "Voice.h"

#include "Params.h"

//==============================================================================
GrapeSound::GrapeSound() {}
GrapeSound::~GrapeSound() {}
bool GrapeSound::appliesToNote(int) { return true; }
bool GrapeSound::appliesToChannel(int) { return true; }

//==============================================================================
GrapeVoice::GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo *currentPositionInfo,
                       GlobalParams &globalParams,
                       VoiceParams &voiceParams,
                       std::vector<MainParams> &mainParamList)
    : perf(juce::PerformanceCounter("voice cycle", 100000)),
      currentPositionInfo(currentPositionInfo),
      globalParams(globalParams),
      voiceParams(voiceParams),
      mainParamList(mainParamList),
      oscs{MultiOsc(), MultiOsc(), MultiOsc()},
      adsr{Adsr(), Adsr()},
      filters{Filter(), Filter()},
      lfos{Osc(), Osc(), Osc()},
      modEnvs{Adsr(), Adsr(), Adsr()} {}
GrapeVoice::~GrapeVoice() { DBG("GrapeVoice's destructor called."); }
bool GrapeVoice::canPlaySound(juce::SynthesiserSound *sound) {
    if (dynamic_cast<GrapeSound *>(sound) != nullptr) {
        return true;
    }
    return false;
}
void GrapeVoice::startNote(int midiNoteNumber,
                           float velocity,
                           juce::SynthesiserSound *sound,
                           int currentPitchWheelPosition) {
    DBG("startNote() midiNoteNumber:" << midiNoteNumber);
    if (GrapeSound *playingSound = dynamic_cast<GrapeSound *>(sound)) {
        auto sampleRate = getSampleRate();
        auto &mainParams = mainParamList[128];
        smoothNote.init(midiNoteNumber);
        if (stolen) {
            smoothVelocity.exponentialInfinite(0.01, velocity, sampleRate);
        } else {
            smoothVelocity.init(velocity);
        }
        stolen = false;

        auto fixedSampleRate = sampleRate * CONTROL_RATE;  // for control

        for (int i = 0; i < NUM_OSC; ++i) {
            if (!stolen) {
                //                oscs[i].setAngle(0.0);
            }
        }
        for (int i = 0; i < NUM_ENVELOPE; ++i) {
            auto &params = mainParams.envelopeParams[i];
            adsr[i].setParams(params.attackCurve, params.attack, 0.0, params.decay, params.sustain, params.release);
            adsr[i].doAttack(fixedSampleRate);
        }
        for (int i = 0; i < NUM_FILTER; ++i) {
            filters[i].initializePastData();
        }
        for (int i = 0; i < NUM_LFO; ++i) {
            lfos[i].setNormalizedAngle(0.0);
        }
        for (int i = 0; i < NUM_MODENV; ++i) {
            auto &params = mainParams.modEnvParams[i];
            if (params.shouldUseHold()) {
                modEnvs[i].setParams(0.5, 0.0, params.wait, params.decay, 0.0, 0.0);
            } else {
                modEnvs[i].setParams(0.5, params.attack, 0.0, params.decay, 0.0, 0.0);
            }
            modEnvs[i].doAttack(fixedSampleRate);
        }
        stepCounter = 0;
    }
}
void GrapeVoice::stopNote(float velocity, bool allowTailOff) {
    DBG("stopNote() allowTailOff:" << std::to_string(allowTailOff));
    if (GrapeSound *playingSound = dynamic_cast<GrapeSound *>(getCurrentlyPlayingSound().get())) {
        if (allowTailOff) {
            auto sampleRate = getSampleRate();
            auto fixedSampleRate = sampleRate * CONTROL_RATE;  // for control
            for (int i = 0; i < NUM_ENVELOPE; ++i) {
                adsr[i].doRelease(fixedSampleRate);
            }
        } else {
            stolen = true;
            for (int i = 0; i < NUM_OSC; ++i) {
                oscs[i].setSampleRate(0.0);  // stop
            }
            for (int i = 0; i < NUM_ENVELOPE; ++i) {
                adsr[i].forceStop();
            }
            clearCurrentNote();
        }
    }
}
void GrapeVoice::glide(int midiNoteNumber, float velocity) {
    auto sampleRate = getSampleRate();
    auto portamentTime = voiceParams.portamentoTime;
    smoothNote.exponentialInfinite(portamentTime, midiNoteNumber, sampleRate);
    smoothVelocity.exponentialInfinite(portamentTime, velocity, sampleRate);
}
void GrapeVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    if (GrapeSound *playingSound = dynamic_cast<GrapeSound *>(getCurrentlyPlayingSound().get())) {
        if (getCurrentlyPlayingNote() == 0) {
            return;
        }
        auto sampleRate = getSampleRate();

        applyParamsBeforeLoop(sampleRate);

        int numChannels = outputBuffer.getNumChannels();
        jassert(numChannels <= 2);

        while (--numSamples >= 0) {
            double out[2]{0, 0};
            auto active = step(out, sampleRate, numChannels);
            for (auto ch = 0; ch < numChannels; ++ch) {
                outputBuffer.addSample(ch, startSample, out[ch]);
            }
            ++startSample;
            if (!active) {
                clearCurrentNote();
                break;
            }
        }
    }
}
void GrapeVoice::applyParamsBeforeLoop(double sampleRate) {
    auto &mainParams = mainParamList[128];
    for (int i = 0; i < NUM_OSC; ++i) {
        oscs[i].setSampleRate(sampleRate);
        oscs[i].setWaveform(mainParams.oscParams[i].waveform, true);
    }
    for (int i = 0; i < NUM_ENVELOPE; ++i) {
        auto &params = mainParams.envelopeParams[i];
        adsr[i].setParams(params.attackCurve, params.attack, 0.0, params.decay, params.sustain, params.release);
    }
    for (int i = 0; i < NUM_FILTER; ++i) {
        filters[i].setSampleRate(sampleRate);
    }
    for (int i = 0; i < NUM_LFO; ++i) {
        auto &params = mainParams.lfoParams[i];
        lfos[i].setSampleRate(params.shouldUseFastFreqFreezed ? sampleRate : sampleRate * CONTROL_RATE);
        lfos[i].setWaveform(params.waveform, false);
    }
    for (int i = 0; i < NUM_MODENV; ++i) {
        auto &params = mainParams.modEnvParams[i];
        if (params.shouldUseHold()) {
            modEnvs[i].setParams(0.5, 0.0, params.wait, params.decay, 0.0, 0.0);
        } else {
            modEnvs[i].setParams(0.5, params.attack, 0.0, params.decay, 0.0, 0.0);
        }
    }
}
bool GrapeVoice::step(double *out, double sampleRate, int numChannels) {
    auto &mainParams = mainParamList[128];
    smoothNote.step();
    smoothVelocity.step();

    double midiNoteNumber = smoothNote.value + globalParams.pitch * voiceParams.pitchBendRange;

    double shiftedNoteNumbers[NUM_OSC]{midiNoteNumber, midiNoteNumber, midiNoteNumber};
    for (int i = 0; i < NUM_OSC; ++i) {
        if (!mainParams.oscParams[i].enabled) {
            continue;
        }
        shiftedNoteNumbers[i] += mainParams.oscParams[i].octave * 12 + mainParams.oscParams[i].coarse;
    }

    if (stepCounter == 0) {
        auto fixedSampleRate = sampleRate * CONTROL_RATE;
        for (int i = 0; i < NUM_ENVELOPE; ++i) {
            adsr[i].step(fixedSampleRate);
        }
        controlModifiers = Modifiers{};
        updateModifiersByModEnv(controlModifiers, fixedSampleRate);
        updateModifiersByLfo(controlModifiers);
    }
    stepCounter++;
    if (stepCounter >= CONTROL_INTERVAL) {
        stepCounter = 0;
    }

    auto modifiers = controlModifiers;  // copy;

    for (int i = 0; i < NUM_LFO; ++i) {
        auto &params = mainParams.lfoParams[i];
        if (!params.enabled) {
            continue;
        }
        if (!params.shouldUseFastFreqFreezed) {
            continue;
        }
        int target = params.targetOsc;
        auto shiftedNoteNumber = target == NUM_OSC ? midiNoteNumber : shiftedNoteNumbers[target];
        shiftedNoteNumber += modifiers.lfoOctShift[i];
        double freq = getMidiNoteInHertzDouble(shiftedNoteNumber) * params.fastFreq;
        double lfoValue = lfos[i].step(freq, 0.0, 0.0);
        auto lfoAmount = params.amount * modifiers.lfoAmountGain[i];
        switch (params.targetType) {
            case LFO_TARGET_TYPE::OSC: {
                int targetIndex = params.targetOsc;
                switch (params.targetOscParam) {
                    case LFO_TARGET_OSC_PARAM::Vibrato:
                    case LFO_TARGET_OSC_PARAM::Tremolo:
                    case LFO_TARGET_OSC_PARAM::Edge: {
                        jassertfalse;
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::FM: {
                        auto v = lfoValue * lfoAmount;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.normalizedAngleShift[oscIndex] += v;
                            }
                        } else {
                            modifiers.normalizedAngleShift[targetIndex] += v;
                        }
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::AM: {
                        auto v = 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.gain[oscIndex] *= v;
                            }
                        } else {
                            modifiers.gain[targetIndex] *= v;
                        }
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::Pan: {
                        jassertfalse;
                        break;
                    }
                }
                break;
            }
            case LFO_TARGET_TYPE::Filter: {
                switch (params.targetFilterParam) {
                    case LFO_TARGET_FILTER_PARAM::Freq:
                    case LFO_TARGET_FILTER_PARAM::Q: {
                        jassertfalse;
                        break;
                    }
                }
                break;
            }
        }
    }

    bool active = false;
    auto panBase = globalParams.pan;
    auto panModAmp = std::min(1.0 - panBase, 1.0 + panBase);
    // ---------------- OSC with Envelope and Filter ----------------
    for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
        auto &p = mainParams.oscParams[oscIndex];
        if (!p.enabled) {
            continue;
        }
        int envelopeIndex = p.envelope;
        if (!adsr[envelopeIndex].isActive()) {
            continue;
        }
        active = true;

        auto freq = getMidiNoteInHertzDouble(shiftedNoteNumbers[oscIndex] + modifiers.octShift[oscIndex] * 12);
        auto edge = p.edge * modifiers.edgeRatio[oscIndex];
        auto pan = panBase + panModAmp * modifiers.panMod[oscIndex];
        auto detune = p.detune * modifiers.detuneRatio[oscIndex];
        auto spread = p.spread * modifiers.spreadRatio[oscIndex];
        jassert(pan >= -1);
        jassert(pan <= 1);

        double o[2]{0, 0};
        oscs[oscIndex].step(p.unison, pan, detune, spread, freq, modifiers.normalizedAngleShift[oscIndex], edge, o);
        auto oscGain = adsr[envelopeIndex].getValue() * modifiers.gain[oscIndex] * p.gain;
        o[0] *= oscGain;
        o[1] *= oscGain;

        for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
            auto &fp = mainParams.filterParams[filterIndex];
            if (!fp.enabled) {
                continue;
            }
            if (fp.target == oscIndex) {
                auto filterType = fp.type;
                double freq;
                if (fp.isFreqAbsoluteFreezed) {
                    double noteShift = modifiers.filterOctShift[filterIndex] * 12;
                    freq = shiftHertsByNotes(fp.hz, noteShift);
                } else {
                    double shiftedNoteNumber = shiftedNoteNumbers[oscIndex];
                    shiftedNoteNumber += fp.semitone;
                    shiftedNoteNumber += modifiers.filterOctShift[filterIndex] * 12;
                    freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                }
                auto q = fp.q;
                if (modifiers.filterQExp[filterIndex] != 1.0) {
                    q = std::pow(q, modifiers.filterQExp[filterIndex]);
                }
                auto gain = fp.gain;
                for (auto ch = 0; ch < numChannels; ++ch) {
                    o[ch] = filters[filterIndex].step(filterType, freq, q, gain, ch, o[ch]);
                }
            }
        }
        out[0] += o[0];
        out[1] += o[1];
    }
    for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
        auto &fp = mainParams.filterParams[filterIndex];
        if (!fp.enabled) {
            continue;
        }
        if (fp.target == NUM_OSC) {  // All
            auto filterType = fp.type;
            double freq;
            if (fp.isFreqAbsoluteFreezed) {
                double noteShift = modifiers.filterOctShift[filterIndex] * 12;
                freq = shiftHertsByNotes(fp.hz, noteShift);
            } else {
                double shiftedNoteNumber = midiNoteNumber + fp.semitone + modifiers.filterOctShift[filterIndex] * 12;
                freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
            }
            auto q = fp.q;
            if (modifiers.filterQExp[filterIndex] != 1.0) {
                q = std::pow(q, modifiers.filterQExp[filterIndex]);
            }
            auto gain = fp.gain;
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
void GrapeVoice::updateModifiersByLfo(Modifiers &modifiers) {
    auto &mainParams = mainParamList[128];
    for (int i = 0; i < NUM_LFO; ++i) {
        auto &params = mainParams.lfoParams[i];
        if (!params.enabled) {
            continue;
        }
        if (params.shouldUseFastFreqFreezed) {
            continue;
        }
        double freq = params.slowFreq;
        if (modifiers.lfoOctShift[i] != 0.0) {
            freq *= std::pow(2.0, modifiers.lfoOctShift[i]);
        }
        double lfoValue = lfos[i].step(freq, 0.0, 0.0);
        auto lfoAmount = params.amount * modifiers.lfoAmountGain[i];
        switch (params.targetType) {
            case LFO_TARGET_TYPE::OSC: {
                int targetIndex = params.targetOsc;
                switch (params.targetOscParam) {
                    case LFO_TARGET_OSC_PARAM::Vibrato: {
                        jassert(lfoValue <= 1.1);
                        auto v = lfoValue * lfoAmount * RECIPROCAL_12;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.octShift[oscIndex] += v;
                            }
                        } else {
                            modifiers.octShift[targetIndex] += v;
                        }
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::Tremolo: {
                        auto v = 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.gain[oscIndex] *= v;
                            }
                        } else {
                            modifiers.gain[targetIndex] *= v;
                        }
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::Edge: {
                        auto v = 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.edgeRatio[oscIndex] *= v;
                            }
                        } else {
                            modifiers.edgeRatio[targetIndex] *= v;
                        }
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::FM:
                    case LFO_TARGET_OSC_PARAM::AM: {
                        jassertfalse;
                        break;
                    }
                    case LFO_TARGET_OSC_PARAM::Pan: {
                        auto v = lfoValue * lfoAmount;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.panMod[oscIndex] += v;
                            }
                        } else {
                            modifiers.panMod[targetIndex] += v;
                        }
                        break;
                    }
                }
                break;
            }
            case LFO_TARGET_TYPE::Filter: {
                int targetIndex = params.targetFilter;
                switch (params.targetFilterParam) {
                    case LFO_TARGET_FILTER_PARAM::Freq: {
                        auto v = lfoValue * lfoAmount;
                        if (targetIndex == NUM_FILTER) {
                            for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                                modifiers.filterOctShift[filterIndex] += v;
                            }
                        } else {
                            modifiers.filterOctShift[targetIndex] += v;
                        }
                        break;
                    }
                    case LFO_TARGET_FILTER_PARAM::Q: {
                        auto v = 1 - ((lfoValue + 1) * 0.5 * lfoAmount);
                        if (targetIndex == NUM_FILTER) {
                            for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                                modifiers.filterQExp[filterIndex] *= v;
                            }
                        } else {
                            modifiers.filterQExp[targetIndex] *= v;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}
void GrapeVoice::updateModifiersByModEnv(Modifiers &modifiers, double sampleRate) {
    auto &mainParams = mainParamList[128];
    for (int i = 0; i < NUM_MODENV; ++i) {
        auto &params = mainParams.modEnvParams[i];
        if (!params.enabled) {
            continue;
        }
        modEnvs[i].step(sampleRate);
        auto modEnvValue = modEnvs[i].getValue();
        switch (params.targetType) {
            case MODENV_TARGET_TYPE::OSC: {
                int targetIndex = params.targetOsc;
                switch (params.targetOscParam) {
                    case MODENV_TARGET_OSC_PARAM::Freq: {
                        auto v = params.peakFreq * modEnvValue;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.octShift[oscIndex] += v;
                            }
                        } else {
                            modifiers.octShift[targetIndex] += v;
                        }
                        break;
                    }
                    case MODENV_TARGET_OSC_PARAM::Edge: {
                        auto v = modEnvValue;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.edgeRatio[oscIndex] *= v;
                            }
                        } else {
                            modifiers.edgeRatio[targetIndex] *= v;
                        }
                        break;
                    }
                    case MODENV_TARGET_OSC_PARAM::Detune: {
                        auto v = params.fadeIn ? 1 - modEnvValue : modEnvValue;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.detuneRatio[oscIndex] *= v;
                            }
                        } else {
                            modifiers.detuneRatio[targetIndex] *= v;
                        }
                        break;
                    }
                    case MODENV_TARGET_OSC_PARAM::Spread: {
                        auto v = params.fadeIn ? 1 - modEnvValue : modEnvValue;
                        if (targetIndex == NUM_OSC) {
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                modifiers.spreadRatio[oscIndex] *= v;
                            }
                        } else {
                            modifiers.spreadRatio[targetIndex] *= v;
                        }
                        break;
                    }
                }
                break;
            }
            case MODENV_TARGET_TYPE::Filter: {
                int targetIndex = params.targetFilter;
                switch (params.targetFilterParam) {
                    case MODENV_TARGET_FILTER_PARAM::Freq: {
                        auto v = params.peakFreq * modEnvValue;
                        if (targetIndex == NUM_FILTER) {
                            for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                                modifiers.filterOctShift[filterIndex] += v;
                            }
                        } else {
                            modifiers.filterOctShift[targetIndex] += v;
                        }
                        break;
                    }
                    case MODENV_TARGET_FILTER_PARAM::Q: {
                        auto v = params.fadeIn ? 1 - modEnvValue : modEnvValue;
                        if (targetIndex == NUM_FILTER) {
                            for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                                modifiers.filterQExp[filterIndex] *= v;
                            }
                        } else {
                            modifiers.filterQExp[targetIndex] *= v;
                        }
                        break;
                    }
                }
                break;
            }
            case MODENV_TARGET_TYPE::LFO: {
                int targetIndex = params.targetLfo;
                switch (params.targetLfoParam) {
                    case MODENV_TARGET_LFO_PARAM::Freq: {
                        auto v = params.peakFreq * modEnvValue;
                        if (targetIndex == NUM_LFO) {
                            for (int lfoIndex = 0; lfoIndex < NUM_LFO; ++lfoIndex) {
                                modifiers.lfoOctShift[lfoIndex] += v;
                            }
                        } else {
                            modifiers.lfoOctShift[targetIndex] += v;
                        }
                        break;
                    }
                    case MODENV_TARGET_LFO_PARAM::Amount: {
                        auto v = params.fadeIn ? 1 - modEnvValue : modEnvValue;
                        if (targetIndex == NUM_LFO) {
                            for (int lfoIndex = 0; lfoIndex < NUM_LFO; ++lfoIndex) {
                                modifiers.lfoAmountGain[lfoIndex] *= v;
                            }
                        } else {
                            modifiers.lfoAmountGain[targetIndex] *= v;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}
