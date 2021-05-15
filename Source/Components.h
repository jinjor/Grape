#pragma once

#include <JuceHeader.h>
#include "Params.h"
#include "LookAndFeel.h"
#include "PluginProcessor.h"

//==============================================================================
class HeaderComponent : public juce::Component
{
public:
    HeaderComponent(std::string name, bool hasEnableButton);
    virtual ~HeaderComponent();
    juce::ToggleButton enabledButton;
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    std::string name;
    bool hasEnableButton;
};

//==============================================================================
class VoiceComponent : public juce::Component, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    VoiceComponent(VoiceParams* params);
    virtual ~VoiceComponent();
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    GrapeLookAndFeel grapeLookAndFeel;
    VoiceParams* _paramsPtr;
   
    HeaderComponent header;
    
    juce::ComboBox modeSelector;
    juce::Slider portamentoTimeSlider;
    juce::Slider pitchBendRangeSlider;

    juce::Label modeLabel;
    juce::Label portamentoTimeLabel;
    juce::Label pitchBendRangeLabel;
};

//==============================================================================
class StatusComponent : public juce::Component, private juce::Timer
{
public:
    StatusComponent(int* polyphony, TimeConsumptionState* timeConsumptionState);
    virtual ~StatusComponent();
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    virtual void timerCallback() override;
    GrapeLookAndFeel grapeLookAndFeel;
    int* polyphony;
    TimeConsumptionState* timeConsumptionState;
   
    HeaderComponent header;
    
    juce::Label polyphonyValueLabel;
    juce::Label timeConsumptionValueLabel;

    juce::Label polyphonyLabel;
    juce::Label timeConsumptionLabel;
};


//==============================================================================
class OscComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    OscComponent(int index, OscParams* params);
    virtual ~OscComponent();
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    OscParams* _paramsPtr;
   
    HeaderComponent header;
    juce::Component body;
    
    juce::ComboBox envelopeSelector;
    juce::ComboBox waveformSelector;
    juce::Slider edgeSlider;
    juce::Slider octaveSlider;
    juce::Slider coarseSlider;
    juce::Slider unisonSlider;
    juce::Slider detuneSlider;
    juce::Slider spreadSlider;
    juce::Slider gainSlider;

    juce::Label envelopeLabel;
    juce::Label waveformLabel;
    juce::Label edgeLabel;
    juce::Label octaveLabel;
    juce::Label coarseLabel;
    juce::Label unisonLabel;
    juce::Label detuneLabel;
    juce::Label spreadLabel;
    juce::Label gainLabel;
};

//==============================================================================
class EnvelopeComponent : public juce::Component, juce::Slider::Listener, private juce::Timer
{
public:
    EnvelopeComponent(int index, EnvelopeParams* params);
    virtual ~EnvelopeComponent();
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    EnvelopeParams* _paramsPtr;
    
    HeaderComponent header;
    
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
};

//==============================================================================
class FilterComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    FilterComponent(int index, FilterParams* params);
    virtual ~FilterComponent();
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    FilterParams* _paramsPtr;
    
    HeaderComponent header;
    
    juce::Component body;
    
    juce::ComboBox targetSelector;
    juce::ComboBox typeSelector;
    juce::ComboBox freqTypeSelector;
    juce::Slider hzSlider;
    juce::Slider octaveSlider;
    juce::Slider qSlider;
    
    juce::Label targetLabel;
    juce::Label typeLabel;
    juce::Label freqTypeLabel;
    juce::Label freqLabel;
    juce::Label qLabel;
};

//==============================================================================
class LfoComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    LfoComponent(int index, LfoParams* params);
    virtual ~LfoComponent();
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    LfoParams* _paramsPtr;
    
    HeaderComponent header;
    
    juce::Component body;
    
    juce::ComboBox targetTypeSelector;
    juce::ComboBox targetOscSelector;
    juce::ComboBox targetFilterSelector;
    juce::ComboBox targetOscParamSelector;
    juce::ComboBox targetFilterParamSelector;
    juce::ComboBox waveformSelector;
    juce::Slider slowFreqSlider;
    juce::Slider fastFreqSlider;
    juce::Slider amountSlider;
    
    juce::Label targetLabel;
    juce::Label typeLabel;
    juce::Label waveformLabel;
    juce::Label freqLabel;
    juce::Label amountLabel;
};

