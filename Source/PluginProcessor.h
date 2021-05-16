#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "Params.h"

//==============================================================================
class AnalyserState
{
public:
    enum {
        fftOrder  = 11,
        fftSize   = 1 << fftOrder
    };
    bool nextFFTBlockReady = false;
    float fftData [2 * fftSize];
    AnalyserState() {};
    ~AnalyserState() {};
    float fifo [fftSize];
    int fifoIndex = 0;
    void pushFFTData(juce::AudioBuffer<float>& buffer) {
        if (buffer.getNumChannels() <= 0) {
            return;
        }
        auto* channelData = buffer.getReadPointer(0);// TODO: R
        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            pushNextSampleIntoFifo(channelData[i]);
        }
    }
    void pushNextSampleIntoFifo (float sample) noexcept
    {
        if (fifoIndex == fftSize)
        {
            if (!nextFFTBlockReady)
            {
                juce::zeromem (fftData, sizeof (fftData));
                memcpy (fftData, fifo, sizeof (fifo));
                nextFFTBlockReady = true;
            }
            fifoIndex = 0;
        }
        fifo[fifoIndex++] = sample;
    }
};

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
class LevelState
{
public:
    double currentLevel[2]{};
    
    LevelState() {};
    ~LevelState() {};
    void push(juce::AudioBuffer<float>& buffer) {
        if (buffer.getNumChannels() <= 0) {
            return;
        }
        int index;
        for(auto ch = 0; ch < 2; ch++) {
            index = lastValueIndex;
            auto* data = buffer.getReadPointer(ch);
            for (auto i = 0; i < buffer.getNumSamples(); ++i) {
                lastValues[ch][index] = data[i];
                index++;
                if(index >= 1024) {
                    index = 0;
                }
            }
            currentLevel[ch] = 0.0;
            for(auto i = 0; i < 1024; ++i) {
                // TODO: how to calc
                currentLevel[ch] = std::max(currentLevel[ch], std::abs(lastValues[ch][i]));
            }
        }
        lastValueIndex = index;
    }
private:
    double lastValues[2][1024]{};
    int lastValueIndex = 0;
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
    LevelState levelState;
    AnalyserState analyserState;
    int polyphony = 0;
    TimeConsumptionState timeConsumptionState;
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    VoiceParams voiceParams;
    OscParams oscParams[NUM_OSC];
    EnvelopeParams envelopeParams[NUM_ENVELOPE];
    FilterParams filterParams[NUM_FILTER];
    LfoParams lfoParams[NUM_LFO];
    ModEnvParams modEnvParams[NUM_MODENV];
    DelayParams delayParams;
    ControlItemParams controlItemParams[NUM_CONTROL];
    
    MonoStack monoStack;
    Modifiers modifiers;
    GrapeSound sound;
    GrapeSynthesiser synth;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrapeAudioProcessor)
};
