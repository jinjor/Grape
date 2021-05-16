#include <JuceHeader.h>
#include "Components.h"
#include "Params.h"

namespace {
const float PANEL_NAME_FONT_SIZE = 15.0f;
const float PARAM_LABEL_FONT_SIZE = 14.0f;
const float PARAM_VALUE_LABEL_FONT_SIZE = 16.0f;
const float PANEL_NAME_HEIGHT = 26.0;
const float LOCAL_MARGIN = 2.0;
const float LABEL_HEIGHT = 20.0;
const float COMBO_BOX_HEIGHT = 28.0;
const juce::Colour TEXT_COLOUR = juce::Colour(200,200,200);
const juce::Colour PANEL_NAME_COLOUR = juce::Colour(50,50,50);
}

//==============================================================================
HeaderComponent::HeaderComponent(std::string name, bool hasEnableButton)
: enabledButton("Enabled")
, name(name)
, hasEnableButton (hasEnableButton)
{
    if(hasEnableButton) {
        addAndMakeVisible(enabledButton);
    }
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
                                     pathBounds.getWidth() + PANEL_NAME_HEIGHT + 4.0)
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
VoiceComponent::VoiceComponent(VoiceParams* params)
: _paramsPtr(params)
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
                          _paramsPtr->PortamentoTime->range.end, 0.001);
    portamentoTimeSlider.setSkewFactorFromMidPoint(0.4);
    portamentoTimeSlider.setValue(_paramsPtr->PortamentoTime->get(), juce::dontSendNotification);
    portamentoTimeSlider.setPopupDisplayEnabled(true, true, nullptr);
    portamentoTimeSlider.setPopupMenuEnabled(true);
    portamentoTimeSlider.setTextValueSuffix(" sec");
    portamentoTimeSlider.addListener(this);
    addAndMakeVisible(portamentoTimeSlider);
    
    pitchBendRangeSlider.setLookAndFeel(&grapeLookAndFeel);
    pitchBendRangeSlider.setRange(_paramsPtr->PitchBendRange->getRange().getStart(),
                                  _paramsPtr->PitchBendRange->getRange().getEnd(), 1);
    pitchBendRangeSlider.setValue(_paramsPtr->PitchBendRange->get(), juce::dontSendNotification);
    pitchBendRangeSlider.setPopupDisplayEnabled(true, true, nullptr);
    pitchBendRangeSlider.setPopupMenuEnabled(true);
    pitchBendRangeSlider.addListener(this);
    addAndMakeVisible(pitchBendRangeSlider);
    
    modeLabel.setFont(paramLabelFont);
    modeLabel.setText("Mode", juce::dontSendNotification);
    modeLabel.setJustificationType(juce::Justification::centred);
    modeLabel.setEditable(false, false, false);
    addAndMakeVisible(modeLabel);
    
    portamentoTimeLabel.setFont(paramLabelFont);
    portamentoTimeLabel.setText("Portamento Time", juce::dontSendNotification);
    portamentoTimeLabel.setJustificationType(juce::Justification::centred);
    portamentoTimeLabel.setEditable(false, false, false);
    addAndMakeVisible(portamentoTimeLabel);
    
    pitchBendRangeLabel.setFont(paramLabelFont);
    pitchBendRangeLabel.setText("PitchBend Range", juce::dontSendNotification);
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
    int height = 60;
    
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    bounds.reduce(0, 10);
    auto boundsWidth = bounds.getWidth();
    {
//        float selectorWidth = 80.0f;
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.24).removeFromTop(height);
        modeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        modeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.38).removeFromTop(height);
        portamentoTimeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        portamentoTimeSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.38).removeFromTop(height);
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
}

