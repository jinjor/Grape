#include "Components.h"

#include <JuceHeader.h>

#include "Params.h"
#include "StyleConstants.h"

using namespace styles;

namespace {

void consumeLabeledKnob(juce::Rectangle<int>& parentArea, juce::Label& label, juce::Slider& knob) {
    auto area = parentArea.removeFromLeft(SLIDER_WIDTH);
    label.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
    knob.setBounds(area.removeFromTop(KNOB_HEIGHT).reduced(LOCAL_MARGIN));
}
void consumeLabeledKnob(juce::Rectangle<int>& parentArea,
                        juce::Label& label,
                        juce::Slider& knob1,
                        juce::Slider& knob2) {
    auto area = parentArea.removeFromLeft(SLIDER_WIDTH);
    label.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
    auto knobBounds = area.removeFromTop(KNOB_HEIGHT).reduced(LOCAL_MARGIN);
    knob1.setBounds(knobBounds);
    knob2.setBounds(knobBounds);
}
void consumeLabeledKnob(juce::Rectangle<int>& parentArea,
                        juce::Label& label1,
                        juce::Slider& knob1,
                        juce::Label& label2,
                        juce::Slider& knob2) {
    auto area1 = parentArea.removeFromLeft(SLIDER_WIDTH);
    auto area2 = area1;
    consumeLabeledKnob(area1, label1, knob1);
    consumeLabeledKnob(area2, label2, knob2);
}
void consumeLabeledComboBox(juce::Rectangle<int>& parentArea, int width, juce::Label& label, juce::Component& box) {
    auto area = parentArea.removeFromLeft(width);
    label.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
    box.setBounds(area.removeFromTop(COMBO_BOX_HEIGHT).reduced(LOCAL_MARGIN));
}
void consumeKeyValueText(
    juce::Rectangle<int>& parentArea, int height, int width, juce::Label& keyLabel, juce::Label& valueLabel) {
    auto area = parentArea.removeFromTop(height);
    keyLabel.setBounds(area.removeFromLeft(width).reduced(LOCAL_MARGIN));
    valueLabel.setBounds(area.reduced(LOCAL_MARGIN));
}

}  // namespace

//==============================================================================
float calcCurrentLevel(int numSamples, float* data) {
    float maxValue = 0.0;
    for (int i = 0; i < numSamples; ++i) {
        maxValue = std::max(maxValue, std::abs(data[i]));
    }
    return juce::Decibels::gainToDecibels(maxValue);
}

//==============================================================================
HeaderComponent::HeaderComponent(std::string name, bool hasEnableButton)
    : enabledButton("Enabled"), name(name), hasEnableButton(hasEnableButton) {
    addAndMakeVisible(enabledButton);
    enabledButton.setEnabled(hasEnableButton);
}
HeaderComponent::~HeaderComponent() {}
void HeaderComponent::paint(juce::Graphics& g) {
    juce::Rectangle<int> bounds = getLocalBounds();
    g.setColour(colour::PANEL_NAME_BACKGROUND);
    g.fillRect(bounds);

    juce::GlyphArrangement ga;
    juce::Font font = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    ga.addLineOfText(font, name, 0, 0);
    juce::Path p;
    ga.createPath(p);
    auto pathBounds = p.getBounds();
    p.applyTransform(juce::AffineTransform()
                         .rotated(-juce::MathConstants<float>::halfPi, 0, 0)
                         .translated(pathBounds.getHeight() / 2 + bounds.getWidth() / 2,
                                     pathBounds.getWidth() + PANEL_NAME_HEIGHT + 2.0));
    g.setColour(colour::TEXT);
    g.fillPath(p);
}
void HeaderComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto enabledButtonArea = bounds.removeFromTop(bounds.getWidth());
    enabledButton.setBounds(enabledButtonArea.reduced(2.0));
}

//==============================================================================
VoiceComponent::VoiceComponent(VoiceParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : params(params),
      controlItemParams(controlItemParams),
      header("VOICE", false),
      modeSelector("Mode"),
      portamentoTimeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                           juce::Slider::TextEntryBoxPosition::NoTextBox),
      pitchBendRangeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                           juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    modeSelector.setLookAndFeel(&grapeLookAndFeel);
    modeSelector.addItemList(params.Mode->getAllValueStrings(), 1);
    modeSelector.setSelectedItemIndex(params.Mode->getIndex(), juce::dontSendNotification);
    modeSelector.setJustificationType(juce::Justification::centred);
    modeSelector.addListener(this);
    addAndMakeVisible(modeSelector);

    portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeel);
    portamentoTimeSlider.setRange(
        params.PortamentoTime->range.start, params.PortamentoTime->range.end, params.PortamentoTime->range.interval);
    portamentoTimeSlider.setSkewFactorFromMidPoint(0.1);
    portamentoTimeSlider.setValue(params.PortamentoTime->get(), juce::dontSendNotification);
    portamentoTimeSlider.setPopupDisplayEnabled(true, true, nullptr);
    portamentoTimeSlider.setScrollWheelEnabled(false);
    portamentoTimeSlider.setTextValueSuffix(" sec");
    portamentoTimeSlider.addListener(this);
    addAndMakeVisible(portamentoTimeSlider);

    pitchBendRangeSlider.setLookAndFeel(&grapeLookAndFeel);
    pitchBendRangeSlider.setRange(
        params.PitchBendRange->getRange().getStart(), params.PitchBendRange->getRange().getEnd(), 1);
    pitchBendRangeSlider.setValue(params.PitchBendRange->get(), juce::dontSendNotification);
    pitchBendRangeSlider.setPopupDisplayEnabled(true, true, nullptr);
    pitchBendRangeSlider.setScrollWheelEnabled(false);
    pitchBendRangeSlider.addListener(this);
    addAndMakeVisible(pitchBendRangeSlider);

    modeLabel.setFont(paramLabelFont);
    modeLabel.setText("Mode", juce::dontSendNotification);
    modeLabel.setJustificationType(juce::Justification::centred);
    modeLabel.setEditable(false, false, false);
    addAndMakeVisible(modeLabel);

    portamentoTimeLabel.setFont(paramLabelFont);
    portamentoTimeLabel.setText("Glide Time", juce::dontSendNotification);
    portamentoTimeLabel.setJustificationType(juce::Justification::centred);
    portamentoTimeLabel.setEditable(false, false, false);
    addAndMakeVisible(portamentoTimeLabel);

    pitchBendRangeLabel.setFont(paramLabelFont);
    pitchBendRangeLabel.setText("PB Range", juce::dontSendNotification);
    pitchBendRangeLabel.setJustificationType(juce::Justification::centred);
    pitchBendRangeLabel.setEditable(false, false, false);
    addAndMakeVisible(pitchBendRangeLabel);

    startTimerHz(30.0f);
}

VoiceComponent::~VoiceComponent() {}

void VoiceComponent::paint(juce::Graphics& g) {}

void VoiceComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    bounds.reduce(0, 10);
    consumeLabeledComboBox(bounds, 70, modeLabel, modeSelector);
    consumeLabeledKnob(bounds, portamentoTimeLabel, portamentoTimeSlider);
    consumeLabeledKnob(bounds, pitchBendRangeLabel, pitchBendRangeSlider);
}
void VoiceComponent::comboBoxChanged(juce::ComboBox* comboBox) {
    if (comboBox == &modeSelector) {
        *params.Mode = modeSelector.getSelectedItemIndex();
    }
}
void VoiceComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &portamentoTimeSlider) {
        *params.PortamentoTime = portamentoTimeSlider.getValue();
    } else if (slider == &pitchBendRangeSlider) {
        *params.PitchBendRange = pitchBendRangeSlider.getValue();
    }
}
void VoiceComponent::timerCallback() {
    portamentoTimeSlider.setValue(params.PortamentoTime->get(), juce::dontSendNotification);
    pitchBendRangeSlider.setValue(params.PitchBendRange->get(), juce::dontSendNotification);

    auto isMono = static_cast<VOICE_MODE>(params.Mode->getIndex()) == VOICE_MODE::Mono;
    portamentoTimeLabel.setEnabled(isMono);
    portamentoTimeSlider.setEnabled(isMono);

    portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(p.TargetType->getIndex());
        switch (targetType) {
            case CONTROL_TARGET_TYPE::Master: {
                auto targetParam = static_cast<CONTROL_TARGET_MISC_PARAM>(p.TargetMiscParam->getIndex());
                switch (targetParam) {
                    case CONTROL_TARGET_MISC_PARAM::PortamentoTime: {
                        portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
StatusComponent::StatusComponent(int* polyphony,
                                 TimeConsumptionState* timeConsumptionState,
                                 LatestDataProvider* latestDataProvider)
    : polyphony(polyphony), timeConsumptionState(timeConsumptionState), latestDataProvider(latestDataProvider) {
    latestDataProvider->addConsumer(&levelConsumer);

    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    juce::Font paramValueLabelFont =
        juce::Font(PARAM_VALUE_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    volumeValueLabel.setFont(paramValueLabelFont);
    volumeValueLabel.setText("0.0dB", juce::dontSendNotification);
    volumeValueLabel.setJustificationType(juce::Justification::left);
    volumeValueLabel.setEditable(false, false, false);
    addAndMakeVisible(volumeValueLabel);

    polyphonyValueLabel.setFont(paramValueLabelFont);
    polyphonyValueLabel.setText(juce::String(*polyphony), juce::dontSendNotification);
    polyphonyValueLabel.setJustificationType(juce::Justification::left);
    polyphonyValueLabel.setEditable(false, false, false);
    addAndMakeVisible(polyphonyValueLabel);

    timeConsumptionValueLabel.setFont(paramValueLabelFont);
    timeConsumptionValueLabel.setText(
        juce::String(juce::roundToInt(timeConsumptionState->currentTimeConsumptionRate * 100)) + "%",
        juce::dontSendNotification);
    timeConsumptionValueLabel.setJustificationType(juce::Justification::left);
    timeConsumptionValueLabel.setEditable(false, false, false);
    addAndMakeVisible(timeConsumptionValueLabel);

    volumeLabel.setFont(paramLabelFont);
    volumeLabel.setText("Peak:", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::right);
    volumeLabel.setEditable(false, false, false);
    addAndMakeVisible(volumeLabel);

    polyphonyLabel.setFont(paramLabelFont);
    polyphonyLabel.setText("Polyphony:", juce::dontSendNotification);
    polyphonyLabel.setJustificationType(juce::Justification::right);
    polyphonyLabel.setEditable(false, false, false);
    addAndMakeVisible(polyphonyLabel);

    timeConsumptionLabel.setFont(paramLabelFont);
    timeConsumptionLabel.setText("Busyness:", juce::dontSendNotification);
    timeConsumptionLabel.setJustificationType(juce::Justification::right);
    timeConsumptionLabel.setEditable(false, false, false);
    addAndMakeVisible(timeConsumptionLabel);

    startTimerHz(4.0f);
}

StatusComponent::~StatusComponent() { latestDataProvider->removeConsumer(&levelConsumer); }

void StatusComponent::paint(juce::Graphics& g) {}

void StatusComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(0, 10);
    auto boundsHeight = bounds.getHeight();
    auto boundsWidth = bounds.getWidth();
    consumeKeyValueText(bounds, boundsHeight / 3, boundsWidth * 0.4, volumeLabel, volumeValueLabel);
    consumeKeyValueText(bounds, boundsHeight / 3, boundsWidth * 0.4, polyphonyLabel, polyphonyValueLabel);
    consumeKeyValueText(bounds, boundsHeight / 3, boundsWidth * 0.4, timeConsumptionLabel, timeConsumptionValueLabel);
}
void StatusComponent::timerCallback() {
    if (overflowWarning > 0) {
        volumeValueLabel.setColour(juce::Label::textColourId, colour::ERROR);
        auto levelStr = juce::String(overflowedLevel, 1) + " dB";
        volumeValueLabel.setText(levelStr, juce::dontSendNotification);

        overflowWarning--;
    } else {
        volumeValueLabel.removeColour(juce::Label::textColourId);

        if (levelConsumer.ready) {
            float levelLdB = calcCurrentLevel(levelConsumer.numSamples, levelConsumer.destinationL);
            float levelRdB = calcCurrentLevel(levelConsumer.numSamples, levelConsumer.destinationR);
            auto leveldB = std::max(levelLdB, levelRdB);
            auto levelStr = (leveldB <= -100 ? "-Inf" : juce::String(leveldB, 1)) + " dB";
            volumeValueLabel.setText(levelStr, juce::dontSendNotification);
            levelConsumer.ready = false;
            if (leveldB > 0) {
                overflowedLevel = leveldB;
                overflowWarning = 4 * 1.2;
            }
        }
    }
    {
        int numVoices = 64;  // TODO: share this
        int value = *polyphony;
        polyphonyValueLabel.setText(juce::String(value), juce::dontSendNotification);
        if (value >= numVoices) {
            polyphonyValueLabel.setColour(juce::Label::textColourId, colour::ERROR);
        } else if (value > numVoices * 0.8) {
            polyphonyValueLabel.setColour(juce::Label::textColourId, colour::WARNING);
        } else {
            polyphonyValueLabel.setColour(juce::Label::textColourId, colour::TEXT);
        }
    }
    {
        auto value = timeConsumptionState->currentTimeConsumptionRate;
        timeConsumptionValueLabel.setText(juce::String(juce::roundToInt(value * 100)) + "%",
                                          juce::dontSendNotification);
        if (value >= 1.0) {
            timeConsumptionValueLabel.setColour(juce::Label::textColourId, colour::ERROR);
        } else if (value >= 0.8) {
            timeConsumptionValueLabel.setColour(juce::Label::textColourId, colour::WARNING);
        } else {
            timeConsumptionValueLabel.setColour(juce::Label::textColourId, colour::TEXT);
        }
    }
}

//==============================================================================
MasterComponent::MasterComponent(GlobalParams& params)
    : params(params),
      header("MASTER", false),
      panSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
      volumeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font panelNameFont = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    panSlider.setLookAndFeel(&grapeLookAndFeelControlled);
    panSlider.setRange(params.Pan->range.start, params.Pan->range.end, 0.01);
    panSlider.setValue(params.Pan->get(), juce::dontSendNotification);
    panSlider.setPopupDisplayEnabled(true, true, nullptr);
    panSlider.setScrollWheelEnabled(false);
    panSlider.addListener(this);
    addAndMakeVisible(panSlider);

    volumeSlider.setLookAndFeel(&grapeLookAndFeel);
    volumeSlider.setRange(params.MasterVolume->range.start, params.MasterVolume->range.end, 0.01);
    volumeSlider.setValue(params.MasterVolume->get(), juce::dontSendNotification);
    volumeSlider.setPopupDisplayEnabled(true, true, nullptr);
    volumeSlider.setScrollWheelEnabled(false);
    //    volumeSlider.textFromValueFunction = [](double gain){ return
    //    juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB"; };
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    panLabel.setFont(paramLabelFont);
    panLabel.setText("Pan", juce::dontSendNotification);
    panLabel.setJustificationType(juce::Justification::centred);
    panLabel.setEditable(false, false, false);
    addAndMakeVisible(panLabel);

    volumeLabel.setFont(paramLabelFont);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel.setEditable(false, false, false);
    addAndMakeVisible(volumeLabel);

    startTimerHz(30.0f);
}

MasterComponent::~MasterComponent() {}

void MasterComponent::paint(juce::Graphics& g) {}

void MasterComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    bounds.reduce(0, 10);
    consumeLabeledKnob(bounds, panLabel, panSlider);
    consumeLabeledKnob(bounds, volumeLabel, volumeSlider);
}
void MasterComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &panSlider) {
        *params.Pan = (float)panSlider.getValue();
    } else if (slider == &volumeSlider) {
        *params.MasterVolume = (float)volumeSlider.getValue();
    }
}
void MasterComponent::timerCallback() {
    panSlider.setValue(params.Pan->get(), juce::dontSendNotification);
    volumeSlider.setValue(params.MasterVolume->get(), juce::dontSendNotification);
}

//==============================================================================
OscComponent::OscComponent(int index, OscParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : index(index),
      params(params),
      controlItemParams(controlItemParams),
      header("OSC " + std::to_string(index + 1), true),
      envelopeSelector("Envelope"),
      waveformSelector("Waveform"),
      edgeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox),
      octaveSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      coarseSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      unisonSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      detuneSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      spreadSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.addListener(this);
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    envelopeSelector.setLookAndFeel(&grapeLookAndFeel);
    envelopeSelector.addItemList(params.Envelope->getAllValueStrings(), 1);
    envelopeSelector.setSelectedItemIndex(params.Envelope->getIndex(), juce::dontSendNotification);
    envelopeSelector.setJustificationType(juce::Justification::centred);
    envelopeSelector.addListener(this);
    body.addAndMakeVisible(envelopeSelector);

    waveformSelector.setLookAndFeel(&grapeLookAndFeel);
    waveformSelector.addItemList(params.Waveform->getAllValueStrings(), 1);
    waveformSelector.setSelectedItemIndex(params.Waveform->getIndex(), juce::dontSendNotification);
    waveformSelector.setJustificationType(juce::Justification::centred);
    waveformSelector.addListener(this);
    body.addAndMakeVisible(waveformSelector);

    edgeSlider.setLookAndFeel(&grapeLookAndFeel);
    edgeSlider.setRange(params.Edge->range.start, params.Edge->range.end, 0.01);
    edgeSlider.setValue(params.Edge->get(), juce::dontSendNotification);
    edgeSlider.setPopupDisplayEnabled(true, true, nullptr);
    edgeSlider.setScrollWheelEnabled(false);
    edgeSlider.addListener(this);
    body.addAndMakeVisible(edgeSlider);

    octaveSlider.setLookAndFeel(&grapeLookAndFeel);
    octaveSlider.setRange(params.Octave->getRange().getStart(), params.Octave->getRange().getEnd(), 1);
    octaveSlider.setValue(params.Octave->get(), juce::dontSendNotification);
    octaveSlider.setPopupDisplayEnabled(true, true, nullptr);
    octaveSlider.setScrollWheelEnabled(false);
    octaveSlider.addListener(this);
    body.addAndMakeVisible(octaveSlider);

    coarseSlider.setLookAndFeel(&grapeLookAndFeel);
    coarseSlider.setRange(params.Coarse->getRange().getStart(), params.Coarse->getRange().getEnd(), 1);
    coarseSlider.setValue(params.Coarse->get(), juce::dontSendNotification);
    coarseSlider.setPopupDisplayEnabled(true, true, nullptr);
    coarseSlider.setScrollWheelEnabled(false);
    coarseSlider.addListener(this);
    body.addAndMakeVisible(coarseSlider);

    unisonSlider.setLookAndFeel(&grapeLookAndFeel);
    unisonSlider.setRange(params.Unison->getRange().getStart(), params.Unison->getRange().getEnd(), 1);
    unisonSlider.setValue(params.Unison->get(), juce::dontSendNotification);
    unisonSlider.setPopupDisplayEnabled(true, true, nullptr);
    unisonSlider.setScrollWheelEnabled(false);
    unisonSlider.addListener(this);
    body.addAndMakeVisible(unisonSlider);

    detuneSlider.setLookAndFeel(&grapeLookAndFeel);
    detuneSlider.setRange(params.Detune->range.start, params.Detune->range.end, 0.01);
    detuneSlider.setValue(params.Detune->get(), juce::dontSendNotification);
    detuneSlider.setPopupDisplayEnabled(true, true, nullptr);
    detuneSlider.setScrollWheelEnabled(false);
    detuneSlider.addListener(this);
    body.addAndMakeVisible(detuneSlider);

    spreadSlider.setLookAndFeel(&grapeLookAndFeel);
    spreadSlider.setRange(params.Spread->range.start, params.Spread->range.end, 0.01);
    spreadSlider.setValue(params.Spread->get(), juce::dontSendNotification);
    spreadSlider.setPopupDisplayEnabled(true, true, nullptr);
    spreadSlider.setScrollWheelEnabled(false);
    spreadSlider.addListener(this);
    body.addAndMakeVisible(spreadSlider);

    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setRange(params.Gain->range.start, params.Gain->range.end, params.Gain->range.interval);
    gainSlider.setValue(params.Gain->get(), juce::dontSendNotification);
    gainSlider.setPopupDisplayEnabled(true, true, nullptr);
    gainSlider.setScrollWheelEnabled(false);
    gainSlider.setSkewFactorFromMidPoint(1.0);
    gainSlider.textFromValueFunction = [](double gain) {
        return juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB";
    };
    gainSlider.addListener(this);
    body.addAndMakeVisible(gainSlider);

    envelopeLabel.setFont(paramLabelFont);
    envelopeLabel.setText("Amp Env", juce::dontSendNotification);
    envelopeLabel.setJustificationType(juce::Justification::centred);
    envelopeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(envelopeLabel);

    waveformLabel.setFont(paramLabelFont);
    waveformLabel.setText("Waveform", juce::dontSendNotification);
    waveformLabel.setJustificationType(juce::Justification::centred);
    waveformLabel.setEditable(false, false, false);
    body.addAndMakeVisible(waveformLabel);

    edgeLabel.setFont(paramLabelFont);
    edgeLabel.setText("Edge", juce::dontSendNotification);
    edgeLabel.setJustificationType(juce::Justification::centred);
    edgeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(edgeLabel);

    octaveLabel.setFont(paramLabelFont);
    octaveLabel.setText("Octave", juce::dontSendNotification);
    octaveLabel.setJustificationType(juce::Justification::centred);
    octaveLabel.setEditable(false, false, false);
    body.addAndMakeVisible(octaveLabel);

    coarseLabel.setFont(paramLabelFont);
    coarseLabel.setText("Semitone", juce::dontSendNotification);
    coarseLabel.setJustificationType(juce::Justification::centred);
    coarseLabel.setEditable(false, false, false);
    body.addAndMakeVisible(coarseLabel);

    unisonLabel.setFont(paramLabelFont);
    unisonLabel.setText("Unison", juce::dontSendNotification);
    unisonLabel.setJustificationType(juce::Justification::centred);
    unisonLabel.setEditable(false, false, false);
    body.addAndMakeVisible(unisonLabel);

    detuneLabel.setFont(paramLabelFont);
    detuneLabel.setText("Detune", juce::dontSendNotification);
    detuneLabel.setJustificationType(juce::Justification::centred);
    detuneLabel.setEditable(false, false, false);
    body.addAndMakeVisible(detuneLabel);

    spreadLabel.setFont(paramLabelFont);
    spreadLabel.setText("Spread", juce::dontSendNotification);
    spreadLabel.setJustificationType(juce::Justification::centred);
    spreadLabel.setEditable(false, false, false);
    body.addAndMakeVisible(spreadLabel);

    gainLabel.setFont(paramLabelFont);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setEditable(false, false, false);
    body.addAndMakeVisible(gainLabel);

    body.setEnabled(params.Enabled->get());
    addAndMakeVisible(body);

    startTimerHz(30.0f);
}

OscComponent::~OscComponent() {}

void OscComponent::paint(juce::Graphics& g) {}

void OscComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto& lowerArea = bounds;
    consumeLabeledComboBox(upperArea, 60, envelopeLabel, envelopeSelector);
    consumeLabeledComboBox(upperArea, 120, waveformLabel, waveformSelector);
    consumeLabeledKnob(upperArea, edgeLabel, edgeSlider);
    consumeLabeledKnob(upperArea, gainLabel, gainSlider);
    consumeLabeledKnob(lowerArea, octaveLabel, octaveSlider);
    consumeLabeledKnob(lowerArea, coarseLabel, coarseSlider);
    consumeLabeledKnob(lowerArea, unisonLabel, unisonSlider);
    consumeLabeledKnob(lowerArea, detuneLabel, detuneSlider);
    consumeLabeledKnob(lowerArea, spreadLabel, spreadSlider);
}
void OscComponent::buttonClicked(juce::Button* button) {
    if (button == &header.enabledButton) {
        *params.Enabled = header.enabledButton.getToggleState();
    }
}
void OscComponent::comboBoxChanged(juce::ComboBox* comboBox) {
    if (comboBox == &envelopeSelector) {
        *params.Envelope = envelopeSelector.getSelectedItemIndex();
    } else if (comboBox == &waveformSelector) {
        *params.Waveform = waveformSelector.getSelectedItemIndex();
    }
}
void OscComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &edgeSlider) {
        *params.Edge = edgeSlider.getValue();
    } else if (slider == &octaveSlider) {
        *params.Octave = octaveSlider.getValue();
    } else if (slider == &coarseSlider) {
        *params.Coarse = coarseSlider.getValue();
    } else if (slider == &unisonSlider) {
        *params.Unison = unisonSlider.getValue();
    } else if (slider == &detuneSlider) {
        *params.Detune = (float)detuneSlider.getValue();
    } else if (slider == &spreadSlider) {
        *params.Spread = (float)spreadSlider.getValue();
    } else if (slider == &gainSlider) {
        *params.Gain = (float)gainSlider.getValue();
    }
}
void OscComponent::timerCallback() {
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    body.setEnabled(params.Enabled->get());
    envelopeSelector.setSelectedItemIndex(params.Envelope->getIndex(), juce::dontSendNotification);
    waveformSelector.setSelectedItemIndex(params.Waveform->getIndex(), juce::dontSendNotification);
    edgeSlider.setValue(params.Edge->get(), juce::dontSendNotification);
    octaveSlider.setValue(params.Octave->get(), juce::dontSendNotification);
    coarseSlider.setValue(params.Coarse->get(), juce::dontSendNotification);
    unisonSlider.setValue(params.Unison->get(), juce::dontSendNotification);
    detuneSlider.setValue(params.Detune->get(), juce::dontSendNotification);
    spreadSlider.setValue(params.Spread->get(), juce::dontSendNotification);
    gainSlider.setValue(params.Gain->get(), juce::dontSendNotification);
    auto hasEdge = OSC_WAVEFORM_VALUES[params.Waveform->getIndex()] == WAVEFORM::Square ||
                   OSC_WAVEFORM_VALUES[params.Waveform->getIndex()] == WAVEFORM::Triangle;
    edgeLabel.setEnabled(hasEdge);
    edgeSlider.setEnabled(hasEdge);
    auto isNoise = OSC_WAVEFORM_VALUES[params.Waveform->getIndex()] == WAVEFORM::White ||
                   OSC_WAVEFORM_VALUES[params.Waveform->getIndex()] == WAVEFORM::Pink;
    unisonLabel.setEnabled(!isNoise);
    unisonSlider.setEnabled(!isNoise);
    detuneLabel.setEnabled(!isNoise);
    detuneSlider.setEnabled(!isNoise);
    spreadLabel.setEnabled(!isNoise);
    spreadSlider.setEnabled(!isNoise);

    edgeSlider.setLookAndFeel(&grapeLookAndFeel);
    detuneSlider.setLookAndFeel(&grapeLookAndFeel);
    spreadSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(p.TargetType->getIndex());
        switch (targetType) {
            case CONTROL_TARGET_TYPE::OSC: {
                int targetIndex = p.TargetOsc->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_OSC_PARAM>(p.TargetOscParam->getIndex());
                if (targetIndex == index) {
                    switch (targetParam) {
                        case CONTROL_TARGET_OSC_PARAM::Edge: {
                            edgeSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                        case CONTROL_TARGET_OSC_PARAM::Detune: {
                            detuneSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                        case CONTROL_TARGET_OSC_PARAM::Spread: {
                            spreadSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                            //                        case
                            //                        CONTROL_TARGET_OSC_PARAM::Pan:
                            //                        {
                            ////
                            /// panSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            //                            break;
                            //                        }
                        case CONTROL_TARGET_OSC_PARAM::Gain: {
                            gainSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
EnvelopeComponent::EnvelopeComponent(int index, EnvelopeParams& params)
    : index(index),
      params(params),
      header("AMP ENV " + std::to_string(index + 1), false),
      attackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      decaySlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                  juce::Slider::TextEntryBoxPosition::NoTextBox),
      sustainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                    juce::Slider::TextEntryBoxPosition::NoTextBox),
      releaseSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                    juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font panelNameFont = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    attackSlider.setLookAndFeel(&grapeLookAndFeel);
    attackSlider.setRange(params.Attack->range.start, params.Attack->range.end, 0.001);
    attackSlider.setSkewFactorFromMidPoint(0.2);
    attackSlider.setValue(params.Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, nullptr);
    attackSlider.setScrollWheelEnabled(false);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);

    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(params.Decay->range.start, params.Decay->range.end, 0.01);
    decaySlider.setSkewFactorFromMidPoint(0.4);
    decaySlider.setValue(params.Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, nullptr);
    decaySlider.setScrollWheelEnabled(false);
    decaySlider.setTextValueSuffix(" sec");
    decaySlider.addListener(this);
    addAndMakeVisible(decaySlider);

    sustainSlider.setLookAndFeel(&grapeLookAndFeel);
    sustainSlider.setRange(params.Sustain->range.start, params.Sustain->range.end, 0.01);
    sustainSlider.setValue(params.Sustain->get(), juce::dontSendNotification);
    sustainSlider.setPopupDisplayEnabled(true, true, nullptr);
    sustainSlider.setScrollWheelEnabled(false);
    //    sustainSlider.textFromValueFunction = [](double gain){ return
    //    juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB"; };
    sustainSlider.textFromValueFunction = [](double gain) { return juce::String(gain * 100, 0) + " %"; };
    sustainSlider.addListener(this);
    addAndMakeVisible(sustainSlider);

    releaseSlider.setLookAndFeel(&grapeLookAndFeel);
    releaseSlider.setRange(params.Release->range.start, params.Release->range.end, 0.01);
    releaseSlider.setSkewFactorFromMidPoint(0.4);
    releaseSlider.setValue(params.Release->get(), juce::dontSendNotification);
    releaseSlider.setPopupDisplayEnabled(true, true, nullptr);
    releaseSlider.setScrollWheelEnabled(false);
    releaseSlider.setTextValueSuffix(" sec");
    releaseSlider.addListener(this);
    addAndMakeVisible(releaseSlider);

    attackLabel.setFont(paramLabelFont);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.setEditable(false, false, false);
    addAndMakeVisible(attackLabel);

    decayLabel.setFont(paramLabelFont);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setEditable(false, false, false);
    addAndMakeVisible(decayLabel);

    sustainLabel.setFont(paramLabelFont);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centred);
    sustainLabel.setEditable(false, false, false);
    addAndMakeVisible(sustainLabel);

    releaseLabel.setFont(paramLabelFont);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.setEditable(false, false, false);
    addAndMakeVisible(releaseLabel);

    startTimerHz(30.0f);
}

EnvelopeComponent::~EnvelopeComponent() {}

void EnvelopeComponent::paint(juce::Graphics& g) {}

void EnvelopeComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto& lowerArea = bounds;
    consumeLabeledKnob(upperArea, attackLabel, attackSlider);
    consumeLabeledKnob(upperArea, decayLabel, decaySlider);
    consumeLabeledKnob(lowerArea, sustainLabel, sustainSlider);
    consumeLabeledKnob(lowerArea, releaseLabel, releaseSlider);
}
void EnvelopeComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &attackSlider) {
        *params.Attack = (float)attackSlider.getValue();
    } else if (slider == &decaySlider) {
        *params.Decay = (float)decaySlider.getValue();
    } else if (slider == &sustainSlider) {
        *params.Sustain = (float)sustainSlider.getValue();
    } else if (slider == &releaseSlider) {
        *params.Release = (float)releaseSlider.getValue();
    }
}
void EnvelopeComponent::timerCallback() {
    attackSlider.setValue(params.Attack->get(), juce::dontSendNotification);
    decaySlider.setValue(params.Decay->get(), juce::dontSendNotification);
    sustainSlider.setValue(params.Sustain->get(), juce::dontSendNotification);
    releaseSlider.setValue(params.Release->get(), juce::dontSendNotification);
}

//==============================================================================
FilterComponent::FilterComponent(int index,
                                 FilterParams& params,
                                 std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : index(index),
      params(params),
      controlItemParams(controlItemParams),
      header("FILTER " + std::to_string(index + 1), true),
      targetSelector("Target"),
      typeSelector("Type"),
      freqTypeSelector("Type"),
      hzSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
      semitoneSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      qSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
      gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    targetSelector.setLookAndFeel(&grapeLookAndFeel);
    targetSelector.addItemList(params.Target->getAllValueStrings(), 1);
    targetSelector.setSelectedItemIndex(params.Target->getIndex(), juce::dontSendNotification);
    targetSelector.setJustificationType(juce::Justification::centred);
    targetSelector.addListener(this);
    body.addAndMakeVisible(targetSelector);

    typeSelector.setLookAndFeel(&grapeLookAndFeel);
    typeSelector.addItemList(params.Type->getAllValueStrings(), 1);
    typeSelector.setSelectedItemIndex(params.Type->getIndex(), juce::dontSendNotification);
    typeSelector.setJustificationType(juce::Justification::centred);
    typeSelector.addListener(this);
    body.addAndMakeVisible(typeSelector);

    freqTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    freqTypeSelector.addItemList(params.FreqType->getAllValueStrings(), 1);
    freqTypeSelector.setSelectedItemIndex(params.FreqType->getIndex(), juce::dontSendNotification);
    freqTypeSelector.setJustificationType(juce::Justification::centred);
    freqTypeSelector.addListener(this);
    body.addAndMakeVisible(freqTypeSelector);

    hzSlider.setLookAndFeel(&grapeLookAndFeel);
    hzSlider.setRange(params.Hz->range.start, params.Hz->range.end, params.Hz->range.interval);
    hzSlider.setSkewFactorFromMidPoint(2000.0f);
    hzSlider.setValue(params.Hz->get(), juce::dontSendNotification);
    hzSlider.setPopupDisplayEnabled(true, true, nullptr);
    hzSlider.setScrollWheelEnabled(false);
    hzSlider.setTextValueSuffix(" Hz");
    hzSlider.addListener(this);
    body.addAndMakeVisible(hzSlider);

    semitoneSlider.setLookAndFeel(&grapeLookAndFeel);
    semitoneSlider.setRange(params.Semitone->getRange().getStart(), params.Semitone->getRange().getEnd(), 1);
    semitoneSlider.setValue(params.Semitone->get(), juce::dontSendNotification);
    semitoneSlider.setPopupDisplayEnabled(true, true, nullptr);
    semitoneSlider.setScrollWheelEnabled(false);
    semitoneSlider.textFromValueFunction = [](double value) -> std::string {
        int cent = value;
        int centAbs = std::abs(cent);
        int oct = centAbs / 12;
        int octFrac = centAbs % 12;
        return (cent == 0 ? " " : cent > 0 ? "+" : "-") + std::to_string(oct) + ":" + std::to_string(octFrac) + " oct";
    };
    semitoneSlider.addListener(this);
    body.addAndMakeVisible(semitoneSlider);

    qSlider.setLookAndFeel(&grapeLookAndFeel);
    qSlider.setRange(params.Q->range.start, params.Q->range.end, 0.01);
    qSlider.setSkewFactorFromMidPoint(1.0f);
    qSlider.setValue(params.Q->get(), juce::dontSendNotification);
    qSlider.setPopupDisplayEnabled(true, true, nullptr);
    qSlider.setScrollWheelEnabled(false);
    qSlider.addListener(this);
    body.addAndMakeVisible(qSlider);

    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setRange(params.Gain->range.start, params.Gain->range.end, 0.01);
    gainSlider.setValue(params.Gain->get(), juce::dontSendNotification);
    gainSlider.setPopupDisplayEnabled(true, true, nullptr);
    gainSlider.setScrollWheelEnabled(false);
    gainSlider.addListener(this);
    body.addAndMakeVisible(gainSlider);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("OSC", juce::dontSendNotification);
    targetLabel.setJustificationType(juce::Justification::centred);
    targetLabel.setEditable(false, false, false);
    body.addAndMakeVisible(targetLabel);

    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);

    freqTypeLabel.setFont(paramLabelFont);
    freqTypeLabel.setText("Freq Type", juce::dontSendNotification);
    freqTypeLabel.setJustificationType(juce::Justification::centred);
    freqTypeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(freqTypeLabel);

    freqLabel.setFont(paramLabelFont);
    freqLabel.setText("Freq", juce::dontSendNotification);
    freqLabel.setJustificationType(juce::Justification::centred);
    freqLabel.setEditable(false, false, false);
    body.addAndMakeVisible(freqLabel);

    qLabel.setFont(paramLabelFont);
    qLabel.setText("Q", juce::dontSendNotification);
    qLabel.setJustificationType(juce::Justification::centred);
    qLabel.setEditable(false, false, false);
    body.addAndMakeVisible(qLabel);

    gainLabel.setFont(paramLabelFont);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setEditable(false, false, false);
    body.addAndMakeVisible(gainLabel);

    addAndMakeVisible(body);

    startTimerHz(30.0f);
}

FilterComponent::~FilterComponent() {}

void FilterComponent::paint(juce::Graphics& g) {}

void FilterComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto& lowerArea = bounds;
    consumeLabeledComboBox(upperArea, 70, targetLabel, targetSelector);
    consumeLabeledComboBox(upperArea, 120, typeLabel, typeSelector);
    consumeLabeledComboBox(lowerArea, 70, freqTypeLabel, freqTypeSelector);
    consumeLabeledKnob(lowerArea, freqLabel, hzSlider, semitoneSlider);
    consumeLabeledKnob(lowerArea, qLabel, qSlider);
    consumeLabeledKnob(lowerArea, gainLabel, gainSlider);
}
void FilterComponent::buttonClicked(juce::Button* button) {
    if (button == &header.enabledButton) {
        *params.Enabled = header.enabledButton.getToggleState();
    }
}
void FilterComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &targetSelector) {
        *params.Target = targetSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &typeSelector) {
        *params.Type = typeSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &freqTypeSelector) {
        *params.FreqType = freqTypeSelector.getSelectedItemIndex();
    }
}
void FilterComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &hzSlider) {
        *params.Hz = (float)hzSlider.getValue();
    } else if (slider == &semitoneSlider) {
        *params.Semitone = semitoneSlider.getValue();
    } else if (slider == &qSlider) {
        *params.Q = (float)qSlider.getValue();
    } else if (slider == &gainSlider) {
        *params.Gain = (float)gainSlider.getValue();
    }
}
void FilterComponent::timerCallback() {
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    body.setEnabled(params.Enabled->get());

    targetSelector.setSelectedItemIndex(params.Target->getIndex(), juce::dontSendNotification);
    typeSelector.setSelectedItemIndex(params.Type->getIndex(), juce::dontSendNotification);
    freqTypeSelector.setSelectedItemIndex(params.FreqType->getIndex(), juce::dontSendNotification);
    hzSlider.setValue(params.Hz->get(), juce::dontSendNotification);
    semitoneSlider.setValue(params.Semitone->get(), juce::dontSendNotification);
    qSlider.setValue(params.Q->get(), juce::dontSendNotification);

    auto freqType = static_cast<FILTER_FREQ_TYPE>(params.FreqType->getIndex());
    hzSlider.setVisible(freqType == FILTER_FREQ_TYPE::Absolute);
    semitoneSlider.setVisible(freqType == FILTER_FREQ_TYPE::Relative);

    auto hasGain = params.hasGain();
    gainLabel.setEnabled(hasGain);
    gainSlider.setEnabled(hasGain);

    hzSlider.setLookAndFeel(&grapeLookAndFeel);
    semitoneSlider.setLookAndFeel(&grapeLookAndFeel);
    qSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(p.TargetType->getIndex());
        switch (targetType) {
            case CONTROL_TARGET_TYPE::Filter: {
                int targetIndex = p.TargetFilter->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_FILTER_PARAM>(p.TargetFilterParam->getIndex());
                if (targetIndex == index) {
                    switch (targetParam) {
                        case CONTROL_TARGET_FILTER_PARAM::Freq: {
                            hzSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            semitoneSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                        case CONTROL_TARGET_FILTER_PARAM::Q: {
                            qSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
LfoComponent::LfoComponent(int index, LfoParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : index(index),
      params(params),
      controlItemParams(controlItemParams),
      header("LFO " + std::to_string(index + 1), true),
      targetTypeSelector("TargetType"),
      targetOscSelector("TargetOsc"),
      targetFilterSelector("TargetFilter"),
      targetOscParamSelector("TargetOscParam"),
      targetFilterParamSelector("TargetFilterParam"),
      waveformSelector("Waveform"),
      slowFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      fastFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      amountSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    targetTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    targetTypeSelector.addItemList(params.TargetType->getAllValueStrings(), 1);
    targetTypeSelector.setSelectedItemIndex(params.TargetType->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setJustificationType(juce::Justification::centred);
    targetTypeSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetTypeSelector);

    targetOscSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscSelector.addItemList(params.TargetOsc->getAllValueStrings(), 1);
    targetOscSelector.setSelectedItemIndex(params.TargetOsc->getIndex(), juce::dontSendNotification);
    targetOscSelector.setJustificationType(juce::Justification::centred);
    targetOscSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetOscSelector);

    targetFilterSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterSelector.addItemList(params.TargetFilter->getAllValueStrings(), 1);
    targetFilterSelector.setSelectedItemIndex(params.TargetFilter->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setJustificationType(juce::Justification::centred);
    targetFilterSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetFilterSelector);

    targetOscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscParamSelector.addItemList(params.TargetOscParam->getAllValueStrings(), 1);
    targetOscParamSelector.setSelectedItemIndex(params.TargetOscParam->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setJustificationType(juce::Justification::centred);
    targetOscParamSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetOscParamSelector);

    targetFilterParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterParamSelector.addItemList(params.TargetFilterParam->getAllValueStrings(), 1);
    targetFilterParamSelector.setSelectedItemIndex(params.TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setJustificationType(juce::Justification::centred);
    targetFilterParamSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetFilterParamSelector);

    waveformSelector.setLookAndFeel(&grapeLookAndFeel);
    waveformSelector.addItemList(params.Waveform->getAllValueStrings(), 1);
    waveformSelector.setSelectedItemIndex(params.Waveform->getIndex(), juce::dontSendNotification);
    waveformSelector.setJustificationType(juce::Justification::centred);
    waveformSelector.addListener(this);
    body.addAndMakeVisible(waveformSelector);

    slowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    slowFreqSlider.setRange(params.SlowFreq->range.start, params.SlowFreq->range.end, params.SlowFreq->range.interval);
    slowFreqSlider.setSkewFactorFromMidPoint(4.0f);
    slowFreqSlider.setValue(params.SlowFreq->get(), juce::dontSendNotification);
    slowFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    slowFreqSlider.setScrollWheelEnabled(false);
    slowFreqSlider.setTextValueSuffix(" Hz");
    slowFreqSlider.addListener(this);
    body.addAndMakeVisible(slowFreqSlider);

    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    fastFreqSlider.setRange(params.FastFreq->range.start, params.FastFreq->range.end, params.FastFreq->range.interval);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0f);
    fastFreqSlider.setValue(params.FastFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0);
    fastFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    fastFreqSlider.setScrollWheelEnabled(false);
    fastFreqSlider.setTextValueSuffix(" x");
    fastFreqSlider.addListener(this);
    body.addAndMakeVisible(fastFreqSlider);

    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setRange(params.Amount->range.start, params.Amount->range.end, 0.01);
    amountSlider.setValue(params.Amount->get(), juce::dontSendNotification);
    amountSlider.setPopupDisplayEnabled(true, true, nullptr);
    amountSlider.setScrollWheelEnabled(false);
    amountSlider.addListener(this);
    body.addAndMakeVisible(amountSlider);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("Destination", juce::dontSendNotification);
    targetLabel.setJustificationType(juce::Justification::centred);
    targetLabel.setEditable(false, false, false);
    body.addAndMakeVisible(targetLabel);

    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);

    waveformLabel.setFont(paramLabelFont);
    waveformLabel.setText("Waveform", juce::dontSendNotification);
    waveformLabel.setJustificationType(juce::Justification::centred);
    waveformLabel.setEditable(false, false, false);
    body.addAndMakeVisible(waveformLabel);

    freqLabel.setFont(paramLabelFont);
    freqLabel.setText("Freq", juce::dontSendNotification);
    freqLabel.setJustificationType(juce::Justification::centred);
    freqLabel.setEditable(false, false, false);
    body.addAndMakeVisible(freqLabel);

    amountLabel.setFont(paramLabelFont);
    amountLabel.setText("Amount", juce::dontSendNotification);
    amountLabel.setJustificationType(juce::Justification::centred);
    amountLabel.setEditable(false, false, false);
    body.addAndMakeVisible(amountLabel);

    body.addAndMakeVisible(targetSelector);
    addAndMakeVisible(body);

    startTimerHz(30.0f);
}

LfoComponent::~LfoComponent() {}

void LfoComponent::paint(juce::Graphics& g) {}

void LfoComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto& lowerArea = bounds;
    consumeLabeledComboBox(upperArea, 280, targetLabel, targetSelector);
    {
        juce::Rectangle<int> selectorsArea = targetSelector.getLocalBounds();
        targetTypeSelector.setBounds(selectorsArea.removeFromLeft(90));
        auto indexArea = selectorsArea.removeFromLeft(70);
        targetOscSelector.setBounds(indexArea);
        targetFilterSelector.setBounds(indexArea);
        auto paramArea = selectorsArea.removeFromLeft(110);
        targetOscParamSelector.setBounds(paramArea);
        targetFilterParamSelector.setBounds(paramArea);
    }
    consumeLabeledComboBox(lowerArea, 120, waveformLabel, waveformSelector);
    consumeLabeledKnob(lowerArea, freqLabel, fastFreqSlider, slowFreqSlider);
    consumeLabeledKnob(lowerArea, amountLabel, amountSlider);
}
void LfoComponent::buttonClicked(juce::Button* button) {
    if (button == &header.enabledButton) {
        *params.Enabled = header.enabledButton.getToggleState();
    }
}
void LfoComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &targetTypeSelector) {
        *params.TargetType = targetTypeSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetOscSelector) {
        *params.TargetOsc = targetOscSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetFilterSelector) {
        *params.TargetFilter = targetFilterSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetOscParamSelector) {
        *params.TargetOscParam = targetOscParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetFilterParamSelector) {
        *params.TargetFilterParam = targetFilterParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &waveformSelector) {
        *params.Waveform = waveformSelector.getSelectedItemIndex();
    }
    resized();  // re-render
}
void LfoComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &slowFreqSlider) {
        *params.SlowFreq = (float)slowFreqSlider.getValue();
    } else if (slider == &fastFreqSlider) {
        *params.FastFreq = (float)fastFreqSlider.getValue();
    } else if (slider == &amountSlider) {
        *params.Amount = (float)amountSlider.getValue();
    }
}
void LfoComponent::timerCallback() {
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    body.setEnabled(params.Enabled->get());

    targetTypeSelector.setSelectedItemIndex(params.TargetType->getIndex(), juce::dontSendNotification);
    targetOscSelector.setSelectedItemIndex(params.TargetOsc->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setSelectedItemIndex(params.TargetFilter->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setSelectedItemIndex(params.TargetOscParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setSelectedItemIndex(params.TargetFilterParam->getIndex(), juce::dontSendNotification);
    waveformSelector.setSelectedItemIndex(params.Waveform->getIndex(), juce::dontSendNotification);

    slowFreqSlider.setValue(params.SlowFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setValue(params.FastFreq->get(), juce::dontSendNotification);
    amountSlider.setValue(params.Amount->get(), juce::dontSendNotification);
    switch (static_cast<LFO_TARGET_TYPE>(params.TargetType->getIndex())) {
        case LFO_TARGET_TYPE::OSC: {
            targetOscSelector.setVisible(true);
            targetFilterSelector.setVisible(false);
            targetOscParamSelector.setVisible(true);
            targetFilterParamSelector.setVisible(false);
            break;
        }
        case LFO_TARGET_TYPE::Filter: {
            targetOscSelector.setVisible(false);
            targetFilterSelector.setVisible(true);
            targetOscParamSelector.setVisible(false);
            targetFilterParamSelector.setVisible(true);
            break;
        }
    }
    if (static_cast<LFO_TARGET_TYPE>(params.TargetType->getIndex()) == LFO_TARGET_TYPE::OSC &&
        (static_cast<LFO_TARGET_OSC_PARAM>(params.TargetOscParam->getIndex()) == LFO_TARGET_OSC_PARAM::FM ||
         static_cast<LFO_TARGET_OSC_PARAM>(params.TargetOscParam->getIndex()) == LFO_TARGET_OSC_PARAM::AM)) {
        fastFreqSlider.setVisible(true);
        slowFreqSlider.setVisible(false);
    } else {
        fastFreqSlider.setVisible(false);
        slowFreqSlider.setVisible(true);
    }

    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    slowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(p.TargetType->getIndex());
        switch (targetType) {
            case CONTROL_TARGET_TYPE::LFO: {
                int targetIndex = p.TargetLfo->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_LFO_PARAM>(p.TargetLfoParam->getIndex());
                if (targetIndex == index) {
                    switch (targetParam) {
                        case CONTROL_TARGET_LFO_PARAM::Freq: {
                            fastFreqSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            slowFreqSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                        case CONTROL_TARGET_LFO_PARAM::Amount: {
                            amountSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                            break;
                        }
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
ModEnvComponent::ModEnvComponent(int index, ModEnvParams& params)
    : index(index),
      params(params),
      header("MOD ENV " + std::to_string(index + 1), true),
      targetTypeSelector("TargetType"),
      targetOscSelector("TargetOsc"),
      targetFilterSelector("TargetFilter"),
      targetOscParamSelector("TargetOscParam"),
      targetFilterParamSelector("TargetFilterParam"),
      peakFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      waitSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox),
      attackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      decaySlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                  juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    targetTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    targetTypeSelector.addItemList(params.TargetType->getAllValueStrings(), 1);
    targetTypeSelector.setSelectedItemIndex(params.TargetType->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setJustificationType(juce::Justification::centred);
    targetTypeSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetTypeSelector);

    targetOscSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscSelector.addItemList(params.TargetOsc->getAllValueStrings(), 1);
    targetOscSelector.setSelectedItemIndex(params.TargetOsc->getIndex(), juce::dontSendNotification);
    targetOscSelector.setJustificationType(juce::Justification::centred);
    targetOscSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetOscSelector);

    targetFilterSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterSelector.addItemList(params.TargetFilter->getAllValueStrings(), 1);
    targetFilterSelector.setSelectedItemIndex(params.TargetFilter->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setJustificationType(juce::Justification::centred);
    targetFilterSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetFilterSelector);

    targetLfoSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoSelector.addItemList(params.TargetLfo->getAllValueStrings(), 1);
    targetLfoSelector.setSelectedItemIndex(params.TargetLfo->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setJustificationType(juce::Justification::centred);
    targetLfoSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetLfoSelector);

    targetOscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscParamSelector.addItemList(params.TargetOscParam->getAllValueStrings(), 1);
    targetOscParamSelector.setSelectedItemIndex(params.TargetOscParam->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setJustificationType(juce::Justification::centred);
    targetOscParamSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetOscParamSelector);

    targetFilterParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterParamSelector.addItemList(params.TargetFilterParam->getAllValueStrings(), 1);
    targetFilterParamSelector.setSelectedItemIndex(params.TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setJustificationType(juce::Justification::centred);
    targetFilterParamSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetFilterParamSelector);

    targetLfoParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoParamSelector.addItemList(params.TargetLfoParam->getAllValueStrings(), 1);
    targetLfoParamSelector.setSelectedItemIndex(params.TargetLfoParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setJustificationType(juce::Justification::centred);
    targetLfoParamSelector.addListener(this);
    targetSelector.addAndMakeVisible(targetLfoParamSelector);

    fadeSelector.setLookAndFeel(&grapeLookAndFeel);
    fadeSelector.addItemList(params.Fade->getAllValueStrings(), 1);
    fadeSelector.setSelectedItemIndex(params.Fade->getIndex(), juce::dontSendNotification);
    fadeSelector.setJustificationType(juce::Justification::centred);
    fadeSelector.addListener(this);
    body.addAndMakeVisible(fadeSelector);

    peakFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    peakFreqSlider.setRange(params.PeakFreq->range.start, params.PeakFreq->range.end, 0.01);
    peakFreqSlider.setValue(params.PeakFreq->get(), juce::dontSendNotification);
    peakFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    peakFreqSlider.setScrollWheelEnabled(false);
    peakFreqSlider.textFromValueFunction = [](double oct) -> juce::String {
        return (oct == 0 ? " " : oct > 0 ? "+" : "-") + juce::String(std::abs(oct), 2) + " oct";
    };
    peakFreqSlider.addListener(this);
    body.addAndMakeVisible(peakFreqSlider);

    waitSlider.setLookAndFeel(&grapeLookAndFeel);
    waitSlider.setRange(params.Wait->range.start, params.Wait->range.end, 0.01);
    waitSlider.setSkewFactorFromMidPoint(0.2f);
    waitSlider.setValue(params.Wait->get(), juce::dontSendNotification);
    waitSlider.setPopupDisplayEnabled(true, true, nullptr);
    waitSlider.setScrollWheelEnabled(false);
    waitSlider.setTextValueSuffix(" sec");
    waitSlider.addListener(this);
    body.addAndMakeVisible(waitSlider);

    attackSlider.setLookAndFeel(&grapeLookAndFeel);
    attackSlider.setRange(params.Attack->range.start, params.Attack->range.end, 0.001);
    attackSlider.setSkewFactorFromMidPoint(0.2f);
    attackSlider.setValue(params.Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, nullptr);
    attackSlider.setScrollWheelEnabled(false);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    body.addAndMakeVisible(attackSlider);

    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(params.Decay->range.start, params.Decay->range.end, 0.01);
    decaySlider.setSkewFactorFromMidPoint(0.4f);
    decaySlider.setValue(params.Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, nullptr);
    decaySlider.setScrollWheelEnabled(false);
    decaySlider.setTextValueSuffix(" sec");
    decaySlider.addListener(this);
    body.addAndMakeVisible(decaySlider);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("Destination", juce::dontSendNotification);
    targetLabel.setJustificationType(juce::Justification::centred);
    targetLabel.setEditable(false, false, false);
    body.addAndMakeVisible(targetLabel);

    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);

    fadeLabel.setFont(paramLabelFont);
    fadeLabel.setText("Fade", juce::dontSendNotification);
    fadeLabel.setJustificationType(juce::Justification::centred);
    fadeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(fadeLabel);

    peakFreqLabel.setFont(paramLabelFont);
    peakFreqLabel.setText("Peak Freq", juce::dontSendNotification);
    peakFreqLabel.setJustificationType(juce::Justification::centred);
    peakFreqLabel.setEditable(false, false, false);
    body.addAndMakeVisible(peakFreqLabel);

    waitLabel.setFont(paramLabelFont);
    waitLabel.setText("Wait", juce::dontSendNotification);
    waitLabel.setJustificationType(juce::Justification::centred);
    waitLabel.setEditable(false, false, false);
    body.addAndMakeVisible(waitLabel);

    attackLabel.setFont(paramLabelFont);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.setEditable(false, false, false);
    body.addAndMakeVisible(attackLabel);

    decayLabel.setFont(paramLabelFont);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setEditable(false, false, false);
    body.addAndMakeVisible(decayLabel);

    body.addAndMakeVisible(targetSelector);
    addAndMakeVisible(body);

    startTimerHz(30.0f);
}

ModEnvComponent::~ModEnvComponent() {}

void ModEnvComponent::paint(juce::Graphics& g) {}

void ModEnvComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto& lowerArea = bounds;
    consumeLabeledComboBox(upperArea, 280, targetLabel, targetSelector);
    {
        auto selectorsArea = targetSelector.getLocalBounds();

        targetTypeSelector.setBounds(selectorsArea.removeFromLeft(90));
        auto indexArea = selectorsArea.removeFromLeft(70);
        targetOscSelector.setBounds(indexArea);
        targetFilterSelector.setBounds(indexArea);
        targetLfoSelector.setBounds(indexArea);
        auto paramArea = selectorsArea.removeFromLeft(110);
        targetOscParamSelector.setBounds(paramArea);
        targetFilterParamSelector.setBounds(paramArea);
        targetLfoParamSelector.setBounds(paramArea);
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        auto area2 = area;
        consumeLabeledKnob(area, peakFreqLabel, peakFreqSlider);
        consumeLabeledComboBox(area2, 120, fadeLabel, fadeSelector);
    }
    consumeLabeledKnob(lowerArea, waitLabel, waitSlider, attackLabel, attackSlider);
    consumeLabeledKnob(lowerArea, decayLabel, decaySlider);
}
void ModEnvComponent::buttonClicked(juce::Button* button) {
    if (button == &header.enabledButton) {
        *params.Enabled = header.enabledButton.getToggleState();
    }
}
void ModEnvComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &targetTypeSelector) {
        *params.TargetType = targetTypeSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetOscSelector) {
        *params.TargetOsc = targetOscSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetFilterSelector) {
        *params.TargetFilter = targetFilterSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetLfoSelector) {
        *params.TargetLfo = targetLfoSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetOscParamSelector) {
        *params.TargetOscParam = targetOscParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetFilterParamSelector) {
        *params.TargetFilterParam = targetFilterParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetLfoParamSelector) {
        *params.TargetLfoParam = targetLfoParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &fadeSelector) {
        *params.Fade = fadeSelector.getSelectedItemIndex();
    }
    resized();  // re-render
}
void ModEnvComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &peakFreqSlider) {
        *params.PeakFreq = (float)peakFreqSlider.getValue();
    } else if (slider == &waitSlider) {
        *params.Wait = (float)waitSlider.getValue();
    } else if (slider == &attackSlider) {
        *params.Attack = (float)attackSlider.getValue();
    } else if (slider == &decaySlider) {
        *params.Decay = (float)decaySlider.getValue();
    }
}
void ModEnvComponent::timerCallback() {
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    body.setEnabled(params.Enabled->get());

    targetTypeSelector.setSelectedItemIndex(params.TargetType->getIndex(), juce::dontSendNotification);
    targetOscSelector.setSelectedItemIndex(params.TargetOsc->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setSelectedItemIndex(params.TargetFilter->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setSelectedItemIndex(params.TargetLfo->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setSelectedItemIndex(params.TargetOscParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setSelectedItemIndex(params.TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setSelectedItemIndex(params.TargetLfoParam->getIndex(), juce::dontSendNotification);
    fadeSelector.setSelectedItemIndex(params.Fade->getIndex(), juce::dontSendNotification);

    peakFreqSlider.setValue(params.PeakFreq->get(), juce::dontSendNotification);
    waitSlider.setValue(params.Wait->get(), juce::dontSendNotification);
    attackSlider.setValue(params.Attack->get(), juce::dontSendNotification);
    decaySlider.setValue(params.Decay->get(), juce::dontSendNotification);
    switch (static_cast<MODENV_TARGET_TYPE>(params.TargetType->getIndex())) {
        case MODENV_TARGET_TYPE::OSC: {
            targetOscSelector.setVisible(true);
            targetFilterSelector.setVisible(false);
            targetLfoSelector.setVisible(false);
            targetOscParamSelector.setVisible(true);
            targetFilterParamSelector.setVisible(false);
            targetLfoParamSelector.setVisible(false);
            break;
        }
        case MODENV_TARGET_TYPE::Filter: {
            targetOscSelector.setVisible(false);
            targetFilterSelector.setVisible(true);
            targetLfoSelector.setVisible(false);
            targetOscParamSelector.setVisible(false);
            targetFilterParamSelector.setVisible(true);
            targetLfoParamSelector.setVisible(false);
            break;
        }
        case MODENV_TARGET_TYPE::LFO: {
            targetOscSelector.setVisible(false);
            targetFilterSelector.setVisible(false);
            targetLfoSelector.setVisible(true);
            targetOscParamSelector.setVisible(false);
            targetFilterParamSelector.setVisible(false);
            targetLfoParamSelector.setVisible(true);
            break;
        }
    }
    if (params.isTargetFreq()) {
        // 1
        peakFreqLabel.setVisible(true);
        fadeLabel.setVisible(false);
        peakFreqSlider.setVisible(true);
        fadeSelector.setVisible(false);
        // 2
        waitLabel.setVisible(false);
        attackLabel.setVisible(true);
        waitSlider.setVisible(false);
        attackSlider.setVisible(true);
    } else {
        // 1
        peakFreqLabel.setVisible(false);
        fadeLabel.setVisible(true);
        peakFreqSlider.setVisible(false);
        fadeSelector.setVisible(true);
        // 2
        if (static_cast<MODENV_FADE>(params.Fade->getIndex()) == MODENV_FADE::In) {
            waitLabel.setVisible(true);
            attackLabel.setVisible(false);
            waitSlider.setVisible(true);
            attackSlider.setVisible(false);
        } else {
            waitLabel.setVisible(false);
            attackLabel.setVisible(true);
            waitSlider.setVisible(false);
            attackSlider.setVisible(true);
        }
    }
}

//==============================================================================
DelayComponent::DelayComponent(DelayParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : params(params),
      controlItemParams(controlItemParams),
      header("DELAY", true),
      typeSelector("Type"),
      timeLSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                  juce::Slider::TextEntryBoxPosition::NoTextBox),
      timeRSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                  juce::Slider::TextEntryBoxPosition::NoTextBox),
      timeSyncLSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                      juce::Slider::TextEntryBoxPosition::NoTextBox),
      timeSyncRSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                      juce::Slider::TextEntryBoxPosition::NoTextBox),
      lowFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                    juce::Slider::TextEntryBoxPosition::NoTextBox),
      highFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      feedbackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      mixSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                juce::Slider::TextEntryBoxPosition::NoTextBox) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    typeSelector.setLookAndFeel(&grapeLookAndFeel);
    typeSelector.addItemList(params.Type->getAllValueStrings(), 1);
    typeSelector.setSelectedItemIndex(params.Type->getIndex(), juce::dontSendNotification);
    typeSelector.setJustificationType(juce::Justification::centred);
    typeSelector.addListener(this);
    body.addAndMakeVisible(typeSelector);

    syncSelector.setLookAndFeel(&grapeLookAndFeel);
    syncSelector.addItemList(params.Sync->getAllValueStrings(), 1);
    syncSelector.setSelectedItemIndex(params.Sync->get(), juce::dontSendNotification);
    syncSelector.setJustificationType(juce::Justification::centred);
    syncSelector.addListener(this);
    body.addAndMakeVisible(syncSelector);

    timeLSlider.setLookAndFeel(&grapeLookAndFeel);
    timeLSlider.setRange(params.TimeL->range.start, params.TimeL->range.end, 0.01);
    timeLSlider.setSkewFactorFromMidPoint(0.4f);
    timeLSlider.setValue(params.TimeL->get(), juce::dontSendNotification);
    timeLSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeLSlider.setScrollWheelEnabled(false);
    timeLSlider.setTextValueSuffix(" sec");
    timeLSlider.addListener(this);
    body.addAndMakeVisible(timeLSlider);

    timeRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeRSlider.setRange(params.TimeR->range.start, params.TimeR->range.end, 0.01);
    timeRSlider.setSkewFactorFromMidPoint(0.4f);
    timeRSlider.setValue(params.TimeR->get(), juce::dontSendNotification);
    timeRSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeRSlider.setScrollWheelEnabled(false);
    timeRSlider.setTextValueSuffix(" sec");
    timeRSlider.addListener(this);
    body.addAndMakeVisible(timeRSlider);

    timeSyncLSlider.setLookAndFeel(&grapeLookAndFeel);
    timeSyncLSlider.setRange(0, DELAY_TIME_SYNC_NAMES.size() - 1, 1);
    timeSyncLSlider.setValue(params.TimeSyncL->getIndex(), juce::dontSendNotification);
    timeSyncLSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeSyncLSlider.setScrollWheelEnabled(false);
    timeSyncLSlider.textFromValueFunction = [](double index) { return DELAY_TIME_SYNC_NAMES[index]; };
    timeSyncLSlider.addListener(this);
    body.addAndMakeVisible(timeSyncLSlider);

    timeSyncRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeSyncRSlider.setRange(0, DELAY_TIME_SYNC_NAMES.size() - 1, 1);
    timeSyncRSlider.setValue(params.TimeSyncR->getIndex(), juce::dontSendNotification);
    timeSyncRSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeSyncRSlider.setScrollWheelEnabled(false);
    timeSyncRSlider.textFromValueFunction = [](double index) { return DELAY_TIME_SYNC_NAMES[index]; };
    timeSyncRSlider.addListener(this);
    body.addAndMakeVisible(timeSyncRSlider);

    lowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    lowFreqSlider.setRange(params.LowFreq->range.start, params.LowFreq->range.end, 1.0);
    lowFreqSlider.setSkewFactorFromMidPoint(2000.0f);
    lowFreqSlider.setValue(params.LowFreq->get(), juce::dontSendNotification);
    lowFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    lowFreqSlider.setScrollWheelEnabled(false);
    lowFreqSlider.setTextValueSuffix(" Hz");
    lowFreqSlider.addListener(this);
    body.addAndMakeVisible(lowFreqSlider);

    highFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    highFreqSlider.setRange(params.HighFreq->range.start, params.HighFreq->range.end, 1.0);
    highFreqSlider.setSkewFactorFromMidPoint(2000.0f);
    highFreqSlider.setValue(params.HighFreq->get(), juce::dontSendNotification);
    highFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    highFreqSlider.setScrollWheelEnabled(false);
    highFreqSlider.setTextValueSuffix(" Hz");
    highFreqSlider.addListener(this);
    body.addAndMakeVisible(highFreqSlider);

    feedbackSlider.setLookAndFeel(&grapeLookAndFeel);
    feedbackSlider.setRange(params.Feedback->range.start, params.Feedback->range.end, 0.01);
    feedbackSlider.setSkewFactorFromMidPoint(0.4f);
    feedbackSlider.setValue(params.Feedback->get(), juce::dontSendNotification);
    feedbackSlider.setPopupDisplayEnabled(true, true, nullptr);
    feedbackSlider.setScrollWheelEnabled(false);
    feedbackSlider.textFromValueFunction = [](double gain) { return juce::String(gain * 100, 0) + " %"; };
    feedbackSlider.addListener(this);
    body.addAndMakeVisible(feedbackSlider);

    mixSlider.setLookAndFeel(&grapeLookAndFeel);
    mixSlider.setRange(params.Mix->range.start, params.Mix->range.end, 0.01);
    mixSlider.setValue(params.Mix->get(), juce::dontSendNotification);
    mixSlider.setPopupDisplayEnabled(true, true, nullptr);
    mixSlider.setScrollWheelEnabled(false);
    mixSlider.addListener(this);
    body.addAndMakeVisible(mixSlider);

    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);

    syncLabel.setFont(paramLabelFont);
    syncLabel.setText("Sync", juce::dontSendNotification);
    syncLabel.setJustificationType(juce::Justification::centred);
    syncLabel.setEditable(false, false, false);
    body.addAndMakeVisible(syncLabel);

    timeLLabel.setFont(paramLabelFont);
    timeLLabel.setText("Time L", juce::dontSendNotification);
    timeLLabel.setJustificationType(juce::Justification::centred);
    timeLLabel.setEditable(false, false, false);
    body.addAndMakeVisible(timeLLabel);

    timeRLabel.setFont(paramLabelFont);
    timeRLabel.setText("Time R", juce::dontSendNotification);
    timeRLabel.setJustificationType(juce::Justification::centred);
    timeRLabel.setEditable(false, false, false);
    body.addAndMakeVisible(timeRLabel);

    lowFreqLabel.setFont(paramLabelFont);
    lowFreqLabel.setText("Lo Cut", juce::dontSendNotification);
    lowFreqLabel.setJustificationType(juce::Justification::centred);
    lowFreqLabel.setEditable(false, false, false);
    body.addAndMakeVisible(lowFreqLabel);

    highFreqLabel.setFont(paramLabelFont);
    highFreqLabel.setText("Hi Cut", juce::dontSendNotification);
    highFreqLabel.setJustificationType(juce::Justification::centred);
    highFreqLabel.setEditable(false, false, false);
    body.addAndMakeVisible(highFreqLabel);

    feedbackLabel.setFont(paramLabelFont);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setEditable(false, false, false);
    body.addAndMakeVisible(feedbackLabel);

    mixLabel.setFont(paramLabelFont);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setEditable(false, false, false);
    body.addAndMakeVisible(mixLabel);

    addAndMakeVisible(body);

    startTimerHz(30.0f);
}

