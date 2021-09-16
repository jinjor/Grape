#include "Components.h"

#include <JuceHeader.h>

#include <utility>

#include "Params.h"

//==============================================================================
float calcCurrentLevel(int numSamples, float* data) {
    float maxValue = 0.0;
    for (int i = 0; i < numSamples; ++i) {
        maxValue = std::max(maxValue, std::abs(data[i]));
    }
    return juce::Decibels::gainToDecibels(maxValue);
}

//==============================================================================
HeaderComponent::HeaderComponent(std::string name, HEADER_CHECK check)
    : enabledButton("Enabled"), name(std::move(name)), check(check) {
    if (check != HEADER_CHECK::Hidden) {
        addAndMakeVisible(enabledButton);
        enabledButton.setEnabled(check == HEADER_CHECK::Enabled);
    }
}
HeaderComponent::~HeaderComponent() {}
void HeaderComponent::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds();
    g.setColour(colour::PANEL_NAME_BACKGROUND);
    g.fillRect(bounds);

    juce::GlyphArrangement ga;
    juce::Font font = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    ga.addFittedText(font, name, 0, 0, bounds.getHeight(), bounds.getWidth(), juce::Justification::right, 1);
    juce::Path p;
    ga.createPath(p);
    auto pathBounds = ga.getBoundingBox(0, name.length(), true);
    p.applyTransform(
        juce::AffineTransform()
            .rotated(-juce::MathConstants<float>::halfPi, 0, 0)
            .translated(0, bounds.getHeight() + (check == HEADER_CHECK::Hidden ? 8.0 : PANEL_NAME_HEIGHT) + 1.0));
    g.setColour(colour::TEXT);
    g.fillPath(p);
}
void HeaderComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    auto enabledButtonArea = bounds.removeFromTop(bounds.getWidth());
    enabledButton.setBounds(enabledButtonArea.reduced(6));
}

//==============================================================================
VoiceComponent::VoiceComponent(VoiceParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : params(params),
      controlItemParams(controlItemParams),
      header("VOICE", HEADER_CHECK::Hidden),
      modeSelector("Mode"),
      portamentoTimeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                           juce::Slider::TextEntryBoxPosition::NoTextBox),
      pitchBendRangeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                           juce::Slider::TextEntryBoxPosition::NoTextBox) {
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    initChoice(modeSelector, params.Mode, this, *this);
    initLinear(portamentoTimeSlider, params.PortamentoTime, 0.001, " sec", nullptr, this, *this);
    initLinear(pitchBendRangeSlider, params.PitchBendRange, this, *this);
    initLabel(modeLabel, "Mode", *this);
    initLabel(portamentoTimeLabel, "Glide Time", *this);
    initLabel(pitchBendRangeLabel, "PB Range", *this);

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

    auto isMono = params.isMonoMode();
    portamentoTimeLabel.setEnabled(isMono);
    portamentoTimeSlider.setEnabled(isMono);

    portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.isControlling(CONTROL_TARGET_MISC_PARAM::PortamentoTime)) {
            portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        }
    }
}