//==============================================================================
StatusComponent::StatusComponent(int* polyphony, TimeConsumptionState* timeConsumptionState)
: polyphony(polyphony)
, timeConsumptionState(timeConsumptionState)
, header("STATUS", false)
{
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    juce::Font paramValueLabelFont = juce::Font(PARAM_VALUE_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    
    header.enabledButton.setLookAndFeel(&grapeLookAndFeel);
    addAndMakeVisible(header);
    
    polyphonyValueLabel.setFont(paramValueLabelFont);
    polyphonyValueLabel.setText(juce::String(*polyphony), juce::dontSendNotification);
    polyphonyValueLabel.setJustificationType(juce::Justification::centred);
    polyphonyValueLabel.setEditable(false, false, false);
    addAndMakeVisible(polyphonyValueLabel);
    
    timeConsumptionValueLabel.setFont(paramValueLabelFont);
    timeConsumptionValueLabel.setText(juce::String(juce::roundToInt(timeConsumptionState->currentTimeConsumptionRate * 100)) + "%", juce::dontSendNotification);
    timeConsumptionValueLabel.setJustificationType(juce::Justification::centred);
    timeConsumptionValueLabel.setEditable(false, false, false);
    addAndMakeVisible(timeConsumptionValueLabel);
    
    polyphonyLabel.setFont(paramLabelFont);
    polyphonyLabel.setText("Polyphony", juce::dontSendNotification);
    polyphonyLabel.setJustificationType(juce::Justification::centred);
    polyphonyLabel.setEditable(false, false, false);
    addAndMakeVisible(polyphonyLabel);
    
    timeConsumptionLabel.setFont(paramLabelFont);
    timeConsumptionLabel.setText("Time Consumption", juce::dontSendNotification);
    timeConsumptionLabel.setJustificationType(juce::Justification::centred);
    timeConsumptionLabel.setEditable(false, false, false);
    addAndMakeVisible(timeConsumptionLabel);
    
    startTimerHz(30.0f);
}

StatusComponent::~StatusComponent()
{}

void StatusComponent::paint(juce::Graphics& g)
{
}

void StatusComponent::resized()
{
    int height = 60;
    
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    bounds.reduce(0, 10);
    auto boundsWidth = bounds.getWidth();
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.5).removeFromTop(height);
        polyphonyLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        polyphonyValueLabel.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(boundsWidth * 0.5).removeFromTop(height);
        timeConsumptionLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        timeConsumptionValueLabel.setBounds(area.reduced(LOCAL_MARGIN));
    }
}
void StatusComponent::timerCallback()
{
    polyphonyValueLabel.setText(juce::String(*polyphony), juce::dontSendNotification);
    timeConsumptionValueLabel.setText(juce::String(juce::roundToInt(timeConsumptionState->currentTimeConsumptionRate * 100)) + "%", juce::dontSendNotification);
}

