#pragma once

#include <JuceHeader.h>
#include "Params.h"
#include "LookAndFeel.h"
#include "PluginProcessor.h"

enum class ANALYSER_MODE { Spectrum, Envelope, Filter };

//==============================================================================
class HeaderComponent : public juce::Component
{
public:
    HeaderComponent(std::string name, bool hasEnableButton);
    virtual ~HeaderComponent();
    HeaderComponent(const HeaderComponent &) = delete;
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
    VoiceComponent(VoiceParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~VoiceComponent();
    VoiceComponent(const VoiceComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    
    VoiceParams& params;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;
   
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
    StatusComponent(int* polyphony, TimeConsumptionState* timeConsumptionState, LatestDataProvider* latestDataProvider);
    virtual ~StatusComponent();
    StatusComponent(const StatusComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    virtual void timerCallback() override;
    GrapeLookAndFeel grapeLookAndFeel;
    int* polyphony;
    TimeConsumptionState* timeConsumptionState;
    LatestDataProvider* latestDataProvider;
    
    juce::Label volumeValueLabel;
    juce::Label polyphonyValueLabel;
    juce::Label timeConsumptionValueLabel;

    juce::Label volumeLabel;
    juce::Label polyphonyLabel;
    juce::Label timeConsumptionLabel;
    
    float levelDataL[2048];
    float levelDataR[2048];
    LatestDataProvider::Consumer levelConsumer {
        levelDataL, levelDataR, 2048, false
    };
    float overflowedLevel = 0;
    int overflowWarning = 0;
};

//==============================================================================
class MasterComponent : public juce::Component, juce::Slider::Listener, private juce::Timer
{
public:
    MasterComponent(GlobalParams& params);
    virtual ~MasterComponent();
    MasterComponent(const MasterComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;

    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    
    GlobalParams& params;
   
    HeaderComponent header;
    
    juce::Slider panSlider;
    juce::Slider volumeSlider;

    juce::Label panLabel;
    juce::Label volumeLabel;
};

//==============================================================================
class OscComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    OscComponent(int index, OscParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~OscComponent();
    OscComponent(const OscComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    
    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    
    OscParams& params;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;
   
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
    EnvelopeComponent(int index, EnvelopeParams& params);
    virtual ~EnvelopeComponent();
    EnvelopeComponent(const EnvelopeComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    EnvelopeParams& params;
    
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
    FilterComponent(int index, FilterParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~FilterComponent();
    FilterComponent(const FilterComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    
    FilterParams& params;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;
    
    HeaderComponent header;
    
    juce::Component body;
    
    juce::ComboBox targetSelector;
    juce::ComboBox typeSelector;
    juce::ComboBox freqTypeSelector;
    juce::Slider hzSlider;
    juce::Slider semitoneSlider;
    juce::Slider qSlider;
    juce::Slider gainSlider;
    
    juce::Label targetLabel;
    juce::Label typeLabel;
    juce::Label freqTypeLabel;
    juce::Label freqLabel;
    juce::Label qLabel;
    juce::Label gainLabel;
};

//==============================================================================
class LfoComponent : public juce::Component, juce::ToggleButton::Listener, juce::ComboBox::Listener, juce::Slider::Listener, private juce::Timer
{
public:
    LfoComponent(int index, LfoParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~LfoComponent();
    LfoComponent(const LfoComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    
    LfoParams& params;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;
    
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
    ModEnvComponent(int index, ModEnvParams& params);
    virtual ~ModEnvComponent();
    ModEnvComponent(const ModEnvComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    GrapeLookAndFeel grapeLookAndFeel;
    ModEnvParams& params;
    
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
    DelayComponent(DelayParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~DelayComponent();
    DelayComponent(const DelayComponent &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    
    virtual void resized() override;
    
private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    
    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    
    DelayParams& params;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;
   
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
class AnalyserToggleItem : public juce::Component
{
public:
    AnalyserToggleItem(std::string name);
    virtual ~AnalyserToggleItem();
    AnalyserToggleItem(const AnalyserToggleItem &) = delete;

    void setValue(bool value) { this->value = value; repaint(); };
    bool getValue() { return value; };
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void toggleItemSelected (AnalyserToggleItem*) = 0;
    };
    void addListener (Listener* e);
private:
    virtual void mouseUp (const juce::MouseEvent& e) override;
    
    juce::ListenerList<Listener> listeners;
    ANALYSER_MODE* analyserMode;
    juce::Label nameLabel;
    bool value;
};

//==============================================================================
class AnalyserToggle : public juce::Component, private AnalyserToggleItem::Listener
{
public:
    AnalyserToggle(ANALYSER_MODE* analyserMode);
    virtual ~AnalyserToggle();
    AnalyserToggle(const AnalyserToggle &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    ANALYSER_MODE* analyserMode;
    AnalyserToggleItem spectrumToggle;
    AnalyserToggleItem envelopeToggle;
    AnalyserToggleItem filterToggle;
    
    virtual void toggleItemSelected(AnalyserToggleItem* toggleItem) override;
};

//==============================================================================
class AnalyserWindow : public juce::Component, private juce::Timer
{
public:
    AnalyserWindow(ANALYSER_MODE* analyserMode, LatestDataProvider* latestDataProvider, MonoStack* monoStack, std::array<EnvelopeParams, NUM_ENVELOPE>& envelopeParams, std::array<OscParams, NUM_OSC>& oscParams, std::array<FilterParams, NUM_FILTER>& filterParams, std::array<ModEnvParams, NUM_MODENV>& modEnvParams);
    virtual ~AnalyserWindow();
    AnalyserWindow(const AnalyserWindow &) = delete;
    
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
    
private:
    enum {
        scopeSize = 512
    };
    ANALYSER_MODE* analyserMode;
    LatestDataProvider* latestDataProvider;
    MonoStack* monoStack;
    std::array<EnvelopeParams, NUM_ENVELOPE>& envelopeParams;
    std::array<OscParams, NUM_OSC>& oscParams;
    std::array<FilterParams, NUM_FILTER>& filterParams;
    std::array<ModEnvParams, NUM_MODENV>& modEnvParams;
    ANALYSER_MODE lastAnalyserMode = ANALYSER_MODE::Spectrum;
    
    // FFT
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    static const int fftOrder = 11;
    static const int fftSize = 2048;
    float fftData[fftSize * 2];
    LatestDataProvider::Consumer fftConsumer {
        fftData, fftData + fftSize, fftSize, false
    };
    float scopeData[scopeSize]{};
    bool readyToDrawFrame = false;
    
    // Level
    float levelDataL[2048];
    float levelDataR[2048];
    LatestDataProvider::Consumer levelConsumer {
        levelDataL, levelDataR, 2048, false
    };
    float currentLevel[2]{};
    float overflowedLevelL = 0;
    float overflowedLevelR = 0;
    int overflowWarningL = 0;
    int overflowWarningR = 0;
    
    // Envelope
    Adsr ampEnvs[NUM_ENVELOPE];
    Adsr modEnvs[NUM_MODENV];
    class SimpleAmpEnvParams {
    public:
        SimpleAmpEnvParams() {}
        SimpleAmpEnvParams(EnvelopeParams& envelopeParams) {
            a = envelopeParams.Attack->get();
            d = envelopeParams.Decay->get();
            s = envelopeParams.Sustain->get();
            r = envelopeParams.Release->get();
        }
        SimpleAmpEnvParams(const SimpleAmpEnvParams &) = delete;
        float a = 0;
        float d = 0;
        float s = 0;
        float r = 0;
        bool equals(SimpleAmpEnvParams& p) {
            return a == p.a && d == p.d && s == p.s && r == p.r;
        }
    };
    class SimpleModEnvParams {
    public:
        SimpleModEnvParams() {}
        SimpleModEnvParams(ModEnvParams& modEnvParams) {
            w = modEnvParams.Wait->get();
            a = modEnvParams.Attack->get();
            d = modEnvParams.Decay->get();
            enabled = modEnvParams.Enabled->get();
            isTargetFreq = modEnvParams.isTargetFreq();
            fadeIn = static_cast<MODENV_FADE>(modEnvParams.Fade->getIndex()) == MODENV_FADE::In;
        }
        SimpleModEnvParams(const SimpleModEnvParams &) = delete;
        float w = 0;
        float a = 0;
        float d = 0;
        bool enabled = false;
        bool isTargetFreq = false;
        bool fadeIn = false;
        bool equals(SimpleModEnvParams& p) {
            return w == p.w && a == p.a && d == p.d && enabled == p.enabled && isTargetFreq == p.isTargetFreq && fadeIn == p.fadeIn;
        }
    };
    SimpleAmpEnvParams lastAmpEnvParams[NUM_ENVELOPE];
    SimpleModEnvParams lastModEnvParams[NUM_MODENV];
    float scopeDataForAmpEnv[NUM_ENVELOPE][scopeSize]{};
    float scopeDataForModEnv[NUM_MODENV][scopeSize]{};
    
    // Filter
    Filter filters[NUM_FILTER];
    class SimpleFilterParams {
    public:
      SimpleFilterParams() {}
      SimpleFilterParams(const SimpleFilterParams &) = delete;
        bool enabled = false;
        int type = -1;
        float freq = 0;
        float q = 0;
        float gain = 0;
        bool equals(SimpleFilterParams& p) {
            return enabled == p.enabled && type == p.type && freq == p.freq && q == p.q & gain == p.gain;
        }
    };
    SimpleFilterParams lastFilterParams[NUM_FILTER];
    int relNoteNumber = 69;
    float filterSource[fftSize * 2]{};
    float scopeDataForFilter[NUM_FILTER][scopeSize]{};
    
    // methods
    virtual void timerCallback() override;
    void drawNextFrameOfSpectrum();
    void drawNextFrameOfLevel();
    void paintSpectrum(juce::Graphics& g, juce::Colour colour, int offsetX, int offsetY, int width, int height, float* scopeData);
    void paintLevel(juce::Graphics& g, int offsetX, int offsetY, int width, int height, float level);
    static float xToHz(float minFreq, float maxFreq, float notmalizedX) {
        return minFreq * std::pow(maxFreq / minFreq, notmalizedX);
    }
    static float getFFTDataByHz(float* processedFFTData, float fftSize, float sampleRate, float hz) {
        float indexFloat = hz * ((fftSize * 0.5) / (sampleRate * 0.5));
        int index = indexFloat;
        float frac = indexFloat - index;
        return processedFFTData[index] * (1 - frac) + processedFFTData[index + 1] * frac;
    }
};

//==============================================================================
class ControlItemComponent : public juce::Component, private juce::ComboBox::Listener, juce::Timer
{
public:
    ControlItemComponent(ControlItemParams& params);
    virtual ~ControlItemComponent();
    ControlItemComponent(const ControlItemComponent &) = delete;

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;
private:
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void timerCallback() override;

    GrapeLookAndFeel grapeLookAndFeel;
    ControlItemParams& params;

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
    ControlComponent(std::array<ControlItemParams, NUM_CONTROL>& params);
    virtual ~ControlComponent();
    ControlComponent(const ControlComponent &) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    
    GrapeLookAndFeel grapeLookAndFeel;

    HeaderComponent header;
    juce::Label numberLabel;
    juce::Label targetLabel;
    std::array<ControlItemComponent, NUM_CONTROL> controlItemComponents;
};