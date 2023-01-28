#pragma once

#include <JuceHeader.h>

#include "Components.h"
#include "PluginProcessor.h"
#include "Voice.h"

//==============================================================================
class GrapeAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer, SectionComponent::Listener {
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
    SectionComponent oscComponents[NUM_OSC];
    SectionComponent envelopeComponents[NUM_ENVELOPE];
    SectionComponent filterComponents[NUM_FILTER];
    SectionComponent lfoComponents[NUM_LFO];
    SectionComponent modEnvComponents[NUM_MODENV];
    SectionComponent delayComponent;
    MasterComponent masterComponent;
    DrumComponent drumComponent;

    virtual void timerCallback() override;
    virtual void enabledChanged(SectionComponent *section) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrapeAudioProcessorEditor)
};
