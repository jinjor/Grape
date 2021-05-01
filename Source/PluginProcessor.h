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
    AnalyserState();
    ~AnalyserState();
    void pushFFTData(juce::AudioBuffer<float>&);
    float fifo [fftSize];
    int fifoIndex = 0;
    void pushNextSampleIntoFifo (float sample) noexcept;
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
    AnalyserState analyserState;
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    OscParams oscParams[NUM_OSC];
    EnvelopeParams envelopeParams[NUM_ENVELOPE];
    FilterParams filterParams[NUM_FILTER];
    LfoParams lfoParams[NUM_LFO];
    ModEnvParams modEnvParams[NUM_MODENV];
    DelayParams delayParams;
    ControlItemParams controlItemParams[NUM_CONTROL];
    
    StereoDelay stereoDelay;
    
    Modifiers modifiers;
    GrapeSound sound;
    GrapeSynthesiser synth;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrapeAudioProcessor)
};
