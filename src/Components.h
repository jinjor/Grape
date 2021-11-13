#pragma once

#include <JuceHeader.h>

#include "LookAndFeel.h"
#include "Params.h"
#include "PluginProcessor.h"
#include "StyleConstants.h"

using namespace styles;

enum class ANALYSER_MODE { Spectrum, Envelope, Filter };

//==============================================================================
class ComponentHelper {
protected:
    GrapeLookAndFeel grapeLookAndFeel;
    GrapeLookAndFeel grapeLookAndFeelControlled = GrapeLookAndFeel(true);
    juce::Font paramLabelFont = juce::Font(PARAM_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");
    juce::Font paramValueLabelFont =
        juce::Font(PARAM_VALUE_LABEL_FONT_SIZE, juce::Font::plain).withTypefaceStyle("Regular");

    void initLabel(juce::Label& label,
                   int fontSize,
                   std::string&& typeFaceStyle,
                   juce::Justification justification,
                   std::string&& text,
                   juce::Component& parent) {
        juce::Font paramLabelFont = juce::Font(fontSize, juce::Font::plain).withTypefaceStyle(typeFaceStyle);

        label.setFont(paramLabelFont);
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(justification);
        label.setInterceptsMouseClicks(false, false);
        parent.addAndMakeVisible(label);
    }
    void initLabel(juce::Label& label, std::string&& text, juce::Component& parent) {
        label.setFont(paramLabelFont);
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setEditable(false, false, false);
        parent.addAndMakeVisible(label);
    }
    void initStatusValue(juce::Label& label, std::string&& text, juce::Component& parent) {
        label.setFont(paramValueLabelFont);
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::left);
        label.setEditable(false, false, false);
        parent.addAndMakeVisible(label);
    }
    void initStatusKey(juce::Label& label, std::string&& text, juce::Component& parent) {
        label.setFont(paramLabelFont);
        label.setText(text + ":", juce::dontSendNotification);
        label.setJustificationType(juce::Justification::right);
        label.setEditable(false, false, false);
        parent.addAndMakeVisible(label);
    }
    void initChoice(juce::ComboBox& box,
                    juce::AudioParameterChoice* param,
                    juce::ComboBox::Listener* listener,
                    juce::Component& parent) {
        box.setLookAndFeel(&grapeLookAndFeel);
        box.addItemList(param->getAllValueStrings(), 1);
        box.setSelectedItemIndex(param->getIndex(), juce::dontSendNotification);
        box.setJustificationType(juce::Justification::centred);
        box.addListener(listener);
        parent.addAndMakeVisible(box);
    }
    void initChoice(juce::ComboBox& box,
                    juce::AudioParameterBool* param,
                    juce::ComboBox::Listener* listener,
                    juce::Component& parent) {
        box.setLookAndFeel(&grapeLookAndFeel);
        box.addItemList(param->getAllValueStrings(), 1);
        box.setSelectedItemIndex(param->get(), juce::dontSendNotification);
        box.setJustificationType(juce::Justification::centred);
        box.addListener(listener);
        parent.addAndMakeVisible(box);
    }
    void initSkewFromMid(juce::Slider& slider,
                         juce::AudioParameterFloat* param,
                         float step,
                         const char* unit,
                         std::function<juce::String(double)>&& format,
                         juce::Slider::Listener* listener,
                         juce::Component& parent) {
        slider.setLookAndFeel(&grapeLookAndFeel);
        auto nrange = NormalisableRange<double>{
            param->range.start, param->range.end, step, param->range.skew, param->range.symmetricSkew};
        slider.setNormalisableRange(nrange);
        slider.setValue(param->get(), juce::dontSendNotification);
        slider.setPopupDisplayEnabled(true, true, nullptr);
        slider.setScrollWheelEnabled(false);
        if (unit != nullptr) {
            slider.setTextValueSuffix(unit);
        }
        if (format != nullptr) {
            slider.textFromValueFunction = format;
        }
        slider.addListener(listener);
        parent.addAndMakeVisible(slider);
    }
    void initLinear(juce::Slider& slider,
                    juce::AudioParameterFloat* param,
                    float step,
                    const char* unit,
                    std::function<juce::String(double)>&& format,
                    juce::Slider::Listener* listener,
                    juce::Component& parent) {
        slider.setLookAndFeel(&grapeLookAndFeel);
        slider.setRange(param->range.start, param->range.end, step);
        slider.setValue(param->get(), juce::dontSendNotification);
        slider.setPopupDisplayEnabled(true, true, nullptr);
        slider.setScrollWheelEnabled(false);
        if (unit != nullptr) {
            slider.setTextValueSuffix(unit);
        }
        if (format != nullptr) {
            slider.textFromValueFunction = format;
        }
        slider.addListener(listener);
        parent.addAndMakeVisible(slider);
    }
    void initLinear(juce::Slider& slider,
                    juce::AudioParameterInt* param,
                    float step,
                    const char* unit,
                    std::function<juce::String(double)>&& format,
                    juce::Slider::Listener* listener,
                    juce::Component& parent) {
        slider.setLookAndFeel(&grapeLookAndFeel);
        slider.setRange(param->getRange().getStart(), param->getRange().getEnd(), step);
        slider.setValue(param->get(), juce::dontSendNotification);
        slider.setPopupDisplayEnabled(true, true, nullptr);
        slider.setScrollWheelEnabled(false);
        if (unit != nullptr) {
            slider.setTextValueSuffix(unit);
        }
        if (format != nullptr) {
            slider.textFromValueFunction = format;
        }
        slider.addListener(listener);
        parent.addAndMakeVisible(slider);
    }
    void initLinear(juce::Slider& slider,
                    juce::AudioParameterFloat* param,
                    float step,
                    juce::Slider::Listener* listener,
                    juce::Component& parent) {
        initLinear(slider, param, step, nullptr, nullptr, listener, parent);
    }
    void initLinear(juce::Slider& slider,
                    juce::AudioParameterInt* param,
                    juce::Slider::Listener* listener,
                    juce::Component& parent) {
        initLinear(slider, param, 1, nullptr, nullptr, listener, parent);
    }
    void initLinearPercent(juce::Slider& slider,
                           juce::AudioParameterFloat* param,
                           float step,
                           juce::Slider::Listener* listener,
                           juce::Component& parent) {
        auto f = [](double gain) { return juce::String(gain * 100, 0) + " %"; };
        initLinear(slider, param, step, nullptr, std::move(f), listener, parent);
    }
    void initEnum(juce::Slider& slider,
                  juce::AudioParameterChoice* param,
                  juce::Slider::Listener* listener,
                  juce::Component& parent) {
        const juce::StringArray& values = param->getAllValueStrings();
        slider.setLookAndFeel(&grapeLookAndFeel);
        slider.setRange(0, values.size() - 1, 1);
        slider.setValue(param->getIndex(), juce::dontSendNotification);
        slider.setPopupDisplayEnabled(true, true, nullptr);
        slider.setScrollWheelEnabled(false);
        slider.textFromValueFunction = [values](double index) { return values[index]; };
        slider.addListener(listener);
        parent.addAndMakeVisible(slider);
    }

