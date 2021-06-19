#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"

namespace {
const juce::Colour BACKGROUND_COLOUR = juce::Colour(40,40,40);
const juce::Colour TEXT_COLOUR = juce::Colour(200,200,200);
const int PANEL_MARGIN = 2;
}

//==============================================================================
GrapeAudioProcessorEditor::GrapeAudioProcessorEditor (GrapeAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
, voiceComponent (&p.voiceParams)
, statusComponent (&p.polyphony, &p.timeConsumptionState, &p.latestDataProvider)
, oscComponents { OscComponent(0, p.oscParams), OscComponent(1, p.oscParams+1), OscComponent(2, p.oscParams+2) }
, envelopeComponents { EnvelopeComponent(0, p.envelopeParams), EnvelopeComponent(1, p.envelopeParams+1) }
, filterComponents { FilterComponent(0, p.filterParams), FilterComponent(1, p.filterParams+1) }
, lfoComponents { LfoComponent(0, p.lfoParams), LfoComponent(1, p.lfoParams+1), LfoComponent(2, p.lfoParams+2) }
, modEnvComponents { ModEnvComponent(0, p.modEnvParams), ModEnvComponent(1, p.modEnvParams+1), ModEnvComponent(2, p.modEnvParams+2) }
, delayComponent { DelayComponent(&p.delayParams) }
, controlComponent { ControlComponent(p.controlItemParams) }
, analyserComponent (&p.latestDataProvider)
{
    getLookAndFeel().setColour(juce::Label::textColourId, TEXT_COLOUR);
    
    addAndMakeVisible (keyboardComponent);
    addAndMakeVisible (voiceComponent);
    addAndMakeVisible (statusComponent);
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
//    setResizable(true, true);
}

GrapeAudioProcessorEditor::~GrapeAudioProcessorEditor()
{
}

//==============================================================================
void GrapeAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto height = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(height * 0.12);
    auto middleArea = bounds.removeFromTop(bounds.getHeight() * 2 / 5);
    
    g.fillAll(BACKGROUND_COLOUR);
    juce::Path p;
    p.addLineSegment(juce::Line<float>(0, upperArea.getBottom() - 0.5, upperArea.getWidth(), upperArea.getBottom() - 0.5), 1.0);
    p.addLineSegment(juce::Line<float>(0, middleArea.getBottom() - 0.5, middleArea.getWidth(), middleArea.getBottom() - 0.5), 1.0);
    g.setColour (juce::Colour(20,20,20));
    g.strokePath(p, juce::PathStrokeType(0.5));
}

void GrapeAudioProcessorEditor::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
        
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    
    auto upperArea = bounds.removeFromTop(height * 0.12).reduced(6, 2);
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width/3);
        voiceComponent.setBounds(area);
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width/3);
        analyserComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea;
        statusComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
    
    auto middleArea = bounds.removeFromTop(bounds.getHeight() * 2 / 5).reduced(6, 2);
    auto middleHeight = middleArea.getHeight();
    auto lowerArea = bounds.reduced(6, 2);
    
    auto leftArea = middleArea.removeFromLeft(width * 0.35);
    auto centreArea = middleArea.removeFromLeft(width * 0.35);
    auto rightArea = middleArea;
    {
        juce::Rectangle<int> leftUpperArea = leftArea.removeFromTop(middleHeight / 2);
        
        auto env0Area = leftUpperArea.removeFromLeft(leftUpperArea.getWidth() / 2);
        auto env1Area = leftUpperArea;
        envelopeComponents[0].setBounds(env0Area.reduced(PANEL_MARGIN));
        envelopeComponents[1].setBounds(env1Area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = leftArea;
        oscComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = centreArea.removeFromTop(middleHeight / 2);
        oscComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = centreArea;
        oscComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = rightArea.removeFromTop(middleHeight / 2);
        filterComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = rightArea;
        filterComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto lfoArea = lowerArea.removeFromLeft(width * 0.31);
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
    auto modEnvArea = lowerArea.removeFromLeft(width * 0.31);
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
    auto effectArea = lowerArea;
    auto effectHeight = effectArea.getHeight();
    {
        juce::Rectangle<int> area = effectArea.removeFromTop(effectHeight / 3);
        delayComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        juce::Rectangle<int> area = effectArea;
        controlComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
}
void GrapeAudioProcessorEditor::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}
