#include "PluginEditor.h"

#include "Params.h"
#include "PluginProcessor.h"
#include "StyleConstants.h"
#include "Voice.h"

using namespace styles;

//==============================================================================
GrapeAudioProcessorEditor::GrapeAudioProcessorEditor(GrapeAudioProcessor &p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      controlComponent{ControlComponent(p.controlItemParams)},
      voiceComponent(p.voiceParams, p.mainParamList, p.controlItemParams),
      analyserToggle(&analyserMode),
      analyserWindow(&analyserMode, &p.latestDataProvider, &p.monoStack, p.voiceParams, p.mainParamList),
      statusComponent(&p.polyphony, &p.timeConsumptionState, &p.latestDataProvider),
      utilComponent(p),
      oscComponents{OscComponent(0, p.voiceParams, p.mainParamList, p.controlItemParams),
                    OscComponent(1, p.voiceParams, p.mainParamList, p.controlItemParams),
                    OscComponent(2, p.voiceParams, p.mainParamList, p.controlItemParams)},
      envelopeComponents{EnvelopeComponent(0, p.voiceParams, p.mainParamList),
                         EnvelopeComponent(1, p.voiceParams, p.mainParamList)},
      filterComponents{FilterComponent(0, p.voiceParams, p.mainParamList, p.controlItemParams),
                       FilterComponent(1, p.voiceParams, p.mainParamList, p.controlItemParams)},
      lfoComponents{LfoComponent(0, p.voiceParams, p.mainParamList, p.controlItemParams),
                    LfoComponent(1, p.voiceParams, p.mainParamList, p.controlItemParams),
                    LfoComponent(2, p.voiceParams, p.mainParamList, p.controlItemParams)},
      modEnvComponents{ModEnvComponent(0, p.voiceParams, p.mainParamList),
                       ModEnvComponent(1, p.voiceParams, p.mainParamList),
                       ModEnvComponent(2, p.voiceParams, p.mainParamList)},
      delayComponent{DelayComponent(p.voiceParams, p.mainParamList, p.controlItemParams)},
      masterComponent{MasterComponent(p.voiceParams, p.mainParamList)},
      drumComponent{DrumComponent(p.voiceParams, p.mainParamList)} {
    getLookAndFeel().setColour(juce::Label::textColourId, colour::TEXT);

    addAndMakeVisible(voiceComponent);
    addAndMakeVisible(analyserToggle);
    addAndMakeVisible(analyserWindow);
    addAndMakeVisible(statusComponent);
    addAndMakeVisible(utilComponent);
    addAndMakeVisible(oscComponents[0]);
    addAndMakeVisible(oscComponents[1]);
    addAndMakeVisible(oscComponents[2]);
    addAndMakeVisible(envelopeComponents[0]);
    addAndMakeVisible(envelopeComponents[1]);
    addAndMakeVisible(filterComponents[0]);
    addAndMakeVisible(filterComponents[1]);
    addAndMakeVisible(lfoComponents[0]);
    addAndMakeVisible(lfoComponents[1]);
    addAndMakeVisible(lfoComponents[2]);
    addAndMakeVisible(modEnvComponents[0]);
    addAndMakeVisible(modEnvComponents[1]);
    addAndMakeVisible(modEnvComponents[2]);
    addAndMakeVisible(delayComponent);
    addAndMakeVisible(masterComponent);
    addAndMakeVisible(drumComponent);
    addAndMakeVisible(controlComponent);
    setSize(1024, 768);
#if JUCE_DEBUG
    setResizable(true, true);  // for debug
#endif
    startTimerHz(30.0f);
}

GrapeAudioProcessorEditor::~GrapeAudioProcessorEditor() {}

//==============================================================================
void GrapeAudioProcessorEditor::paint(juce::Graphics &g) {
    auto bounds = getLocalBounds();
    auto height = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(height * 0.12);
    auto middleArea = bounds.removeFromTop(bounds.getHeight() * 2 / 5);

    g.fillAll(colour::BACKGROUND);

    auto areas = std::array{upperArea, middleArea};
    for (auto &area : areas) {
        {
            juce::Path p;
            p.addLineSegment(juce::Line<float>(0, area.getBottom() - 0.5, area.getWidth(), area.getBottom() - 0.5), 0);
            g.setColour(juce::Colour(10, 10, 10));
            g.strokePath(p, juce::PathStrokeType(1));
        }
        {
            juce::Path p;
            p.addLineSegment(juce::Line<float>(0, area.getBottom() + 0.5, area.getWidth(), area.getBottom() + 0.5), 0);
            g.setColour(juce::Colour(60, 60, 60));
            g.strokePath(p, juce::PathStrokeType(1));
        }
    }
}

void GrapeAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();

    auto width = bounds.getWidth();
    auto height = bounds.getHeight();

    auto upperHeight = height * 0.12;
    auto middleHeight = (height - upperHeight) * 2 / 5;
    {
        auto upperArea = bounds.removeFromTop(upperHeight).reduced(AREA_PADDING_X, AREA_PADDING_Y);
        auto sideWidth = width * 0.36;
        auto centreWidth = width - sideWidth * 2;

        auto voiceWidth = sideWidth * 2 / 3;
        auto analyserToggleWidth = sideWidth * 1 / 3;
        auto statusWidth = sideWidth * 0.5;

        voiceComponent.setBounds(upperArea.removeFromLeft(voiceWidth));
        analyserToggle.setBounds(upperArea.removeFromLeft(analyserToggleWidth).reduced(3));
        analyserWindow.setBounds(upperArea.removeFromLeft(centreWidth).reduced(3));
        statusComponent.setBounds(upperArea.removeFromLeft(statusWidth));
        utilComponent.setBounds(upperArea);
    }
    {
        auto middleArea = bounds.removeFromTop(middleHeight).reduced(AREA_PADDING_X, AREA_PADDING_Y);
        auto leftWidth = (width - PANEL_MARGIN_X * 2) * 0.35;
        auto centreWidth = (width - PANEL_MARGIN_X * 2) * 0.35;

        auto middleHeight = middleArea.getHeight();
        auto halfPanelHeight = (middleHeight - PANEL_MARGIN_Y) * 0.5;
        auto quarterPanelHeight = (halfPanelHeight - PANEL_MARGIN_Y) * 0.5;
        {
            auto area = middleArea.removeFromLeft(leftWidth);
            envelopeComponents[0].setBounds(area.removeFromTop(quarterPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            envelopeComponents[1].setBounds(area.removeFromTop(quarterPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            oscComponents[0].setBounds(area);
        }
        middleArea.removeFromLeft(PANEL_MARGIN_X);
        {
            auto area = middleArea.removeFromLeft(centreWidth);
            oscComponents[1].setBounds(area.removeFromTop(halfPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            oscComponents[2].setBounds(area);
        }
        middleArea.removeFromLeft(PANEL_MARGIN_X);
        {
            auto &area = middleArea;
            filterComponents[0].setBounds(area.removeFromTop(halfPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            filterComponents[1].setBounds(area);
        }
    }
    {
        auto lowerArea = bounds.reduced(AREA_PADDING_X, AREA_PADDING_Y);
        auto leftWidth = (width - PANEL_MARGIN_X * 2) * 0.31;
        auto centreWidth = (width - PANEL_MARGIN_X * 2) * 0.31;

        auto lowerHeight = lowerArea.getHeight();
        auto lfoPanelHeight = (lowerHeight - PANEL_MARGIN_Y * 2) / 3;
        auto modEnvPanelHeight = (lowerHeight - PANEL_MARGIN_Y * 2) / 3;
        auto delayPanelHeight = (lowerHeight - PANEL_MARGIN_Y * 2) / 3;
        auto masterPanelHeight = (lowerHeight - delayPanelHeight - PANEL_MARGIN_Y) * 1 / 3;
        auto drumPanelHeight = (lowerHeight - delayPanelHeight - PANEL_MARGIN_Y) * 1 / 3;
        {
            auto area = lowerArea.removeFromLeft(leftWidth);
            lfoComponents[0].setBounds(area.removeFromTop(lfoPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            lfoComponents[1].setBounds(area.removeFromTop(lfoPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            lfoComponents[2].setBounds(area.removeFromTop(lfoPanelHeight));
        }
        lowerArea.removeFromLeft(PANEL_MARGIN_X);
        {
            auto area = lowerArea.removeFromLeft(leftWidth);
            modEnvComponents[0].setBounds(area.removeFromTop(modEnvPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            modEnvComponents[1].setBounds(area.removeFromTop(modEnvPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            modEnvComponents[2].setBounds(area.removeFromTop(modEnvPanelHeight));
        }
        lowerArea.removeFromLeft(PANEL_MARGIN_X);
        {
            auto &area = lowerArea;
            delayComponent.setBounds(area.removeFromTop(delayPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            masterComponent.setBounds(area.removeFromTop(masterPanelHeight));
            area.removeFromTop(PANEL_MARGIN_Y);
            controlComponent.setBounds(area);
            drumComponent.setBounds(area.removeFromTop(drumPanelHeight));
        }
    }
}

void GrapeAudioProcessorEditor::timerCallback() {
    auto isDrumMode = audioProcessor.voiceParams.isDrumMode();
    drumComponent.setVisible(isDrumMode);
    controlComponent.setVisible(!isDrumMode);
}