//==============================================================================
StatusComponent::StatusComponent(int* polyphony,
                                 TimeConsumptionState* timeConsumptionState,
                                 LatestDataProvider* latestDataProvider)
    : polyphony(polyphony), timeConsumptionState(timeConsumptionState), latestDataProvider(latestDataProvider) {
    latestDataProvider->addConsumer(&levelConsumer);

    initStatusValue(volumeValueLabel, "0.0dB", *this);
    initStatusValue(polyphonyValueLabel, std::to_string(*polyphony), *this);
    initStatusValue(timeConsumptionValueLabel,
                    std::to_string(juce::roundToInt(timeConsumptionState->currentTimeConsumptionRate * 100)) + "%",
                    *this);

    initStatusKey(volumeLabel, "Peak", *this);
    initStatusKey(polyphonyLabel, "Polyphony", *this);
    initStatusKey(timeConsumptionLabel, "Busyness", *this);

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
      header("MASTER", HEADER_CHECK::Hidden),
      panSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
      volumeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox) {
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    initLinear(panSlider, params.Pan, 0.01, this, *this);
    initLinear(volumeSlider, params.MasterVolume, 0.01, this, *this);
    initLabel(panLabel, "Pan", *this);
    initLabel(volumeLabel, "Volume", *this);

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
      header("OSC " + std::to_string(index + 1), HEADER_CHECK::Enabled),
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
    header.enabledButton.addListener(this);
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    initChoice(envelopeSelector, params.Envelope, this, body);
    initChoice(waveformSelector, params.Waveform, this, body);
    initLinear(edgeSlider, params.Edge, 0.01, this, body);
    initLinear(octaveSlider, params.Octave, this, body);
    initLinear(coarseSlider, params.Coarse, this, body);
    initLinear(unisonSlider, params.Unison, this, body);
    initLinear(detuneSlider, params.Detune, 0.01, this, body);
    initLinear(spreadSlider, params.Spread, 0.01, this, body);
    auto formatGain = [](double gain) { return juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB"; };
    initSkewFromMid(gainSlider, params.Gain, 0.01f, nullptr, std::move(formatGain), this, body);
    initLabel(envelopeLabel, "Amp Env", body);
    initLabel(waveformLabel, "Waveform", body);
    initLabel(edgeLabel, "Edge", body);
    initLabel(octaveLabel, "Octave", body);
    initLabel(coarseLabel, "Semitone", body);
    initLabel(unisonLabel, "Unison", body);
    initLabel(coarseLabel, "Semitone", body);
    initLabel(detuneLabel, "Detune", body);
    initLabel(spreadLabel, "Spread", body);
    initLabel(gainLabel, "Gain", body);

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

    auto hasEdge = params.hasEdge();
    edgeLabel.setEnabled(hasEdge);
    edgeSlider.setEnabled(hasEdge);

    auto isNoise = params.isNoise();
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
        if (p.isControlling(CONTROL_TARGET_OSC_PARAM::Edge, index)) {
            edgeSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        } else if (p.isControlling(CONTROL_TARGET_OSC_PARAM::Detune, index)) {
            detuneSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        } else if (p.isControlling(CONTROL_TARGET_OSC_PARAM::Spread, index)) {
            spreadSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        } else if (p.isControlling(CONTROL_TARGET_OSC_PARAM::Gain, index)) {
            gainSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        }
    }
}