//==============================================================================
OscComponent::OscComponent(int index, OscParams* params)
: index(index)
, _paramsPtr(params)
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
    edgeSlider.setPopupMenuEnabled(true);
    edgeSlider.addListener(this);
    body.addAndMakeVisible(edgeSlider);
    
    octaveSlider.setLookAndFeel(&grapeLookAndFeel);
    octaveSlider.setRange(_paramsPtr->Octave->getRange().getStart(),
                          _paramsPtr->Octave->getRange().getEnd(), 1);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    octaveSlider.setPopupDisplayEnabled(true, true, nullptr);
    octaveSlider.setPopupMenuEnabled(true);
    octaveSlider.addListener(this);
    body.addAndMakeVisible(octaveSlider);
    
    coarseSlider.setLookAndFeel(&grapeLookAndFeel);
    coarseSlider.setRange(_paramsPtr->Coarse->getRange().getStart(),
                         _paramsPtr->Coarse->getRange().getEnd(), 1);
    coarseSlider.setValue(_paramsPtr->Coarse->get(), juce::dontSendNotification);
    coarseSlider.setPopupDisplayEnabled(true, true, nullptr);
    coarseSlider.setPopupMenuEnabled(true);
    coarseSlider.addListener(this);
    body.addAndMakeVisible(coarseSlider);
    
    unisonSlider.setLookAndFeel(&grapeLookAndFeel);
    unisonSlider.setRange(_paramsPtr->Unison->getRange().getStart(),
                         _paramsPtr->Unison->getRange().getEnd(), 1);
    unisonSlider.setValue(_paramsPtr->Unison->get(), juce::dontSendNotification);
    unisonSlider.setPopupDisplayEnabled(true, true, nullptr);
    unisonSlider.setPopupMenuEnabled(true);
    unisonSlider.addListener(this);
    body.addAndMakeVisible(unisonSlider);
    
    detuneSlider.setLookAndFeel(&grapeLookAndFeel);
    detuneSlider.setRange(_paramsPtr->Detune->range.start,
                         _paramsPtr->Detune->range.end, 0.01);
    detuneSlider.setValue(_paramsPtr->Detune->get(), juce::dontSendNotification);
    detuneSlider.setPopupDisplayEnabled(true, true, nullptr);
    detuneSlider.setPopupMenuEnabled(true);
    detuneSlider.addListener(this);
    body.addAndMakeVisible(detuneSlider);
    
    spreadSlider.setLookAndFeel(&grapeLookAndFeel);
    spreadSlider.setRange(_paramsPtr->Spread->range.start,
                         _paramsPtr->Spread->range.end, 0.01);
    spreadSlider.setValue(_paramsPtr->Spread->get(), juce::dontSendNotification);
    spreadSlider.setPopupDisplayEnabled(true, true, nullptr);
    spreadSlider.setPopupMenuEnabled(true);
    spreadSlider.addListener(this);
    body.addAndMakeVisible(spreadSlider);
    
    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setRange(_paramsPtr->Gain->range.start,
                         _paramsPtr->Gain->range.end, 0.01);
    gainSlider.setValue(_paramsPtr->Gain->get(), juce::dontSendNotification);
    gainSlider.setPopupDisplayEnabled(true, true, nullptr);
    gainSlider.setPopupMenuEnabled(true);
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
    int width = 60;
    int height = 60;
    
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
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth).removeFromTop(height);
        envelopeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        envelopeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        float selectorWidth = 120.0f;
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth).removeFromTop(height);
        waveformLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        waveformSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        edgeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        edgeSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        gainLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        gainSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        octaveLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        octaveSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        coarseLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        coarseSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        unisonLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        unisonSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        detuneLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        detuneSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
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
    edgeSlider.setEnabled(OSC_WAVEFORM_VALUES[_paramsPtr->Waveform->getIndex()] == WAVEFORM::Square ||
                          OSC_WAVEFORM_VALUES[_paramsPtr->Waveform->getIndex()] == WAVEFORM::Triangle);
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
    attackSlider.setPopupMenuEnabled(true);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    
    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(_paramsPtr->Decay->range.start,
                         _paramsPtr->Decay->range.end, 0.01);
    decaySlider.setSkewFactorFromMidPoint(0.4);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, nullptr);
    decaySlider.setPopupMenuEnabled(true);
    decaySlider.setTextValueSuffix(" sec");
    decaySlider.addListener(this);
    addAndMakeVisible(decaySlider);
    
    sustainSlider.setLookAndFeel(&grapeLookAndFeel);
    sustainSlider.setRange(_paramsPtr->Sustain->range.start,
                           _paramsPtr->Sustain->range.end, 0.01);
    sustainSlider.setValue(_paramsPtr->Sustain->get(), juce::dontSendNotification);
    sustainSlider.setPopupDisplayEnabled(true, true, nullptr);
    sustainSlider.setPopupMenuEnabled(true);
    sustainSlider.addListener(this);
    addAndMakeVisible(sustainSlider);
    
    releaseSlider.setLookAndFeel(&grapeLookAndFeel);
    releaseSlider.setRange(_paramsPtr->Release->range.start,
                           _paramsPtr->Release->range.end, 0.01);
    releaseSlider.setSkewFactorFromMidPoint(0.4);
    releaseSlider.setValue(_paramsPtr->Release->get(), juce::dontSendNotification);
    releaseSlider.setPopupDisplayEnabled(true, true, nullptr);
    releaseSlider.setPopupMenuEnabled(true);
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
    int width = 60;
    int height = 60;
    
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight/2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        attackLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        attackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        decayLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        decaySlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        sustainLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        sustainSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
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
FilterComponent::FilterComponent(int index, FilterParams* params)
: index(index)
, _paramsPtr(params)
, header("FILTER " + std::to_string(index+1), true)
, targetSelector("Target")
, typeSelector("Type")
, freqTypeSelector("Type")
, hzSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, octaveSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, qSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
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
                         _paramsPtr->Hz->range.end, 1.0);
    hzSlider.setSkewFactorFromMidPoint(2000.0f);
    hzSlider.setValue(_paramsPtr->Hz->get(), juce::dontSendNotification);
    hzSlider.setPopupDisplayEnabled(true, true, nullptr);
    hzSlider.setPopupMenuEnabled(true);
    hzSlider.setTextValueSuffix(" Hz");
    hzSlider.addListener(this);
    body.addAndMakeVisible(hzSlider);
    
    octaveSlider.setLookAndFeel(&grapeLookAndFeel);
    octaveSlider.setRange(_paramsPtr->Octave->range.start,
                         _paramsPtr->Octave->range.end, 1.0/12);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    octaveSlider.setPopupDisplayEnabled(true, true, nullptr);
    octaveSlider.setPopupMenuEnabled(true);
    octaveSlider.setTextValueSuffix(" oct");
    octaveSlider.addListener(this);
    body.addAndMakeVisible(octaveSlider);
    
    qSlider.setLookAndFeel(&grapeLookAndFeel);
    qSlider.setRange(_paramsPtr->Q->range.start,
                           _paramsPtr->Q->range.end, 0.01);
    qSlider.setSkewFactorFromMidPoint(1.0f);
    qSlider.setValue(_paramsPtr->Q->get(), juce::dontSendNotification);
    qSlider.setPopupDisplayEnabled(true, true, nullptr);
    qSlider.setPopupMenuEnabled(true);
    qSlider.addListener(this);
    body.addAndMakeVisible(qSlider);

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
    int width = 60;
    int height = 60;
    
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
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth).removeFromTop(height);
        targetLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        targetSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        float selectorWidth = 120.0f;
        juce::Rectangle<int> area = upperArea.removeFromLeft(selectorWidth).removeFromTop(height);
        typeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        typeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        float selectorWidth = 120.0f;
        juce::Rectangle<int> area = lowerArea.removeFromLeft(selectorWidth).removeFromTop(height);
        freqTypeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        freqTypeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        freqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        hzSlider.setBounds(area.reduced(LOCAL_MARGIN));
        octaveSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        qLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        qSlider.setBounds(area.reduced(LOCAL_MARGIN));
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
    else if(slider == &octaveSlider)
    {
        *_paramsPtr->Octave = (float)octaveSlider.getValue();
    }
    else if(slider == &qSlider)
    {
        *_paramsPtr->Q = (float)qSlider.getValue();
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
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    qSlider.setValue(_paramsPtr->Q->get(), juce::dontSendNotification);
    
    auto freqType = static_cast<FILTER_FREQ_TYPE>(_paramsPtr->FreqType->getIndex());
    hzSlider.setVisible(freqType == FILTER_FREQ_TYPE::Absolute);
    octaveSlider.setVisible(freqType == FILTER_FREQ_TYPE::Relative);
}

