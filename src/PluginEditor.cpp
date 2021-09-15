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
      voiceComponent(p.voiceParams, p.controlItemParams),
      analyserToggle(&analyserMode),
      analyserWindow(&analyserMode,
                     &p.latestDataProvider,
                     &p.monoStack,
                     p.envelopeParams,
                     p.oscParams,
                     p.filterParams,
                     p.modEnvParams),
      statusComponent(&p.polyphony, &p.timeConsumptionState, &p.latestDataProvider),
      masterComponent(p.globalParams),
      oscComponents{OscComponent(0, p.oscParams[0], p.controlItemParams),
                    OscComponent(1, p.oscParams[1], p.controlItemParams),
                    OscComponent(2, p.oscParams[2], p.controlItemParams)},
      envelopeComponents{EnvelopeComponent(0, p.envelopeParams[0]), EnvelopeComponent(1, p.envelopeParams[1])},
      filterComponents{FilterComponent(0, p.filterParams[0], p.controlItemParams),
                       FilterComponent(1, p.filterParams[1], p.controlItemParams)},
      lfoComponents{LfoComponent(0, p.lfoParams[0], p.controlItemParams),
                    LfoComponent(1, p.lfoParams[1], p.controlItemParams),
                    LfoComponent(2, p.lfoParams[2], p.controlItemParams)},
      modEnvComponents{ModEnvComponent(0, p.modEnvParams[0]),
                       ModEnvComponent(1, p.modEnvParams[1]),
                       ModEnvComponent(2, p.modEnvParams[2])},
      delayComponent{DelayComponent(p.delayParams, p.controlItemParams)} {
    getLookAndFeel().setColour(juce::Label::textColourId, colour::TEXT);

    addAndMakeVisible(voiceComponent);
    addAndMakeVisible(analyserToggle);
    addAndMakeVisible(analyserWindow);
    addAndMakeVisible(statusComponent);
    addAndMakeVisible(masterComponent);
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
    addAndMakeVisible(controlComponent);
    setSize(1024, 768);
#if JUCE_DEBUG
    setResizable(true, true);  // for debug
#endif
}

GrapeAudioProcessorEditor::~GrapeAudioProcessorEditor() {}

//==============================================================================
void GrapeAudioProcessorEditor::paint(juce::Graphics &g) {
    juce::Rectangle<int> bounds = getLocalBounds();
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