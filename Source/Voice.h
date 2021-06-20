#pragma once

#include <JuceHeader.h>
#include "Params.h"
#include "DSP.h"

namespace {
const int NUM_OSC = 3;
const int NUM_ENVELOPE = 2;
const int NUM_FILTER = 2;
const int NUM_LFO = 3;
const int NUM_MODENV = 3;
const int NUM_CONTROL = 6;
const double A = 1.0 / 12.0;
const double X = std::pow(2.0, 1.0/12.0);
const double Y = 440.0 / std::pow(X, 69);
}

//==============================================================================
class SparseLog
{
public:
    SparseLog(int count) : count(count){}
    ~SparseLog(){}
    void log(std::string key, std::string message) {
        if (map.find(key) == map.end()) {
            map[key] = 0;
        }
        if(map[key] == count) {
            DBG(key << ": " << message);
            map[key] = 0;
        } else {
            map[key]++;
        }
    }
    void log(std::string key, double message) {
        log(key, std::to_string(message));
    }
private:
    int count;
    std::map<std::string, int> map;
};

//==============================================================================
class GrapeSound   : public juce::SynthesiserSound
{
public:
    GrapeSound();
    ~GrapeSound();
    bool appliesToNote    (int) override;
    bool appliesToChannel (int) override;
};

//==============================================================================
class MonoStack {
public:
    int latestNoteNumber = 0;
    int firstNoteNumber = 0;
    MonoStack(){};
    ~MonoStack(){};
    bool push(int noteNumber, float velocity) {
        if(noteNumber == 0 || velocity == 0.0f) {
            return false;
        }
        if(firstNoteNumber == 0) {
            firstNoteNumber = noteNumber;
        }
        remove(noteNumber);
        notes[noteNumber].velocity = velocity;
        notes[noteNumber].next = latestNoteNumber;
        latestNoteNumber = noteNumber;
        return true;
    }
    bool remove(int noteNumber) {
        if(latestNoteNumber == 0) {
            return false;
        }
        auto next = notes[noteNumber].next;
        notes[noteNumber].velocity = 0.0f;
        notes[noteNumber].next = 0;
        if(latestNoteNumber == noteNumber) {
            latestNoteNumber = next;
            if(latestNoteNumber == 0) {
                firstNoteNumber = 0;
            }
            return true;
        }
        auto currentNoteNumber = latestNoteNumber;
        while(notes[currentNoteNumber].next != 0) {
            if(notes[currentNoteNumber].next == noteNumber) {
                notes[currentNoteNumber].next = next;
                return true;
            }
            currentNoteNumber = notes[currentNoteNumber].next;
        }
        return false;
    }
    float getVelocity(int noteNumber) {
        return notes[noteNumber].velocity;
    }
    void reset() {
        std::fill_n(notes, 128, NoteInfo{});
    }
private:
    struct NoteInfo {
        float velocity = 0.0f;
        int next = 0;
    };
    NoteInfo notes[128]{};
};

//==============================================================================
// TODO: consider resetting
// TODO: multi channel
class Modifiers {
public:
    Modifiers(VoiceParams* voiceParams, ControlItemParams* controlItemParams);
    ~Modifiers() {};
    VoiceParams* voiceParams;
    ControlItemParams* controlItemParams;
    double angleShift[NUM_OSC] {};
    double octShift[NUM_OSC] {};
    double edgeRatio[NUM_OSC] {};
    double panBase[NUM_OSC] {};
    double panMod[NUM_OSC] {};
    double detuneRatio[NUM_OSC] {};
    double spreadRatio[NUM_OSC] {};
    double gain[NUM_OSC] {};
    double filterOctShift[NUM_FILTER] {};
    double filterQExp[NUM_FILTER] {};
    double lfoOctShift[NUM_LFO] {};
    double lfoAmountGain[NUM_LFO] {};
    double portamentoAmount = 0.0;
    double delayAmount = 0.0;
    double masterVolume = 1.0;
private:
    int pitch = 8192;
    int cc[127] {};
};

