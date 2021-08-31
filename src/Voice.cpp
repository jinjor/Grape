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
                       std::array<OscParams, NUM_OSC> &oscParams,
                       std::array<EnvelopeParams, NUM_ENVELOPE> &envelopeParams,
                       std::array<FilterParams, NUM_FILTER> &filterParams,
                       std::array<LfoParams, NUM_LFO> &lfoParams,
                       std::array<ModEnvParams, NUM_MODENV> &modEnvParams)
    : perf(juce::PerformanceCounter("voice cycle", 100000)),
      currentPositionInfo(currentPositionInfo),
      globalParams(globalParams),
      voiceParams(voiceParams),
      oscParams(oscParams),
      envelopeParams(envelopeParams),
      filterParams(filterParams),
      lfoParams(lfoParams),
      modEnvParams(modEnvParams),
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
        smoothNote.init(midiNoteNumber);
        if (stolen) {
            smoothVelocity.exponential(0.01, velocity, sampleRate);
        } else {
            smoothVelocity.init(velocity);
        }
        stolen = false;

        for (int i = 0; i < NUM_OSC; ++i) {
            if (!stolen) {
                //                oscs[i].setAngle(0.0);
            }
        }
        for (int i = 0; i < NUM_ENVELOPE; ++i) {
            adsr[i].setParams(envelopeParams[i].Attack->get(),
                              0.0,
                              envelopeParams[i].Decay->get(),
                              envelopeParams[i].Sustain->get(),
                              envelopeParams[i].Release->get());
            adsr[i].doAttack(sampleRate);
        }
        for (int i = 0; i < NUM_FILTER; ++i) {
            filters[i].initializePastData();
        }
        for (int i = 0; i < NUM_LFO; ++i) {
            lfos[i].setNormalizedAngle(0.0);
        }
        for (int i = 0; i < NUM_MODENV; ++i) {
            if (modEnvParams[i].shouldUseHold()) {
                modEnvs[i].setParams(0.0, modEnvParams[i].Wait->get(), modEnvParams[i].Decay->get(), 0.0, 0.0);
            } else {
                modEnvs[i].setParams(modEnvParams[i].Attack->get(), 0.0, modEnvParams[i].Decay->get(), 0.0, 0.0);
            }
            modEnvs[i].doAttack(sampleRate);
        }
    }
}
void GrapeVoice::stopNote(float velocity, bool allowTailOff) {
    DBG("stopNote() allowTailOff:" << std::to_string(allowTailOff));
    if (GrapeSound *playingSound = dynamic_cast<GrapeSound *>(getCurrentlyPlayingSound().get())) {
        if (allowTailOff) {
            auto sampleRate = getSampleRate();
            for (int i = 0; i < NUM_ENVELOPE; ++i) {
                adsr[i].doRelease(sampleRate);
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
    auto portamentTime = voiceParams.PortamentoTime->get();
    smoothNote.exponential(portamentTime, midiNoteNumber, sampleRate);
    smoothVelocity.exponential(portamentTime, velocity, sampleRate);
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
    // TODO: move to renderVoices()
    globalParams.freeze();
    voiceParams.freeze();
    for (int i = 0; i < NUM_OSC; ++i) {
        oscParams[i].freeze();
    }
    for (int i = 0; i < NUM_FILTER; ++i) {
        filterParams[i].freeze();
    }
    for (int i = 0; i < NUM_LFO; ++i) {
        lfoParams[i].freeze();
    }
    for (int i = 0; i < NUM_MODENV; ++i) {
        modEnvParams[i].freeze();
    }

    for (int i = 0; i < NUM_OSC; ++i) {
        oscs[i].setSampleRate(sampleRate);
        oscs[i].setWaveform(OSC_WAVEFORM_VALUES[oscParams[i].Waveform->getIndex()]);
    }
    for (int i = 0; i < NUM_ENVELOPE; ++i) {
        adsr[i].setParams(envelopeParams[i].Attack->get(),
                          0.0,
                          envelopeParams[i].Decay->get(),
                          envelopeParams[i].Sustain->get(),
                          envelopeParams[i].Release->get());
    }
    for (int i = 0; i < NUM_FILTER; ++i) {
        filters[i].setSampleRate(sampleRate);
    }
    for (int i = 0; i < NUM_LFO; ++i) {
        lfos[i].setSampleRate(sampleRate);
        lfos[i].setWaveform(LFO_WAVEFORM_VALUES[lfoParams[i].Waveform->getIndex()]);
    }
    for (int i = 0; i < NUM_MODENV; ++i) {
        if (modEnvParams[i].shouldUseHold()) {
            modEnvs[i].setParams(0.0, modEnvParams[i].Wait->get(), modEnvParams[i].Decay->get(), 0.0, 0.0);
        } else {
            modEnvs[i].setParams(envelopeParams[i].Attack->get(), 0.0, modEnvParams[i].Decay->get(), 0.0, 0.0);
        }
    }
}
bool GrapeVoice::step(double *out, double sampleRate, int numChannels) {
    smoothNote.step();
    smoothVelocity.step();

    double midiNoteNumber = smoothNote.value + globalParams.pitch * voiceParams.pitchBendRange;

    double shiftedNoteNumbers[NUM_OSC]{midiNoteNumber, midiNoteNumber, midiNoteNumber};
    for (int i = 0; i < NUM_OSC; ++i) {
        if (!oscParams[i].enabled) {
            continue;
        }
        shiftedNoteNumbers[i] += oscParams[i].octave * 12 + oscParams[i].coarse;
    }
    auto modifiers = Modifiers{};

    for (int i = 0; i < NUM_ENVELOPE; ++i) {
        adsr[i].step(sampleRate);
    }

    // ---------------- MODENV ----------------
    for (int i = 0; i < NUM_MODENV; ++i) {
        auto &params = modEnvParams[i];
        if (!params.enabled) {
            continue;
        }
        modEnvs[i].step(sampleRate);
        auto modEnvValue = modEnvs[i].getValue();
        auto targetType = static_cast<MODENV_TARGET_TYPE>(params.targetType);
        switch (targetType) {
            case MODENV_TARGET_TYPE::OSC: {
                int targetIndex = params.targetOsc;
                auto targetParam = static_cast<MODENV_TARGET_OSC_PARAM>(params.targetOscParam);
                switch (targetParam) {
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
                        auto isFadeIn = static_cast<MODENV_FADE>(params.fade) == MODENV_FADE::In;
                        auto v = isFadeIn ? 1 - modEnvValue : modEnvValue;
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
                        auto isFadeIn = static_cast<MODENV_FADE>(params.fade) == MODENV_FADE::In;
                        auto v = isFadeIn ? 1 - modEnvValue : modEnvValue;
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
                auto targetParam = static_cast<MODENV_TARGET_FILTER_PARAM>(params.targetFilterParam);
                switch (targetParam) {
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
                        auto isFadeIn = static_cast<MODENV_FADE>(params.fade) == MODENV_FADE::In;
                        auto v = isFadeIn ? 1 - modEnvValue : modEnvValue;
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
                auto targetParam = static_cast<MODENV_TARGET_LFO_PARAM>(params.targetLfoParam);
                switch (targetParam) {
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
                        auto isFadeIn = static_cast<MODENV_FADE>(params.fade) == MODENV_FADE::In;
                        auto v = isFadeIn ? 1 - modEnvValue : modEnvValue;
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
    // ---------------- LFO ----------------
    for (int i = 0; i < NUM_LFO; ++i) {
        auto &params = lfoParams[i];
        if (!params.enabled) {
            continue;
        }
        double lfoValue;
        double freq;
        if (params.shouldUseFastFreqFreezed) {
            int target = params.targetOsc;
            auto shiftedNoteNumber = target == NUM_OSC ? midiNoteNumber : shiftedNoteNumbers[target];
            shiftedNoteNumber += modifiers.lfoOctShift[i];
            freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
            freq *= params.fastFreq;
        } else {
            freq = params.slowFreq;
            if (modifiers.lfoOctShift[i] != 0.0) {
                freq *= std::pow(2.0, modifiers.lfoOctShift[i]);
            }
        }
        lfoValue = lfos[i].step(freq, 0.0, 0.0);
        auto lfoAmount = params.amount * modifiers.lfoAmountGain[i];
        auto targetType = static_cast<LFO_TARGET_TYPE>(params.targetType);
        switch (targetType) {
            case LFO_TARGET_TYPE::OSC: {
                int targetIndex = params.targetOsc;
                auto param = static_cast<LFO_TARGET_OSC_PARAM>(params.targetOscParam);
                switch (param) {
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
                auto param = static_cast<LFO_TARGET_FILTER_PARAM>(params.targetFilterParam);
                switch (param) {
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
    bool active = false;
    auto panBase = globalParams.pan;
    auto panModAmp = std::min(1.0 - panBase, 1.0 + panBase);
    // ---------------- OSC with Envelope and Filter ----------------
    for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
        auto &p = oscParams[oscIndex];
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
            auto &fp = filterParams[filterIndex];
            if (!fp.enabled) {
                continue;
            }
            if (fp.target == oscIndex) {
                auto filterType = static_cast<FILTER_TYPE>(fp.type);
                double freq;
                switch (static_cast<FILTER_FREQ_TYPE>(fp.freqType)) {
                    case FILTER_FREQ_TYPE::Absolute: {
                        double noteShift = modifiers.filterOctShift[filterIndex] * 12;
                        freq = shiftHertsByNotes(fp.hz, noteShift);
                        break;
                    }
                    case FILTER_FREQ_TYPE::Relative: {
                        double shiftedNoteNumber = shiftedNoteNumbers[oscIndex];
                        shiftedNoteNumber += fp.semitone;
                        shiftedNoteNumber += modifiers.filterOctShift[filterIndex] * 12;
                        freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                        break;
                    }
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
        auto &fp = filterParams[filterIndex];
        if (!fp.enabled) {
            continue;
        }
        if (fp.target == NUM_OSC) {  // All
            auto filterType = static_cast<FILTER_TYPE>(fp.type);
            double freq;
            switch (static_cast<FILTER_FREQ_TYPE>(fp.freqType)) {
                case FILTER_FREQ_TYPE::Absolute: {
                    double noteShift = modifiers.filterOctShift[filterIndex] * 12;
                    freq = shiftHertsByNotes(fp.hz, noteShift);
                    break;
                }
                case FILTER_FREQ_TYPE::Relative: {
                    double shiftedNoteNumber = midiNoteNumber;
                    shiftedNoteNumber += fp.semitone;
                    shiftedNoteNumber += modifiers.filterOctShift[filterIndex] * 12;
                    freq = getMidiNoteInHertzDouble(shiftedNoteNumber);
                    break;
                }
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
