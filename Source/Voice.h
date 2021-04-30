#pragma once

#include <JuceHeader.h>
#include "Params.h"
#include "DSP.h"

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
class GrapeVoice   : public juce::SynthesiserVoice
{
public:
    GrapeVoice(juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo, OscParams* oscParams, EnvelopeParams* envelopeParams, FilterParams* filterParams, LfoParams* lfoParams, ModEnvParams* modEnvParams);
    ~GrapeVoice();
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    virtual void pitchWheelMoved (int) override;
    virtual void controllerMoved (int, int) override;
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
private:
    juce::PerformanceCounter perf;
    juce::AudioPlayHead::CurrentPositionInfo* currentPositionInfo;
    
    OscParams* oscParams;
    EnvelopeParams* envelopeParams;
    FilterParams* filterParams;
    LfoParams* lfoParams;
    ModEnvParams* modEnvParams;
    MultiOsc oscs[NUM_OSC];
    Adsr adsr[NUM_ENVELOPE];
    Filter filters[NUM_FILTER];
    Osc lfos[NUM_LFO];
    Adsr modEnvs[NUM_MODENV];
    
    int pitch = 0;
    int cc[128]{};
    
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