//==============================================================================
class GrapeVoice   : public juce::SynthesiserVoice
{
public:
    GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo,
               VoiceParams* voiceParams,
               OscParams* oscParams,
               EnvelopeParams* envelopeParams,
               FilterParams* filterParams,
               LfoParams* lfoParams,
               ModEnvParams* modEnvParams,
               Modifiers* modifiers);
    ~GrapeVoice();
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void glide(int midiNoteNumber, float velocity);
    virtual void pitchWheelMoved (int) override {};
    virtual void controllerMoved (int, int) override {};
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
private:
    juce::PerformanceCounter perf;
    juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo;
    
    VoiceParams* voiceParams;
    OscParams* oscParams;
    EnvelopeParams* envelopeParams;
    FilterParams* filterParams;
    LfoParams* lfoParams;
    ModEnvParams* modEnvParams;
    Modifiers* modifiers;
    
    MultiOsc oscs[NUM_OSC];
    Adsr adsr[NUM_ENVELOPE];
    Filter filters[NUM_FILTER];
    Osc lfos[NUM_LFO];
    Adsr modEnvs[NUM_MODENV];
    
    TransitiveValue smoothNote;
    TransitiveValue smoothVelocity;
    bool stolen = false;
    bool isActive();
    SparseLog sparseLog = SparseLog(10000);
    double getMidiNoteInHertzDouble (double noteNumber)
    {
        return 440.0 * std::pow (2.0, (noteNumber - 69) * A);
//        return Y * std::pow(X, noteNumber);// こっちの方がパフォーマンス悪かった
    }
    double shiftHertsByNotes (double herts, double notes)
    {
        return herts * std::pow (2.0, notes * A);
    }
};