DelayComponent::~DelayComponent() {}

void DelayComponent::paint(juce::Graphics& g) {}

void DelayComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);

    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto& lowerArea = bounds;
    consumeLabeledComboBox(upperArea, 120, typeLabel, typeSelector);
    consumeLabeledComboBox(upperArea, 80, syncLabel, syncSelector);
    consumeLabeledKnob(upperArea, lowFreqLabel, lowFreqSlider);
    consumeLabeledKnob(upperArea, highFreqLabel, highFreqSlider);
    consumeLabeledKnob(lowerArea, timeLLabel, timeLSlider, timeSyncLSlider);
    consumeLabeledKnob(lowerArea, timeRLabel, timeRSlider, timeSyncRSlider);
    consumeLabeledKnob(lowerArea, feedbackLabel, feedbackSlider);
    consumeLabeledKnob(lowerArea, mixLabel, mixSlider);
}
void DelayComponent::buttonClicked(juce::Button* button) {
    if (button == &header.enabledButton) {
        *params.Enabled = header.enabledButton.getToggleState();
    }
}
void DelayComponent::comboBoxChanged(juce::ComboBox* comboBox) {
    if (comboBox == &typeSelector) {
        *params.Type = typeSelector.getSelectedItemIndex();
    } else if (comboBox == &syncSelector) {
        *params.Sync = syncSelector.getSelectedItemIndex();
    }
}
void DelayComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &timeLSlider) {
        *params.TimeL = (float)timeLSlider.getValue();
    } else if (slider == &timeRSlider) {
        *params.TimeR = (float)timeRSlider.getValue();
    } else if (slider == &timeSyncLSlider) {
        *params.TimeSyncL = timeSyncLSlider.getValue();
    } else if (slider == &timeSyncRSlider) {
        *params.TimeSyncR = timeSyncRSlider.getValue();
    } else if (slider == &lowFreqSlider) {
        *params.LowFreq = (float)lowFreqSlider.getValue();
    } else if (slider == &highFreqSlider) {
        *params.HighFreq = (float)highFreqSlider.getValue();
    } else if (slider == &feedbackSlider) {
        *params.Feedback = (float)feedbackSlider.getValue();
    } else if (slider == &mixSlider) {
        *params.Mix = (float)mixSlider.getValue();
    }
}
void DelayComponent::timerCallback() {
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    body.setEnabled(params.Enabled->get());

    typeSelector.setSelectedItemIndex(params.Type->getIndex(), juce::dontSendNotification);
    syncSelector.setSelectedItemIndex(params.Sync->get(), juce::dontSendNotification);
    timeLSlider.setVisible(!params.Sync->get());
    timeRSlider.setVisible(!params.Sync->get());
    timeSyncLSlider.setVisible(params.Sync->get());
    timeSyncRSlider.setVisible(params.Sync->get());
    timeLSlider.setValue(params.TimeL->get(), juce::dontSendNotification);
    timeRSlider.setValue(params.TimeR->get(), juce::dontSendNotification);
    timeSyncLSlider.setValue(params.TimeSyncL->getIndex(), juce::dontSendNotification);
    timeSyncRSlider.setValue(params.TimeSyncR->getIndex(), juce::dontSendNotification);

    lowFreqSlider.setValue(params.LowFreq->get(), juce::dontSendNotification);
    highFreqSlider.setValue(params.HighFreq->get(), juce::dontSendNotification);
    feedbackSlider.setValue(params.Feedback->get(), juce::dontSendNotification);
    mixSlider.setValue(params.Mix->get(), juce::dontSendNotification);

    mixSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(p.TargetType->getIndex());
        switch (targetType) {
            case CONTROL_TARGET_TYPE::Master: {
                auto targetParam = static_cast<CONTROL_TARGET_MISC_PARAM>(p.TargetMiscParam->getIndex());
                switch (targetParam) {
                    case CONTROL_TARGET_MISC_PARAM::DelayMix: {
                        mixSlider.setLookAndFeel(&grapeLookAndFeelControlled);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
ControlItemComponent::ControlItemComponent(ControlItemParams& params)
    : params(params),
      numberSelector("Number"),
      targetTypeSelector("TargetType"),
      targetOscSelector("TargetOsc"),
      targetFilterSelector("TargetFilter"),
      targetOscParamSelector("TargetOscParam"),
      targetFilterParamSelector("TargetFilterParam"),
      targetMiscParamSelector("TargetMiscParam") {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    numberSelector.setLookAndFeel(&grapeLookAndFeel);
    numberSelector.addItemList(params.Number->getAllValueStrings(), 1);
    numberSelector.setSelectedItemIndex(params.Number->getIndex(), juce::dontSendNotification);
    numberSelector.setJustificationType(juce::Justification::centred);
    numberSelector.addListener(this);
    addAndMakeVisible(numberSelector);

    targetTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    targetTypeSelector.addItemList(params.TargetType->getAllValueStrings(), 1);
    targetTypeSelector.setSelectedItemIndex(params.TargetType->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setJustificationType(juce::Justification::centred);
    targetTypeSelector.addListener(this);
    addAndMakeVisible(targetTypeSelector);

    targetOscSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscSelector.addItemList(params.TargetOsc->getAllValueStrings(), 1);
    targetOscSelector.setSelectedItemIndex(params.TargetOsc->getIndex(), juce::dontSendNotification);
    targetOscSelector.setJustificationType(juce::Justification::centred);
    targetOscSelector.addListener(this);
    addAndMakeVisible(targetOscSelector);

    targetFilterSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterSelector.addItemList(params.TargetFilter->getAllValueStrings(), 1);
    targetFilterSelector.setSelectedItemIndex(params.TargetFilter->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setJustificationType(juce::Justification::centred);
    targetFilterSelector.addListener(this);
    addAndMakeVisible(targetFilterSelector);

    targetLfoSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoSelector.addItemList(params.TargetLfo->getAllValueStrings(), 1);
    targetLfoSelector.setSelectedItemIndex(params.TargetLfo->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setJustificationType(juce::Justification::centred);
    targetLfoSelector.addListener(this);
    addAndMakeVisible(targetLfoSelector);

    targetOscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscParamSelector.addItemList(params.TargetOscParam->getAllValueStrings(), 1);
    targetOscParamSelector.setSelectedItemIndex(params.TargetOscParam->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setJustificationType(juce::Justification::centred);
    targetOscParamSelector.addListener(this);
    addAndMakeVisible(targetOscParamSelector);

    targetFilterParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterParamSelector.addItemList(params.TargetFilterParam->getAllValueStrings(), 1);
    targetFilterParamSelector.setSelectedItemIndex(params.TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setJustificationType(juce::Justification::centred);
    targetFilterParamSelector.addListener(this);
    addAndMakeVisible(targetFilterParamSelector);

    targetLfoParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoParamSelector.addItemList(params.TargetLfoParam->getAllValueStrings(), 1);
    targetLfoParamSelector.setSelectedItemIndex(params.TargetLfoParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setJustificationType(juce::Justification::centred);
    targetLfoParamSelector.addListener(this);
    addAndMakeVisible(targetLfoParamSelector);

    targetMiscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetMiscParamSelector.addItemList(params.TargetMiscParam->getAllValueStrings(), 1);
    targetMiscParamSelector.setSelectedItemIndex(params.TargetMiscParam->getIndex(), juce::dontSendNotification);
    targetMiscParamSelector.setJustificationType(juce::Justification::centred);
    targetMiscParamSelector.addListener(this);
    addAndMakeVisible(targetMiscParamSelector);

    startTimerHz(30.0f);
}

ControlItemComponent::~ControlItemComponent() {}

void ControlItemComponent::paint(juce::Graphics& g) {}

void ControlItemComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    {
        juce::Rectangle<int> area =
            bounds.removeFromLeft(bounds.getWidth() / 5).removeFromTop(COMBO_BOX_HEIGHT).reduced(LOCAL_MARGIN);
        numberSelector.setBounds(area);
    }
    {
        juce::Rectangle<int> area = bounds.removeFromTop(COMBO_BOX_HEIGHT).reduced(LOCAL_MARGIN);

        targetTypeSelector.setBounds(area.removeFromLeft(90));

        targetMiscParamSelector.setBounds(area);

        auto indexArea = area.removeFromLeft(70);
        targetOscSelector.setBounds(indexArea);
        targetFilterSelector.setBounds(indexArea);
        targetLfoSelector.setBounds(indexArea);

        auto paramArea = area;
        targetOscParamSelector.setBounds(paramArea);
        targetFilterParamSelector.setBounds(paramArea);
        targetLfoParamSelector.setBounds(paramArea);
    }
}
void ControlItemComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &numberSelector) {
        *params.Number = numberSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetTypeSelector) {
        *params.TargetType = targetTypeSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetOscSelector) {
        *params.TargetOsc = targetOscSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetFilterSelector) {
        *params.TargetFilter = targetFilterSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetLfoSelector) {
        *params.TargetLfo = targetLfoSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetOscParamSelector) {
        *params.TargetOscParam = targetOscParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetFilterParamSelector) {
        *params.TargetFilterParam = targetFilterParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetLfoParamSelector) {
        *params.TargetLfoParam = targetLfoParamSelector.getSelectedItemIndex();
    } else if (comboBoxThatHasChanged == &targetMiscParamSelector) {
        *params.TargetMiscParam = targetMiscParamSelector.getSelectedItemIndex();
    }
    resized();  // re-render
}
void ControlItemComponent::timerCallback() {
    numberSelector.setSelectedItemIndex(params.Number->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setSelectedItemIndex(params.TargetType->getIndex(), juce::dontSendNotification);
    targetOscSelector.setSelectedItemIndex(params.TargetOsc->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setSelectedItemIndex(params.TargetFilter->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setSelectedItemIndex(params.TargetLfo->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setSelectedItemIndex(params.TargetOscParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setSelectedItemIndex(params.TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setSelectedItemIndex(params.TargetLfoParam->getIndex(), juce::dontSendNotification);
    targetMiscParamSelector.setSelectedItemIndex(params.TargetMiscParam->getIndex(), juce::dontSendNotification);
    auto enabled = params.Number->getIndex() != 0;
    targetTypeSelector.setEnabled(enabled);
    targetOscSelector.setEnabled(enabled);
    targetFilterSelector.setEnabled(enabled);
    targetLfoSelector.setEnabled(enabled);
    targetOscParamSelector.setEnabled(enabled);
    targetFilterParamSelector.setEnabled(enabled);
    targetLfoParamSelector.setEnabled(enabled);
    targetMiscParamSelector.setEnabled(enabled);
    switch (static_cast<CONTROL_TARGET_TYPE>(params.TargetType->getIndex())) {
        case CONTROL_TARGET_TYPE::OSC: {
            targetOscSelector.setVisible(true);
            targetFilterSelector.setVisible(false);
            targetLfoSelector.setVisible(false);
            targetOscParamSelector.setVisible(true);
            targetFilterParamSelector.setVisible(false);
            targetLfoParamSelector.setVisible(false);
            targetMiscParamSelector.setVisible(false);
            break;
        }
        case CONTROL_TARGET_TYPE::Filter: {
            targetOscSelector.setVisible(false);
            targetFilterSelector.setVisible(true);
            targetLfoSelector.setVisible(false);
            targetOscParamSelector.setVisible(false);
            targetFilterParamSelector.setVisible(true);
            targetLfoParamSelector.setVisible(false);
            targetMiscParamSelector.setVisible(false);
            break;
        }
        case CONTROL_TARGET_TYPE::LFO: {
            targetOscSelector.setVisible(false);
            targetFilterSelector.setVisible(false);
            targetLfoSelector.setVisible(true);
            targetOscParamSelector.setVisible(false);
            targetFilterParamSelector.setVisible(false);
            targetLfoParamSelector.setVisible(true);
            targetMiscParamSelector.setVisible(false);
            break;
        }
        case CONTROL_TARGET_TYPE::Master: {
            targetOscSelector.setVisible(false);
            targetFilterSelector.setVisible(false);
            targetLfoSelector.setVisible(false);
            targetOscParamSelector.setVisible(false);
            targetFilterParamSelector.setVisible(false);
            targetLfoParamSelector.setVisible(false);
            targetMiscParamSelector.setVisible(true);
            break;
        }
    }
}

//==============================================================================
ControlComponent::ControlComponent(std::array<ControlItemParams, NUM_CONTROL>& params)
    : header("CONTROLS", false),
      controlItemComponents{ControlItemComponent(params[0]),
                            ControlItemComponent(params[1]),
                            ControlItemComponent(params[2]),
                            ControlItemComponent(params[3]),
                            ControlItemComponent(params[4]),
                            ControlItemComponent(params[5])} {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    numberLabel.setFont(paramLabelFont);
    numberLabel.setText("CC", juce::dontSendNotification);
    numberLabel.setJustificationType(juce::Justification::centred);
    numberLabel.setEditable(false, false, false);
    addAndMakeVisible(numberLabel);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("Destination", juce::dontSendNotification);
    targetLabel.setJustificationType(juce::Justification::centred);
    targetLabel.setEditable(false, false, false);
    addAndMakeVisible(targetLabel);

    for (int i = 0; i < NUM_CONTROL; i++) {
        addAndMakeVisible(controlItemComponents[i]);
    }
}

ControlComponent::~ControlComponent() {}

void ControlComponent::paint(juce::Graphics& g) {}

void ControlComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto width = bounds.getWidth();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    auto labelArea = bounds.removeFromTop(LABEL_HEIGHT);
    numberLabel.setBounds(labelArea.removeFromLeft(width / 5));
    targetLabel.setBounds(labelArea);

    auto itemHeight = bounds.getHeight() / NUM_CONTROL;
    for (int i = 0; i < NUM_CONTROL; i++) {
        controlItemComponents[i].setBounds(bounds.removeFromTop(itemHeight));
    }
}

//==============================================================================
AnalyserToggleItem::AnalyserToggleItem(std::string name) {
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    nameLabel.setFont(paramLabelFont);
    nameLabel.setText(name, juce::dontSendNotification);
    nameLabel.setJustificationType(juce::Justification::right);
    nameLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(nameLabel);
}
AnalyserToggleItem::~AnalyserToggleItem() {}
void AnalyserToggleItem::paint(juce::Graphics& g) {
    juce::Rectangle<int> bounds = getLocalBounds().removeFromRight(3).reduced(0, 4);

    auto color = value ? colour::SELECT : colour::PIT;
    g.setColour(color);
    g.fillRect(bounds);
}
void AnalyserToggleItem::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    bounds.removeFromRight(5);
    nameLabel.setBounds(bounds);
}
void AnalyserToggleItem::addListener(Listener* l) { listeners.add(l); }
void AnalyserToggleItem::mouseUp(const juce::MouseEvent& e) {
    std::cout << "mouseup:" << nameLabel.getText() << std::endl;
    Component::BailOutChecker checker(this);
    //    if (checker.shouldBailOut()) {
    //        return;
    //    }
    if (e.mouseWasClicked()) {
        if (!value) {
            value = true;
            listeners.callChecked(checker, [this](AnalyserToggleItem::Listener& l) { l.toggleItemSelected(this); });
        }
    }
}

//==============================================================================
AnalyserToggle::AnalyserToggle(ANALYSER_MODE* analyserMode)
    : analyserMode(analyserMode), spectrumToggle("Spectrum"), envelopeToggle("Envelope"), filterToggle("Filter") {
    spectrumToggle.addListener(this);
    addAndMakeVisible(spectrumToggle);

    envelopeToggle.addListener(this);
    addAndMakeVisible(envelopeToggle);

    filterToggle.addListener(this);
    addAndMakeVisible(filterToggle);

    spectrumToggle.setValue(*analyserMode == ANALYSER_MODE::Spectrum);
    envelopeToggle.setValue(*analyserMode == ANALYSER_MODE::Envelope);
    filterToggle.setValue(*analyserMode == ANALYSER_MODE::Filter);
}
AnalyserToggle::~AnalyserToggle() {}
void AnalyserToggle::paint(juce::Graphics& g) {}
void AnalyserToggle::resized() {
    juce::Rectangle<int> bounds = getLocalBounds().reduced(2, 4);
    spectrumToggle.setBounds(bounds.removeFromTop(25));
    envelopeToggle.setBounds(bounds.removeFromTop(25));
    filterToggle.setBounds(bounds.removeFromTop(25));
}
void AnalyserToggle::toggleItemSelected(AnalyserToggleItem* toggleItem) {
    if (toggleItem == &spectrumToggle) {
        *analyserMode = ANALYSER_MODE::Spectrum;
    } else if (toggleItem == &envelopeToggle) {
        *analyserMode = ANALYSER_MODE::Envelope;
    } else if (toggleItem == &filterToggle) {
        *analyserMode = ANALYSER_MODE::Filter;
    }
    spectrumToggle.setValue(*analyserMode == ANALYSER_MODE::Spectrum);
    envelopeToggle.setValue(*analyserMode == ANALYSER_MODE::Envelope);
    filterToggle.setValue(*analyserMode == ANALYSER_MODE::Filter);
}

//==============================================================================
AnalyserWindow::AnalyserWindow(ANALYSER_MODE* analyserMode,
                               LatestDataProvider* latestDataProvider,
                               MonoStack* monoStack,
                               std::array<EnvelopeParams, NUM_ENVELOPE>& envelopeParams,
                               std::array<OscParams, NUM_OSC>& oscParams,
                               std::array<FilterParams, NUM_FILTER>& filterParams,
                               std::array<ModEnvParams, NUM_MODENV>& modEnvParams)
    : analyserMode(analyserMode),
      latestDataProvider(latestDataProvider),
      monoStack(monoStack),
      envelopeParams(envelopeParams),
      oscParams(oscParams),
      filterParams(filterParams),
      modEnvParams(modEnvParams),
      forwardFFT(fftOrder),
      window(fftSize, juce::dsp::WindowingFunction<float>::hann),
      lastAmpEnvParams{SimpleAmpEnvParams(), SimpleAmpEnvParams()},
      lastModEnvParams{SimpleModEnvParams(), SimpleModEnvParams(), SimpleModEnvParams()} {
    latestDataProvider->addConsumer(&fftConsumer);
    latestDataProvider->addConsumer(&levelConsumer);

    startTimerHz(30.0f);
}
AnalyserWindow::~AnalyserWindow() {
    latestDataProvider->removeConsumer(&fftConsumer);
    latestDataProvider->removeConsumer(&levelConsumer);
}

void AnalyserWindow::resized() {}
void AnalyserWindow::timerCallback() {
    stopTimer();
    bool shouldRepaint = false;

    switch (*analyserMode) {
        case ANALYSER_MODE::Spectrum: {
            lastAnalyserMode = ANALYSER_MODE::Spectrum;
            if (fftConsumer.ready) {
                auto hasData = drawNextFrameOfSpectrum();
                fftConsumer.ready = false;
                readyToDrawFrame = true;
                shouldRepaint = shouldRepaint || hasData;
            }
            if (levelConsumer.ready) {
                auto hasData = drawNextFrameOfLevel();
                levelConsumer.ready = false;
                //        readyToDrawFrame = true;
                shouldRepaint = shouldRepaint || hasData;
            }
            break;
        }
        case ANALYSER_MODE::Envelope: {
            if (lastAnalyserMode != ANALYSER_MODE::Envelope) {
                shouldRepaint = true;
            }
            lastAnalyserMode = ANALYSER_MODE::Envelope;
            auto changed = false;
            for (int i = 0; i < NUM_ENVELOPE; i++) {
                auto p = SimpleAmpEnvParams(envelopeParams[i]);
                if (!lastAmpEnvParams[i].equals(p)) {
                    changed = true;
                }
                lastAmpEnvParams[i] = p;
            }
            for (int i = 0; i < NUM_MODENV; i++) {
                auto p = SimpleModEnvParams(modEnvParams[i]);
                if (!lastModEnvParams[i].equals(p)) {
                    changed = true;
                }
                lastModEnvParams[i] = p;
            }
            if (!changed) {
                break;
            }
            auto maxAD = std::max(
                {envelopeParams[0].Attack->get() + envelopeParams[0].Decay->get() * 4,
                 envelopeParams[1].Attack->get() + envelopeParams[1].Decay->get() * 4,
                 (modEnvParams[0].shouldUseHold() ? modEnvParams[0].Wait->get() : modEnvParams[0].Attack->get()) +
                     modEnvParams[0].Decay->get() * 4,
                 (modEnvParams[1].shouldUseHold() ? modEnvParams[1].Wait->get() : modEnvParams[1].Attack->get()) +
                     modEnvParams[1].Decay->get() * 4,
                 (modEnvParams[2].shouldUseHold() ? modEnvParams[2].Wait->get() : modEnvParams[2].Attack->get()) +
                     modEnvParams[2].Decay->get() * 4});
            auto maxR = std::max({envelopeParams[0].Release->get() * 4, envelopeParams[1].Release->get() * 4});
            auto maxSec = maxAD + maxR;
            auto sampleRate = (float)scopeSize / maxSec;
            for (int i = 0; i < NUM_ENVELOPE; i++) {
                ampEnvs[i].setParams(envelopeParams[i].Attack->get(),
                                     0,
                                     envelopeParams[i].Decay->get(),
                                     envelopeParams[i].Sustain->get(),
                                     envelopeParams[i].Release->get());
            }
            for (int i = 0; i < NUM_MODENV; i++) {
                if (modEnvParams[i].shouldUseHold()) {
                    modEnvs[i].setParams(0.0, modEnvParams[i].Wait->get(), modEnvParams[i].Decay->get(), 0.0, 0.0);
                } else {
                    modEnvs[i].setParams(modEnvParams[i].Attack->get(), 0.0, modEnvParams[i].Decay->get(), 0.0, 0.0);
                }
            }
            for (int i = 0; i < NUM_ENVELOPE; i++) {
                ampEnvs[i].doAttack(sampleRate);
            }
            for (int i = 0; i < NUM_MODENV; i++) {
                modEnvs[i].doAttack(sampleRate);
            }
            int releasePoint = sampleRate * maxAD;
            for (int pos = 0; pos < scopeSize; pos++) {
                if (pos == releasePoint) {
                    for (int i = 0; i < NUM_ENVELOPE; i++) {
                        ampEnvs[i].doRelease(sampleRate);
                    }
                }
                for (int i = 0; i < NUM_ENVELOPE; i++) {
                    scopeDataForAmpEnv[i][pos] = ampEnvs[i].getValue();
                    ampEnvs[i].step(sampleRate);
                }
                for (int i = 0; i < NUM_MODENV; i++) {
                    auto value = 0.0f;
                    if (modEnvParams[i].Enabled->get()) {
                        value = modEnvs[i].getValue();
                        if (!modEnvParams[i].isTargetFreq() &&
                            static_cast<MODENV_FADE>(modEnvParams[i].Fade->getIndex()) == MODENV_FADE::In) {
                            value = 1 - value;
                        }
                    }
                    scopeDataForModEnv[i][pos] = value;
                    modEnvs[i].step(sampleRate);
                }
            }
            for (int i = 0; i < NUM_ENVELOPE; i++) {
                ampEnvs[i].forceStop();
            }
            for (int i = 0; i < NUM_MODENV; i++) {
                modEnvs[i].forceStop();
            }
            readyToDrawFrame = true;
            shouldRepaint = true;
            break;
        }
        case ANALYSER_MODE::Filter: {
            if (lastAnalyserMode != ANALYSER_MODE::Filter) {
                shouldRepaint = true;
            }
            lastAnalyserMode = ANALYSER_MODE::Filter;

            bool noteChanged = false;
            if (monoStack->latestNoteNumber != 0) {
                auto newNoteNumber = monoStack->latestNoteNumber;
                noteChanged = relNoteNumber != newNoteNumber;
                relNoteNumber = newNoteNumber;
            }
            for (int i = 0; i < NUM_FILTER; ++i) {
                auto filterType = static_cast<FILTER_TYPE>(filterParams[i].Type->getIndex());
                double freq;
                bool isRel = false;
                switch (static_cast<FILTER_FREQ_TYPE>(filterParams[i].FreqType->getIndex())) {
                    case FILTER_FREQ_TYPE::Absolute: {
                        freq = filterParams[i].Hz->get();
                        break;
                    }
                    case FILTER_FREQ_TYPE::Relative: {
                        isRel = true;
                        auto target = filterParams[i].Target->getIndex();
                        float lastNote = relNoteNumber;
                        if (target != NUM_OSC) {
                            lastNote += oscParams[target].Octave->get() * 12;
                            lastNote += oscParams[target].Coarse->get();
                        }
                        lastNote += filterParams[i].Semitone->get();
                        freq = 440.0 * std::pow(2.0, (float)(lastNote - 69) / 12);
                        break;
                    }
                }
                auto q = filterParams[i].Q->get();
                auto gain = filterParams[i].Gain->get();

                auto params = SimpleFilterParams();
                params.enabled = filterParams[i].Enabled->get();
                params.type = filterParams[i].Type->getIndex();
                params.freq = freq;
                params.q = q;
                params.gain = gain;
                if (lastFilterParams[i].equals(params) && !(isRel && noteChanged)) {
                    continue;
                }
                lastFilterParams[i] = params;

                readyToDrawFrame = true;
                shouldRepaint = true;

                if (!filterParams[i].Enabled->get()) {
                    continue;
                }
                auto& filter = filters[i];
                auto sampleRate = 48000;  // TODO: ?
                filter.setSampleRate(sampleRate);
                filter.initializePastData();
                std::fill_n(filterSource, fftSize * 2, 0);
                filterSource[0] = 1;
                for (int i = 0; i < fftSize; i++) {
                    filterSource[i] = filter.step(filterType, freq, q, gain, 0, filterSource[i]);
                }
                forwardFFT.performFrequencyOnlyForwardTransform(filterSource);

                auto minFreq = 40.0f;
                auto maxFreq = 20000.0f;
                auto mindB = -50.0f;
                auto maxdB = 50.0f;
                for (int j = 0; j < scopeSize; ++j) {
                    float hz = xToHz(minFreq, maxFreq, (float)j / scopeSize);
                    float gain = getFFTDataByHz(filterSource, fftSize, sampleRate, hz);
                    auto level = juce::jmap(juce::Decibels::gainToDecibels(gain), mindB, maxdB, 0.0f, 1.0f);
                    scopeDataForFilter[i][j] = level;
                }
            }
            break;
        }
    }
    startTimerHz(30.0f);
    if (shouldRepaint) {
        repaint();
    }
}

bool AnalyserWindow::drawNextFrameOfSpectrum() {
    bool hasData = false;
    for (int i = 0; i < fftSize; i++) {
        fftData[i] = (fftData[i] + fftData[i + fftSize]) * 0.5f;
        if (fftData[i] != 0.0f) {
            hasData = true;
        }
        fftData[i + fftSize] = 0;
    }
    if (!hasData) {
        return false;
    }
    window.multiplyWithWindowingTable(fftData, fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);

    auto sampleRate = 48000;  // TODO: ?
    auto minFreq = 40.0f;
    auto maxFreq = 20000.0f;
    auto mindB = -100.0f;
    auto maxdB = 0.0f;
    for (int i = 0; i < scopeSize; ++i) {
        float hz = xToHz(minFreq, maxFreq, (float)i / scopeSize);
        float gain = getFFTDataByHz(fftData, fftSize, sampleRate, hz);
        auto level = juce::jmap(juce::Decibels::gainToDecibels(gain) - juce::Decibels::gainToDecibels((float)fftSize),
                                mindB,
                                maxdB,
                                0.0f,
                                1.0f);
        scopeData[i] = level;
    }
    return true;
}
bool AnalyserWindow::drawNextFrameOfLevel() {
    auto mindB = -100.0f;
    auto maxdB = 0.0f;
    bool hasData = false;
    for (int i = 0; i < 2; i++) {
        auto* data = i == 0 ? levelConsumer.destinationL : levelConsumer.destinationR;
        auto db = calcCurrentLevel(levelConsumer.numSamples, data);
        currentLevel[i] = juce::jmap(db, mindB, maxdB, 0.0f, 1.0f);
        if (currentLevel[i] > mindB) {
            hasData = true;
        }
        if (db > 0) {
            (i == 0 ? overflowedLevelL : overflowedLevelR) = db;
            (i == 0 ? overflowWarningL : overflowWarningR) = 30 * 1.2;
        }
    }
    return hasData;
}
void AnalyserWindow::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    juce::Rectangle<int> bounds = getLocalBounds();

    if (readyToDrawFrame) {
        auto offsetX = 2;
        auto offsetY = 2;
        auto displayBounds = bounds.reduced(offsetX, offsetY);
        auto height = displayBounds.getHeight();

        switch (*analyserMode) {
            case ANALYSER_MODE::Spectrum: {
                auto levelWidth = 8;
                auto spectrumWidth = displayBounds.getWidth() - levelWidth * 2;

                paintSpectrum(g, colour::ANALYSER_LINE, offsetX, offsetY, spectrumWidth, height, scopeData);
                offsetX += spectrumWidth;
                paintLevel(g, offsetX, offsetY, levelWidth, height, currentLevel[0]);
                offsetX += levelWidth;
                paintLevel(g, offsetX, offsetY, levelWidth, height, currentLevel[1]);
                break;
            }
            case ANALYSER_MODE::Envelope: {
                auto spectrumWidth = displayBounds.getWidth();
                for (int i = NUM_MODENV - 1; i >= 0; i--) {
                    if (!modEnvParams[i].Enabled->get()) {
                        continue;
                    }
                    juce::Colour colour = colour::ANALYSER_LINE2;
                    paintSpectrum(g, colour, offsetX, offsetY, spectrumWidth, height, &scopeDataForModEnv[i][0]);
                }
                for (int i = NUM_ENVELOPE - 1; i >= 0; i--) {
                    juce::Colour colour = colour::ANALYSER_LINE;
                    paintSpectrum(g, colour, offsetX, offsetY, spectrumWidth, height, &scopeDataForAmpEnv[i][0]);
                }
                break;
            }
            case ANALYSER_MODE::Filter: {
                for (int i = 0; i < NUM_FILTER; i++) {
                    if (!filterParams[i].Enabled->get()) {
                        continue;
                    }
                    auto spectrumWidth = displayBounds.getWidth();
                    bool isRel = static_cast<FILTER_FREQ_TYPE>(filterParams[i].FreqType->getIndex()) ==
                                 FILTER_FREQ_TYPE::Relative;
                    juce::Colour colour = isRel ? colour::ANALYSER_LINE2 : colour::ANALYSER_LINE;
                    paintSpectrum(g, colour, offsetX, offsetY, spectrumWidth, height, &scopeDataForFilter[i][0]);
                }
                break;
            }
        }
    }
    g.setColour(juce::Colour(30, 30, 30));
    g.drawRect(bounds, 2.0f);
}
void AnalyserWindow::paintSpectrum(
    juce::Graphics& g, juce::Colour colour, int offsetX, int offsetY, int width, int height, float* scopeData) {
    g.setColour(colour);
    for (int i = 1; i < scopeSize; ++i) {
        g.drawLine({offsetX + (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                    offsetY - 0.5f + juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                    offsetX + (float)juce::jmap(i, 0, scopeSize - 1, 0, width),
                    offsetY - 0.5f + juce::jmap(scopeData[i], 0.0f, 1.0f, (float)height, 0.0f)});
    }
}
void AnalyserWindow::paintLevel(juce::Graphics& g, int offsetX, int offsetY, int width, int height, float level) {
    g.setColour(colour::ANALYSER_LINE);
    if (overflowWarningL > 0) {
        g.setColour(colour::ERROR);
        overflowWarningL--;
    }
    int barWidth = width - 1;
    int barHeight = level * height;
    g.fillRect(offsetX + 1, offsetY + height - barHeight, barWidth, barHeight);
}
