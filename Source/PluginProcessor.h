#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "Params.h"

//==============================================================================
//class AnalyserState
//{
//public:
//    enum {
//        fftOrder  = 11,
//        fftSize   = 1 << fftOrder
//    };
//    bool nextFFTBlockReady = false;
//    float fftData [2 * fftSize];
//    AnalyserState() {};
//    ~AnalyserState() {};
//    void pushFFTData(juce::AudioBuffer<float>& buffer) {
//        if (buffer.getNumChannels() <= 0) {
//            return;
//        }
//        auto* channelData = buffer.getReadPointer(0);// TODO: R
//        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
//            pushNextSampleIntoFifo(channelData[i]);
//        }
//    }
//private:
//    float fifo [fftSize];
//    int fifoIndex = 0;
//    void pushNextSampleIntoFifo (float sample) noexcept
//    {
//        if (fifoIndex == fftSize)
//        {
//            if (!nextFFTBlockReady)
//            {
//                juce::zeromem (fftData, sizeof (fftData));
//                memcpy (fftData, fifo, sizeof (fifo));
//                nextFFTBlockReady = true;
//            }
//            fifoIndex = 0;
//        }
//        fifo[fifoIndex++] = sample;
//    }
//};

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
//class LevelState
//{
//public:
//    enum { numSamples = 4096 };
//    double levelData[2 * numSamples]{};
//    bool nextBlockReady = false;
//
//    LevelState() {};
//    ~LevelState() {};
//    void push(juce::AudioBuffer<float>& buffer) {
//        if (buffer.getNumChannels() <= 0) {
//            return;
//        }
//        int index;
//        bool _nextBlockReady;
//        for(auto ch = 0; ch < 2; ch++) {
//            index = fifoIndex;
//            _nextBlockReady = nextBlockReady;
//            auto* data = buffer.getReadPointer(ch);
//            for (auto i = 0; i < buffer.getNumSamples(); ++i) {
//                fifo[numSamples * ch + index] = data[i];
//                index++;
//                if(index >= numSamples) {
//                    if(!nextBlockReady) {
//                        memcpy(levelData + numSamples * ch, fifo + numSamples * ch, (sizeof (fifo)) / 2);
//                        _nextBlockReady = true;
//                    }
//                    index = 0;
//                }
//            }
//        }
//        fifoIndex = index;
//        nextBlockReady = _nextBlockReady;
//    }
//private:
//    double fifo[2 * numSamples]{};
//    int fifoIndex = 0;
//};

//==============================================================================
class LatestDataProvider
{
public:
    class Consumer {
    public:
        float* destinationL;
        float* destinationR;
        int numSamples;
        bool ready;
    };
    enum { numSamples = 4096 };
    std::vector<Consumer*> consumers;
    
    LatestDataProvider() {};
    ~LatestDataProvider() {};
    void addConsumer(Consumer* c) {
        consumers.push_back(c);
    }
    void push(juce::AudioBuffer<float>& buffer) {
        if (buffer.getNumChannels() <= 0) {
            return;
        }
        auto* dataL = buffer.getReadPointer(0);
        auto* dataR = buffer.getReadPointer(1);
        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            fifoL[fifoIndex] = dataL[i];
            fifoR[fifoIndex] = dataR[i];
            fifoIndex++;
            if(fifoIndex >= numSamples) {
                for(auto* consumer : consumers) {
                    if(!consumer->ready) {
                        memcpy(consumer->destinationL, fifoL, sizeof(float) * consumer->numSamples);
                        memcpy(consumer->destinationR, fifoR, sizeof(float) * consumer->numSamples);
                        consumer->ready = true;
                    }
                }
                fifoIndex = 0;
            }
        }
    }
private:
    float fifoL[numSamples]{};
    float fifoR[numSamples]{};
    int fifoIndex = 0;
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
//    LevelState levelState;
    LatestDataProvider latestDataProvider;
    
//    AnalyserState analyserState;
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
    GrapeSynthesiser synth;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrapeAudioProcessor)
};
