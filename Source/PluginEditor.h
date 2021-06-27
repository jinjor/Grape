#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Voice.h"
#include "Components.h"

//==============================================================================
class GrapeAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer, juce::KeyListener
{
public:
    GrapeAudioProcessorEditor (GrapeAudioProcessor&);
    ~GrapeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GrapeAudioProcessor& audioProcessor;
    ANALYSER_MODE analyserMode = ANALYSER_MODE::Envelope;
    
    ControlComponent controlComponent;
    VoiceComponent voiceComponent;
    AnalyserToggle analyserToggle;
    AnalyserWindow analyserWindow;
    StatusComponent statusComponent;
    MasterComponent masterComponent;
    OscComponent oscComponents[NUM_OSC];
    EnvelopeComponent envelopeComponents[NUM_ENVELOPE];
    FilterComponent filterComponents[NUM_FILTER];
    LfoComponent lfoComponents[NUM_LFO];
    ModEnvComponent modEnvComponents[NUM_MODENV];
    DelayComponent delayComponent;
    
    bool benchmarking = false;
    int benchmarkCounter = 0;
    
    void timerCallback() override;
    bool keyPressed(const juce::KeyPress& press, juce::Component* originatingComponent) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrapeAudioProcessorEditor)
};