    void consumeLabeledKnob(juce::Rectangle<int>& parentArea, juce::Label& label, juce::Slider& knob) {
        parentArea.removeFromLeft(LOCAL_MARGIN);
        auto area = parentArea.removeFromLeft(SLIDER_WIDTH);
        label.setBounds(area.removeFromTop(LABEL_HEIGHT));
        area.removeFromTop(LOCAL_MARGIN);
        knob.setBounds(area.removeFromTop(KNOB_HEIGHT));
    }
    void consumeLabeledKnob(juce::Rectangle<int>& parentArea,
                            juce::Label& label,
                            juce::Slider& knob1,
                            juce::Slider& knob2) {
        parentArea.removeFromLeft(LOCAL_MARGIN);
        auto area = parentArea.removeFromLeft(SLIDER_WIDTH);
        label.setBounds(area.removeFromTop(LABEL_HEIGHT));
        area.removeFromTop(LOCAL_MARGIN);
        auto knobBounds = area.removeFromTop(KNOB_HEIGHT);
        knob1.setBounds(knobBounds);
        knob2.setBounds(knobBounds);
    }
    void consumeLabeledKnob(juce::Rectangle<int>& parentArea,
                            juce::Label& label1,
                            juce::Slider& knob1,
                            juce::Label& label2,
                            juce::Slider& knob2) {
        auto copied = parentArea;
        consumeLabeledKnob(parentArea, label1, knob1);
        consumeLabeledKnob(copied, label2, knob2);
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
};

//==============================================================================

enum class HEADER_CHECK { Hidden, Disabled, Enabled };

class HeaderComponent : public juce::Component {
public:
    HeaderComponent(std::string name, HEADER_CHECK check);
    virtual ~HeaderComponent();
    HeaderComponent(const HeaderComponent&) = delete;
    juce::ToggleButton enabledButton;
    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;

private:
    std::string name;
    HEADER_CHECK check;
};

//==============================================================================
class VoiceComponent : public juce::Component,
                       juce::ComboBox::Listener,
                       juce::Slider::Listener,
                       private juce::Timer,
                       ComponentHelper {
public:
    VoiceComponent(VoiceParams& params, std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~VoiceComponent();
    VoiceComponent(const VoiceComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;

private:
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;

    VoiceParams& params;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;

    HeaderComponent header;

    juce::ComboBox modeSelector;
    juce::Slider portamentoTimeSlider;
    juce::Slider pitchBendRangeSlider;
    juce::ComboBox targetNoteKindSelector;
    juce::ComboBox targetNoteOctSelector;

    juce::Label modeLabel;
    juce::Label portamentoTimeLabel;
    juce::Label pitchBendRangeLabel;
    juce::Label targetNoteLabel;
};

//==============================================================================
class StatusComponent : public juce::Component, private juce::Timer, ComponentHelper {
public:
    StatusComponent(int* polyphony, TimeConsumptionState* timeConsumptionState, LatestDataProvider* latestDataProvider);
    virtual ~StatusComponent();
    StatusComponent(const StatusComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;

private:
    virtual void timerCallback() override;
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
    LatestDataProvider::Consumer levelConsumer{levelDataL, levelDataR, 2048, false};
    float overflowedLevel = 0;
    int overflowWarning = 0;
};

//==============================================================================
class MasterComponent : public juce::Component, juce::Slider::Listener, private juce::Timer, ComponentHelper {
public:
    MasterComponent(GlobalParams& params);
    virtual ~MasterComponent();
    MasterComponent(const MasterComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;

    GlobalParams& params;

    HeaderComponent header;

    juce::Slider panSlider;
    juce::Slider volumeSlider;

    juce::Label panLabel;
    juce::Label volumeLabel;
};

//==============================================================================
class OscComponent : public juce::Component,
                     juce::ToggleButton::Listener,
                     juce::ComboBox::Listener,
                     juce::Slider::Listener,
                     private juce::Timer,
                     ComponentHelper {
public:
    OscComponent(int index,
                 VoiceParams& voiceParams,
                 std::vector<MainParams>& mainParamList,
                 std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~OscComponent();
    OscComponent(const OscComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;

    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;
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

    OscParams& getSelectedOscParams() {
        return mainParamList[voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128].oscParams[index];
    }
};

//==============================================================================
class EnvelopeComponent : public juce::Component, juce::Slider::Listener, private juce::Timer, ComponentHelper {
public:
    EnvelopeComponent(int index, VoiceParams& voiceParams, std::vector<MainParams>& mainParamList);
    virtual ~EnvelopeComponent();
    EnvelopeComponent(const EnvelopeComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;
    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;

    HeaderComponent header;

    juce::Slider attackCurveSlider;
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackCurveLabel;
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    EnvelopeParams& getSelectedEnvelopeParams() {
        return mainParamList[voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128].envelopeParams[index];
    }
};

//==============================================================================
class FilterComponent : public juce::Component,
                        juce::ToggleButton::Listener,
                        juce::ComboBox::Listener,
                        juce::Slider::Listener,
                        private juce::Timer,
                        ComponentHelper {
public:
    FilterComponent(int index,
                    VoiceParams& voiceParams,
                    std::vector<MainParams>& mainParamList,
                    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~FilterComponent();
    FilterComponent(const FilterComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;

    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;
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

    FilterParams& getSelectedFilterParams() {
        return mainParamList[voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128].filterParams[index];
    }
};

//==============================================================================
class LfoComponent : public juce::Component,
                     juce::ToggleButton::Listener,
                     juce::ComboBox::Listener,
                     juce::Slider::Listener,
                     private juce::Timer,
                     ComponentHelper {
public:
    LfoComponent(int index,
                 VoiceParams& voiceParams,
                 std::vector<MainParams>& mainParamList,
                 std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~LfoComponent();
    LfoComponent(const LfoComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;

    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;
    std::array<ControlItemParams, NUM_CONTROL>& controlItemParams;

    HeaderComponent header;

    juce::Component body;
    juce::Component targetSelector;

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

    LfoParams& getSelectedLfoParams() {
        return mainParamList[voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128].lfoParams[index];
    }
};

//==============================================================================
class ModEnvComponent : public juce::Component,
                        juce::ToggleButton::Listener,
                        juce::ComboBox::Listener,
                        juce::Slider::Listener,
                        private juce::Timer,
                        ComponentHelper {
public:
    ModEnvComponent(int index, VoiceParams& voiceParams, std::vector<MainParams>& mainParamList);
    virtual ~ModEnvComponent();
    ModEnvComponent(const ModEnvComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;
    int index;

    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;

    HeaderComponent header;

    juce::Component body;
    juce::Component targetSelector;

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

    ModEnvParams& getSelectedModEnvParams() {
        return mainParamList[voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128].modEnvParams[index];
    }
};

//==============================================================================
class DelayComponent : public juce::Component,
                       juce::ToggleButton::Listener,
                       juce::ComboBox::Listener,
                       juce::Slider::Listener,
                       private juce::Timer,
                       ComponentHelper {
public:
    DelayComponent(VoiceParams& voiceParams,
                   std::vector<MainParams>& mainParamList,
                   std::array<ControlItemParams, NUM_CONTROL>& controlItemParams);
    virtual ~DelayComponent();
    DelayComponent(const DelayComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    virtual void buttonClicked(juce::Button* button) override;
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void timerCallback() override;

    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;
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

    DelayParams& getSelectedDelayParams() {
        return mainParamList[voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128].delayParams;
    }
};

//==============================================================================
class AnalyserToggleItem : public juce::Component, private ComponentHelper {
public:
    AnalyserToggleItem(std::string name);
    virtual ~AnalyserToggleItem();
    AnalyserToggleItem(const AnalyserToggleItem&) = delete;

    void setValue(bool value) {
        this->value = value;
        repaint();
    };
    bool getValue() { return value; };

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;

    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void toggleItemSelected(AnalyserToggleItem*) = 0;
    };
    void addListener(Listener* e);

private:
    virtual void mouseUp(const juce::MouseEvent& e) override;

    juce::ListenerList<Listener> listeners;
    ANALYSER_MODE* analyserMode;
    juce::Label nameLabel;
    bool value;
};

//==============================================================================
class AnalyserToggle : public juce::Component, private AnalyserToggleItem::Listener {
public:
    AnalyserToggle(ANALYSER_MODE* analyserMode);
    virtual ~AnalyserToggle();
    AnalyserToggle(const AnalyserToggle&) = delete;

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
class AnalyserWindow : public juce::Component, private juce::Timer {
public:
    AnalyserWindow(ANALYSER_MODE* analyserMode,
                   LatestDataProvider* latestDataProvider,
                   MonoStack* monoStack,
                   VoiceParams& voiceParams,
                   std::vector<MainParams>& mainParamList);
    virtual ~AnalyserWindow();
    AnalyserWindow(const AnalyserWindow&) = delete;

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;

private:
    enum { scopeSize = 512 };
    ANALYSER_MODE* analyserMode;
    LatestDataProvider* latestDataProvider;
    MonoStack* monoStack;
    VoiceParams& voiceParams;
    std::vector<MainParams>& mainParamList;
    ANALYSER_MODE lastAnalyserMode = ANALYSER_MODE::Spectrum;

    // FFT
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    static const int fftOrder = 11;
    static const int fftSize = 2048;
    float fftData[fftSize * 2];
    LatestDataProvider::Consumer fftConsumer{fftData, fftData + fftSize, fftSize, false};
    float scopeData[scopeSize]{};
    bool readyToDrawFrame = false;

    // Level
    float levelDataL[2048];
    float levelDataR[2048];
    LatestDataProvider::Consumer levelConsumer{levelDataL, levelDataR, 2048, false};
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
            ac = envelopeParams.AttackCurve->get();
            a = envelopeParams.Attack->get();
            d = envelopeParams.Decay->get();
            s = envelopeParams.Sustain->get();
            r = envelopeParams.Release->get();
        }
        SimpleAmpEnvParams(const SimpleAmpEnvParams&) = delete;
        float ac = 0;
        float a = 0;
        float d = 0;
        float s = 0;
        float r = 0;
        bool equals(SimpleAmpEnvParams& p) { return ac == p.ac && a == p.a && d == p.d && s == p.s && r == p.r; }
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
        SimpleModEnvParams(const SimpleModEnvParams&) = delete;
        float w = 0;
        float a = 0;
        float d = 0;
        bool enabled = false;
        bool isTargetFreq = false;
        bool fadeIn = false;
        bool equals(SimpleModEnvParams& p) {
            return w == p.w && a == p.a && d == p.d && enabled == p.enabled && isTargetFreq == p.isTargetFreq &&
                   fadeIn == p.fadeIn;
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
        SimpleFilterParams(const SimpleFilterParams&) = delete;
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
    bool drawNextFrameOfSpectrum();
    bool drawNextFrameOfLevel();
    void paintSpectrum(
        juce::Graphics& g, juce::Colour colour, int offsetX, int offsetY, int width, int height, float* scopeData);
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
class ControlItemComponent : public juce::Component, private juce::ComboBox::Listener, juce::Timer, ComponentHelper {
public:
    ControlItemComponent(ControlItemParams& params);
    virtual ~ControlItemComponent();
    ControlItemComponent(const ControlItemComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;
    virtual void resized() override;

private:
    virtual void comboBoxChanged(juce::ComboBox* comboBox) override;
    virtual void timerCallback() override;

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
class ControlComponent : public juce::Component, private ComponentHelper {
public:
    ControlComponent(std::array<ControlItemParams, NUM_CONTROL>& params);
    virtual ~ControlComponent();
    ControlComponent(const ControlComponent&) = delete;

    virtual void paint(juce::Graphics& g) override;

    virtual void resized() override;

private:
    HeaderComponent header;
    juce::Label numberLabel;
    juce::Label targetLabel;
    std::array<ControlItemComponent, NUM_CONTROL> controlItemComponents;
};