//==============================================================================
class GrapeSynthesiser   : public juce::Synthesiser
{
public:
    GrapeSynthesiser(juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo, MonoStack* monoStack, ControlItemParams* controlItemParams, VoiceParams* voiceParams, OscParams* oscParams, DelayParams* delayParams) : currentPositionInfo(currentPositionInfo), monoStack(monoStack), controlItemParams(controlItemParams), voiceParams(voiceParams), oscParams(oscParams), delayParams(delayParams) {
        addSound(new GrapeSound());
    }
    ~GrapeSynthesiser() {}
    virtual void handleMidiEvent (const juce::MidiMessage& m) override {
        const int channel = m.getChannel();
        if(static_cast<VOICE_MODE>(voiceParams->Mode->getIndex()) == VOICE_MODE::Mono) {
            jassert(getNumVoices() == 1);
            if (m.isNoteOn())
            {
                auto midiNoteNumber = m.getNoteNumber();
                auto velocity = m.getFloatVelocity();
                for (auto* sound : sounds)
                {
                    if (sound->appliesToNote (midiNoteNumber) && sound->appliesToChannel (channel))
                    {
                        if(GrapeVoice* voice = dynamic_cast<GrapeVoice*>(voices[0]) ) {
                            bool shouldGlide = monoStack->latestNoteNumber != 0;
                            monoStack->push(midiNoteNumber, velocity);
                            if(shouldGlide) {
                                jassert(voices[0]->isPlayingChannel (channel));
                                voice->glide(midiNoteNumber, velocity);
                                return;
                            }
                        }
                    }
                }
            }
            else if (m.isNoteOff())
            {
                auto midiNoteNumber = m.getNoteNumber();
                for (auto* sound : sounds)
                {
                    if (sound->appliesToNote (midiNoteNumber) && sound->appliesToChannel (channel))
                    {
                        if(GrapeVoice* voice = dynamic_cast<GrapeVoice*>(voices[0]) ) {
                            int firstNoteNumber = monoStack->firstNoteNumber;
                            bool removed = monoStack->remove(midiNoteNumber);
                            jassert(removed);
                            bool shouldGlide = monoStack->latestNoteNumber != 0;
                            if(shouldGlide) {
                                jassert(voices[0]->isPlayingChannel (channel));
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
            else if (m.isAllNotesOff())
            {
                monoStack->reset();
            }
            else if (m.isAllSoundOff())
            {
                monoStack->reset();
            }
        }
        Synthesiser::handleMidiEvent(m);
        
    }
    void handleController (const int midiChannel,
                           const int controllerNumber,
                           const int controllerValue) override
    {
        DBG("handleController: " << midiChannel << ", " << controllerNumber << ", " << controllerValue);
        juce::Synthesiser::handleController(midiChannel, controllerNumber, controllerValue);
        if(getSound(0)->appliesToChannel(midiChannel)) {
            controllerMoved(controllerNumber, controllerValue);
        }
    }
    void handlePitchWheel (const int midiChannel, const int wheelValue) override
    {
        DBG("handlePitchWheel: " << midiChannel << ", " << wheelValue);
        juce::Synthesiser::handlePitchWheel(midiChannel, wheelValue);
        if(getSound(0)->appliesToChannel(midiChannel)) {
//            modifiers->pitchWheelMoved(wheelValue);
        }
    }
    void renderVoices (juce::AudioBuffer<float>& buffer, int startSample, int numSamples) override
    {
        juce::Synthesiser::renderVoices(buffer, startSample, numSamples);
        
        stereoDelay.setParams(getSampleRate(),
                              currentPositionInfo->bpm,
                              static_cast<DELAY_TYPE>(delayParams->Type->getIndex()),
                              delayParams->Sync->get(),
                              delayParams->TimeL->get(),
                              delayParams->TimeR->get(),
                              DELAY_TIME_SYNC_VALUES[delayParams->TimeSyncL->getIndex()],
                              DELAY_TIME_SYNC_VALUES[delayParams->TimeSyncR->getIndex()],
                              delayParams->LowFreq->get(),
                              delayParams->HighFreq->get(),
                              delayParams->Feedback->get(),
                              delayParams->Mix->get()/* * modifiers->delayAmount*/);
        
        auto* leftIn = buffer.getReadPointer(0, startSample);
        auto* rightIn = buffer.getReadPointer(1, startSample);
        auto* leftOut = buffer.getWritePointer(0, startSample);
        auto* rightOut = buffer.getWritePointer(1, startSample);
        
        for(int i = 0; i < numSamples; ++i)
        {
            double sample[2] { leftIn[i], rightIn[i] };
            
            // Delay
            if(delayParams->Enabled->get()) {
                stereoDelay.step(sample);
            }
            
            // Master Volume
//            sample[0] *= modifiers->masterVolume;
//            sample[1] *= modifiers->masterVolume;
            
            leftOut[i] = sample[0];
            rightOut[i] = sample[1];
        }
    }
//    void renderVoices (juce::AudioBuffer<double>& buffer, int startSample, int numSamples) override
//    {
//        juce::Synthesiser::renderVoices(buffer, startSample, numSamples);
//    }
    void controllerMoved(int number, int value) {
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
//                                    octShift[oscIndex] = 4.0 * normalizedValue;
                                    break;
                                }
                                case CONTROL_TARGET_OSC_PARAM::Edge: {
//                                    edgeRatio[oscIndex] = normalizedValue;
                                    break;
                                }
                                case CONTROL_TARGET_OSC_PARAM::Detune: {
//                                    detuneRatio[oscIndex] = normalizedValue;
                                    break;
                                }
                                case CONTROL_TARGET_OSC_PARAM::Spread: {
//                                    spreadRatio[oscIndex] = normalizedValue;
                                    break;
                                }
                                case CONTROL_TARGET_OSC_PARAM::Pan: {
//                                    panBase[oscIndex] = normalizedValue * 2 - 1.0;
                                    break;
                                }
                                case CONTROL_TARGET_OSC_PARAM::Gain: {
//                                    gain[oscIndex] = normalizedValue;
//                                    *oscParams[oscIndex].Gain = normalizedValue;
                                    *oscParams[oscIndex].Gain = oscParams[oscIndex].Gain->range.convertFrom0to1(normalizedValue);
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
//                                    filterOctShift[filterIndex] = 4.0 * normalizedValue;
                                    break;
                                }
                                case CONTROL_TARGET_FILTER_PARAM::Q: {
//                                    filterQExp[filterIndex] = normalizedValue;
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
//                                    lfoOctShift[lfoIndex] = 4.0 * normalizedValue;
                                    break;
                                }
                                case CONTROL_TARGET_LFO_PARAM::Amount: {
//                                    lfoAmountGain[lfoIndex] = normalizedValue;
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
//                            portamentoAmount = normalizedValue;
                            break;
                        }
                        case CONTROL_TARGET_MISC_PARAM::DelayAmount: {
//                            delayAmount = normalizedValue;
                            *delayParams->Mix = normalizedValue;
                            break;
                        }
                        case CONTROL_TARGET_MISC_PARAM::MasterVolume: {
//                            masterVolume = normalizedValue;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
private:
    juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo;
    
    MonoStack* monoStack;
    ControlItemParams* controlItemParams;
    
    VoiceParams* voiceParams;
    OscParams* oscParams;
    DelayParams* delayParams;
    
    StereoDelay stereoDelay;
};
