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
      utilComponent(),
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
      masterComponent(p.globalParams),
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

    auto upperArea = bounds.removeFromTop(height * 0.12).reduced(6, 2);
    {
        auto upperLeftArea = upperArea.removeFromLeft(width * 0.36);
        auto upperRightArea = upperArea.removeFromRight(width * 0.36);
        {
            auto area = upperLeftArea.removeFromLeft(width * 0.24);
            voiceComponent.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            auto area = upperLeftArea;
            analyserToggle.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            auto area = upperArea;
            analyserWindow.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            auto area = upperRightArea.removeFromLeft(width * 0.18);
            statusComponent.setBounds(area.reduced(PANEL_MARGIN));
        }
        {
            auto area = upperRightArea;
            utilComponent.setBounds(area.reduced(PANEL_MARGIN));
        }
    }

    auto middleArea = bounds.removeFromTop(bounds.getHeight() * 2 / 5).reduced(6, 2);
    auto middleHeight = middleArea.getHeight();
    auto lowerArea = bounds.reduced(6, 2);

    auto leftArea = middleArea.removeFromLeft(width * 0.35);
    auto centreArea = middleArea.removeFromLeft(width * 0.35);
    auto rightArea = middleArea;
    {
        auto leftUpperArea = leftArea.removeFromTop(middleHeight / 2);
        auto upperArea = leftUpperArea.removeFromTop(leftUpperArea.getHeight() * 0.5);
        auto lowerArea = leftUpperArea;

        envelopeComponents[0].setBounds(upperArea.reduced(PANEL_MARGIN));
        envelopeComponents[1].setBounds(lowerArea.reduced(PANEL_MARGIN));
    }
    {
        auto area = leftArea;
        oscComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = centreArea.removeFromTop(middleHeight / 2);
        oscComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = centreArea;
        oscComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = rightArea.removeFromTop(middleHeight / 2);
        filterComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = rightArea;
        filterComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto lfoArea = lowerArea.removeFromLeft(width * 0.31);
    auto lfoHeight = lfoArea.getHeight();
    {
        auto area = lfoArea.removeFromTop(lfoHeight / 3);
        lfoComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = lfoArea.removeFromTop(lfoHeight / 3);
        lfoComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = lfoArea.removeFromTop(lfoHeight / 3);
        lfoComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto modEnvArea = lowerArea.removeFromLeft(width * 0.31);
    auto modEnvHeight = modEnvArea.getHeight();
    {
        auto area = modEnvArea.removeFromTop(modEnvHeight / 3);
        modEnvComponents[0].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = modEnvArea.removeFromTop(modEnvHeight / 3);
        modEnvComponents[1].setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = modEnvArea.removeFromTop(modEnvHeight / 3);
        modEnvComponents[2].setBounds(area.reduced(PANEL_MARGIN));
    }
    auto effectArea = lowerArea;
    auto effectHeight = effectArea.getHeight();
    {
        auto area = effectArea.removeFromTop(effectHeight / 3);
        delayComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto area = effectArea.removeFromTop(effectHeight * 2 / 9);
        masterComponent.setBounds(area.reduced(PANEL_MARGIN));
    }
    {
        auto controlArea = effectArea;
        controlComponent.setBounds(controlArea.reduced(PANEL_MARGIN));

        auto drumArea = effectArea.removeFromTop(effectHeight * 2 / 9);
        drumComponent.setBounds(drumArea.reduced(PANEL_MARGIN));
    }
}

void GrapeAudioProcessorEditor::timerCallback() {
    auto isDrumMode = audioProcessor.voiceParams.isDrumMode();
    drumComponent.setVisible(isDrumMode);
    controlComponent.setVisible(!isDrumMode);
}