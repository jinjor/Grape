#include <JuceHeader.h>
#include "Components.h"
#include "Params.h"

namespace {
const float PANEL_NAME_FONT_SIZE = 15.0f;
const float PARAM_LABEL_FONT_SIZE = 14.0f;
const int PANEL_NAME_HEIGHT = 26;
const int LOCAL_MARGIN = 2;
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
OscComponent::OscComponent(int index, OscParams* params)
: index(index)
, _paramsPtr(params)
//, enabledButton("Enabled")
, header("OSC " + std::to_string(index+1), true)
, envelopeSelector("Envelope")
, waveformSelector("Waveform")
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
    
    octaveSlider.setLookAndFeel(&grapeLookAndFeel);
    octaveSlider.setRange(_paramsPtr->Octave->getRange().getStart(),
                          _paramsPtr->Octave->getRange().getEnd(), 1);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    octaveSlider.setPopupDisplayEnabled(true, true, this);
    octaveSlider.setPopupMenuEnabled(true);
    octaveSlider.addListener(this);
    body.addAndMakeVisible(octaveSlider);
    
    coarseSlider.setLookAndFeel(&grapeLookAndFeel);
    coarseSlider.setRange(_paramsPtr->Coarse->getRange().getStart(),
                         _paramsPtr->Coarse->getRange().getEnd(), 1);
    coarseSlider.setValue(_paramsPtr->Coarse->get(), juce::dontSendNotification);
    coarseSlider.setPopupDisplayEnabled(true, true, this);
    coarseSlider.setPopupMenuEnabled(true);
    coarseSlider.addListener(this);
    body.addAndMakeVisible(coarseSlider);
    
    unisonSlider.setLookAndFeel(&grapeLookAndFeel);
    unisonSlider.setRange(_paramsPtr->Unison->getRange().getStart(),
                         _paramsPtr->Unison->getRange().getEnd(), 1);
    unisonSlider.setValue(_paramsPtr->Unison->get(), juce::dontSendNotification);
    unisonSlider.setPopupDisplayEnabled(true, true, this);
    unisonSlider.setPopupMenuEnabled(true);
    unisonSlider.addListener(this);
    body.addAndMakeVisible(unisonSlider);
    
    detuneSlider.setLookAndFeel(&grapeLookAndFeel);
    detuneSlider.setRange(_paramsPtr->Detune->range.start,
                         _paramsPtr->Detune->range.end, 0.01);
    detuneSlider.setValue(_paramsPtr->Detune->get(), juce::dontSendNotification);
    detuneSlider.setPopupDisplayEnabled(true, true, this);
    detuneSlider.setPopupMenuEnabled(true);
    detuneSlider.addListener(this);
    body.addAndMakeVisible(detuneSlider);
    
    spreadSlider.setLookAndFeel(&grapeLookAndFeel);
    spreadSlider.setRange(_paramsPtr->Spread->range.start,
                         _paramsPtr->Spread->range.end, 0.01);
    spreadSlider.setValue(_paramsPtr->Spread->get(), juce::dontSendNotification);
    spreadSlider.setPopupDisplayEnabled(true, true, this);
    spreadSlider.setPopupMenuEnabled(true);
    spreadSlider.addListener(this);
    body.addAndMakeVisible(spreadSlider);
    
    gainSlider.setLookAndFeel(&grapeLookAndFeel);
    gainSlider.setRange(_paramsPtr->Gain->range.start,
                         _paramsPtr->Gain->range.end, 0.01);
    gainSlider.setValue(_paramsPtr->Gain->get(), juce::dontSendNotification);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    gainSlider.setPopupMenuEnabled(true);
    gainSlider.addListener(this);
    body.addAndMakeVisible(gainSlider);
    
    envelopeLabel.setFont(paramLabelFont);
    envelopeLabel.setText("Envelope", juce::dontSendNotification);
    envelopeLabel.setJustificationType(juce::Justification::centred);
    envelopeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(envelopeLabel);
    
