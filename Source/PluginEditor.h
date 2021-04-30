#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Voice.h"
#include "Components.h"

//==============================================================================
/**
*/
class GrapeAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    GrapeAudioProcessorEditor (GrapeAudioProcessor&);
    ~GrapeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GrapeAudioProcessor& audioProcessor;
    juce::MidiKeyboardComponent keyboardComponent;
    OscComponent oscComponents[NUM_OSC];
    EnvelopeComponent envelopeComponents[NUM_ENVELOPE];
    FilterComponent filterComponents[NUM_FILTER];
    LfoComponent lfoComponents[NUM_LFO];
    ModEnvComponent modEnvComponents[NUM_MODENV];
    DelayComponent delayComponent;
    ControlComponent controlComponent;
    AnalyserComponent analyserComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrapeAudioProcessorEditor)
    
    void timerCallback() override;
};
