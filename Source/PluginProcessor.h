#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "Params.h"

//==============================================================================
class TimeConsumptionState
{
public:
    float currentTimeConsumptionRate = 0.0f;
    
    TimeConsumptionState() {};
    ~TimeConsumptionState() {};
    void push(double sampleRate, int numSamples, double seconds) {
        if(currentSampleRate != sampleRate) {
            totalNumSamples = 0;
            totalSeconds = 0.0f;
        }
        currentSampleRate = sampleRate;
        totalNumSamples += numSamples;
        totalSeconds += seconds;
        if(totalNumSamples > currentSampleRate) {
            auto timeLimitPerSample = 1.0 / currentSampleRate;
            currentTimeConsumptionRate = totalSeconds / totalNumSamples / timeLimitPerSample;
            totalNumSamples = 0;
            totalSeconds = 0.0f;
        }
    }
private:
    double currentSampleRate = 0.0;
    int totalNumSamples = 0;
    double totalSeconds = 0.0;
};

//==============================================================================
class LatestDataProvider
{
public:
    class Consumer {
    public:
        float* destinationL;
        float* destinationR;
        int numSamples = 0;
        bool ready = false;
    };
    enum { numSamples = 2048 };
    std::vector<Consumer*> consumers;
    
    LatestDataProvider() {
        std::cout << "consumers.size(): " << consumers.size() << std::endl;
    };
    ~LatestDataProvider() {};
    void addConsumer(Consumer* c) {
        std::lock_guard<std::mutex> lock(mtx);
        jassert(c->numSamples <= numSamples);
        consumers.push_back(c);
    }
    void push(juce::AudioBuffer<float>& buffer) {
        if (buffer.getNumChannels() <= 0) {
            return;
        }
        std::lock_guard<std::mutex> lock(mtx);
        auto* dataL = buffer.getReadPointer(0);
        auto* dataR = buffer.getReadPointer(1);
        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            fifoL[fifoIndex] = dataL[i];
            fifoR[fifoIndex] = dataR[i];
            fifoIndex++;
            for(auto* consumer : consumers) {
                if(!consumer->ready && fifoIndex >= consumer->numSamples) {
                    memcpy(consumer->destinationL, fifoL + fifoIndex - consumer->numSamples, sizeof(float) * consumer->numSamples);
                    memcpy(consumer->destinationR, fifoR + fifoIndex - consumer->numSamples, sizeof(float) * consumer->numSamples);
                    consumer->ready = true;
                }
            }
            if(fifoIndex >= numSamples) {
                fifoIndex = 0;
            }
        }
    }
private:
    float fifoL[numSamples]{};
    float fifoR[numSamples]{};
    int fifoIndex = 0;
    std::mutex mtx;
};

//==============================================================================
class GrapeAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GrapeAudioProcessor();
    ~GrapeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    int currentProgram = 0;
    juce::MidiKeyboardState keyboardState;
    LatestDataProvider latestDataProvider;
    int polyphony = 0;
    TimeConsumptionState timeConsumptionState;
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    GlobalParams globalParams;
    VoiceParams voiceParams;
    OscParams oscParams[NUM_OSC];
    EnvelopeParams envelopeParams[NUM_ENVELOPE];
    FilterParams filterParams[NUM_FILTER];
    LfoParams lfoParams[NUM_LFO];
    ModEnvParams modEnvParams[NUM_MODENV];
    DelayParams delayParams;
    ControlItemParams controlItemParams[NUM_CONTROL];
    
    MonoStack monoStack;
    GrapeSynthesiser synth;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrapeAudioProcessor)
};
