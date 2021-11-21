#pragma once

#include <JuceHeader.h>

#include "Components.h"
#include "PluginProcessor.h"
#include "Voice.h"

//==============================================================================
class GrapeAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
    GrapeAudioProcessorEditor(GrapeAudioProcessor &);
    ~GrapeAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    GrapeAudioProcessor &audioProcessor;
    ANALYSER_MODE analyserMode = ANALYSER_MODE::Spectrum;

    ControlComponent controlComponent;
    VoiceComponent voiceComponent;
    AnalyserToggle analyserToggle;
    AnalyserWindow analyserWindow;
    StatusComponent statusComponent;
    UtilComponent utilComponent;
    OscComponent oscComponents[NUM_OSC];
    EnvelopeComponent envelopeComponents[NUM_ENVELOPE];
    FilterComponent filterComponents[NUM_FILTER];
    LfoComponent lfoComponents[NUM_LFO];
    ModEnvComponent modEnvComponents[NUM_MODENV];
    DelayComponent delayComponent;
    MasterComponent masterComponent;
    DrumComponent drumComponent;

    virtual void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrapeAudioProcessorEditor)
};
