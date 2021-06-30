#include <JuceHeader.h>
#include "Components.h"
#include "Params.h"

//==============================================================================
float calcCurrentLevel(int numSamples, float* data) {
    float maxValue = 0.0;
    for (int i = 0; i < numSamples; ++i)
    {
        maxValue = std::max(maxValue, std::abs(data[i]));
    }
    return juce::Decibels::gainToDecibels (maxValue);
}

//==============================================================================
HeaderComponent::HeaderComponent(std::string name, bool hasEnableButton)
: enabledButton("Enabled")
, name(name)
, hasEnableButton (hasEnableButton)
{
    addAndMakeVisible(enabledButton);
    enabledButton.setEnabled(hasEnableButton);
}
HeaderComponent::~HeaderComponent(){}
void HeaderComponent::paint(juce::Graphics& g)
{
    juce::Rectangle<int> bounds = getLocalBounds();
    g.setColour(PANEL_NAME_COLOUR);
    g.fillRect(bounds);
    
    juce::GlyphArrangement ga;
    juce::Font font = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    ga.addLineOfText(font, name, 0, 0);
    juce::Path p;
    ga.createPath(p);
    auto pathBounds = p.getBounds();
    p.applyTransform(juce::AffineTransform()
                         .rotated(-juce::MathConstants<float>::halfPi, 0, 0)
                         .translated(pathBounds.getHeight()/2 + bounds.getWidth()/2,
                                     pathBounds.getWidth() + PANEL_NAME_HEIGHT + 2.0)
                         );
    g.setColour(TEXT_COLOUR);
    g.fillPath(p);
}
void HeaderComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto enabledButtonArea = bounds.removeFromTop(bounds.getWidth());
    enabledButton.setBounds(enabledButtonArea.reduced(2.0));
}

//==============================================================================
VoiceComponent::VoiceComponent(VoiceParams* params, ControlItemParams* controlItemParams)
: _paramsPtr(params)
, controlItemParams(controlItemParams)
, header("VOICE", false)
, modeSelector("Mode")
, portamentoTimeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, pitchBendRangeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);

    modeSelector.setLookAndFeel(&grapeLookAndFeel);
    modeSelector.addItemList(_paramsPtr->Mode->getAllValueStrings(), 1);
    modeSelector.setSelectedItemIndex(_paramsPtr->Mode->getIndex(), juce::dontSendNotification);
    modeSelector.setJustificationType(juce::Justification::centred);
    modeSelector.addListener(this);
    addAndMakeVisible(modeSelector);
    
    portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeel);
    portamentoTimeSlider.setRange(_paramsPtr->PortamentoTime->range.start,
                                  _paramsPtr->PortamentoTime->range.end,
                                  _paramsPtr->PortamentoTime->range.interval);
    portamentoTimeSlider.setSkewFactorFromMidPoint(0.1);
    portamentoTimeSlider.setValue(_paramsPtr->PortamentoTime->get(), juce::dontSendNotification);
    portamentoTimeSlider.setPopupDisplayEnabled(true, true, nullptr);
    portamentoTimeSlider.setScrollWheelEnabled(false);
    portamentoTimeSlider.setTextValueSuffix(" sec");
    portamentoTimeSlider.addListener(this);
    addAndMakeVisible(portamentoTimeSlider);
    
    pitchBendRangeSlider.setLookAndFeel(&grapeLookAndFeel);
    pitchBendRangeSlider.setRange(_paramsPtr->PitchBendRange->getRange().getStart(),
                                  _paramsPtr->PitchBendRange->getRange().getEnd(), 1);
    pitchBendRangeSlider.setValue(_paramsPtr->PitchBendRange->get(), juce::dontSendNotification);
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
    pitchBendRangeLabel.setText("Bend Range", juce::dontSendNotification);
    pitchBendRangeLabel.setJustificationType(juce::Justification::centred);
    pitchBendRangeLabel.setEditable(false, false, false);
    addAndMakeVisible(pitchBendRangeLabel);
    
    startTimerHz(30.0f);
}

VoiceComponent::~VoiceComponent()
{}

void VoiceComponent::paint(juce::Graphics& g)
{
}

void VoiceComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    bounds.reduce(0, 10);
    auto boundsWidth = bounds.getWidth();
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.34);
        modeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        modeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.33);
        portamentoTimeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        portamentoTimeSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.33);
        pitchBendRangeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        pitchBendRangeSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void VoiceComponent::comboBoxChanged(juce::ComboBox* comboBox) {
    if(comboBox == &modeSelector)
    {
        *_paramsPtr->Mode = modeSelector.getSelectedItemIndex();
    }
}
void VoiceComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &portamentoTimeSlider)
    {
        *_paramsPtr->PortamentoTime = portamentoTimeSlider.getValue();
    }
    else if(slider == &pitchBendRangeSlider)
    {
        *_paramsPtr->PitchBendRange = pitchBendRangeSlider.getValue();
    }
}
void VoiceComponent::timerCallback()
{
    portamentoTimeSlider.setValue(_paramsPtr->PortamentoTime->get(), juce::dontSendNotification);
    pitchBendRangeSlider.setValue(_paramsPtr->PitchBendRange->get(), juce::dontSendNotification);
    
    auto isMono = static_cast<VOICE_MODE>(_paramsPtr->Mode->getIndex()) == VOICE_MODE::Mono;
    portamentoTimeLabel.setEnabled(isMono);
    portamentoTimeSlider.setEnabled(isMono);
    
    portamentoTimeSlider.setLookAndFeel(&grapeLookAndFeel);
    for(int i = 0; i < NUM_CONTROL; ++i) {
        auto params = &controlItemParams[i];
        if(params->Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(params->TargetType->getIndex());
        switch(targetType) {
            case CONTROL_TARGET_TYPE::Master: {
                auto targetParam = static_cast<CONTROL_TARGET_MISC_PARAM>(params->TargetMiscParam->getIndex());
                switch(targetParam) {
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
StatusComponent::StatusComponent(int* polyphony, TimeConsumptionState* timeConsumptionState, LatestDataProvider* latestDataProvider)
: polyphony(polyphony)
, timeConsumptionState(timeConsumptionState)
, latestDataProvider(latestDataProvider)
{
    latestDataProvider->addConsumer(&levelConsumer);
    
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    juce::Font paramValueLabelFont = juce::Font(PARAM_VALUE_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
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
    timeConsumptionValueLabel.setText(juce::String(juce::roundToInt(timeConsumptionState->currentTimeConsumptionRate * 100)) + "%", juce::dontSendNotification);
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

StatusComponent::~StatusComponent()
{
    latestDataProvider->removeConsumer(&levelConsumer);
}

void StatusComponent::paint(juce::Graphics& g)
{
}

void StatusComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    
    bounds.reduce(0, 10);
    auto boundsHeight = bounds.getHeight();
    auto boundsWidth = bounds.getWidth();
    {
        juce::Rectangle<int> area = bounds.removeFromTop(boundsHeight / 3);
        volumeLabel.setBounds(area.removeFromLeft(boundsWidth * 0.4).reduced(LOCAL_MARGIN));
        volumeValueLabel.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromTop(boundsHeight / 3);
        polyphonyLabel.setBounds(area.removeFromLeft(boundsWidth * 0.4).reduced(LOCAL_MARGIN));
        polyphonyValueLabel.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromTop(boundsHeight / 3);
        timeConsumptionLabel.setBounds(area.removeFromLeft(boundsWidth * 0.4).reduced(LOCAL_MARGIN));
        timeConsumptionValueLabel.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void StatusComponent::timerCallback()
{
    if(overflowWarning > 0) {
        volumeValueLabel.setColour(juce::Label::textColourId, ERROR_COLOUR);
        auto levelStr = juce::String(overflowedLevel, 1) + " dB";
        volumeValueLabel.setText(levelStr, juce::dontSendNotification);
        
        overflowWarning--;
    } else {
        volumeValueLabel.removeColour(juce::Label::textColourId);
        
        if(levelConsumer.ready) {
            float levelLdB = calcCurrentLevel(levelConsumer.numSamples, levelConsumer.destinationL);
            float levelRdB = calcCurrentLevel(levelConsumer.numSamples, levelConsumer.destinationR);
            auto leveldB = std::max(levelLdB, levelRdB);
            auto levelStr = (leveldB <= -100 ? "-Inf" : juce::String(leveldB, 1)) + " dB";
            volumeValueLabel.setText(levelStr, juce::dontSendNotification);
            levelConsumer.ready = false;
            if(leveldB > 0) {
                overflowedLevel = leveldB;
                overflowWarning = 4 * 1.2;
            }
        }
    }
    {
        int numVoices = 64;// TODO: share this
        int value = *polyphony;
        polyphonyValueLabel.setText(juce::String(value), juce::dontSendNotification);
        if(value >= numVoices) {
            polyphonyValueLabel.setColour(juce::Label::textColourId, ERROR_COLOUR);
        } else if(value > numVoices * 0.8) {
            polyphonyValueLabel.setColour(juce::Label::textColourId, WARNING_COLOUR);
        } else {
            polyphonyValueLabel.setColour(juce::Label::textColourId, TEXT_COLOUR);
        }
    }
    {
        auto value = timeConsumptionState->currentTimeConsumptionRate;
        timeConsumptionValueLabel.setText(juce::String(juce::roundToInt(value * 100)) + "%", juce::dontSendNotification);
        if(value >= 1.0) {
            timeConsumptionValueLabel.setColour(juce::Label::textColourId, ERROR_COLOUR);
        } else if(value >= 0.8) {
            timeConsumptionValueLabel.setColour(juce::Label::textColourId, WARNING_COLOUR);
        } else {
            timeConsumptionValueLabel.setColour(juce::Label::textColourId, TEXT_COLOUR);
        }
    }
}

//==============================================================================
MasterComponent::MasterComponent(GlobalParams* params)
: _paramsPtr(params)
, header("MASTER", false)
, panSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, volumeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font panelNameFont = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);
    
    panSlider.setLookAndFeel(&grapeLookAndFeelControlled);
    panSlider.setRange(_paramsPtr->Pan->range.start,
                          _paramsPtr->Pan->range.end, 0.01);
    panSlider.setValue(_paramsPtr->Pan->get(), juce::dontSendNotification);
    panSlider.setPopupDisplayEnabled(true, true, nullptr);
    panSlider.setScrollWheelEnabled(false);
    panSlider.addListener(this);
    addAndMakeVisible(panSlider);
    
    volumeSlider.setLookAndFeel(&grapeLookAndFeelControlled);
    volumeSlider.setRange(_paramsPtr->MasterVolume->range.start,
                          _paramsPtr->MasterVolume->range.end, 0.01);
    volumeSlider.setValue(_paramsPtr->MasterVolume->get(), juce::dontSendNotification);
    volumeSlider.setPopupDisplayEnabled(true, true, nullptr);
    volumeSlider.setScrollWheelEnabled(false);
//    volumeSlider.textFromValueFunction = [](double gain){ return juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB"; };
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

MasterComponent::~MasterComponent()
{}

void MasterComponent::paint(juce::Graphics& g)
{
}

void MasterComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    bounds.reduce(0, 10);
//    auto boundsWidth = bounds.getWidth();
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(SLIDER_WIDTH);
        panLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        panSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(SLIDER_WIDTH);
        volumeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        volumeSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void MasterComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &panSlider)
    {
        *_paramsPtr->Pan = (float)panSlider.getValue();
    }
    else if(slider == &volumeSlider)
    {
        *_paramsPtr->MasterVolume = (float)volumeSlider.getValue();
    }
}
void MasterComponent::timerCallback()
{
    panSlider.setValue(_paramsPtr->Pan->get(), juce::dontSendNotification);
    volumeSlider.setValue(_paramsPtr->MasterVolume->get(), juce::dontSendNotification);
}

//==============================================================================
OscComponent::OscComponent(int index, OscParams* params, ControlItemParams* controlItemParams)
: index(index)
, _paramsPtr(params)
, controlItemParams(controlItemParams)
, header("OSC " + std::to_string(index+1), true)
, envelopeSelector("Envelope")
, waveformSelector("Waveform")
, edgeSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, octaveSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, coarseSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, unisonSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, detuneSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, spreadSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    header.enabledButton.addListener(this);
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);
    
    envelopeSelector.setLookAndFeel(&grapeLookAndFeel);
    envelopeSelector.addItemList(_paramsPtr->Envelope->getAllValueStrings(), 1);
    envelopeSelector.setSelectedItemIndex(_paramsPtr->Envelope->getIndex(), juce::dontSendNotification);
    envelopeSelector.setJustificationType(juce::Justification::centred);
    envelopeSelector.addListener(this);
    body.addAndMakeVisible(envelopeSelector);
    
    waveformSelector.setLookAndFeel(&grapeLookAndFeel);
    waveformSelector.addItemList(_paramsPtr->Waveform->getAllValueStrings(), 1);
    waveformSelector.setSelectedItemIndex(_paramsPtr->Waveform->getIndex(), juce::dontSendNotification);
    waveformSelector.setJustificationType(juce::Justification::centred);
    waveformSelector.addListener(this);
    body.addAndMakeVisible(waveformSelector);
    
    edgeSlider.setLookAndFeel(&grapeLookAndFeel);
    edgeSlider.setRange(_paramsPtr->Edge->range.start,
                         _paramsPtr->Edge->range.end, 0.01);
    edgeSlider.setValue(_paramsPtr->Edge->get(), juce::dontSendNotification);
    edgeSlider.setPopupDisplayEnabled(true, true, nullptr);
    edgeSlider.setScrollWheelEnabled(false);
    edgeSlider.addListener(this);
    body.addAndMakeVisible(edgeSlider);
    
    octaveSlider.setLookAndFeel(&grapeLookAndFeel);
    octaveSlider.setRange(_paramsPtr->Octave->getRange().getStart(),
                          _paramsPtr->Octave->getRange().getEnd(), 1);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    octaveSlider.setPopupDisplayEnabled(true, true, nullptr);
    octaveSlider.setScrollWheelEnabled(false);
    octaveSlider.addListener(this);
    body.addAndMakeVisible(octaveSlider);
    
    coarseSlider.setLookAndFeel(&grapeLookAndFeel);
    coarseSlider.setRange(_paramsPtr->Coarse->getRange().getStart(),
                         _paramsPtr->Coarse->getRange().getEnd(), 1);
    coarseSlider.setValue(_paramsPtr->Coarse->get(), juce::dontSendNotification);
    coarseSlider.setPopupDisplayEnabled(true, true, nullptr);
    coarseSlider.setScrollWheelEnabled(false);
    coarseSlider.addListener(this);
    body.addAndMakeVisible(coarseSlider);
    
    unisonSlider.setLookAndFeel(&grapeLookAndFeel);
    unisonSlider.setRange(_paramsPtr->Unison->getRange().getStart(),
                         _paramsPtr->Unison->getRange().getEnd(), 1);
    unisonSlider.setValue(_paramsPtr->Unison->get(), juce::dontSendNotification);
    unisonSlider.setPopupDisplayEnabled(true, true, nullptr);
    unisonSlider.setScrollWheelEnabled(false);
    unisonSlider.addListener(this);
    body.addAndMakeVisible(unisonSlider);
    
    detuneSlider.setLookAndFeel(&grapeLookAndFeel);
    detuneSlider.setRange(_paramsPtr->Detune->range.start,
                         _paramsPtr->Detune->range.end, 0.01);
    detuneSlider.setValue(_paramsPtr->Detune->get(), juce::dontSendNotification);
    detuneSlider.setPopupDisplayEnabled(true, true, nullptr);
    detuneSlider.setScrollWheelEnabled(false);
    detuneSlider.addListener(this);
    body.addAndMakeVisible(detuneSlider);
    
    spreadSlider.setLookAndFeel(&grapeLookAndFeel);
    spreadSlider.setRange(_paramsPtr->Spread->range.start,
                         _paramsPtr->Spread->range.end, 0.01);
    spreadSlider.setValue(_paramsPtr->Spread->get(), juce::dontSendNotification);
    spreadSlider.setPopupDisplayEnabled(true, true, nullptr);
    spreadSlider.setScrollWheelEnabled(false);
    spreadSlider.addListener(this);
    body.addAndMakeVisible(spreadSlider);
    
    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setRange(_paramsPtr->Gain->range.start,
                        _paramsPtr->Gain->range.end,
                        _paramsPtr->Gain->range.interval);
    gainSlider.setValue(_paramsPtr->Gain->get(), juce::dontSendNotification);
    gainSlider.setPopupDisplayEnabled(true, true, nullptr);
    gainSlider.setScrollWheelEnabled(false);
    gainSlider.setSkewFactorFromMidPoint(1.0);
    gainSlider.textFromValueFunction = [](double gain){ return juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB"; };
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
    
    body.setEnabled(_paramsPtr->Enabled->get());
    addAndMakeVisible(body);

    startTimerHz(30.0f);
}

OscComponent::~OscComponent()
{}

void OscComponent::paint(juce::Graphics& g)
{
}

void OscComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight/2);
    auto lowerArea = bounds;
    {
        float selectorWidth = 60.0f;
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth);
        envelopeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        envelopeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        float selectorWidth = 120.0f;
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth);
        waveformLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        waveformSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(SLIDER_WIDTH);
        edgeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        edgeSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(SLIDER_WIDTH);
        gainLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        gainSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        octaveLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        octaveSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        coarseLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        coarseSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        unisonLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        unisonSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        detuneLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        detuneSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        spreadLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        spreadSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void OscComponent::buttonClicked(juce::Button* button) {
    if(button == &header.enabledButton)
    {
        *_paramsPtr->Enabled = header.enabledButton.getToggleState();
    }
}
void OscComponent::comboBoxChanged(juce::ComboBox* comboBox) {
    if(comboBox == &envelopeSelector)
    {
        *_paramsPtr->Envelope = envelopeSelector.getSelectedItemIndex();
    }
    else if(comboBox == &waveformSelector)
    {
        *_paramsPtr->Waveform = waveformSelector.getSelectedItemIndex();
    }
}
void OscComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &edgeSlider)
    {
        *_paramsPtr->Edge = edgeSlider.getValue();
    }
    else if(slider == &octaveSlider)
    {
        *_paramsPtr->Octave = octaveSlider.getValue();
    }
    else if(slider == &coarseSlider)
    {
        *_paramsPtr->Coarse = coarseSlider.getValue();
    }
    else if(slider == &unisonSlider)
    {
        *_paramsPtr->Unison = unisonSlider.getValue();
    }
    else if(slider == &detuneSlider)
    {
        *_paramsPtr->Detune = (float)detuneSlider.getValue();
    }
    else if(slider == &spreadSlider)
    {
        *_paramsPtr->Spread = (float)spreadSlider.getValue();
    }
    else if(slider == &gainSlider)
    {
        *_paramsPtr->Gain = (float)gainSlider.getValue();
    }
}
void OscComponent::timerCallback()
{
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    body.setEnabled(_paramsPtr->Enabled->get());
    envelopeSelector.setSelectedItemIndex(_paramsPtr->Envelope->getIndex(), juce::dontSendNotification);
    waveformSelector.setSelectedItemIndex(_paramsPtr->Waveform->getIndex(), juce::dontSendNotification);
    edgeSlider.setValue(_paramsPtr->Edge->get(), juce::dontSendNotification);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    coarseSlider.setValue(_paramsPtr->Coarse->get(), juce::dontSendNotification);
    unisonSlider.setValue(_paramsPtr->Unison->get(), juce::dontSendNotification);
    detuneSlider.setValue(_paramsPtr->Detune->get(), juce::dontSendNotification);
    spreadSlider.setValue(_paramsPtr->Spread->get(), juce::dontSendNotification);
    gainSlider.setValue(_paramsPtr->Gain->get(), juce::dontSendNotification);
    auto hasEdge = OSC_WAVEFORM_VALUES[_paramsPtr->Waveform->getIndex()] == WAVEFORM::Square ||
                   OSC_WAVEFORM_VALUES[_paramsPtr->Waveform->getIndex()] == WAVEFORM::Triangle;
    edgeLabel.setEnabled(hasEdge);
    edgeSlider.setEnabled(hasEdge);
    auto isNoise = OSC_WAVEFORM_VALUES[_paramsPtr->Waveform->getIndex()] == WAVEFORM::White ||
                   OSC_WAVEFORM_VALUES[_paramsPtr->Waveform->getIndex()] == WAVEFORM::Pink;
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
    for(int i = 0; i < NUM_CONTROL; ++i) {
        auto params = &controlItemParams[i];
        if(params->Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(params->TargetType->getIndex());
        switch(targetType) {
            case CONTROL_TARGET_TYPE::OSC: {
                int targetIndex = params->TargetOsc->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_OSC_PARAM>(params->TargetOscParam->getIndex());
                if(targetIndex == index) {
                    switch(targetParam) {
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
//                        case CONTROL_TARGET_OSC_PARAM::Pan: {
////                                panSlider.setLookAndFeel(&grapeLookAndFeelControlled);
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
EnvelopeComponent::EnvelopeComponent(int index, EnvelopeParams* params)
: index(index)
, _paramsPtr(params)
, header("AMP ENV " + std::to_string(index+1), false)
, attackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, decaySlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, sustainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, releaseSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font panelNameFont = juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold");
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);
    
    attackSlider.setLookAndFeel(&grapeLookAndFeel);
    attackSlider.setRange(_paramsPtr->Attack->range.start,
                          _paramsPtr->Attack->range.end, 0.001);
    attackSlider.setSkewFactorFromMidPoint(0.2);
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, nullptr);
    attackSlider.setScrollWheelEnabled(false);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    
    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(_paramsPtr->Decay->range.start,
                         _paramsPtr->Decay->range.end, 0.01);
    decaySlider.setSkewFactorFromMidPoint(0.4);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, nullptr);
    decaySlider.setScrollWheelEnabled(false);
    decaySlider.setTextValueSuffix(" sec");
    decaySlider.addListener(this);
    addAndMakeVisible(decaySlider);
    
    sustainSlider.setLookAndFeel(&grapeLookAndFeel);
    sustainSlider.setRange(_paramsPtr->Sustain->range.start,
                           _paramsPtr->Sustain->range.end, 0.01);
    sustainSlider.setValue(_paramsPtr->Sustain->get(), juce::dontSendNotification);
    sustainSlider.setPopupDisplayEnabled(true, true, nullptr);
    sustainSlider.setScrollWheelEnabled(false);
//    sustainSlider.textFromValueFunction = [](double gain){ return juce::String(juce::Decibels::gainToDecibels(gain), 2) + " dB"; };
    sustainSlider.textFromValueFunction = [](double gain){ return juce::String(gain * 100, 0) + " %"; };
    sustainSlider.addListener(this);
    addAndMakeVisible(sustainSlider);
    
    releaseSlider.setLookAndFeel(&grapeLookAndFeel);
    releaseSlider.setRange(_paramsPtr->Release->range.start,
                           _paramsPtr->Release->range.end, 0.01);
    releaseSlider.setSkewFactorFromMidPoint(0.4);
    releaseSlider.setValue(_paramsPtr->Release->get(), juce::dontSendNotification);
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

EnvelopeComponent::~EnvelopeComponent()
{}

void EnvelopeComponent::paint(juce::Graphics& g)
{
}

void EnvelopeComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight/2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(SLIDER_WIDTH);
        attackLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        attackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(SLIDER_WIDTH);
        decayLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        decaySlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        sustainLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        sustainSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        releaseLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        releaseSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void EnvelopeComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &attackSlider)
    {
        *_paramsPtr->Attack = (float)attackSlider.getValue();
    }
    else if(slider == &decaySlider)
    {
        *_paramsPtr->Decay = (float)decaySlider.getValue();
    }
    else if(slider == &sustainSlider)
    {
        *_paramsPtr->Sustain = (float)sustainSlider.getValue();
    }
    else if(slider == &releaseSlider)
    {
        *_paramsPtr->Release = (float)releaseSlider.getValue();
    }
}
void EnvelopeComponent::timerCallback()
{
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    sustainSlider.setValue(_paramsPtr->Sustain->get(), juce::dontSendNotification);
    releaseSlider.setValue(_paramsPtr->Release->get(), juce::dontSendNotification);
}

//==============================================================================
FilterComponent::FilterComponent(int index, FilterParams* params, ControlItemParams* controlItemParams)
: index(index)
, _paramsPtr(params)
, controlItemParams(controlItemParams)
, header("FILTER " + std::to_string(index+1), true)
, targetSelector("Target")
, typeSelector("Type")
, freqTypeSelector("Type")
, hzSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, semitoneSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, qSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    targetSelector.setLookAndFeel(&grapeLookAndFeel);
    targetSelector.addItemList(_paramsPtr->Target->getAllValueStrings(), 1);
    targetSelector.setSelectedItemIndex(_paramsPtr->Target->getIndex(), juce::dontSendNotification);
    targetSelector.setJustificationType(juce::Justification::centred);
    targetSelector.addListener(this);
    body.addAndMakeVisible(targetSelector);
    
    typeSelector.setLookAndFeel(&grapeLookAndFeel);
    typeSelector.addItemList(_paramsPtr->Type->getAllValueStrings(), 1);
    typeSelector.setSelectedItemIndex(_paramsPtr->Type->getIndex(), juce::dontSendNotification);
    typeSelector.setJustificationType(juce::Justification::centred);
    typeSelector.addListener(this);
    body.addAndMakeVisible(typeSelector);
    
    freqTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    freqTypeSelector.addItemList(_paramsPtr->FreqType->getAllValueStrings(), 1);
    freqTypeSelector.setSelectedItemIndex(_paramsPtr->FreqType->getIndex(), juce::dontSendNotification);
    freqTypeSelector.setJustificationType(juce::Justification::centred);
    freqTypeSelector.addListener(this);
    body.addAndMakeVisible(freqTypeSelector);
    
    hzSlider.setLookAndFeel(&grapeLookAndFeel);
    hzSlider.setRange(_paramsPtr->Hz->range.start,
                      _paramsPtr->Hz->range.end,
                      _paramsPtr->Hz->range.interval);
    hzSlider.setSkewFactorFromMidPoint(2000.0f);
    hzSlider.setValue(_paramsPtr->Hz->get(), juce::dontSendNotification);
    hzSlider.setPopupDisplayEnabled(true, true, nullptr);
    hzSlider.setScrollWheelEnabled(false);
    hzSlider.setTextValueSuffix(" Hz");
    hzSlider.addListener(this);
    body.addAndMakeVisible(hzSlider);
    
    semitoneSlider.setLookAndFeel(&grapeLookAndFeel);
    semitoneSlider.setRange(_paramsPtr->Semitone->getRange().getStart(),
                         _paramsPtr->Semitone->getRange().getEnd(), 1);
    semitoneSlider.setValue(_paramsPtr->Semitone->get(), juce::dontSendNotification);
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
    qSlider.setRange(_paramsPtr->Q->range.start,
                           _paramsPtr->Q->range.end, 0.01);
    qSlider.setSkewFactorFromMidPoint(1.0f);
    qSlider.setValue(_paramsPtr->Q->get(), juce::dontSendNotification);
    qSlider.setPopupDisplayEnabled(true, true, nullptr);
    qSlider.setScrollWheelEnabled(false);
    qSlider.addListener(this);
    body.addAndMakeVisible(qSlider);
    
    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setRange(_paramsPtr->Gain->range.start,
                           _paramsPtr->Gain->range.end, 0.01);
    gainSlider.setValue(_paramsPtr->Gain->get(), juce::dontSendNotification);
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

FilterComponent::~FilterComponent()
{}

void FilterComponent::paint(juce::Graphics& g)
{
}

void FilterComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        float selectorWidth = 70.0f;
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth);
        targetLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        targetSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        float selectorWidth = 120.0f;
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth);
        typeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        typeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        float selectorWidth = 70.0f;
        juce::Rectangle<int> area = lowerArea.removeFromLeft(selectorWidth);
        freqTypeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        freqTypeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        freqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        hzSlider.setBounds(area.reduced(LOCAL_MARGIN));
        semitoneSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        qLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        qSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        gainLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        gainSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void FilterComponent::buttonClicked(juce::Button* button) {
    if(button == &header.enabledButton)
    {
        *_paramsPtr->Enabled = header.enabledButton.getToggleState();
    }
}
void FilterComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == &targetSelector)
    {
        *_paramsPtr->Target = targetSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &typeSelector)
    {
        *_paramsPtr->Type = typeSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &freqTypeSelector)
    {
        *_paramsPtr->FreqType = freqTypeSelector.getSelectedItemIndex();
    }
}
void FilterComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &hzSlider)
    {
        *_paramsPtr->Hz = (float)hzSlider.getValue();
    }
    else if(slider == &semitoneSlider)
    {
        *_paramsPtr->Semitone = semitoneSlider.getValue();
    }
    else if(slider == &qSlider)
    {
        *_paramsPtr->Q = (float)qSlider.getValue();
    }
    else if(slider == &gainSlider)
    {
        *_paramsPtr->Gain = (float)gainSlider.getValue();
    }
}
void FilterComponent::timerCallback()
{
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    body.setEnabled(_paramsPtr->Enabled->get());
    
    targetSelector.setSelectedItemIndex(_paramsPtr->Target->getIndex(), juce::dontSendNotification);
    typeSelector.setSelectedItemIndex(_paramsPtr->Type->getIndex(), juce::dontSendNotification);
    freqTypeSelector.setSelectedItemIndex(_paramsPtr->FreqType->getIndex(), juce::dontSendNotification);
    hzSlider.setValue(_paramsPtr->Hz->get(), juce::dontSendNotification);
    semitoneSlider.setValue(_paramsPtr->Semitone->get(), juce::dontSendNotification);
    qSlider.setValue(_paramsPtr->Q->get(), juce::dontSendNotification);
    
    auto freqType = static_cast<FILTER_FREQ_TYPE>(_paramsPtr->FreqType->getIndex());
    hzSlider.setVisible(freqType == FILTER_FREQ_TYPE::Absolute);
    semitoneSlider.setVisible(freqType == FILTER_FREQ_TYPE::Relative);
    
    auto hasGain = _paramsPtr->hasGain();
    gainLabel.setEnabled(hasGain);
    gainSlider.setEnabled(hasGain);
    
    hzSlider.setLookAndFeel(&grapeLookAndFeel);
    semitoneSlider.setLookAndFeel(&grapeLookAndFeel);
    qSlider.setLookAndFeel(&grapeLookAndFeel);
    for(int i = 0; i < NUM_CONTROL; ++i) {
        auto params = &controlItemParams[i];
        if(params->Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(params->TargetType->getIndex());
        switch(targetType) {
            case CONTROL_TARGET_TYPE::Filter: {
                int targetIndex = params->TargetFilter->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_FILTER_PARAM>(params->TargetFilterParam->getIndex());
                for(int filterIndex = 0; filterIndex < NUM_FILTER; ++filterIndex) {
                    if(targetIndex == filterIndex) {
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
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
LfoComponent::LfoComponent(int index, LfoParams* params, ControlItemParams* controlItemParams)
: index(index)
, _paramsPtr(params)
, controlItemParams(controlItemParams)
, header("LFO " + std::to_string(index+1), true)
, targetTypeSelector("TargetType")
, targetOscSelector("TargetOsc")
, targetFilterSelector("TargetFilter")
, targetOscParamSelector("TargetOscParam")
, targetFilterParamSelector("TargetFilterParam")
, waveformSelector("Waveform")
, slowFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, fastFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, amountSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    targetTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    targetTypeSelector.addItemList(_paramsPtr->TargetType->getAllValueStrings(), 1);
    targetTypeSelector.setSelectedItemIndex(_paramsPtr->TargetType->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setJustificationType(juce::Justification::centred);
    targetTypeSelector.addListener(this);
    body.addAndMakeVisible(targetTypeSelector);
    
    targetOscSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscSelector.addItemList(_paramsPtr->TargetOsc->getAllValueStrings(), 1);
    targetOscSelector.setSelectedItemIndex(_paramsPtr->TargetOsc->getIndex(), juce::dontSendNotification);
    targetOscSelector.setJustificationType(juce::Justification::centred);
    targetOscSelector.addListener(this);
    body.addAndMakeVisible(targetOscSelector);
    
    targetFilterSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterSelector.addItemList(_paramsPtr->TargetFilter->getAllValueStrings(), 1);
    targetFilterSelector.setSelectedItemIndex(_paramsPtr->TargetFilter->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setJustificationType(juce::Justification::centred);
    targetFilterSelector.addListener(this);
    body.addAndMakeVisible(targetFilterSelector);
    
    targetOscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscParamSelector.addItemList(_paramsPtr->TargetOscParam->getAllValueStrings(), 1);
    targetOscParamSelector.setSelectedItemIndex(_paramsPtr->TargetOscParam->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setJustificationType(juce::Justification::centred);
    targetOscParamSelector.addListener(this);
    body.addAndMakeVisible(targetOscParamSelector);
    
    targetFilterParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterParamSelector.addItemList(_paramsPtr->TargetFilterParam->getAllValueStrings(), 1);
    targetFilterParamSelector.setSelectedItemIndex(_paramsPtr->TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setJustificationType(juce::Justification::centred);
    targetFilterParamSelector.addListener(this);
    body.addAndMakeVisible(targetFilterParamSelector);
    
    waveformSelector.setLookAndFeel(&grapeLookAndFeel);
    waveformSelector.addItemList(_paramsPtr->Waveform->getAllValueStrings(), 1);
    waveformSelector.setSelectedItemIndex(_paramsPtr->Waveform->getIndex(), juce::dontSendNotification);
    waveformSelector.setJustificationType(juce::Justification::centred);
    waveformSelector.addListener(this);
    body.addAndMakeVisible(waveformSelector);
    
    slowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    slowFreqSlider.setRange(_paramsPtr->SlowFreq->range.start,
                            _paramsPtr->SlowFreq->range.end,
                            _paramsPtr->SlowFreq->range.interval);
    slowFreqSlider.setSkewFactorFromMidPoint(4.0f);
    slowFreqSlider.setValue(_paramsPtr->SlowFreq->get(), juce::dontSendNotification);
    slowFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    slowFreqSlider.setScrollWheelEnabled(false);
    slowFreqSlider.setTextValueSuffix(" Hz");
    slowFreqSlider.addListener(this);
    body.addAndMakeVisible(slowFreqSlider);
    
    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    fastFreqSlider.setRange(_paramsPtr->FastFreq->range.start,
                            _paramsPtr->FastFreq->range.end,
                            _paramsPtr->FastFreq->range.interval);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0f);
    fastFreqSlider.setValue(_paramsPtr->FastFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0);
    fastFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    fastFreqSlider.setScrollWheelEnabled(false);
    fastFreqSlider.setTextValueSuffix(" x");
    fastFreqSlider.addListener(this);
    body.addAndMakeVisible(fastFreqSlider);
    
    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setRange(_paramsPtr->Amount->range.start,
                           _paramsPtr->Amount->range.end, 0.01);
    amountSlider.setValue(_paramsPtr->Amount->get(), juce::dontSendNotification);
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
    
    addAndMakeVisible(body);
    
    startTimerHz(30.0f);
}

LfoComponent::~LfoComponent()
{}

void LfoComponent::paint(juce::Graphics& g)
{
}

void LfoComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(280);
        targetLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        auto selectorsArea = area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT);
        
        targetTypeSelector.setBounds(selectorsArea.removeFromLeft(90));
        auto indexArea = selectorsArea.removeFromLeft(70);
        targetOscSelector.setBounds(indexArea);
        targetFilterSelector.setBounds(indexArea);
        auto paramArea = selectorsArea.removeFromLeft(110);
        targetOscParamSelector.setBounds(paramArea);
        targetFilterParamSelector.setBounds(paramArea);
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(120);
        waveformLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        waveformSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        freqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        fastFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
        slowFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        amountLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        amountSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void LfoComponent::buttonClicked(juce::Button* button) {
    if(button == &header.enabledButton)
    {
        *_paramsPtr->Enabled = header.enabledButton.getToggleState();
    }
}
void LfoComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == &targetTypeSelector)
    {
        *_paramsPtr->TargetType = targetTypeSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetOscSelector)
    {
        *_paramsPtr->TargetOsc = targetOscSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetFilterSelector)
    {
        *_paramsPtr->TargetFilter = targetFilterSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetOscParamSelector)
    {
        *_paramsPtr->TargetOscParam = targetOscParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetFilterParamSelector)
    {
        *_paramsPtr->TargetFilterParam = targetFilterParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &waveformSelector)
    {
        *_paramsPtr->Waveform = waveformSelector.getSelectedItemIndex();
    }
    resized();// re-render
}
void LfoComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &slowFreqSlider)
    {
        *_paramsPtr->SlowFreq = (float)slowFreqSlider.getValue();
    }
    else if(slider == &fastFreqSlider)
    {
        *_paramsPtr->FastFreq = (float)fastFreqSlider.getValue();
    }
    else if(slider == &amountSlider)
    {
        *_paramsPtr->Amount = (float)amountSlider.getValue();
    }
}
void LfoComponent::timerCallback()
{
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    body.setEnabled(_paramsPtr->Enabled->get());
    
    targetTypeSelector.setSelectedItemIndex(_paramsPtr->TargetType->getIndex(), juce::dontSendNotification);
    targetOscSelector.setSelectedItemIndex(_paramsPtr->TargetOsc->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setSelectedItemIndex(_paramsPtr->TargetFilter->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setSelectedItemIndex(_paramsPtr->TargetOscParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setSelectedItemIndex(_paramsPtr->TargetFilterParam->getIndex(), juce::dontSendNotification);
    waveformSelector.setSelectedItemIndex(_paramsPtr->Waveform->getIndex(), juce::dontSendNotification);
    
    slowFreqSlider.setValue(_paramsPtr->SlowFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setValue(_paramsPtr->FastFreq->get(), juce::dontSendNotification);
    amountSlider.setValue(_paramsPtr->Amount->get(), juce::dontSendNotification);
    switch(static_cast<LFO_TARGET_TYPE>(_paramsPtr->TargetType->getIndex())) {
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
    if(static_cast<LFO_TARGET_TYPE>(_paramsPtr->TargetType->getIndex()) == LFO_TARGET_TYPE::OSC &&
       (static_cast<LFO_TARGET_OSC_PARAM>(_paramsPtr->TargetOscParam->getIndex()) == LFO_TARGET_OSC_PARAM::FM ||
        static_cast<LFO_TARGET_OSC_PARAM>(_paramsPtr->TargetOscParam->getIndex()) == LFO_TARGET_OSC_PARAM::AM
        )
       ) {
        fastFreqSlider.setVisible(true);
        slowFreqSlider.setVisible(false);
    } else {
        fastFreqSlider.setVisible(false);
        slowFreqSlider.setVisible(true);
    }
    
    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    slowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    for(int i = 0; i < NUM_CONTROL; ++i) {
        auto params = &controlItemParams[i];
        if(params->Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(params->TargetType->getIndex());
        switch(targetType) {
            case CONTROL_TARGET_TYPE::LFO: {
                int targetIndex = params->TargetLfo->getIndex();
                auto targetParam = static_cast<CONTROL_TARGET_LFO_PARAM>(params->TargetLfoParam->getIndex());
                for(int lfoIndex = 0; lfoIndex < NUM_MODENV; ++lfoIndex) {
                    if(targetIndex == lfoIndex) {
                        switch(targetParam) {
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
                }
                break;
            }
            default:
                break;
        }
    }
}

//==============================================================================
ModEnvComponent::ModEnvComponent(int index, ModEnvParams* params)
: index(index)
, _paramsPtr(params)
, header("MOD ENV " + std::to_string(index+1), true)
, targetTypeSelector("TargetType")
, targetOscSelector("TargetOsc")
, targetFilterSelector("TargetFilter")
, targetOscParamSelector("TargetOscParam")
, targetFilterParamSelector("TargetFilterParam")
, peakFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, waitSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, attackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, decaySlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);

    targetTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    targetTypeSelector.addItemList(_paramsPtr->TargetType->getAllValueStrings(), 1);
    targetTypeSelector.setSelectedItemIndex(_paramsPtr->TargetType->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setJustificationType(juce::Justification::centred);
    targetTypeSelector.addListener(this);
    body.addAndMakeVisible(targetTypeSelector);
    
    targetOscSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscSelector.addItemList(_paramsPtr->TargetOsc->getAllValueStrings(), 1);
    targetOscSelector.setSelectedItemIndex(_paramsPtr->TargetOsc->getIndex(), juce::dontSendNotification);
    targetOscSelector.setJustificationType(juce::Justification::centred);
    targetOscSelector.addListener(this);
    body.addAndMakeVisible(targetOscSelector);
    
    targetFilterSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterSelector.addItemList(_paramsPtr->TargetFilter->getAllValueStrings(), 1);
    targetFilterSelector.setSelectedItemIndex(_paramsPtr->TargetFilter->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setJustificationType(juce::Justification::centred);
    targetFilterSelector.addListener(this);
    body.addAndMakeVisible(targetFilterSelector);
    
    targetLfoSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoSelector.addItemList(_paramsPtr->TargetLfo->getAllValueStrings(), 1);
    targetLfoSelector.setSelectedItemIndex(_paramsPtr->TargetLfo->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setJustificationType(juce::Justification::centred);
    targetLfoSelector.addListener(this);
    body.addAndMakeVisible(targetLfoSelector);
    
    targetOscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscParamSelector.addItemList(_paramsPtr->TargetOscParam->getAllValueStrings(), 1);
    targetOscParamSelector.setSelectedItemIndex(_paramsPtr->TargetOscParam->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setJustificationType(juce::Justification::centred);
    targetOscParamSelector.addListener(this);
    body.addAndMakeVisible(targetOscParamSelector);
    
    targetFilterParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterParamSelector.addItemList(_paramsPtr->TargetFilterParam->getAllValueStrings(), 1);
    targetFilterParamSelector.setSelectedItemIndex(_paramsPtr->TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setJustificationType(juce::Justification::centred);
    targetFilterParamSelector.addListener(this);
    body.addAndMakeVisible(targetFilterParamSelector);
    
    targetLfoParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoParamSelector.addItemList(_paramsPtr->TargetLfoParam->getAllValueStrings(), 1);
    targetLfoParamSelector.setSelectedItemIndex(_paramsPtr->TargetLfoParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setJustificationType(juce::Justification::centred);
    targetLfoParamSelector.addListener(this);
    body.addAndMakeVisible(targetLfoParamSelector);
    
    fadeSelector.setLookAndFeel(&grapeLookAndFeel);
    fadeSelector.addItemList(_paramsPtr->Fade->getAllValueStrings(), 1);
    fadeSelector.setSelectedItemIndex(_paramsPtr->Fade->getIndex(), juce::dontSendNotification);
    fadeSelector.setJustificationType(juce::Justification::centred);
    fadeSelector.addListener(this);
    body.addAndMakeVisible(fadeSelector);
    
    peakFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    peakFreqSlider.setRange(_paramsPtr->PeakFreq->range.start,
                           _paramsPtr->PeakFreq->range.end, 0.01);
    peakFreqSlider.setValue(_paramsPtr->PeakFreq->get(), juce::dontSendNotification);
    peakFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    peakFreqSlider.setScrollWheelEnabled(false);
    peakFreqSlider.textFromValueFunction = [](double oct) -> juce::String {
        return (oct == 0 ? " " : oct > 0 ? "+" : "-") + juce::String(std::abs(oct), 2) + " oct";
    };
    peakFreqSlider.addListener(this);
    body.addAndMakeVisible(peakFreqSlider);
    
    waitSlider.setLookAndFeel(&grapeLookAndFeel);
    waitSlider.setRange(_paramsPtr->Wait->range.start,
                           _paramsPtr->Wait->range.end, 0.01);
    waitSlider.setSkewFactorFromMidPoint(0.2f);
    waitSlider.setValue(_paramsPtr->Wait->get(), juce::dontSendNotification);
    waitSlider.setPopupDisplayEnabled(true, true, nullptr);
    waitSlider.setScrollWheelEnabled(false);
    waitSlider.setTextValueSuffix(" sec");
    waitSlider.addListener(this);
    body.addAndMakeVisible(waitSlider);
    
    attackSlider.setLookAndFeel(&grapeLookAndFeel);
    attackSlider.setRange(_paramsPtr->Attack->range.start,
                           _paramsPtr->Attack->range.end, 0.001);
    attackSlider.setSkewFactorFromMidPoint(0.2f);
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, nullptr);
    attackSlider.setScrollWheelEnabled(false);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    body.addAndMakeVisible(attackSlider);
    
    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(_paramsPtr->Decay->range.start,
                           _paramsPtr->Decay->range.end, 0.01);
    decaySlider.setSkewFactorFromMidPoint(0.4f);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
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
    
    addAndMakeVisible(body);
    
    startTimerHz(30.0f);
}

ModEnvComponent::~ModEnvComponent()
{}

void ModEnvComponent::paint(juce::Graphics& g)
{
}

void ModEnvComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(280);
        targetLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        
        auto selectorsArea = area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT);
        
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
        auto lebelArea = area.removeFromTop(LABEL_HEIGHT);
        peakFreqLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        fadeLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        peakFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
        fadeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        auto lebelArea = area.removeFromTop(LABEL_HEIGHT);
        waitLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        attackLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        waitSlider.setBounds(area.reduced(LOCAL_MARGIN));
        attackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        decayLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        decaySlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void ModEnvComponent::buttonClicked(juce::Button* button) {
    if(button == &header.enabledButton)
    {
        *_paramsPtr->Enabled = header.enabledButton.getToggleState();
    }
}
void ModEnvComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == &targetTypeSelector)
    {
        *_paramsPtr->TargetType = targetTypeSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetOscSelector)
    {
        *_paramsPtr->TargetOsc = targetOscSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetFilterSelector)
    {
        *_paramsPtr->TargetFilter = targetFilterSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetLfoSelector)
    {
        *_paramsPtr->TargetLfo = targetLfoSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetOscParamSelector)
    {
        *_paramsPtr->TargetOscParam = targetOscParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetFilterParamSelector)
    {
        *_paramsPtr->TargetFilterParam = targetFilterParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetLfoParamSelector)
    {
        *_paramsPtr->TargetLfoParam = targetLfoParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &fadeSelector)
    {
        *_paramsPtr->Fade = fadeSelector.getSelectedItemIndex();
    }
    resized();// re-render
}
void ModEnvComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &peakFreqSlider)
    {
        *_paramsPtr->PeakFreq = (float)peakFreqSlider.getValue();
    }
    else if(slider == &waitSlider)
    {
        *_paramsPtr->Wait = (float)waitSlider.getValue();
    }
    else if(slider == &attackSlider)
    {
        *_paramsPtr->Attack = (float)attackSlider.getValue();
    }
    else if(slider == &decaySlider)
    {
        *_paramsPtr->Decay = (float)decaySlider.getValue();
    }
}
void ModEnvComponent::timerCallback()
{
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    body.setEnabled(_paramsPtr->Enabled->get());
    
    targetTypeSelector.setSelectedItemIndex(_paramsPtr->TargetType->getIndex(), juce::dontSendNotification);
    targetOscSelector.setSelectedItemIndex(_paramsPtr->TargetOsc->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setSelectedItemIndex(_paramsPtr->TargetFilter->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setSelectedItemIndex(_paramsPtr->TargetLfo->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setSelectedItemIndex(_paramsPtr->TargetOscParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setSelectedItemIndex(_paramsPtr->TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setSelectedItemIndex(_paramsPtr->TargetLfoParam->getIndex(), juce::dontSendNotification);
    fadeSelector.setSelectedItemIndex(_paramsPtr->Fade->getIndex(), juce::dontSendNotification);
    
    peakFreqSlider.setValue(_paramsPtr->PeakFreq->get(), juce::dontSendNotification);
    waitSlider.setValue(_paramsPtr->Wait->get(), juce::dontSendNotification);
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    switch(static_cast<MODENV_TARGET_TYPE>(_paramsPtr->TargetType->getIndex())) {
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
    if(_paramsPtr->isTargetFreq()) {
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
        if(static_cast<MODENV_FADE>(_paramsPtr->Fade->getIndex()) == MODENV_FADE::In) {
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
DelayComponent::DelayComponent(DelayParams* params, ControlItemParams* controlItemParams)
: _paramsPtr(params)
, controlItemParams(controlItemParams)
, header("DELAY", true)
, typeSelector("Type")
, timeLSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, timeRSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, timeSyncLSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, timeSyncRSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, lowFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, highFreqSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, feedbackSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, mixSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    header.enabledButton.addListener(this);
    addAndMakeVisible(header);
    
    typeSelector.setLookAndFeel(&grapeLookAndFeel);
    typeSelector.addItemList(_paramsPtr->Type->getAllValueStrings(), 1);
    typeSelector.setSelectedItemIndex(_paramsPtr->Type->getIndex(), juce::dontSendNotification);
    typeSelector.setJustificationType(juce::Justification::centred);
    typeSelector.addListener(this);
    body.addAndMakeVisible(typeSelector);
    
    syncSelector.setLookAndFeel(&grapeLookAndFeel);
    syncSelector.addItemList(_paramsPtr->Sync->getAllValueStrings(), 1);
    syncSelector.setSelectedItemIndex(_paramsPtr->Sync->get(), juce::dontSendNotification);
    syncSelector.setJustificationType(juce::Justification::centred);
    syncSelector.addListener(this);
    body.addAndMakeVisible(syncSelector);
    
    timeLSlider.setLookAndFeel(&grapeLookAndFeel);
    timeLSlider.setRange(_paramsPtr->TimeL->range.start,
                          _paramsPtr->TimeL->range.end, 0.01);
    timeLSlider.setSkewFactorFromMidPoint(0.4f);
    timeLSlider.setValue(_paramsPtr->TimeL->get(), juce::dontSendNotification);
    timeLSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeLSlider.setScrollWheelEnabled(false);
    timeLSlider.setTextValueSuffix(" sec");
    timeLSlider.addListener(this);
    body.addAndMakeVisible(timeLSlider);
    
    timeRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeRSlider.setRange(_paramsPtr->TimeR->range.start,
                          _paramsPtr->TimeR->range.end, 0.01);
    timeRSlider.setSkewFactorFromMidPoint(0.4f);
    timeRSlider.setValue(_paramsPtr->TimeR->get(), juce::dontSendNotification);
    timeRSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeRSlider.setScrollWheelEnabled(false);
    timeRSlider.setTextValueSuffix(" sec");
    timeRSlider.addListener(this);
    body.addAndMakeVisible(timeRSlider);
    
    timeSyncLSlider.setLookAndFeel(&grapeLookAndFeel);
    timeSyncLSlider.setRange(0, DELAY_TIME_SYNC_NAMES.size() - 1, 1);
    timeSyncLSlider.setValue(_paramsPtr->TimeSyncL->getIndex(), juce::dontSendNotification);
    timeSyncLSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeSyncLSlider.setScrollWheelEnabled(false);
    timeSyncLSlider.textFromValueFunction = [](double index){ return DELAY_TIME_SYNC_NAMES[index]; };
    timeSyncLSlider.addListener(this);
    body.addAndMakeVisible(timeSyncLSlider);
    
    timeSyncRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeSyncRSlider.setRange(0, DELAY_TIME_SYNC_NAMES.size() - 1, 1);
    timeSyncRSlider.setValue(_paramsPtr->TimeSyncR->getIndex(), juce::dontSendNotification);
    timeSyncRSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeSyncRSlider.setScrollWheelEnabled(false);
    timeSyncRSlider.textFromValueFunction = [](double index){ return DELAY_TIME_SYNC_NAMES[index]; };
    timeSyncRSlider.addListener(this);
    body.addAndMakeVisible(timeSyncRSlider);
    
    lowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    lowFreqSlider.setRange(_paramsPtr->LowFreq->range.start,
                          _paramsPtr->LowFreq->range.end, 1.0);
    lowFreqSlider.setSkewFactorFromMidPoint(2000.0f);
    lowFreqSlider.setValue(_paramsPtr->LowFreq->get(), juce::dontSendNotification);
    lowFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    lowFreqSlider.setScrollWheelEnabled(false);
    lowFreqSlider.setTextValueSuffix(" Hz");
    lowFreqSlider.addListener(this);
    body.addAndMakeVisible(lowFreqSlider);
    
    highFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    highFreqSlider.setRange(_paramsPtr->HighFreq->range.start,
                          _paramsPtr->HighFreq->range.end, 1.0);
    highFreqSlider.setSkewFactorFromMidPoint(2000.0f);
    highFreqSlider.setValue(_paramsPtr->HighFreq->get(), juce::dontSendNotification);
    highFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    highFreqSlider.setScrollWheelEnabled(false);
    highFreqSlider.setTextValueSuffix(" Hz");
    highFreqSlider.addListener(this);
    body.addAndMakeVisible(highFreqSlider);
    
    feedbackSlider.setLookAndFeel(&grapeLookAndFeel);
    feedbackSlider.setRange(_paramsPtr->Feedback->range.start,
                          _paramsPtr->Feedback->range.end, 0.01);
    feedbackSlider.setSkewFactorFromMidPoint(0.4f);
    feedbackSlider.setValue(_paramsPtr->Feedback->get(), juce::dontSendNotification);
    feedbackSlider.setPopupDisplayEnabled(true, true, nullptr);
    feedbackSlider.setScrollWheelEnabled(false);
    feedbackSlider.textFromValueFunction = [](double gain){ return juce::String(gain * 100, 0) + " %"; };
    feedbackSlider.addListener(this);
    body.addAndMakeVisible(feedbackSlider);
    
    mixSlider.setLookAndFeel(&grapeLookAndFeel);
    mixSlider.setRange(_paramsPtr->Mix->range.start,
                          _paramsPtr->Mix->range.end, 0.01);
    mixSlider.setValue(_paramsPtr->Mix->get(), juce::dontSendNotification);
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

DelayComponent::~DelayComponent()
{}

void DelayComponent::paint(juce::Graphics& g)
{
}

void DelayComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(120);
        typeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        typeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(80);
        syncLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        syncSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(SLIDER_WIDTH);
        lowFreqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        lowFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(SLIDER_WIDTH);
        highFreqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        highFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        timeLLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        auto sliderBounds = area.reduced(LOCAL_MARGIN);
        timeLSlider.setBounds(sliderBounds);
        timeSyncLSlider.setBounds(sliderBounds);
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        timeRLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        auto sliderBounds = area.reduced(LOCAL_MARGIN);
        timeRSlider.setBounds(sliderBounds);
        timeSyncRSlider.setBounds(sliderBounds);
    }

    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        feedbackLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        feedbackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(SLIDER_WIDTH);
        mixLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        mixSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void DelayComponent::buttonClicked(juce::Button* button) {
    if(button == &header.enabledButton)
    {
        *_paramsPtr->Enabled = header.enabledButton.getToggleState();
    }
}
void DelayComponent::comboBoxChanged(juce::ComboBox* comboBox) {
    if(comboBox == &typeSelector)
    {
        *_paramsPtr->Type = typeSelector.getSelectedItemIndex();
    }
    else if(comboBox == &syncSelector)
    {
        *_paramsPtr->Sync = syncSelector.getSelectedItemIndex();
    }
}
void DelayComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &timeLSlider)
    {
        *_paramsPtr->TimeL = (float)timeLSlider.getValue();
    }
    else if(slider == &timeRSlider)
    {
        *_paramsPtr->TimeR = (float)timeRSlider.getValue();
    }
    else if(slider == &timeSyncLSlider)
    {
        *_paramsPtr->TimeSyncL = timeSyncLSlider.getValue();
    }
    else if(slider == &timeSyncRSlider)
    {
        *_paramsPtr->TimeSyncR = timeSyncRSlider.getValue();
    }
    else if(slider == &lowFreqSlider)
    {
        *_paramsPtr->LowFreq = (float)lowFreqSlider.getValue();
    }
    else if(slider == &highFreqSlider)
    {
        *_paramsPtr->HighFreq = (float)highFreqSlider.getValue();
    }
    else if(slider == &feedbackSlider)
    {
        *_paramsPtr->Feedback = (float)feedbackSlider.getValue();
    }
    else if(slider == &mixSlider)
    {
        *_paramsPtr->Mix = (float)mixSlider.getValue();
    }
}
void DelayComponent::timerCallback()
{
    header.enabledButton.setToggleState(_paramsPtr->Enabled->get(), juce::dontSendNotification);
    body.setEnabled(_paramsPtr->Enabled->get());
    
    typeSelector.setSelectedItemIndex(_paramsPtr->Type->getIndex(), juce::dontSendNotification);
    syncSelector.setSelectedItemIndex(_paramsPtr->Sync->get(), juce::dontSendNotification);
    timeLSlider.setVisible(!_paramsPtr->Sync->get());
    timeRSlider.setVisible(!_paramsPtr->Sync->get());
    timeSyncLSlider.setVisible(_paramsPtr->Sync->get());
    timeSyncRSlider.setVisible(_paramsPtr->Sync->get());
    timeLSlider.setValue(_paramsPtr->TimeL->get(), juce::dontSendNotification);
    timeRSlider.setValue(_paramsPtr->TimeR->get(), juce::dontSendNotification);
    timeSyncLSlider.setValue(_paramsPtr->TimeSyncL->getIndex(), juce::dontSendNotification);
    timeSyncRSlider.setValue(_paramsPtr->TimeSyncR->getIndex(), juce::dontSendNotification);

    lowFreqSlider.setValue(_paramsPtr->LowFreq->get(), juce::dontSendNotification);
    highFreqSlider.setValue(_paramsPtr->HighFreq->get(), juce::dontSendNotification);
    feedbackSlider.setValue(_paramsPtr->Feedback->get(), juce::dontSendNotification);
    mixSlider.setValue(_paramsPtr->Mix->get(), juce::dontSendNotification);
    
    mixSlider.setLookAndFeel(&grapeLookAndFeel);
    for(int i = 0; i < NUM_CONTROL; ++i) {
        auto params = &controlItemParams[i];
        if(params->Number->getIndex() <= 0) {
            continue;
        }
        auto targetType = static_cast<CONTROL_TARGET_TYPE>(params->TargetType->getIndex());
        switch(targetType) {
            case CONTROL_TARGET_TYPE::Master: {
                auto targetParam = static_cast<CONTROL_TARGET_MISC_PARAM>(params->TargetMiscParam->getIndex());
                switch(targetParam) {
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
ControlItemComponent::ControlItemComponent(ControlItemParams* params)
: _paramsPtr(params)
, numberSelector("Number")
, targetTypeSelector("TargetType")
, targetOscSelector("TargetOsc")
, targetFilterSelector("TargetFilter")
, targetOscParamSelector("TargetOscParam")
, targetFilterParamSelector("TargetFilterParam")
, targetMiscParamSelector("TargetMiscParam")
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    numberSelector.setLookAndFeel(&grapeLookAndFeel);
    numberSelector.addItemList(_paramsPtr->Number->getAllValueStrings(), 1);
    numberSelector.setSelectedItemIndex(_paramsPtr->Number->getIndex(), juce::dontSendNotification);
    numberSelector.setJustificationType(juce::Justification::centred);
    numberSelector.addListener(this);
    addAndMakeVisible(numberSelector);
    
    targetTypeSelector.setLookAndFeel(&grapeLookAndFeel);
    targetTypeSelector.addItemList(_paramsPtr->TargetType->getAllValueStrings(), 1);
    targetTypeSelector.setSelectedItemIndex(_paramsPtr->TargetType->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setJustificationType(juce::Justification::centred);
    targetTypeSelector.addListener(this);
    addAndMakeVisible(targetTypeSelector);
    
    targetOscSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscSelector.addItemList(_paramsPtr->TargetOsc->getAllValueStrings(), 1);
    targetOscSelector.setSelectedItemIndex(_paramsPtr->TargetOsc->getIndex(), juce::dontSendNotification);
    targetOscSelector.setJustificationType(juce::Justification::centred);
    targetOscSelector.addListener(this);
    addAndMakeVisible(targetOscSelector);
    
    targetFilterSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterSelector.addItemList(_paramsPtr->TargetFilter->getAllValueStrings(), 1);
    targetFilterSelector.setSelectedItemIndex(_paramsPtr->TargetFilter->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setJustificationType(juce::Justification::centred);
    targetFilterSelector.addListener(this);
    addAndMakeVisible(targetFilterSelector);
    
    targetLfoSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoSelector.addItemList(_paramsPtr->TargetLfo->getAllValueStrings(), 1);
    targetLfoSelector.setSelectedItemIndex(_paramsPtr->TargetLfo->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setJustificationType(juce::Justification::centred);
    targetLfoSelector.addListener(this);
    addAndMakeVisible(targetLfoSelector);
    
    targetOscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetOscParamSelector.addItemList(_paramsPtr->TargetOscParam->getAllValueStrings(), 1);
    targetOscParamSelector.setSelectedItemIndex(_paramsPtr->TargetOscParam->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setJustificationType(juce::Justification::centred);
    targetOscParamSelector.addListener(this);
    addAndMakeVisible(targetOscParamSelector);
    
    targetFilterParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetFilterParamSelector.addItemList(_paramsPtr->TargetFilterParam->getAllValueStrings(), 1);
    targetFilterParamSelector.setSelectedItemIndex(_paramsPtr->TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setJustificationType(juce::Justification::centred);
    targetFilterParamSelector.addListener(this);
    addAndMakeVisible(targetFilterParamSelector);
    
    targetLfoParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetLfoParamSelector.addItemList(_paramsPtr->TargetLfoParam->getAllValueStrings(), 1);
    targetLfoParamSelector.setSelectedItemIndex(_paramsPtr->TargetLfoParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setJustificationType(juce::Justification::centred);
    targetLfoParamSelector.addListener(this);
    addAndMakeVisible(targetLfoParamSelector);
    
    targetMiscParamSelector.setLookAndFeel(&grapeLookAndFeel);
    targetMiscParamSelector.addItemList(_paramsPtr->TargetMiscParam->getAllValueStrings(), 1);
    targetMiscParamSelector.setSelectedItemIndex(_paramsPtr->TargetMiscParam->getIndex(), juce::dontSendNotification);
    targetMiscParamSelector.setJustificationType(juce::Justification::centred);
    targetMiscParamSelector.addListener(this);
    addAndMakeVisible(targetMiscParamSelector);
    
    startTimerHz(30.0f);
}

ControlItemComponent::~ControlItemComponent()
{}

void ControlItemComponent::paint(juce::Graphics& g)
{
}

void ControlItemComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(bounds.getWidth()/5).reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT);
        numberSelector.setBounds(area);
    }
    {
        juce::Rectangle<int> area = bounds.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT);
        
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
void ControlItemComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == &numberSelector)
    {
        *_paramsPtr->Number = numberSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetTypeSelector)
    {
        *_paramsPtr->TargetType = targetTypeSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetOscSelector)
    {
        *_paramsPtr->TargetOsc = targetOscSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetFilterSelector)
    {
        *_paramsPtr->TargetFilter = targetFilterSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetLfoSelector)
    {
        *_paramsPtr->TargetLfo = targetLfoSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetOscParamSelector)
    {
        *_paramsPtr->TargetOscParam = targetOscParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetFilterParamSelector)
    {
        *_paramsPtr->TargetFilterParam = targetFilterParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetLfoParamSelector)
    {
        *_paramsPtr->TargetLfoParam = targetLfoParamSelector.getSelectedItemIndex();
    }
    else if(comboBoxThatHasChanged == &targetMiscParamSelector)
    {
        *_paramsPtr->TargetMiscParam = targetMiscParamSelector.getSelectedItemIndex();
    }
    resized();// re-render
}
void ControlItemComponent::timerCallback()
{
    numberSelector.setSelectedItemIndex(_paramsPtr->Number->getIndex(), juce::dontSendNotification);
    targetTypeSelector.setSelectedItemIndex(_paramsPtr->TargetType->getIndex(), juce::dontSendNotification);
    targetOscSelector.setSelectedItemIndex(_paramsPtr->TargetOsc->getIndex(), juce::dontSendNotification);
    targetFilterSelector.setSelectedItemIndex(_paramsPtr->TargetFilter->getIndex(), juce::dontSendNotification);
    targetLfoSelector.setSelectedItemIndex(_paramsPtr->TargetLfo->getIndex(), juce::dontSendNotification);
    targetOscParamSelector.setSelectedItemIndex(_paramsPtr->TargetOscParam->getIndex(), juce::dontSendNotification);
    targetFilterParamSelector.setSelectedItemIndex(_paramsPtr->TargetFilterParam->getIndex(), juce::dontSendNotification);
    targetLfoParamSelector.setSelectedItemIndex(_paramsPtr->TargetLfoParam->getIndex(), juce::dontSendNotification);
    targetMiscParamSelector.setSelectedItemIndex(_paramsPtr->TargetMiscParam->getIndex(), juce::dontSendNotification);
    auto enabled = _paramsPtr->Number->getIndex() != 0;
    targetTypeSelector.setEnabled(enabled);
    targetOscSelector.setEnabled(enabled);
    targetFilterSelector.setEnabled(enabled);
    targetLfoSelector.setEnabled(enabled);
    targetOscParamSelector.setEnabled(enabled);
    targetFilterParamSelector.setEnabled(enabled);
    targetLfoParamSelector.setEnabled(enabled);
    targetMiscParamSelector.setEnabled(enabled);
    switch(static_cast<CONTROL_TARGET_TYPE>(_paramsPtr->TargetType->getIndex())) {
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
ControlComponent::ControlComponent(ControlItemParams* params)
: header("CONTROLS", false)
, controlItemComponents {
    ControlItemComponent(params),
    ControlItemComponent(params+1),
    ControlItemComponent(params+2),
    ControlItemComponent(params+3),
    ControlItemComponent(params+4),
    ControlItemComponent(params+5)
}
{
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
    
    for(int i = 0; i < NUM_CONTROL; i++) {
        addAndMakeVisible(controlItemComponents[i]);
    }
}

ControlComponent::~ControlComponent()
{}

void ControlComponent::paint(juce::Graphics& g)
{
}

void ControlComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    auto width = bounds.getWidth();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    auto labelArea = bounds.removeFromTop(LABEL_HEIGHT);
    numberLabel.setBounds(labelArea.removeFromLeft(width / 5));
    targetLabel.setBounds(labelArea);
    
    auto itemHeight = bounds.getHeight() / NUM_CONTROL;
    for(int i = 0; i < NUM_CONTROL; i++) {
        controlItemComponents[i].setBounds(bounds.removeFromTop(itemHeight));
    }
}

//==============================================================================
AnalyserToggleItem::AnalyserToggleItem(std::string name)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    nameLabel.setFont(paramLabelFont);
    nameLabel.setText(name, juce::dontSendNotification);
    nameLabel.setJustificationType(juce::Justification::right);
    nameLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(nameLabel);
}
AnalyserToggleItem::~AnalyserToggleItem() {}
void AnalyserToggleItem::paint(juce::Graphics& g)
{
    juce::Rectangle<int> bounds = getLocalBounds().removeFromRight(3).reduced(0, 4);
    
    auto color = value ? COLOUR_SELECT : COLOUR_PIT;
    g.setColour(color);
    g.fillRect(bounds);
}
void AnalyserToggleItem::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    bounds.removeFromRight(5);
    nameLabel.setBounds(bounds);
}
void AnalyserToggleItem::addListener (Listener* l) {
    listeners.add (l);
}
void AnalyserToggleItem::mouseUp (const juce::MouseEvent& e)
{
    std::cout << "mouseup:" << nameLabel.getText() << std::endl;
    Component::BailOutChecker checker (this);
//    if (checker.shouldBailOut()) {
//        return;
//    }
    if (e.mouseWasClicked()) {
        if(!value) {
            value = true;
            listeners.callChecked (checker, [this] (AnalyserToggleItem::Listener& l) { l.toggleItemSelected (this); });
        }
    }
}

//==============================================================================
AnalyserToggle::AnalyserToggle(ANALYSER_MODE* analyserMode)
: analyserMode(analyserMode)
, spectrumToggle("Spectrum")
, envelopeToggle("Envelope")
, filterToggle("Filter")
{
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
AnalyserToggle::~AnalyserToggle() {
}
void AnalyserToggle::paint(juce::Graphics& g)
{
}
void AnalyserToggle::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds().reduced(2, 4);
    spectrumToggle.setBounds(bounds.removeFromTop(25));
    envelopeToggle.setBounds(bounds.removeFromTop(25));
    filterToggle.setBounds(bounds.removeFromTop(25));
}
void AnalyserToggle::toggleItemSelected(AnalyserToggleItem* toggleItem)
{
    if(toggleItem == &spectrumToggle) {
        *analyserMode = ANALYSER_MODE::Spectrum;
    }
    else if(toggleItem == &envelopeToggle) {
        *analyserMode = ANALYSER_MODE::Envelope;
    }
    else if(toggleItem == &filterToggle) {
        *analyserMode = ANALYSER_MODE::Filter;
    }
    spectrumToggle.setValue(*analyserMode == ANALYSER_MODE::Spectrum);
    envelopeToggle.setValue(*analyserMode == ANALYSER_MODE::Envelope);
    filterToggle.setValue(*analyserMode == ANALYSER_MODE::Filter);
}

//==============================================================================
AnalyserWindow::AnalyserWindow(ANALYSER_MODE* analyserMode, LatestDataProvider* latestDataProvider, MonoStack* monoStack, EnvelopeParams* envelopeParams, OscParams* oscParams, FilterParams* filterParams, ModEnvParams* modEnvParams)
: analyserMode(analyserMode)
, latestDataProvider(latestDataProvider)
, monoStack(monoStack)
, envelopeParams(envelopeParams)
, oscParams(oscParams)
, filterParams(filterParams)
, modEnvParams(modEnvParams)
, forwardFFT (fftOrder)
, window (fftSize, juce::dsp::WindowingFunction<float>::hann)
, lastAdsrParams { SimpleAdsrParams(), SimpleAdsrParams() }
, lastModEnvParams { SimpleModEnvParams(), SimpleModEnvParams(), SimpleModEnvParams() }
{
    latestDataProvider->addConsumer(&fftConsumer);
    latestDataProvider->addConsumer(&levelConsumer);

    startTimerHz(30.0f);
}
AnalyserWindow::~AnalyserWindow() {
    latestDataProvider->removeConsumer(&fftConsumer);
    latestDataProvider->removeConsumer(&levelConsumer);
}

void AnalyserWindow::resized() {}
void AnalyserWindow::timerCallback()
{
    bool shouldRepaint = false;
    
    switch(*analyserMode){
        case ANALYSER_MODE::Spectrum: {
            lastAnalyserMode = ANALYSER_MODE::Spectrum;
            if (fftConsumer.ready)
            {
                drawNextFrameOfSpectrum();
                fftConsumer.ready = false;
                readyToDrawFrame = true;
                shouldRepaint = true;
            }
            if (levelConsumer.ready)
            {
                drawNextFrameOfLevel();
                levelConsumer.ready = false;
        //        readyToDrawFrame = true;
                shouldRepaint = true;
            }
            break;
        }
        case ANALYSER_MODE::Envelope: {
            if(lastAnalyserMode != ANALYSER_MODE::Envelope) {
                shouldRepaint = true;
            }
            lastAnalyserMode = ANALYSER_MODE::Envelope;
            auto changed = false;
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                auto p = SimpleAdsrParams(envelopeParams[i]);
                if(!lastAdsrParams[i].equals(p)) {
                    changed = true;
                }
                lastAdsrParams[i] = p;
            }
            for(int i = 0; i < NUM_MODENV; i++) {
                auto p = SimpleModEnvParams(modEnvParams[i]);
                if(!lastModEnvParams[i].equals(p)) {
                    changed = true;
                }
                lastModEnvParams[i] = p;
            }
            if(!changed) {
                break;
            }
            auto maxAD = std::max({ envelopeParams[0].Attack->get() + envelopeParams[0].Decay->get() * 4,
                                    envelopeParams[1].Attack->get() + envelopeParams[1].Decay->get() * 4,
                                    (modEnvParams[0].shouldUseHold() ? modEnvParams[0].Wait->get() : modEnvParams[0].Attack->get()) + modEnvParams[0].Decay->get() * 4,
                                    (modEnvParams[1].shouldUseHold() ? modEnvParams[1].Wait->get() : modEnvParams[1].Attack->get()) + modEnvParams[1].Decay->get() * 4,
                                    (modEnvParams[2].shouldUseHold() ? modEnvParams[2].Wait->get() : modEnvParams[2].Attack->get()) + modEnvParams[2].Decay->get() * 4 });
            auto maxR = std::max({ envelopeParams[0].Release->get() * 4,
                                   envelopeParams[1].Release->get() * 4 });
            auto maxSec = maxAD + maxR;
            auto sampleRate = (float)scopeSize / maxSec;
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].setParams(envelopeParams[i].Attack->get(),
                                  0,
                                  envelopeParams[i].Decay->get(),
                                  envelopeParams[i].Sustain->get(),
                                  envelopeParams[i].Release->get());
            }
            for(int i = 0; i < NUM_MODENV; i++) {
                if(modEnvParams[i].shouldUseHold()) {
                    modEnvs[i].setParams(0.0,
                                         modEnvParams[i].Wait->get(),
                                         modEnvParams[i].Decay->get(),
                                         0.0,
                                         0.0);
                } else {
                    modEnvs[i].setParams(modEnvParams[i].Attack->get(),
                                         0.0,
                                         modEnvParams[i].Decay->get(),
                                         0.0,
                                         0.0);
                }
            }
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].doAttack(sampleRate);
            }
            for(int i = 0; i < NUM_MODENV; i++) {
                modEnvs[i].doAttack(sampleRate);
            }
            int releasePoint = sampleRate * maxAD;
            for(int pos = 0; pos < scopeSize; pos++) {
                if(pos == releasePoint) {
                    for(int i = 0; i < NUM_ENVELOPE; i++) {
                        adsr[i].doRelease(sampleRate);
                    }
                }
                for(int i = 0; i < NUM_ENVELOPE; i++) {
                    scopeDataForEnvelope[i][pos] = adsr[i].getValue();
                    adsr[i].step(sampleRate);
                }
                for(int i = 0; i < NUM_MODENV; i++) {
                    auto value = 0.0f;
                    if(modEnvParams[i].Enabled->get()) {
                        value = modEnvs[i].getValue();
                        if(!modEnvParams[i].isTargetFreq() && static_cast<MODENV_FADE>(modEnvParams[i].Fade->getIndex()) == MODENV_FADE::In) {
                            value = 1 - value;
                        }
                    }
                    scopeDataForEnvelope[i + NUM_ENVELOPE][pos] = value;
                    modEnvs[i].step(sampleRate);
                }
            }
            for(int i = 0; i < NUM_ENVELOPE; i++) {
                adsr[i].forceStop();
            }
            for(int i = 0; i < NUM_MODENV; i++) {
                modEnvs[i].forceStop();
            }
            readyToDrawFrame = true;
            shouldRepaint = true;
            break;
        }
        case ANALYSER_MODE::Filter: {
            if(lastAnalyserMode != ANALYSER_MODE::Filter) {
                shouldRepaint = true;
            }
            lastAnalyserMode = ANALYSER_MODE::Filter;
            
            bool noteChanged = false;
            if(monoStack->latestNoteNumber != 0) {
                auto newNoteNumber = monoStack->latestNoteNumber;
                noteChanged = relNoteNumber != newNoteNumber;
                relNoteNumber = newNoteNumber;
            }
            for(int i = 0; i < NUM_FILTER; ++i) {
                auto filterType = static_cast<FILTER_TYPE>(filterParams[i].Type->getIndex());
                double freq;
                bool isRel = false;
                switch(static_cast<FILTER_FREQ_TYPE>(filterParams[i].FreqType->getIndex())) {
                    case FILTER_FREQ_TYPE::Absolute: {
                        freq = filterParams[i].Hz->get();
                        break;
                    }
                    case FILTER_FREQ_TYPE::Relative: {
                        isRel = true;
                        auto target = filterParams[i].Target->getIndex();
                        float lastNote = relNoteNumber;
                        if(target != NUM_OSC) {
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
                if(lastFilterParams[i].equals(params) && !(isRel && noteChanged)) {
                    continue;
                }
                lastFilterParams[i] = params;
                
                readyToDrawFrame = true;
                shouldRepaint = true;
                
                if(!filterParams[i].Enabled->get()) {
                    // TODO: utility?
                    for (int j = 0; j < scopeSize; ++j)
                    {
                        scopeDataForFilter[i][j] = 0;
                    }
                    continue;
                }
                auto& filter = filters[i];
                auto sampleRate = 48000;
                filter.setSampleRate(sampleRate);// TODO: ?
                filter.initializePastData();
                std::fill_n(filterSource, fftSize * 2, 0);
                filterSource[0] = 1;
                for(int i = 0; i < fftSize; i++) {
                    filterSource[i] = filter.step(filterType, freq, q, gain, 0, filterSource[i]);
                }
                forwardFFT.performFrequencyOnlyForwardTransform(filterSource);
                
                auto minFreq = 20.0f;
                auto maxFreq = 20000.0f;
                auto mindB = -50.0f;
                auto maxdB =  50.0f;
                for (int j = 0; j < scopeSize; ++j)
                {
                    float indexFloat = (minFreq * std::pow(maxFreq / minFreq, (float)j / scopeSize)) * ((fftSize * 0.5) / (sampleRate * 0.5));
                    int index = indexFloat;
                    float frac = indexFloat - index;
                    float gain = filterSource[index] * (1 - frac) + filterSource[index + 1] * frac;
                    auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels(gain)),
                                             mindB, maxdB, 0.0f, 1.0f);
                    scopeDataForFilter[i][j] = level;
                }
            }
            break;
        }
    }
    if(shouldRepaint) {
        repaint();
    }
}
void AnalyserWindow::drawNextFrameOfSpectrum()
{
    for(int i = 0; i < fftSize; i++) {
        fftData[i] = (fftData[i] + fftData[i + fftSize]) * 0.5f;
        fftData[i + fftSize] = 0;
    }
    window.multiplyWithWindowingTable(fftData, fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);

    auto mindB = -100.0f;
    auto maxdB =    0.0f;

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
        auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (fftData[fftDataIndex])
                                                           - juce::Decibels::gainToDecibels ((float) fftSize)),
                                 mindB, maxdB, 0.0f, 1.0f);
        scopeData[i] = level;
    }
}
void AnalyserWindow::drawNextFrameOfLevel()
{
    auto mindB = -100.0f;
    auto maxdB =    0.0f;
    for(int i = 0; i < 2; i++) {
        auto* data = i == 0 ? levelConsumer.destinationL : levelConsumer.destinationR;
        auto db = calcCurrentLevel(levelConsumer.numSamples, data);
        currentLevel[i] = juce::jmap(juce::jlimit(mindB, maxdB, db), mindB, maxdB, 0.0f, 1.0f);
        if(db > 0) {
            (i == 0 ? overflowedLevelL : overflowedLevelR) = db;
            (i == 0 ? overflowWarningL : overflowWarningR) = 30 * 1.2;
        }
    }
}
void AnalyserWindow::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    juce::Rectangle<int> bounds = getLocalBounds();
    g.setColour(juce::Colour(30,30,30));
    g.drawRect(bounds, 2.0f);
//    g.setOpacity(1.0f);
    
    if(readyToDrawFrame) {
        auto offsetX = 2;
        auto offsetY = 2;
        bounds.reduce(offsetX, offsetY);
        auto height = bounds.getHeight();
        
        switch(*analyserMode) {
            case ANALYSER_MODE::Spectrum: {
                auto levelWidth = 8;
                auto spectrumWidth = bounds.getWidth() - levelWidth * 2;

                paintSpectrum(g, ANALYSER_LINE_COLOUR, offsetX, offsetY, spectrumWidth, height, scopeData);
                offsetX += spectrumWidth;
                paintLevel(g, offsetX, offsetY, levelWidth, height, currentLevel[0]);
                offsetX += levelWidth;
                paintLevel(g, offsetX, offsetY, levelWidth, height, currentLevel[1]);
                break;
            }
            case ANALYSER_MODE::Envelope: {
                for(int i = NUM_ENVELOPE + NUM_MODENV - 1; i >= 0; i--) {
                    auto spectrumWidth = bounds.getWidth();
                    juce::Colour colour = i >= NUM_ENVELOPE ? ANALYSER_LINE_COLOUR2 : ANALYSER_LINE_COLOUR;
                    paintSpectrum(g, colour, offsetX, offsetY, spectrumWidth, height, &scopeDataForEnvelope[i][0]);
                }
                break;
            }
            case ANALYSER_MODE::Filter: {
                for(int i = 0; i < NUM_FILTER; i++) {
                    auto spectrumWidth = bounds.getWidth();
                    bool isRel = static_cast<FILTER_FREQ_TYPE>(filterParams[i].FreqType->getIndex()) == FILTER_FREQ_TYPE::Relative;
                    juce::Colour colour = isRel ? ANALYSER_LINE_COLOUR2 : ANALYSER_LINE_COLOUR;
                    paintSpectrum(g, colour, offsetX, offsetY, spectrumWidth, height, &scopeDataForFilter[i][0]);
                }
                break;
            }
        }
    }
}
void AnalyserWindow::paintSpectrum(juce::Graphics& g, juce::Colour colour, int offsetX, int offsetY, int width, int height, float* scopeData)
{
    g.setColour(colour);
    for (int i = 1; i < scopeSize; ++i)
    {
        g.drawLine ({ offsetX + (float) juce::jmap (i - 1, 0, scopeSize - 1, 0, width),
                      offsetY - 0.5f +         juce::jmap (scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                      offsetX + (float) juce::jmap (i,     0, scopeSize - 1, 0, width),
                      offsetY - 0.5f +         juce::jmap (scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }
}
void AnalyserWindow::paintLevel(juce::Graphics& g, int offsetX, int offsetY, int width, int height, float level)
{
    g.setColour(ANALYSER_LINE_COLOUR);
    if(overflowWarningL > 0) {
        g.setColour(ERROR_COLOUR);
        overflowWarningL--;
    }
    int barWidth = width - 1;
    int barHeight = juce::jmax(1.0f, level * height);
    g.fillRect(offsetX + 1, offsetY + height - barHeight, barWidth, barHeight);
}