    waveformLabel.setFont(paramLabelFont);
    waveformLabel.setText("Waveform", juce::dontSendNotification);
    waveformLabel.setJustificationType(juce::Justification::centred);
    waveformLabel.setEditable(false, false, false);
    body.addAndMakeVisible(waveformLabel);
    
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
    juce::Rectangle<int> bounds = getLocalBounds();
    juce::Rectangle<int> nameArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    g.setColour(PANEL_NAME_COLOUR);
    g.fillRect(nameArea);
    
    juce::GlyphArrangement ga;
    ga.addLineOfText(juce::Font(PANEL_NAME_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Bold"),
                     "OSC " + std::to_string(index+1), 0, 0);
    juce::Path p;
    ga.createPath(p);
    auto pathBounds = p.getBounds();
    p.applyTransform(juce::AffineTransform()
                         .rotated(-juce::MathConstants<float>::halfPi, 0, 0)
                         .translated(pathBounds.getHeight()/2 + nameArea.getWidth()/2, pathBounds.getWidth() + PANEL_NAME_HEIGHT + 4.0)
                         );
    g.setColour(juce::Colours::white);
    g.fillPath(p);
    g.setColour(PANEL_NAME_COLOUR);
}

void OscComponent::resized()
{
    int labelHeight = 20;
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
        juce::Rectangle<int> area = upperArea.removeFromLeft(60).removeFromTop(height);
        envelopeLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        envelopeSelector.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(120).removeFromTop(height);
        waveformLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        waveformSelector.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        octaveLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        octaveSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        coarseLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        coarseSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        unisonLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        unisonSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        detuneLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        detuneSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        spreadLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        spreadSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        gainLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        gainSlider.setBounds(area.reduced(LOCAL_MARGIN));
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
    if(slider == &octaveSlider)
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
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    coarseSlider.setValue(_paramsPtr->Coarse->get(), juce::dontSendNotification);
    unisonSlider.setValue(_paramsPtr->Unison->get(), juce::dontSendNotification);
    detuneSlider.setValue(_paramsPtr->Detune->get(), juce::dontSendNotification);
    spreadSlider.setValue(_paramsPtr->Spread->get(), juce::dontSendNotification);
    gainSlider.setValue(_paramsPtr->Gain->get(), juce::dontSendNotification);
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
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, this);
    attackSlider.setPopupMenuEnabled(true);
    attackSlider.setTextValueSuffix(" seconds");
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    
    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(_paramsPtr->Decay->range.start,
                         _paramsPtr->Decay->range.end, 0.01);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, this);
    decaySlider.setPopupMenuEnabled(true);
    decaySlider.setTextValueSuffix(" seconds");
    decaySlider.addListener(this);
    addAndMakeVisible(decaySlider);
    
    sustainSlider.setLookAndFeel(&grapeLookAndFeel);
    sustainSlider.setRange(_paramsPtr->Sustain->range.start,
                           _paramsPtr->Sustain->range.end, 0.01);
    sustainSlider.setValue(_paramsPtr->Sustain->get(), juce::dontSendNotification);
    sustainSlider.setPopupDisplayEnabled(true, true, this);
    sustainSlider.setPopupMenuEnabled(true);
    sustainSlider.addListener(this);
    addAndMakeVisible(sustainSlider);
    
    releaseSlider.setLookAndFeel(&grapeLookAndFeel);
    releaseSlider.setRange(_paramsPtr->Release->range.start,
                           _paramsPtr->Release->range.end, 0.01);
    releaseSlider.setValue(_paramsPtr->Release->get(), juce::dontSendNotification);
    releaseSlider.setPopupDisplayEnabled(true, true, this);
    releaseSlider.setPopupMenuEnabled(true);
    releaseSlider.setTextValueSuffix(" seconds");
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
    int labelHeight = 20;
    int width = 60;
    int height = 60;
    
    juce::Rectangle<int> bounds = getLocalBounds();
    auto headerArea = bounds.removeFromLeft(PANEL_NAME_HEIGHT);
    header.setBounds(headerArea);
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(width).removeFromTop(height);
        attackLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        attackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(width).removeFromTop(height);
        decayLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        decaySlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(width).removeFromTop(height);
        sustainLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        sustainSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = bounds.removeFromLeft(width).removeFromTop(height);
        releaseLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
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
    
    octaveSlider.setLookAndFeel(&grapeLookAndFeel);
    octaveSlider.setRange(_paramsPtr->Octave->range.start,
                         _paramsPtr->Octave->range.end, 1.0/12);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    octaveSlider.setPopupDisplayEnabled(true, true, this);
    octaveSlider.setPopupMenuEnabled(true);
    octaveSlider.setTextValueSuffix(" octave");
    octaveSlider.addListener(this);
    body.addAndMakeVisible(octaveSlider);
    
    qSlider.setLookAndFeel(&grapeLookAndFeel);
    qSlider.setRange(_paramsPtr->Q->range.start,
                           _paramsPtr->Q->range.end, 0.01);
    qSlider.setValue(_paramsPtr->Q->get(), juce::dontSendNotification);
    qSlider.setPopupDisplayEnabled(true, true, this);
    qSlider.setPopupMenuEnabled(true);
    qSlider.addListener(this);
    body.addAndMakeVisible(qSlider);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("Target", juce::dontSendNotification);
    targetLabel.setJustificationType(juce::Justification::centred);
    targetLabel.setEditable(false, false, false);
    body.addAndMakeVisible(targetLabel);
    
    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);
    
    octaveLabel.setFont(paramLabelFont);
    octaveLabel.setText("Freq", juce::dontSendNotification);
    octaveLabel.setJustificationType(juce::Justification::centred);
    octaveLabel.setEditable(false, false, false);
    body.addAndMakeVisible(octaveLabel);
    
    qLabel.setFont(paramLabelFont);
    qLabel.setText("Q", juce::dontSendNotification);
    qLabel.setJustificationType(juce::Justification::centred);
    qLabel.setEditable(false, false, false);
    body.addAndMakeVisible(qLabel);
    
    body.setEnabled(_paramsPtr->Enabled->get());
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
    int labelHeight = 20;
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
        juce::Rectangle<int> area = upperArea.removeFromLeft(90).removeFromTop(height);
        targetLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        targetSelector.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(120).removeFromTop(height);
        typeLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        typeSelector.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        octaveLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        octaveSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        qLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
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
}
void FilterComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &octaveSlider)
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
    targetSelector.setSelectedItemIndex(_paramsPtr->Target->getIndex(), juce::dontSendNotification);
    typeSelector.setSelectedItemIndex(_paramsPtr->Type->getIndex(), juce::dontSendNotification);
    octaveSlider.setValue(_paramsPtr->Octave->get(), juce::dontSendNotification);
    qSlider.setValue(_paramsPtr->Q->get(), juce::dontSendNotification);
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
    
    slowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    slowFreqSlider.setRange(_paramsPtr->SlowFreq->range.start,
                           _paramsPtr->SlowFreq->range.end, 0.01);
    slowFreqSlider.setValue(_paramsPtr->SlowFreq->get(), juce::dontSendNotification);
    slowFreqSlider.setPopupDisplayEnabled(true, true, this);
    slowFreqSlider.setPopupMenuEnabled(true);
    slowFreqSlider.setTextValueSuffix("Hz");
    slowFreqSlider.addListener(this);
    body.addAndMakeVisible(slowFreqSlider);
    
    fastFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    fastFreqSlider.setRange(_paramsPtr->FastFreq->range.start,
                           _paramsPtr->FastFreq->range.end, 0.01);
    fastFreqSlider.setValue(_paramsPtr->FastFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setSkewFactorFromMidPoint(1.0);
    fastFreqSlider.setPopupDisplayEnabled(true, true, this);
    fastFreqSlider.setPopupMenuEnabled(true);
    fastFreqSlider.setTextValueSuffix("x");
    fastFreqSlider.addListener(this);
    body.addAndMakeVisible(fastFreqSlider);
    
    amountSlider.setLookAndFeel(&grapeLookAndFeel);
    amountSlider.setRange(_paramsPtr->Amount->range.start,
                           _paramsPtr->Amount->range.end, 0.01);
    amountSlider.setValue(_paramsPtr->Amount->get(), juce::dontSendNotification);
    amountSlider.setPopupDisplayEnabled(true, true, this);
    amountSlider.setPopupMenuEnabled(true);
    amountSlider.addListener(this);
    body.addAndMakeVisible(amountSlider);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("Target", juce::dontSendNotification);
    targetLabel.setJustificationType(juce::Justification::centred);
    targetLabel.setEditable(false, false, false);
    body.addAndMakeVisible(targetLabel);
    
    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);
    
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
    
    body.setEnabled(_paramsPtr->Enabled->get());
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
    int labelHeight = 20;
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
        targetLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        targetTypeSelector.setBounds(area.removeFromLeft(90).reduced(LOCAL_MARGIN));
        auto indexArea = area.removeFromLeft(80);
        targetOscSelector.setBounds(indexArea.reduced(LOCAL_MARGIN));
        targetFilterSelector.setBounds(indexArea.reduced(LOCAL_MARGIN));
        auto paramArea = area.removeFromLeft(110);
        targetOscParamSelector.setBounds(paramArea.reduced(LOCAL_MARGIN));
        targetFilterParamSelector.setBounds(paramArea.reduced(LOCAL_MARGIN));
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
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        freqLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        fastFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
        slowFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
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
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        amountLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
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
    
    slowFreqSlider.setValue(_paramsPtr->SlowFreq->get(), juce::dontSendNotification);
    fastFreqSlider.setValue(_paramsPtr->FastFreq->get(), juce::dontSendNotification);
    amountSlider.setValue(_paramsPtr->Amount->get(), juce::dontSendNotification);
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
    peakFreqSlider.setPopupDisplayEnabled(true, true, this);
    peakFreqSlider.setPopupMenuEnabled(true);
    peakFreqSlider.setTextValueSuffix("x");
    peakFreqSlider.addListener(this);
    body.addAndMakeVisible(peakFreqSlider);
    
    waitSlider.setLookAndFeel(&grapeLookAndFeel);
    waitSlider.setRange(_paramsPtr->Wait->range.start,
                           _paramsPtr->Wait->range.end, 0.01);
    waitSlider.setValue(_paramsPtr->Wait->get(), juce::dontSendNotification);
    waitSlider.setPopupDisplayEnabled(true, true, this);
    waitSlider.setPopupMenuEnabled(true);
    waitSlider.addListener(this);
    body.addAndMakeVisible(waitSlider);
    
    attackSlider.setLookAndFeel(&grapeLookAndFeel);
    attackSlider.setRange(_paramsPtr->Attack->range.start,
                           _paramsPtr->Attack->range.end, 0.01);
    attackSlider.setValue(_paramsPtr->Attack->get(), juce::dontSendNotification);
    attackSlider.setPopupDisplayEnabled(true, true, this);
    attackSlider.setPopupMenuEnabled(true);
    attackSlider.addListener(this);
    body.addAndMakeVisible(attackSlider);
    
    decaySlider.setLookAndFeel(&grapeLookAndFeel);
    decaySlider.setRange(_paramsPtr->Decay->range.start,
                           _paramsPtr->Decay->range.end, 0.01);
    decaySlider.setValue(_paramsPtr->Decay->get(), juce::dontSendNotification);
    decaySlider.setPopupDisplayEnabled(true, true, this);
    decaySlider.setPopupMenuEnabled(true);
    decaySlider.addListener(this);
    body.addAndMakeVisible(decaySlider);

    targetLabel.setFont(paramLabelFont);
    targetLabel.setText("Target", juce::dontSendNotification);
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
    
    body.setEnabled(_paramsPtr->Enabled->get());
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
    int labelHeight = 20;
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
        targetLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        targetTypeSelector.setBounds(area.removeFromLeft(90).reduced(LOCAL_MARGIN));
        auto indexArea = area.removeFromLeft(80);
        targetOscSelector.setBounds(indexArea.reduced(LOCAL_MARGIN));
        targetFilterSelector.setBounds(indexArea.reduced(LOCAL_MARGIN));
        targetLfoSelector.setBounds(indexArea.reduced(LOCAL_MARGIN));
        auto paramArea = area.removeFromLeft(110);
        targetOscParamSelector.setBounds(paramArea.reduced(LOCAL_MARGIN));
        targetFilterParamSelector.setBounds(paramArea.reduced(LOCAL_MARGIN));
        targetLfoParamSelector.setBounds(paramArea.reduced(LOCAL_MARGIN));
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
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        auto lebelArea = area.removeFromTop(labelHeight);
        peakFreqLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        fadeLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        peakFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
        fadeSelector.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        auto lebelArea = area.removeFromTop(labelHeight);
        waitLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        attackLabel.setBounds(lebelArea.reduced(LOCAL_MARGIN));
        waitSlider.setBounds(area.reduced(LOCAL_MARGIN));
        attackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
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
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        decayLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
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
}

