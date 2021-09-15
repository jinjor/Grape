#pragma once

#include <JuceHeader.h>

#include "DSP.h"
#include "Params.h"

namespace {
const int NUM_OSC = 3;
const int NUM_ENVELOPE = 2;
const int NUM_FILTER = 2;
const int NUM_LFO = 3;
const int NUM_MODENV = 3;
const int NUM_CONTROL = 6;
const double A = 1.0 / 12.0;
const double X = std::pow(2.0, 1.0 / 12.0);
const double Y = 440.0 / std::pow(X, 69);
const int CONTROL_INTERVAL = 16;
const double CONTROL_RATE = 1.0 / CONTROL_INTERVAL;

void freezeParams(GlobalParams &globalParams,
                  VoiceParams &voiceParams,
                  std::array<OscParams, NUM_OSC> &oscParams,
                  std::array<EnvelopeParams, NUM_ENVELOPE> &envelopeParams,
                  std::array<FilterParams, NUM_FILTER> &filterParams,
                  std::array<LfoParams, NUM_LFO> &lfoParams,
                  std::array<ModEnvParams, NUM_MODENV> &modEnvParams,
                  DelayParams &delayParams,
                  std::array<ControlItemParams, NUM_CONTROL> &controlItemParams) {
    globalParams.freeze();
    voiceParams.freeze();
    for (int i = 0; i < NUM_OSC; ++i) {
        oscParams[i].freeze();
    }
    for (int i = 0; i < NUM_ENVELOPE; ++i) {
        envelopeParams[i].freeze();
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
    delayParams.freeze();
    for (int i = 0; i < NUM_CONTROL; ++i) {
        controlItemParams[i].freeze();
    }
}
}  // namespace

//==============================================================================
class SparseLog {
public:
    SparseLog(int count) : count(count) {}
    ~SparseLog() {}
    void log(std::string &key, std::string &message) {
        if (map.find(key) == map.end()) {
            map[key] = 0;
        }
        if (map[key] == count) {
            DBG(key << ": " << message);
            map[key] = 0;
        } else {
            map[key]++;
        }
    }
    void log(std::string &key, double message) {
        auto s = std::to_string(message);
        log(key, s);
    }

private:
    int count;
    std::map<std::string, int> map;
};

//==============================================================================
class GrapeSound : public juce::SynthesiserSound {
public:
    GrapeSound();
    ~GrapeSound();
    bool appliesToNote(int) override;
    bool appliesToChannel(int) override;
};

//==============================================================================
class MonoStack {
public:
    int latestNoteNumber = 0;
    int firstNoteNumber = 0;
    MonoStack(){};
    ~MonoStack(){};
    bool push(int noteNumber, float velocity) {
        if (noteNumber == 0 || velocity == 0.0f) {
            return false;
        }
        if (firstNoteNumber == 0) {
            firstNoteNumber = noteNumber;
        }
        remove(noteNumber);
        notes[noteNumber].velocity = velocity;
        notes[noteNumber].next = latestNoteNumber;
        latestNoteNumber = noteNumber;
        return true;
    }
    bool remove(int noteNumber) {
        if (latestNoteNumber == 0) {
            return false;
        }
        auto next = notes[noteNumber].next;
        notes[noteNumber].velocity = 0.0f;
        notes[noteNumber].next = 0;
        if (latestNoteNumber == noteNumber) {
            latestNoteNumber = next;
            if (latestNoteNumber == 0) {
                firstNoteNumber = 0;
            }
            return true;
        }
        auto currentNoteNumber = latestNoteNumber;
        while (notes[currentNoteNumber].next != 0) {
            if (notes[currentNoteNumber].next == noteNumber) {
                notes[currentNoteNumber].next = next;
                return true;
            }
            currentNoteNumber = notes[currentNoteNumber].next;
        }
        return false;
    }
    float getVelocity(int noteNumber) { return notes[noteNumber].velocity; }
    void reset() { std::fill_n(notes, 128, NoteInfo{}); }

private:
    struct NoteInfo {
        float velocity = 0.0f;
        int next = 0;
    };
    NoteInfo notes[128]{};
};

//==============================================================================
struct Modifiers {
    double normalizedAngleShift[NUM_OSC]{0.0, 0.0, 0.0};
    double octShift[NUM_OSC]{0.0, 0.0, 0.0};
    double edgeRatio[NUM_OSC]{1.0, 1.0, 1.0};
    double panMod[NUM_OSC]{0.0, 0.0, 0.0};
    double detuneRatio[NUM_OSC]{1.0, 1.0, 1.0};
    double spreadRatio[NUM_OSC]{1.0, 1.0, 1.0};
    double gain[NUM_OSC]{1.0, 1.0, 1.0};
    double filterOctShift[NUM_FILTER]{0.0, 0.0};
    double filterQExp[NUM_FILTER]{1.0, 1.0};
    double lfoOctShift[NUM_LFO]{0.0, 0.0, 0.0};
    double lfoAmountGain[NUM_LFO]{1.0, 1.0, 1.0};
};

//==============================================================================
class GrapeVoice : public juce::SynthesiserVoice {
public:
    GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo *currentPositionInfo,
               GlobalParams &globalParams,
               VoiceParams &voiceParams,
               std::array<OscParams, NUM_OSC> &oscParams,
               std::array<EnvelopeParams, NUM_ENVELOPE> &envelopeParams,
               std::array<FilterParams, NUM_FILTER> &filterParams,
               std::array<LfoParams, NUM_LFO> &lfoParams,
               std::array<ModEnvParams, NUM_MODENV> &modEnvParams);
    ~GrapeVoice();
    bool canPlaySound(juce::SynthesiserSound *sound) override;
    void startNote(int midiNoteNumber,
                   float velocity,
                   juce::SynthesiserSound *,
                   int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void glide(int midiNoteNumber, float velocity);
    virtual void pitchWheelMoved(int) override{};
    virtual void controllerMoved(int, int) override{};
    void renderNextBlock(juce::AudioSampleBuffer &outputBuffer, int startSample, int numSamples) override;
    void applyParamsBeforeLoop(double sampleRate);
    bool step(double *out, double sampleRate, int numChannels);

private:
    juce::PerformanceCounter perf;
    juce::AudioPlayHead::CurrentPositionInfo *currentPositionInfo;

    GlobalParams &globalParams;
    VoiceParams &voiceParams;
    std::array<OscParams, NUM_OSC> &oscParams;
    std::array<EnvelopeParams, NUM_ENVELOPE> &envelopeParams;
    std::array<FilterParams, NUM_FILTER> &filterParams;
    std::array<LfoParams, NUM_LFO> &lfoParams;
    std::array<ModEnvParams, NUM_MODENV> &modEnvParams;

    MultiOsc oscs[NUM_OSC];
    Adsr adsr[NUM_ENVELOPE];
    Filter filters[NUM_FILTER];
    Osc lfos[NUM_LFO];
    Adsr modEnvs[NUM_MODENV];

    TransitiveValue smoothNote;
    TransitiveValue smoothVelocity;
    bool stolen = false;
    int stepCounter = 0;
    Modifiers controlModifiers = Modifiers{};
    SparseLog sparseLog = SparseLog(10000);
    double getMidiNoteInHertzDouble(double noteNumber) {
        return 440.0 * std::pow(2.0, (noteNumber - 69) * A);
        //        return Y * std::pow(X, noteNumber);// こっちの方がパフォーマンス悪かった
    }
    double shiftHertsByNotes(double herts, double notes) { return herts * std::pow(2.0, notes * A); }
    void updateModifiersByLfo(Modifiers &modifiers);
    void updateModifiersByModEnv(Modifiers &modifiers, double sampleRate);
};

//==============================================================================
class GrapeSynthesiser : public juce::Synthesiser {
public:
    GrapeSynthesiser(juce::AudioPlayHead::CurrentPositionInfo *currentPositionInfo,
                     MonoStack *monoStack,
                     std::array<ControlItemParams, NUM_CONTROL> &controlItemParams,
                     GlobalParams &globalParams,
                     VoiceParams &voiceParams,
                     std::array<OscParams, NUM_OSC> &oscParams,
                     std::array<EnvelopeParams, NUM_ENVELOPE> &envelopeParams,
                     std::array<FilterParams, NUM_FILTER> &filterParams,
                     std::array<LfoParams, NUM_LFO> &lfoParams,
                     std::array<ModEnvParams, NUM_MODENV> &modEnvParams,
                     DelayParams &delayParams)
        : currentPositionInfo(currentPositionInfo),
          monoStack(monoStack),
          controlItemParams(controlItemParams),
          globalParams(globalParams),
          voiceParams(voiceParams),
          oscParams(oscParams),
          envelopeParams(envelopeParams),
          filterParams(filterParams),
          lfoParams(lfoParams),
          modEnvParams(modEnvParams),
          delayParams(delayParams) {
        addSound(new GrapeSound());
    }
    ~GrapeSynthesiser() {}
    virtual void renderNextBlock(AudioBuffer<float> &outputAudio,
                                 const MidiBuffer &inputMidi,
                                 int startSample,
                                 int numSamples) {
        freezeParams(globalParams,
                     voiceParams,
                     oscParams,
                     envelopeParams,
                     filterParams,
                     lfoParams,
                     modEnvParams,
                     delayParams,
                     controlItemParams);
        juce::Synthesiser::renderNextBlock(outputAudio, inputMidi, startSample, numSamples);
    }
    virtual void handleMidiEvent(const juce::MidiMessage &m) override {
        const int channel = m.getChannel();
        if (m.isNoteOn()) {
            auto midiNoteNumber = m.getNoteNumber();
            auto velocity = m.getFloatVelocity();
            for (auto *sound : sounds) {
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(channel)) {
                    if (GrapeVoice *voice = dynamic_cast<GrapeVoice *>(voices[0])) {
                        bool playingNotesExist = monoStack->latestNoteNumber != 0;
                        monoStack->push(midiNoteNumber, velocity);
                        if (voiceParams.isMonoModeFreezed) {
                            jassert(getNumVoices() == 1);
                            if (voice->isPlayingChannel(channel) && playingNotesExist) {
                                voice->glide(midiNoteNumber, velocity);
                                return;
                            }
                        }
                    }
                }
            }
        } else if (m.isNoteOff()) {
            auto midiNoteNumber = m.getNoteNumber();
            for (auto *sound : sounds) {
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(channel)) {
                    if (GrapeVoice *voice = dynamic_cast<GrapeVoice *>(voices[0])) {
                        int firstNoteNumber = monoStack->firstNoteNumber;
                        bool removed = monoStack->remove(midiNoteNumber);
                        if (!removed) {
                            return;
                        }
                        bool playingNotesExist = monoStack->latestNoteNumber != 0;
                        if (voiceParams.isMonoModeFreezed) {
                            jassert(getNumVoices() == 1);
                            if (voice->isPlayingChannel(channel) && playingNotesExist) {
                                auto velocity = monoStack->getVelocity(monoStack->latestNoteNumber);
                                jassert(velocity != 0);
                                voice->glide(monoStack->latestNoteNumber, velocity);
                                return;
                            }
                            noteOff(channel, firstNoteNumber, m.getFloatVelocity(), true);
                            return;
                        }
                    }
                }
            }
        } else if (m.isAllNotesOff()) {
            monoStack->reset();
        } else if (m.isAllSoundOff()) {
            monoStack->reset();
        }
        Synthesiser::handleMidiEvent(m);
    }
    void handleController(const int midiChannel, const int controllerNumber, const int controllerValue) override {
        DBG("handleController: " << midiChannel << ", " << controllerNumber << ", " << controllerValue);
        juce::Synthesiser::handleController(midiChannel, controllerNumber, controllerValue);
        if (getSound(0)->appliesToChannel(midiChannel)) {
            controllerMoved(controllerNumber, controllerValue);
        }
    }
    void handlePitchWheel(const int midiChannel, const int wheelValue) override {
        DBG("handlePitchWheel: " << midiChannel << ", " << wheelValue);
        juce::Synthesiser::handlePitchWheel(midiChannel, wheelValue);
        if (getSound(0)->appliesToChannel(midiChannel)) {
            pitchWheelMoved(wheelValue);
        }
    }
    void renderVoices(juce::AudioBuffer<float> &buffer, int startSample, int numSamples) override {
        juce::Synthesiser::renderVoices(buffer, startSample, numSamples);

        stereoDelay.setParams(getSampleRate(),
                              currentPositionInfo->bpm,
                              delayParams.type,
                              delayParams.sync,
                              delayParams.timeL,
                              delayParams.timeR,
                              delayParams.timeSyncL,
                              delayParams.timeSyncR,
                              delayParams.lowFreq,
                              delayParams.highFreq,
                              delayParams.feedback,
                              delayParams.mix);

        auto *leftIn = buffer.getReadPointer(0, startSample);
        auto *rightIn = buffer.getReadPointer(1, startSample);
        auto *leftOut = buffer.getWritePointer(0, startSample);
        auto *rightOut = buffer.getWritePointer(1, startSample);

        auto delayEnabled = delayParams.enabled;
        auto expression = globalParams.expression;
        auto masterVolume = globalParams.masterVolume * globalParams.midiVolume;
        for (int i = 0; i < numSamples; ++i) {
            double sample[2]{leftIn[i] * expression, rightIn[i] * expression};

            // Delay
            if (delayEnabled) {
                stereoDelay.step(sample);
            }

            // Master Volume
            sample[0] *= masterVolume;
            sample[1] *= masterVolume;

            leftOut[i] = sample[0];
            rightOut[i] = sample[1];
        }
    }
    void controllerMoved(int number, int value) {
        auto normalizedValue = value / 127.0;

        // predefined
        switch (number) {
            case 7:
                globalParams.setMidiVolumeFromControl(normalizedValue);
                globalParams.freeze();
                break;
            case 10:
                globalParams.setPanFromControl(normalizedValue);
                globalParams.freeze();
                break;
            case 11:
                globalParams.setExpressionFromControl(normalizedValue);
                globalParams.freeze();
                break;
            default: {
                // custom
                for (int i = 0; i < NUM_CONTROL; ++i) {
                    auto &params = controlItemParams[i];
                    if (number != params.number) {
                        continue;
                    }
                    switch (params.targetType) {
                        case CONTROL_TARGET_TYPE::OSC: {
                            int targetIndex = params.targetOsc;
                            auto targetParam = params.targetOscParam;
                            for (int oscIndex = 0; oscIndex < NUM_OSC; ++oscIndex) {
                                if (targetIndex == oscIndex) {
                                    auto &params = oscParams[oscIndex];
                                    switch (targetParam) {
                                        case CONTROL_TARGET_OSC_PARAM::Edge: {
                                            params.setEdgeFromControl(normalizedValue);
                                            params.freeze();
                                            break;
                                        }
                                        case CONTROL_TARGET_OSC_PARAM::Detune: {
                                            params.setDetuneFromControl(normalizedValue);
                                            params.freeze();
                                            break;
                                        }
                                        case CONTROL_TARGET_OSC_PARAM::Spread: {
                                            params.setSpreadFromControl(normalizedValue);
                                            params.freeze();
                                            break;
                                        }
                                        case CONTROL_TARGET_OSC_PARAM::Gain: {
                                            params.setGainFromControl(normalizedValue);
                                            params.freeze();
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case CONTROL_TARGET_TYPE::Filter: {
                            int targetIndex = params.targetFilter;
                            auto targetParam = params.targetFilterParam;
                            for (int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                                if (targetIndex == filterIndex) {
                                    auto &params = filterParams[filterIndex];
                                    switch (targetParam) {
                                        case CONTROL_TARGET_FILTER_PARAM::Freq: {
                                            if (params.isFreqAbsoluteFreezed) {
                                                params.setHzFromControl(normalizedValue);
                                                params.freeze();
                                            } else {
                                                params.setSemitoneFromControl(normalizedValue);
                                                params.freeze();
                                            }
                                            break;
                                        }
                                        case CONTROL_TARGET_FILTER_PARAM::Q: {
                                            params.setQFromControl(normalizedValue);
                                            params.freeze();
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case CONTROL_TARGET_TYPE::LFO: {
                            int targetIndex = params.targetLfo;
                            auto targetParam = params.targetLfoParam;
                            for (int lfoIndex = 0; lfoIndex < NUM_MODENV; ++lfoIndex) {
                                if (targetIndex == lfoIndex) {
                                    auto &params = lfoParams[lfoIndex];
                                    switch (targetParam) {
                                        case CONTROL_TARGET_LFO_PARAM::Freq: {
                                            if (params.shouldUseFastFreqFreezed) {
                                                params.setFastFreqFromControl(normalizedValue);
                                                params.freeze();
                                            } else {
                                                params.setSlowFreqFromControl(normalizedValue);
                                                params.freeze();
                                            }
                                            break;
                                        }
                                        case CONTROL_TARGET_LFO_PARAM::Amount: {
                                            params.setAmountFromControl(normalizedValue);
                                            params.freeze();
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case CONTROL_TARGET_TYPE::Master: {
                            switch (params.targetMiscParam) {
                                case CONTROL_TARGET_MISC_PARAM::PortamentoTime: {
                                    voiceParams.setPortamentoTimeFromControl(normalizedValue);
                                    voiceParams.freeze();
                                    break;
                                }
                                case CONTROL_TARGET_MISC_PARAM::DelayMix: {
                                    delayParams.setMixFromControl(normalizedValue);
                                    delayParams.freeze();
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    void pitchWheelMoved(int value) {
        value -= 8192;
        *globalParams.Pitch = value >= 0 ? value / 8191.0 : value / 8192.0;
        globalParams.freeze();
    }

private:
    juce::AudioPlayHead::CurrentPositionInfo *currentPositionInfo;

    MonoStack *monoStack;
    std::array<ControlItemParams, NUM_CONTROL> &controlItemParams;

    GlobalParams &globalParams;
    VoiceParams &voiceParams;
    std::array<OscParams, NUM_OSC> &oscParams;
    std::array<EnvelopeParams, NUM_ENVELOPE> &envelopeParams;
    std::array<FilterParams, NUM_FILTER> &filterParams;
    std::array<LfoParams, NUM_LFO> &lfoParams;
    std::array<ModEnvParams, NUM_LFO> &modEnvParams;
    DelayParams &delayParams;

    StereoDelay stereoDelay;
};