//==============================================================================
EnvelopeComponent::EnvelopeComponent(int index, EnvelopeParams& params)
    : index(index),
      params(params),
      header("AMP ENV " + std::to_string(index + 1), HEADER_CHECK::Disabled),
      attackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      decaySlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                  juce::Slider::TextEntryBoxPosition::NoTextBox),
      sustainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                    juce::Slider::TextEntryBoxPosition::NoTextBox),
      releaseSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                    juce::Slider::TextEntryBoxPosition::NoTextBox) {
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    initSkewFromMid(attackSlider, params.Attack, 0.001, " sec", nullptr, this, *this);
    initSkewFromMid(decaySlider, params.Decay, 0.01, " sec", nullptr, this, *this);
    initLinearPercent(sustainSlider, params.Sustain, 0.01, this, *this);
    initSkewFromMid(releaseSlider, params.Release, 0.01, " sec", nullptr, this, *this);
    initLabel(attackLabel, "Attack", *this);
    initLabel(decayLabel, "Decay", *this);
    initLabel(sustainLabel, "Sustain", *this);
    initLabel(releaseLabel, "Release", *this);

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
      header("FILTER " + std::to_string(index + 1), HEADER_CHECK::Enabled),
      targetSelector("Target"),
      typeSelector("Type"),
      freqTypeSelector("Type"),
      hzSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
      semitoneSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      qSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
      gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox) {
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    initChoice(targetSelector, params.Target, this, body);
    initChoice(typeSelector, params.Type, this, body);
    initChoice(freqTypeSelector, params.FreqType, this, body);
    initSkewFromMid(hzSlider, params.Hz, 0.01f, " Hz", nullptr, this, body);
    auto formatSemitone = [](double value) -> std::string {
        int cent = value;
        int centAbs = std::abs(cent);
        int oct = centAbs / 12;
        int octFrac = centAbs % 12;
        return (cent == 0 ? " " : cent > 0 ? "+" : "-") + std::to_string(oct) + ":" + std::to_string(octFrac) + " oct";
    };
    initLinear(semitoneSlider, params.Semitone, 0.01, nullptr, std::move(formatSemitone), this, body);
    initSkewFromMid(qSlider, params.Q, 0.01, nullptr, nullptr, this, body);
    initLinear(gainSlider, params.Gain, 0.01, " dB", nullptr, this, body);
    initLabel(targetLabel, "OSC", body);
    initLabel(typeLabel, "Type", body);
    initLabel(freqTypeLabel, "Freq Type", body);
    initLabel(freqLabel, "Freq", body);
    initLabel(qLabel, "Q", body);
    initLabel(gainLabel, "Gain", body);

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

    hzSlider.setVisible(params.isFreqAbsolute());
    semitoneSlider.setVisible(!params.isFreqAbsolute());

    auto hasGain = params.hasGain();
    gainLabel.setEnabled(hasGain);
    gainSlider.setEnabled(hasGain);

    hzSlider.setLookAndFeel(&grapeLookAndFeel);
    semitoneSlider.setLookAndFeel(&grapeLookAndFeel);
    qSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.isControlling(CONTROL_TARGET_FILTER_PARAM::Freq, index)) {
            hzSlider.setLookAndFeel(&grapeLookAndFeelControlled);
            semitoneSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        } else if (p.isControlling(CONTROL_TARGET_FILTER_PARAM::Q, index)) {
            qSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        }
    }
}

//==============================================================================
LfoComponent::LfoComponent(int index, LfoParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : index(index),
      params(params),
      controlItemParams(controlItemParams),
      header("LFO " + std::to_string(index + 1), HEADER_CHECK::Enabled),
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
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    initChoice(targetTypeSelector, params.TargetType, this, targetSelector);
    initChoice(targetOscSelector, params.TargetOsc, this, targetSelector);
    initChoice(targetFilterSelector, params.TargetFilter, this, targetSelector);
    initChoice(targetOscParamSelector, params.TargetOscParam, this, targetSelector);
    initChoice(targetFilterParamSelector, params.TargetFilterParam, this, targetSelector);
    initChoice(waveformSelector, params.Waveform, this, body);
    initSkewFromMid(slowFreqSlider, params.SlowFreq, 0.01f, " Hz", nullptr, this, body);
    initSkewFromMid(fastFreqSlider, params.FastFreq, 0.01f, " x", nullptr, this, body);
    initLinear(amountSlider, params.Amount, 0.01, this, body);

    initLabel(targetLabel, "Destination", body);
    initLabel(typeLabel, "Type", body);
    initLabel(waveformLabel, "Waveform", body);
    initLabel(freqLabel, "Freq", body);
    initLabel(amountLabel, "Amount", body);

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

    auto targetType = params.getTargetType();
    targetOscSelector.setVisible(targetType == LFO_TARGET_TYPE::OSC);
    targetOscParamSelector.setVisible(targetType == LFO_TARGET_TYPE::OSC);
    targetFilterSelector.setVisible(targetType == LFO_TARGET_TYPE::Filter);
    targetFilterParamSelector.setVisible(targetType == LFO_TARGET_TYPE::Filter);

    auto shouldUseFastFreq = params.shouldUseFastFreq();
    fastFreqSlider.setVisible(shouldUseFastFreq);
    slowFreqSlider.setVisible(!shouldUseFastFreq);

    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    slowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    for (auto& p : controlItemParams) {
        if (p.isControlling(CONTROL_TARGET_LFO_PARAM::Freq, index)) {
            fastFreqSlider.setLookAndFeel(&grapeLookAndFeelControlled);
            slowFreqSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        } else if (p.isControlling(CONTROL_TARGET_LFO_PARAM::Amount, index)) {
            amountSlider.setLookAndFeel(&grapeLookAndFeelControlled);
        }
    }
}

