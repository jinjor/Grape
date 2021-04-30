#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"

namespace {
const float KEY_HEIGHT = 40.0f;
const juce::Colour BACKGROUND_COLOUR = juce::Colour(40,40,40);
const juce::Colour TEXT_COLOUR = juce::Colour(200,200,200);
const int PANEL_MARGIN = 2;
}

//==============================================================================
GrapeAudioProcessorEditor::GrapeAudioProcessorEditor (GrapeAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
, oscComponents { OscComponent(0, p.oscParams), OscComponent(1, p.oscParams+1), OscComponent(2, p.oscParams+2) }
, envelopeComponents { EnvelopeComponent(0, p.envelopeParams), EnvelopeComponent(1, p.envelopeParams+1) }
, filterComponents { FilterComponent(0, p.filterParams), FilterComponent(1, p.filterParams+1) }
, lfoComponents { LfoComponent(0, p.lfoParams), LfoComponent(1, p.lfoParams+1), LfoComponent(2, p.lfoParams+2) }
, modEnvComponents { ModEnvComponent(0, p.modEnvParams), ModEnvComponent(1, p.modEnvParams+1), ModEnvComponent(2, p.modEnvParams+2) }
, delayComponent { DelayComponent(&p.delayParams) }
, controlComponent { ControlComponent(p.controlItemParams) }
, analyserComponent (&p.analyserState)
{
    getLookAndFeel().setColour(juce::Label::textColourId, TEXT_COLOUR);
    
    addAndMakeVisible (keyboardComponent);
    addAndMakeVisible (oscComponents[0]);
    addAndMakeVisible (oscComponents[1]);
    addAndMakeVisible (oscComponents[2]);
    addAndMakeVisible (envelopeComponents[0]);
    addAndMakeVisible (envelopeComponents[1]);
    addAndMakeVisible (filterComponents[0]);
    addAndMakeVisible (filterComponents[1]);
    addAndMakeVisible (lfoComponents[0]);
    addAndMakeVisible (lfoComponents[1]);
    addAndMakeVisible (lfoComponents[2]);
    addAndMakeVisible (modEnvComponents[0]);
    addAndMakeVisible (modEnvComponents[1]);
    addAndMakeVisible (modEnvComponents[2]);
    addAndMakeVisible (delayComponent);
    addAndMakeVisible (controlComponent);
    addAndMakeVisible (analyserComponent);
    setSize (1024, 768);
    startTimer (400);
    setResizable(true, true);
}

GrapeAudioProcessorEditor::~GrapeAudioProcessorEditor()
{
}

//==============================================================================
void GrapeAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(BACKGROUND_COLOUR);
}

void GrapeAudioProcessorEditor::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
        
//    keyboardComponent.setBounds(bounds.removeFromBottom(KEY_HEIGHT));
    bounds.reduce(5, 5);
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    
    auto upperArea = bounds.removeFromTop(height * 0.5);
    auto upperHeight = upperArea.getHeight();
    auto leftArea = upperArea.removeFromLeft(width * 0.33);
    auto rightArea = upperArea.removeFromRight(width * 0.33);
    {
        juce::Rectangle<int> area = upperArea.removeFromTop(upperHeight / 3);
        oscComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromTop(upperHeight / 3);
        oscComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromTop(upperHeight / 3);
        oscComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = leftArea.removeFromTop(upperHeight / 3);
        envelopeComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = leftArea.removeFromTop(upperHeight / 3);
        envelopeComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = rightArea.removeFromTop(upperHeight / 3);
        filterComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = rightArea.removeFromTop(upperHeight / 3);
        filterComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto lfoArea = bounds.removeFromLeft(width * 0.33);
    auto lfoHeight = lfoArea.getHeight();
    {
        juce::Rectangle<int> area = lfoArea.removeFromTop(lfoHeight / 3);
        lfoComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lfoArea.removeFromTop(lfoHeight / 3);
        lfoComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lfoArea.removeFromTop(lfoHeight / 3);
        lfoComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto modEnvArea = bounds.removeFromLeft(width * 0.33);
    auto modEnvHeight = modEnvArea.getHeight();
    {
        juce::Rectangle<int> area = modEnvArea.removeFromTop(modEnvHeight / 3);
        modEnvComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = modEnvArea.removeFromTop(modEnvHeight / 3);
        modEnvComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = modEnvArea.removeFromTop(modEnvHeight / 3);
        modEnvComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto effectArea = bounds;
    auto effectHeight = effectArea.getHeight();
    {
        juce::Rectangle<int> area = effectArea.removeFromTop(effectHeight / 3);
        delayComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = effectArea;
        controlComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
//    {
//        juce::Rectangle<int> area = effectArea;
//        analyserComponent.setBounds(area.reduced(PANEL_MARGIN));
//    }
    
}
void GrapeAudioProcessorEditor::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}