//==============================================================================
LfoComponent::LfoComponent(int index, LfoParams* params)
: index(index)
, _paramsPtr(params)
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
                           _paramsPtr->SlowFreq->range.end, 0.01);
    slowFreqSlider.setSkewFactorFromMidPoint(2.0f);
    slowFreqSlider.setValue(_paramsPtr->SlowFreq->get(), juce::dontSendNotification);
    slowFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    slowFreqSlider.setPopupMenuEnabled(true);
    slowFreqSlider.setTextValueSuffix(" Hz");
    slowFreqSlider.addListener(this);
    body.addAndMakeVisible(slowFreqSlider);
    
    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    fastFreqSlider.setRange(_paramsPtr->FastFreq->range.start,
                           _paramsPtr->FastFreq->range.end, 0.01);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0f);
    fastFreqSlider.setValue(_paramsPtr->FastFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0);
    fastFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    fastFreqSlider.setPopupMenuEnabled(true);
    fastFreqSlider.setTextValueSuffix(" x");
    fastFreqSlider.addListener(this);
    body.addAndMakeVisible(fastFreqSlider);
    
    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setRange(_paramsPtr->Amount->range.start,
                           _paramsPtr->Amount->range.end, 0.01);
    amountSlider.setValue(_paramsPtr->Amount->get(), juce::dontSendNotification);
    amountSlider.setPopupDisplayEnabled(true, true, nullptr);
    amountSlider.setPopupMenuEnabled(true);
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
    int width = 60;
    int height = 60;

    juce::Rectangle<int> bounds = getLocalBounds();
    
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(280).removeFromTop(height);
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
        juce::Rectangle<int> area = lowerArea.removeFromLeft(120).removeFromTop(height);
        waveformLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        waveformSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        freqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        fastFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
        slowFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
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
    peakFreqSlider.setPopupMenuEnabled(true);
    peakFreqSlider.setTextValueSuffix(" oct");
    peakFreqSlider.addListener(this);
    body.addAndMakeVisible(peakFreqSlider);
    
    waitSlider.setLookAndFeel(&grapeLookAndFeel);
    waitSlider.setRange(_paramsPtr->Wait->range.start,
                           _paramsPtr->Wait->range.end, 0.01);
    waitSlider.setSkewFactorFromMidPoint(0.2f);
    waitSlider.setValue(_paramsPtr->Wait->get(), juce::dontSendNotification);
    waitSlider.setPopupDisplayEnabled(true, true, nullptr);
    waitSlider.setPopupMenuEnabled(true);
    waitSlider.setTextValueSuffix(" sec");
    waitSlider.addListener(this);
    body.addAndMakeVisible(waitSlider);
    
    attackSlider.setLookAndFeel(&grapeLookAndFeel);
    attackSlider.setRange(_paramsPtr->Attack->range.start,
                           _paramsPtr->Attack->range.end, 0.01);
    attackSlider.setSkewFactorFromMidPoint(0.2f);
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, nullptr);
    attackSlider.setPopupMenuEnabled(true);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    body.addAndMakeVisible(attackSlider);
    
    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(_paramsPtr->Decay->range.start,
                           _paramsPtr->Decay->range.end, 0.01);
    decaySlider.setSkewFactorFromMidPoint(0.4f);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, nullptr);
    decaySlider.setPopupMenuEnabled(true);
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
    int width = 60;
    int height = 60;

    juce::Rectangle<int> bounds = getLocalBounds();

    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(280).removeFromTop(height);
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
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        auto lebelArea = area.removeFromTop(LABEL_HEIGHT);
        peakFreqLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        fadeLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        peakFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
        fadeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        auto lebelArea = area.removeFromTop(LABEL_HEIGHT);
        waitLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        attackLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        waitSlider.setBounds(area.reduced(LOCAL_MARGIN));
        attackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
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
DelayComponent::DelayComponent(DelayParams* params)
: _paramsPtr(params)
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
    timeLSlider.setPopupMenuEnabled(true);
    timeLSlider.setTextValueSuffix(" sec");
    timeLSlider.addListener(this);
    body.addAndMakeVisible(timeLSlider);
    
    timeRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeRSlider.setRange(_paramsPtr->TimeR->range.start,
                          _paramsPtr->TimeR->range.end, 0.01);
    timeRSlider.setSkewFactorFromMidPoint(0.4f);
    timeRSlider.setValue(_paramsPtr->TimeR->get(), juce::dontSendNotification);
    timeRSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeRSlider.setPopupMenuEnabled(true);
    timeRSlider.setTextValueSuffix(" sec");
    timeRSlider.addListener(this);
    body.addAndMakeVisible(timeRSlider);
    
    timeSyncLSlider.setLookAndFeel(&grapeLookAndFeel);
    timeSyncLSlider.setRange(0, DELAY_TIME_SYNC_NAMES.size() - 1, 1);
    timeSyncLSlider.setValue(_paramsPtr->TimeSyncL->getIndex(), juce::dontSendNotification);
    timeSyncLSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeSyncLSlider.setPopupMenuEnabled(true);
    timeSyncLSlider.textFromValueFunction = [](double index){ return DELAY_TIME_SYNC_NAMES[index]; };
    timeSyncLSlider.addListener(this);
    body.addAndMakeVisible(timeSyncLSlider);
    
    timeSyncRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeSyncRSlider.setRange(0, DELAY_TIME_SYNC_NAMES.size() - 1, 1);
    timeSyncRSlider.setValue(_paramsPtr->TimeSyncR->getIndex(), juce::dontSendNotification);
    timeSyncRSlider.setPopupDisplayEnabled(true, true, nullptr);
    timeSyncRSlider.setPopupMenuEnabled(true);
    timeSyncRSlider.textFromValueFunction = [](double index){ return DELAY_TIME_SYNC_NAMES[index]; };
    timeSyncRSlider.addListener(this);
    body.addAndMakeVisible(timeSyncRSlider);
    
    lowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    lowFreqSlider.setRange(_paramsPtr->LowFreq->range.start,
                          _paramsPtr->LowFreq->range.end, 1.0);
    lowFreqSlider.setSkewFactorFromMidPoint(2000.0f);
    lowFreqSlider.setValue(_paramsPtr->LowFreq->get(), juce::dontSendNotification);
    lowFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    lowFreqSlider.setPopupMenuEnabled(true);
    lowFreqSlider.setTextValueSuffix(" Hz");
    lowFreqSlider.addListener(this);
    body.addAndMakeVisible(lowFreqSlider);
    
    highFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    highFreqSlider.setRange(_paramsPtr->HighFreq->range.start,
                          _paramsPtr->HighFreq->range.end, 1.0);
    highFreqSlider.setSkewFactorFromMidPoint(2000.0f);
    highFreqSlider.setValue(_paramsPtr->HighFreq->get(), juce::dontSendNotification);
    highFreqSlider.setPopupDisplayEnabled(true, true, nullptr);
    highFreqSlider.setPopupMenuEnabled(true);
    highFreqSlider.setTextValueSuffix(" Hz");
    highFreqSlider.addListener(this);
    body.addAndMakeVisible(highFreqSlider);
    
    feedbackSlider.setLookAndFeel(&grapeLookAndFeel);
    feedbackSlider.setRange(_paramsPtr->Feedback->range.start,
                          _paramsPtr->Feedback->range.end, 0.01);
    feedbackSlider.setSkewFactorFromMidPoint(0.4f);
    feedbackSlider.setValue(_paramsPtr->Feedback->get(), juce::dontSendNotification);
    feedbackSlider.setPopupDisplayEnabled(true, true, nullptr);
    feedbackSlider.setPopupMenuEnabled(true);
    feedbackSlider.setTextValueSuffix(" sec");
    feedbackSlider.addListener(this);
    body.addAndMakeVisible(feedbackSlider);
    
    mixSlider.setLookAndFeel(&grapeLookAndFeel);
    mixSlider.setRange(_paramsPtr->Mix->range.start,
                          _paramsPtr->Mix->range.end, 0.01);
    mixSlider.setValue(_paramsPtr->Mix->get(), juce::dontSendNotification);
    mixSlider.setPopupDisplayEnabled(true, true, nullptr);
    mixSlider.setPopupMenuEnabled(true);
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
    int width = 60;
    int height = 60;
    
    juce::Rectangle<int> bounds = getLocalBounds();
    
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    
    body.setBounds(bounds);
    bounds = body.getLocalBounds();
    auto bodyHeight = bounds.getHeight();
    auto upperArea = bounds.removeFromTop(bodyHeight / 2);
    auto lowerArea = bounds;
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(120).removeFromTop(height);
        typeLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        typeSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(80).removeFromTop(height);
        syncLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        syncSelector.setBounds(area.reduced(LOCAL_MARGIN).removeFromTop(COMBO_BOX_HEIGHT));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        lowFreqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        lowFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        highFreqLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        highFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        timeLLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        auto sliderBounds = area.reduced(LOCAL_MARGIN);
        timeLSlider.setBounds(sliderBounds);
        timeSyncLSlider.setBounds(sliderBounds);
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        timeRLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        auto sliderBounds = area.reduced(LOCAL_MARGIN);
        timeRSlider.setBounds(sliderBounds);
        timeSyncRSlider.setBounds(sliderBounds);
    }

    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        feedbackLabel.setBounds(area.removeFromTop(LABEL_HEIGHT).reduced(LOCAL_MARGIN));
        feedbackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
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
AnalyserComponent::AnalyserComponent(AnalyserState* analyserState)
: analyserState(analyserState)
, forwardFFT (analyserState->fftOrder)
, window (analyserState->fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    startTimerHz(30.0f);
}
AnalyserComponent::~AnalyserComponent(){}
void AnalyserComponent::drawNextFrameOfSpectrum()
{
    window.multiplyWithWindowingTable(analyserState->fftData, analyserState->fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform (analyserState->fftData);

    auto mindB = -100.0f;
    auto maxdB =    0.0f;

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit (0, analyserState->fftSize / 2, (int) (skewedProportionX * (float) analyserState->fftSize * 0.5f));
        auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (analyserState->fftData[fftDataIndex])
                                                           - juce::Decibels::gainToDecibels ((float) analyserState->fftSize)),
                                 mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;
    }
}

void AnalyserComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setOpacity(1.0f);
    g.setColour(juce::Colour(100,190,140));
    if(readyToDrawFrame) {
        drawFrame(g);
    }
}

void AnalyserComponent::resized()
{
    // TODO: ?
}
void AnalyserComponent::timerCallback()
{
    if (analyserState->nextFFTBlockReady)
    {
        drawNextFrameOfSpectrum();
        analyserState->nextFFTBlockReady = false;
        readyToDrawFrame = true;
        repaint();
    }
}
void AnalyserComponent::drawFrame(juce::Graphics& g)
{
    auto width  = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();
    for (int i = 1; i < scopeSize; ++i)
    {
        g.drawLine ({ (float) juce::jmap (i - 1, 0, scopeSize - 1, 0, width),
                              juce::jmap (scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                      (float) juce::jmap (i,     0, scopeSize - 1, 0, width),
                              juce::jmap (scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }
}