//==============================================================================
ModEnvComponent::ModEnvComponent(int index, ModEnvParams& params)
    : index(index),
      params(params),
      header("MOD ENV " + std::to_string(index + 1), HEADER_CHECK::Enabled),
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
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    initChoice(targetTypeSelector, params.TargetType, this, targetSelector);
    initChoice(targetOscSelector, params.TargetOsc, this, targetSelector);
    initChoice(targetFilterSelector, params.TargetFilter, this, targetSelector);
    initChoice(targetLfoSelector, params.TargetLfo, this, targetSelector);
    initChoice(targetOscParamSelector, params.TargetOscParam, this, targetSelector);
    initChoice(targetFilterParamSelector, params.TargetFilterParam, this, targetSelector);
    initChoice(targetLfoParamSelector, params.TargetLfoParam, this, targetSelector);
    initChoice(fadeSelector, params.Fade, this, body);
    auto formatPeakFreq = [](double oct) -> juce::String {
        return (oct == 0 ? " " : oct > 0 ? "+" : "-") + juce::String(std::abs(oct), 2) + " oct";
    };
    initLinear(peakFreqSlider, params.PeakFreq, 0.01, nullptr, std::move(formatPeakFreq), this, body);
    initSkewFromMid(waitSlider, params.Wait, 0.01, " sec", nullptr, this, body);
    initSkewFromMid(attackSlider, params.Attack, 0.001, " sec", nullptr, this, body);
    initSkewFromMid(decaySlider, params.Decay, 0.01, " sec", nullptr, this, body);
    initLabel(targetLabel, "Destination", body);
    initLabel(typeLabel, "Type", body);
    initLabel(fadeLabel, "Fade", body);
    initLabel(peakFreqLabel, "Peak Freq", body);
    initLabel(waitLabel, "Wait", body);
    initLabel(attackLabel, "Attack", body);
    initLabel(decayLabel, "Decay", body);

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

    auto targetType = params.getTargetType();
    targetOscSelector.setVisible(targetType == MODENV_TARGET_TYPE::OSC);
    targetOscParamSelector.setVisible(targetType == MODENV_TARGET_TYPE::OSC);
    targetFilterSelector.setVisible(targetType == MODENV_TARGET_TYPE::Filter);
    targetFilterParamSelector.setVisible(targetType == MODENV_TARGET_TYPE::Filter);
    targetLfoSelector.setVisible(targetType == MODENV_TARGET_TYPE::LFO);
    targetLfoParamSelector.setVisible(targetType == MODENV_TARGET_TYPE::LFO);

    auto isTargetFreq = params.isTargetFreq();
    auto isFadeIn = params.isFadeIn();
    peakFreqLabel.setVisible(isTargetFreq);
    peakFreqSlider.setVisible(isTargetFreq);
    fadeLabel.setVisible(!isTargetFreq);
    fadeSelector.setVisible(!isTargetFreq);
    waitLabel.setVisible(!isTargetFreq && isFadeIn);
    waitSlider.setVisible(!isTargetFreq && isFadeIn);
    attackLabel.setVisible(isTargetFreq || !isFadeIn);
    attackSlider.setVisible(isTargetFreq || !isFadeIn);
}

