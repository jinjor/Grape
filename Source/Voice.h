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
const int NUM_CONTROL = 1;
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
// TODO: consider resetting
// TODO: multi channel
class Modifiers {
public:
    Modifiers(ControlItemParams* controlItemParams);
    ~Modifiers() {};
    ControlItemParams* controlItemParams;
    double angleShift[NUM_OSC] {};
    double octShift[NUM_OSC] {};
    double detuneRatio[NUM_OSC] {};
    double spreadRatio[NUM_OSC] {};
    //        double pan[NUM_OSC] {};
    double gain[NUM_OSC] {};
    double filterOctShift[NUM_FILTER] {};
    double filterQExp[NUM_FILTER] {};
    double lfoOctShift[NUM_LFO] {};
    double lfoAmountGain[NUM_LFO] {};
    void pitchWheelMoved (int);
    void controllerMoved (int, int);
private:
    int pitch = 8192;
    int cc[127] {};
};

//==============================================================================
class GrapeVoice   : public juce::SynthesiserVoice
{
public:
    GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo,
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
    virtual void pitchWheelMoved (int) override {};
    virtual void controllerMoved (int, int) override {};
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
private:
    juce::PerformanceCounter perf;
    juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo;
    
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
    
    int midiNoteNumber = 0;
    TransitiveValue smoothVelocity;
    bool stolen = false;
    bool isActive();
    SparseLog sparseLog = SparseLog(10000);
    double getMidiNoteInHertzDouble (double noteNumber)
    {
        return 440.0 * std::pow (2.0, (noteNumber - 69) * A);
//        return Y * std::pow(X, noteNumber);// こっちの方がパフォーマンス悪かった
    }
};

//==============================================================================
class GrapeSynthesiser   : public juce::Synthesiser
{
public:
    GrapeSynthesiser(GrapeSound* sound, Modifiers* modifiers) : sound(sound), modifiers(modifiers) {}
    ~GrapeSynthesiser() {}
    void handleController (const int midiChannel,
                                        const int controllerNumber,
                                        const int controllerValue) override
    {
        DBG("handleController: " << midiChannel << ", " << controllerNumber << ", " << controllerValue);
        juce::Synthesiser::handleController(midiChannel, controllerNumber, controllerValue);
        if(sound->appliesToChannel(midiChannel)) {
            modifiers->controllerMoved(controllerNumber, controllerValue);
        }
    }
    void handlePitchWheel (const int midiChannel, const int wheelValue) override
    {
        DBG("handlePitchWheel: " << midiChannel << ", " << wheelValue);
        juce::Synthesiser::handlePitchWheel(midiChannel, wheelValue);
        if(sound->appliesToChannel(midiChannel)) {
            modifiers->pitchWheelMoved(wheelValue);
        }
    }
private:
    GrapeSound* sound;
    Modifiers* modifiers;
};