//==============================================================================
DelayComponent::DelayComponent(DelayParams* params)
: _paramsPtr(params)
, header("DELAY", true)
, typeSelector("Type")
, timeLSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
, timeRSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
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
    
    timeLSlider.setLookAndFeel(&grapeLookAndFeel);
    timeLSlider.setRange(_paramsPtr->TimeL->range.start,
                          _paramsPtr->TimeL->range.end, 0.01);
    timeLSlider.setValue(_paramsPtr->TimeL->get(), juce::dontSendNotification);
    timeLSlider.setPopupDisplayEnabled(true, true, this);
    timeLSlider.setPopupMenuEnabled(true);
    timeLSlider.addListener(this);
    body.addAndMakeVisible(timeLSlider);
    
    timeRSlider.setLookAndFeel(&grapeLookAndFeel);
    timeRSlider.setRange(_paramsPtr->TimeR->range.start,
                          _paramsPtr->TimeR->range.end, 0.01);
    timeRSlider.setValue(_paramsPtr->TimeR->get(), juce::dontSendNotification);
    timeRSlider.setPopupDisplayEnabled(true, true, this);
    timeRSlider.setPopupMenuEnabled(true);
    timeRSlider.addListener(this);
    body.addAndMakeVisible(timeRSlider);
    
    
    lowFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    lowFreqSlider.setRange(_paramsPtr->LowFreq->range.start,
                          _paramsPtr->LowFreq->range.end, 1.0);
    lowFreqSlider.setValue(_paramsPtr->LowFreq->get(), juce::dontSendNotification);
    lowFreqSlider.setPopupDisplayEnabled(true, true, this);
    lowFreqSlider.setPopupMenuEnabled(true);
    lowFreqSlider.addListener(this);
    body.addAndMakeVisible(lowFreqSlider);
    
    highFreqSlider.setLookAndFeel(&grapeLookAndFeel);
    highFreqSlider.setRange(_paramsPtr->HighFreq->range.start,
                          _paramsPtr->HighFreq->range.end, 1.0);
    highFreqSlider.setValue(_paramsPtr->HighFreq->get(), juce::dontSendNotification);
    highFreqSlider.setPopupDisplayEnabled(true, true, this);
    highFreqSlider.setPopupMenuEnabled(true);
    highFreqSlider.addListener(this);
    body.addAndMakeVisible(highFreqSlider);
    
    feedbackSlider.setLookAndFeel(&grapeLookAndFeel);
    feedbackSlider.setRange(_paramsPtr->Feedback->range.start,
                          _paramsPtr->Feedback->range.end, 0.01);
    feedbackSlider.setValue(_paramsPtr->Feedback->get(), juce::dontSendNotification);
    feedbackSlider.setPopupDisplayEnabled(true, true, this);
    feedbackSlider.setPopupMenuEnabled(true);
    feedbackSlider.addListener(this);
    body.addAndMakeVisible(feedbackSlider);
    
    mixSlider.setLookAndFeel(&grapeLookAndFeel);
    mixSlider.setRange(_paramsPtr->Mix->range.start,
                          _paramsPtr->Mix->range.end, 0.01);
    mixSlider.setValue(_paramsPtr->Mix->get(), juce::dontSendNotification);
    mixSlider.setPopupDisplayEnabled(true, true, this);
    mixSlider.setPopupMenuEnabled(true);
    mixSlider.addListener(this);
    body.addAndMakeVisible(mixSlider);
    
    typeLabel.setFont(paramLabelFont);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.setEditable(false, false, false);
    body.addAndMakeVisible(typeLabel);
    
    timeLLabel.setFont(paramLabelFont);
    timeLLabel.setText("Time(L)", juce::dontSendNotification);
    timeLLabel.setJustificationType(juce::Justification::centred);
    timeLLabel.setEditable(false, false, false);
    body.addAndMakeVisible(timeLLabel);
    
    timeRLabel.setFont(paramLabelFont);
    timeRLabel.setText("Time(R)", juce::dontSendNotification);
    timeRLabel.setJustificationType(juce::Justification::centred);
    timeRLabel.setEditable(false, false, false);
    body.addAndMakeVisible(timeRLabel);
    
    lowFreqLabel.setFont(paramLabelFont);
    lowFreqLabel.setText("Low Freq", juce::dontSendNotification);
    lowFreqLabel.setJustificationType(juce::Justification::centred);
    lowFreqLabel.setEditable(false, false, false);
    body.addAndMakeVisible(lowFreqLabel);
    
    highFreqLabel.setFont(paramLabelFont);
    highFreqLabel.setText("high Freq", juce::dontSendNotification);
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
    
    body.setEnabled(_paramsPtr->Enabled->get());
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
    int labelHeight = 20;
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
        typeLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        typeSelector.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        lowFreqLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        lowFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = upperArea.removeFromLeft(width).removeFromTop(height);
        highFreqLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        highFreqSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        timeLLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        timeLSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        timeRLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        timeRSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }

    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        feedbackLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
        feedbackSlider.setBounds(area.reduced(LOCAL_MARGIN));
    }
    {
        juce::Rectangle<int> area = lowerArea.removeFromLeft(width).removeFromTop(height);
        mixLabel.setBounds(area.removeFromTop(labelHeight).reduced(LOCAL_MARGIN));
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
    timeLSlider.setValue(_paramsPtr->TimeL->get(), juce::dontSendNotification);
    timeRSlider.setValue(_paramsPtr->TimeR->get(), juce::dontSendNotification);
    lowFreqSlider.setValue(_paramsPtr->LowFreq->get(), juce::dontSendNotification);
    highFreqSlider.setValue(_paramsPtr->HighFreq->get(), juce::dontSendNotification);
    feedbackSlider.setValue(_paramsPtr->Feedback->get(), juce::dontSendNotification);
    mixSlider.setValue(_paramsPtr->Mix->get(), juce::dontSendNotification);
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
    g.fillAll (juce::Colours::black);
    g.setOpacity (1.0f);
    g.setColour (juce::Colours::white);
    if(readyToDrawFrame) {
        drawFrame (g);
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
