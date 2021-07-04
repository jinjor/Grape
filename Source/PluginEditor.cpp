#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"
#include "Params.h"

//==============================================================================
GrapeAudioProcessorEditor::GrapeAudioProcessorEditor (GrapeAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, controlComponent { ControlComponent(p.controlItemParams) }
, voiceComponent (&p.voiceParams, p.controlItemParams)
, analyserToggle(&analyserMode)
, analyserWindow(&analyserMode, &p.latestDataProvider, &p.monoStack, p.envelopeParams, p.oscParams, p.filterParams, p.modEnvParams)
, statusComponent (&p.polyphony, &p.timeConsumptionState, &p.latestDataProvider)
, masterComponent (&p.globalParams)
, oscComponents { OscComponent(0, p.oscParams, p.controlItemParams), OscComponent(1, p.oscParams+1, p.controlItemParams), OscComponent(2, p.oscParams+2, p.controlItemParams) }
, envelopeComponents { EnvelopeComponent(0, p.envelopeParams), EnvelopeComponent(1, p.envelopeParams+1) }
, filterComponents { FilterComponent(0, p.filterParams, p.controlItemParams), FilterComponent(1, p.filterParams+1, p.controlItemParams) }
, lfoComponents { LfoComponent(0, p.lfoParams, p.controlItemParams), LfoComponent(1, p.lfoParams+1, p.controlItemParams), LfoComponent(2, p.lfoParams+2, p.controlItemParams) }
, modEnvComponents { ModEnvComponent(0, p.modEnvParams), ModEnvComponent(1, p.modEnvParams+1), ModEnvComponent(2, p.modEnvParams+2) }
, delayComponent { DelayComponent(&p.delayParams, p.controlItemParams) }
{
    getLookAndFeel().setColour(juce::Label::textColourId, TEXT_COLOUR);
    
    addAndMakeVisible (voiceComponent);
    addAndMakeVisible (analyserToggle);
    addAndMakeVisible (analyserWindow);
    addAndMakeVisible (statusComponent);
    addAndMakeVisible (masterComponent);
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
    setSize (1024, 768);
    startTimer (60);
    addKeyListener(this);
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
        auto upperLeftArea = upperArea.removeFromLeft(width * 0.36);
        auto upperRightArea = upperArea.removeFromRight(width * 0.36);
        {
            juce::Rectangle<int> area = upperLeftArea.removeFromLeft(width * 0.24);
            voiceComponent.setBounds(area);
        }
        {
            juce::Rectangle<int> area = upperLeftArea;
            analyserToggle.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            juce::Rectangle<int> area = upperArea;
            analyserWindow.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            juce::Rectangle<int> area = upperRightArea.removeFromLeft(width * 0.18);
            statusComponent.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            juce::Rectangle<int> area = upperRightArea;
            masterComponent.setBounds(area.reduced(PANEL_MARGIN));
        }
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
    if(benchmarking) {
        if(0 <= benchmarkCounter && benchmarkCounter <= 127) {
            audioProcessor.keyboardState.noteOn(1, benchmarkCounter, 0.2);
        }
        if(0 <= benchmarkCounter - 16 && benchmarkCounter - 16 <= 127) {
            audioProcessor.keyboardState.noteOff(1, benchmarkCounter - 16, 0.2);
        }
        if(benchmarkCounter >= 160) {
            benchmarking = false;
            benchmarkCounter = 0;
        } else {
            benchmarkCounter++;
        }
    }
}
bool GrapeAudioProcessorEditor::keyPressed(const juce::KeyPress& press, juce::Component* originatingComponent)
{
//#if JUCE_DEBUG
    std::cout << "pressed: " << press.getKeyCode() << std::endl;
    if (press.isKeyCode('P') || press.isKeyCode('Q'))
    {
        if(benchmarking) {
            benchmarking = false;
        } else {
            if(press.isKeyCode('P')) {
                *audioProcessor.oscParams[0].Enabled = true;
                *audioProcessor.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
                *audioProcessor.oscParams[1].Enabled = true;
                *audioProcessor.oscParams[1].Waveform = OSC_WAVEFORM_NAMES.indexOf("Square");
                *audioProcessor.oscParams[1].Edge = 0.5;
                *audioProcessor.oscParams[2].Enabled = true;
                *audioProcessor.oscParams[2].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
                *audioProcessor.oscParams[2].Unison = 4;
                *audioProcessor.filterParams[0].Enabled = true;
                *audioProcessor.filterParams[1].Enabled = true;
                *audioProcessor.filterParams[1].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
                *audioProcessor.lfoParams[0].Enabled = true;
                *audioProcessor.lfoParams[1].Enabled = true;
                *audioProcessor.lfoParams[1].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("FM");
                *audioProcessor.lfoParams[2].Enabled = true;
                *audioProcessor.lfoParams[2].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("Filter");
                *audioProcessor.lfoParams[2].TargetFilterParam = LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
                *audioProcessor.modEnvParams[0].Enabled = true;
                *audioProcessor.modEnvParams[0].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("OSC");
                *audioProcessor.modEnvParams[0].TargetOscParam = MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq");
                *audioProcessor.modEnvParams[1].Enabled = true;
                *audioProcessor.modEnvParams[1].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("Filter");
                *audioProcessor.modEnvParams[1].TargetFilterParam = MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
                *audioProcessor.modEnvParams[2].Enabled = true;
                *audioProcessor.modEnvParams[2].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("LFO");
                *audioProcessor.modEnvParams[2].TargetLfoParam = MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Freq");
                *audioProcessor.delayParams.Enabled = true;
            }
            benchmarking = true;
        }
        return true;
    }
//#endif
    return false;
}