//==============================================================================
class ModEnvComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    ModEnvComponent(int index, ModEnvParams* params);
    virtual ~ModEnvComponent();
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    ModEnvParams* _paramsPtr;
    
    HeaderComponent header;
    
    juce::Component body;
    
    juce::ComboBox targetTypeSelector;
    juce::ComboBox targetOscSelector;
    juce::ComboBox targetFilterSelector;
    juce::ComboBox targetLfoSelector;
    juce::ComboBox targetOscParamSelector;
    juce::ComboBox targetFilterParamSelector;
    juce::ComboBox targetLfoParamSelector;
    juce::ComboBox fadeSelector;
    juce::Slider peakFreqSlider;
    juce::Slider waitSlider;
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    
    juce::Label targetLabel;
    juce::Label typeLabel;
    juce::Label fadeLabel;
    juce::Label peakFreqLabel;
    juce::Label waitLabel;
    juce::Label attackLabel;
    juce::Label decayLabel;
};

//==============================================================================
class DelayComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    DelayComponent(DelayParams* params);
    virtual ~DelayComponent();
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    GrapeLookAndFeel grapeLookAndFeel;
    DelayParams* _paramsPtr;
   
    HeaderComponent header;
    
    juce::Component body;
    
    juce::ComboBox typeSelector;
    juce::ComboBox syncSelector;
    juce::Slider timeLSlider;
    juce::Slider timeRSlider;
    juce::Slider timeSyncLSlider;
    juce::Slider timeSyncRSlider;
    juce::Slider lowFreqSlider;
    juce::Slider highFreqSlider;
    juce::Slider feedbackSlider;
    juce::Slider mixSlider;

    juce::Label typeLabel;
    juce::Label syncLabel;
    juce::Label timeLLabel;
    juce::Label timeRLabel;
    juce::Label lowFreqLabel;
    juce::Label highFreqLabel;
    juce::Label feedbackLabel;
    juce::Label mixLabel;
};

//==============================================================================
class AnalyserComponent : public juce::Component, private juce::Timer
{
public:
    AnalyserComponent(AnalyserState* analyserState);
    virtual ~AnalyserComponent();
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    enum {
        scopeSize = 512
    };
    AnalyserState* analyserState;
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    float scopeData[scopeSize];
    bool readyToDrawFrame = false;

    virtual void timerCallback() override;
    void drawNextFrameOfSpectrum();
    void drawFrame(juce::Graphics& g);
};

//==============================================================================
class ControlItemComponent : public juce::Component, private juce::ComboBox::Listener, juce::Timer
{
public:
    ControlItemComponent(ControlItemParams* params);
    virtual ~ControlItemComponent();

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
private:
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void timerCallback() override;

    GrapeLookAndFeel grapeLookAndFeel;
    ControlItemParams* _paramsPtr;

    juce::ComboBox numberSelector;
    juce::ComboBox targetTypeSelector;
    juce::ComboBox targetOscSelector;
    juce::ComboBox targetFilterSelector;
    juce::ComboBox targetLfoSelector;
    
    juce::ComboBox targetModEnvSelector;
    juce::ComboBox targetOscParamSelector;
    juce::ComboBox targetFilterParamSelector;
    juce::ComboBox targetLfoParamSelector;
    juce::ComboBox targetModEnvParamSelector;
    juce::ComboBox targetMiscParamSelector;
};

//==============================================================================
class ControlComponent : public juce::Component
{
public:
    ControlComponent(ControlItemParams* params);
    virtual ~ControlComponent();

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    
    GrapeLookAndFeel grapeLookAndFeel;
    ModEnvParams* _paramsPtr;

    HeaderComponent header;
    juce::Label numberLabel;
    juce::Label targetLabel;
    ControlItemComponent controlItemComponents[NUM_CONTROL];
};