//==============================================================================
DelayComponent::DelayComponent(DelayParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams)
    : params(params),
      controlItemParams(controlItemParams),
      header("DELAY", HEADER_CHECK::Enabled),
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
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(params.Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    initChoice(typeSelector, params.Type, this, body);
    initChoice(syncSelector, params.Sync, this, body);
    initSkewFromMid(timeLSlider, params.TimeL, 0.01, " sec", nullptr, this, body);
    initSkewFromMid(timeRSlider, params.TimeR, 0.01, " sec", nullptr, this, body);
    initEnum(timeSyncLSlider, params.TimeSyncL, this, body);
    initEnum(timeSyncRSlider, params.TimeSyncR, this, body);
    initSkewFromMid(lowFreqSlider, params.LowFreq, 1.0, " Hz", nullptr, this, body);
    initSkewFromMid(highFreqSlider, params.HighFreq, 1.0, " Hz", nullptr, this, body);
    initLinearPercent(feedbackSlider, params.Feedback, 0.01, this, body);
    initLinear(mixSlider, params.Mix, 0.01, this, body);

    initLabel(typeLabel, "Type", body);
    initLabel(syncLabel, "Sync", body);
    initLabel(timeLLabel, "Time L", body);
    initLabel(timeRLabel, "Time R", body);
    initLabel(lowFreqLabel, "Lo Cut", body);
    initLabel(highFreqLabel, "Hi Cut", body);
    initLabel(feedbackLabel, "Feedback", body);
    initLabel(mixLabel, "Mix", body);

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
        if (p.isControlling(CONTROL_TARGET_MISC_PARAM::DelayMix)) {
            mixSlider.setLookAndFeel(&grapeLookAndFeelControlled);
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
    initChoice(numberSelector, params.Number, this, *this);
    initChoice(targetTypeSelector, params.TargetType, this, *this);
    initChoice(targetOscSelector, params.TargetOsc, this, *this);
    initChoice(targetFilterSelector, params.TargetFilter, this, *this);
    initChoice(targetLfoSelector, params.TargetLfo, this, *this);
    initChoice(targetOscParamSelector, params.TargetOscParam, this, *this);
    initChoice(targetFilterParamSelector, params.TargetFilterParam, this, *this);
    initChoice(targetLfoParamSelector, params.TargetLfoParam, this, *this);
    initChoice(targetMiscParamSelector, params.TargetMiscParam, this, *this);

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

    auto targetType = params.getTargetType();
    targetOscSelector.setVisible(targetType == CONTROL_TARGET_TYPE::OSC);
    targetOscParamSelector.setVisible(targetType == CONTROL_TARGET_TYPE::OSC);
    targetFilterSelector.setVisible(targetType == CONTROL_TARGET_TYPE::Filter);
    targetFilterParamSelector.setVisible(targetType == CONTROL_TARGET_TYPE::Filter);
    targetLfoSelector.setVisible(targetType == CONTROL_TARGET_TYPE::LFO);
    targetLfoParamSelector.setVisible(targetType == CONTROL_TARGET_TYPE::LFO);
    targetMiscParamSelector.setVisible(targetType == CONTROL_TARGET_TYPE::Master);
}

//==============================================================================
ControlComponent::ControlComponent(std::array<ControlItemParams, NUM_CONTROL>& params)
    : header("CONTROLS", HEADER_CHECK::Disabled),
      controlItemComponents{ControlItemComponent(params[0]),
                            ControlItemComponent(params[1]),
                            ControlItemComponent(params[2]),
                            ControlItemComponent(params[3]),
                            ControlItemComponent(params[4]),
                            ControlItemComponent(params[5])} {
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    initLabel(numberLabel, "CC", *this);
    initLabel(targetLabel, "Destination", *this);

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
    initLabel(nameLabel, PARAM_LABEL_FONT_SIZE, "Regular", juce::Justification::right, std::move(name), *this);
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
    juce::Rectangle<int> bounds = getLocalBounds();
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
                        if (!modEnvParams[i].isTargetFreq() && modEnvParams[i].isFadeIn()) {
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
                auto filterType = filterParams[i].getType();
                double freq;
                bool isRel = false;
                switch (filterParams[i].getFreqType()) {
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
                    bool isRel = !filterParams[i].isFreqAbsolute();
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
