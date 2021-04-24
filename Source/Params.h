#pragma once

#include <JuceHeader.h>

namespace {

enum class OSC_WAVEFORM { Sine, Noise };
const juce::StringArray OSC_WAVEFORM_NAMES = juce::StringArray("Sine", "Noise");
const OSC_WAVEFORM OSC_WAVEFORM_VALUES[2] = {OSC_WAVEFORM::Sine, OSC_WAVEFORM::Noise};

const juce::StringArray OSC_ENV_NAMES = juce::StringArray("1", "2");
const int OSC_ENV_VALUES[2] = {0x1, 0x2};
}
namespace {
const juce::StringArray FILTER_TARGET_NAMES = juce::StringArray("1", "2", "3", "All");

enum class FILTER_TYPE { Lowpass, Highpass };
const juce::StringArray FILTER_TYPE_NAMES = juce::StringArray("Lowpass", "Highpass");
const FILTER_TYPE FILTER_TYPE_VALUES[2] = {FILTER_TYPE::Lowpass, FILTER_TYPE::Highpass};
}
namespace {
enum class LFO_TARGET_TYPE { OSC, Filter };
const juce::StringArray LFO_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter");
const LFO_TARGET_TYPE LFO_TARGET_TYPE_VALUES[2] = {LFO_TARGET_TYPE::OSC, LFO_TARGET_TYPE::Filter};

const juce::StringArray LFO_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray LFO_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");

enum class LFO_TARGET_OSC_PARAM { Vibrato, Tremolo, FM, AM };
const juce::StringArray LFO_TARGET_OSC_PARAM_NAMES = juce::StringArray("Vibrato", "Tremolo", "FM", "AM");
const LFO_TARGET_OSC_PARAM LFO_TARGET_OSC_PARAM_VALUES[4] = {LFO_TARGET_OSC_PARAM::Vibrato, LFO_TARGET_OSC_PARAM::Tremolo, LFO_TARGET_OSC_PARAM::FM, LFO_TARGET_OSC_PARAM::AM };

enum class LFO_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray LFO_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");
const LFO_TARGET_FILTER_PARAM LFO_TARGET_FILTER_PARAM_VALUES[2] = {LFO_TARGET_FILTER_PARAM::Freq, LFO_TARGET_FILTER_PARAM::Q };
}
namespace {
enum class MODENV_TARGET_TYPE { OSC, Filter, LFO };
const juce::StringArray MODENV_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter", "LFO");
const MODENV_TARGET_TYPE MODENV_TARGET_TYPE_VALUES[3] = {MODENV_TARGET_TYPE::OSC, MODENV_TARGET_TYPE::Filter, MODENV_TARGET_TYPE::LFO};

const juce::StringArray MODENV_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray MODENV_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");
const juce::StringArray MODENV_TARGET_LFO_NAMES = juce::StringArray("1", "2", "3", "All");

enum class MODENV_TARGET_OSC_PARAM { Freq, Detune, Spread };
const juce::StringArray MODENV_TARGET_OSC_PARAM_NAMES = juce::StringArray("Freq", "Detune", "Spread");
const MODENV_TARGET_OSC_PARAM MODENV_TARGET_OSC_PARAM_VALUES[3] = { MODENV_TARGET_OSC_PARAM::Freq, MODENV_TARGET_OSC_PARAM::Detune, MODENV_TARGET_OSC_PARAM::Spread };

enum class MODENV_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray MODENV_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");
const MODENV_TARGET_FILTER_PARAM MODENV_TARGET_FILTER_PARAM_VALUES[2] = {MODENV_TARGET_FILTER_PARAM::Freq, MODENV_TARGET_FILTER_PARAM::Q };

enum class MODENV_TARGET_LFO_PARAM { Freq, Amount };
const juce::StringArray MODENV_TARGET_LFO_PARAM_NAMES = juce::StringArray("Freq", "Amount");
const MODENV_TARGET_LFO_PARAM MODENV_TARGET_LFO_PARAM_VALUES[2] = {MODENV_TARGET_LFO_PARAM::Freq, MODENV_TARGET_LFO_PARAM::Amount };

enum class MODENV_FADE { In, Out };
const juce::StringArray MODENV_FADE_NAMES = juce::StringArray("In", "Out");
const MODENV_FADE MODENV_FADE_VALUES[2] = {MODENV_FADE::In, MODENV_FADE::Out };
}
//==============================================================================
class SynthParametersBase
{
public:
    virtual ~SynthParametersBase() {}
    virtual void addAllParameters(juce::AudioProcessor& processor) = 0;
    virtual void saveParameters(juce::XmlElement& xml) = 0;
    virtual void loadParameters(juce::XmlElement& xml) = 0;
};

//==============================================================================
class OscParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* Waveform;
    juce::AudioParameterInt* Octave;
    juce::AudioParameterInt* Coarse;
    juce::AudioParameterInt* Unison;
    juce::AudioParameterFloat* Detune;
    juce::AudioParameterFloat* Spread;
    juce::AudioParameterFloat* Gain;
    juce::AudioParameterChoice* Envelope;
    
    OscParams(juce::AudioParameterBool* enabled,
              juce::AudioParameterChoice* waveform,
              juce::AudioParameterInt* octave,
              juce::AudioParameterInt* coarse,
              juce::AudioParameterInt* unison,
              juce::AudioParameterFloat* detune,
              juce::AudioParameterFloat* spread,
              juce::AudioParameterFloat* gain,
              juce::AudioParameterChoice* envelope);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};

//==============================================================================
class EnvelopeParams : public SynthParametersBase
{
public:
    juce::AudioParameterFloat* Attack;
    juce::AudioParameterFloat* Decay;
    juce::AudioParameterFloat* Sustain;
    juce::AudioParameterFloat* Release;
    
    EnvelopeParams(juce::AudioParameterFloat* attack,
                  juce::AudioParameterFloat* decay,
                  juce::AudioParameterFloat* sustain,
                  juce::AudioParameterFloat* release);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};

//==============================================================================
class FilterParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* Target;
    juce::AudioParameterChoice* Type;
    juce::AudioParameterFloat* Octave;
    juce::AudioParameterFloat* Q;
    FilterParams(juce::AudioParameterBool* enabled,
                 juce::AudioParameterChoice* target,
                 juce::AudioParameterChoice* type,
                 juce::AudioParameterFloat* octave,
                 juce::AudioParameterFloat* q);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};

//==============================================================================
class LfoParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* TargetType;
    juce::AudioParameterChoice* TargetOsc;
    juce::AudioParameterChoice* TargetFilter;
    juce::AudioParameterChoice* TargetOscParam;
    juce::AudioParameterChoice* TargetFilterParam;
    juce::AudioParameterFloat* SlowFreq;
    juce::AudioParameterFloat* FastFreq;
    juce::AudioParameterFloat* Amount;
    LfoParams(juce::AudioParameterBool* enabled,
              juce::AudioParameterChoice* targetType,
              juce::AudioParameterChoice* targetOsc,
              juce::AudioParameterChoice* targetFilter,
              juce::AudioParameterChoice* targetOscParam,
              juce::AudioParameterChoice* targetFilterParam,
              juce::AudioParameterFloat* slowFreq,
              juce::AudioParameterFloat* fastFreq,
              juce::AudioParameterFloat* amount);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool shouldUseFastFreq() {
        if(LFO_TARGET_TYPE_VALUES[TargetType->getIndex()] == LFO_TARGET_TYPE::OSC) {
            auto targetOscParam = LFO_TARGET_OSC_PARAM_VALUES[TargetOscParam->getIndex()];
            switch(targetOscParam) {
                case LFO_TARGET_OSC_PARAM::FM:
                case LFO_TARGET_OSC_PARAM::AM:
                    return true;
                    break;
                default:
                    break;
            }
        }
        return false;
    }
private:
};

//==============================================================================
class ModEnvParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* TargetType;
    juce::AudioParameterChoice* TargetOsc;
    juce::AudioParameterChoice* TargetFilter;
    juce::AudioParameterChoice* TargetLfo;
    juce::AudioParameterChoice* TargetOscParam;
    juce::AudioParameterChoice* TargetFilterParam;
    juce::AudioParameterChoice* TargetLfoParam;
    juce::AudioParameterChoice* Fade;
    juce::AudioParameterFloat* PeakFreq;
    juce::AudioParameterFloat* Wait;
    juce::AudioParameterFloat* Attack;
    juce::AudioParameterFloat* Decay;
    ModEnvParams(juce::AudioParameterBool* enabled,
              juce::AudioParameterChoice* targetType,
              juce::AudioParameterChoice* targetOsc,
              juce::AudioParameterChoice* targetFilter,
              juce::AudioParameterChoice* targetLfo,
              juce::AudioParameterChoice* targetOscParam,
              juce::AudioParameterChoice* targetFilterParam,
              juce::AudioParameterChoice* targetLfoParam,
              juce::AudioParameterChoice* fade,
              juce::AudioParameterFloat* peakFreq,
              juce::AudioParameterFloat* wait,
              juce::AudioParameterFloat* attack,
              juce::AudioParameterFloat* decay);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool isTargetFreq() {
        auto targetType = MODENV_TARGET_TYPE_VALUES[TargetType->getIndex()];
        return (targetType == MODENV_TARGET_TYPE::OSC    && MODENV_TARGET_OSC_PARAM_VALUES[TargetOscParam->getIndex()] == MODENV_TARGET_OSC_PARAM::Freq)
            || (targetType == MODENV_TARGET_TYPE::Filter && MODENV_TARGET_FILTER_PARAM_VALUES[TargetFilterParam->getIndex()] == MODENV_TARGET_FILTER_PARAM::Freq)
            || (targetType == MODENV_TARGET_TYPE::LFO    && MODENV_TARGET_LFO_PARAM_VALUES[TargetLfoParam->getIndex()] == MODENV_TARGET_LFO_PARAM::Freq);
    }
    bool shouldUseHold() {
        return !isTargetFreq() && MODENV_FADE_VALUES[Fade->getIndex()] == MODENV_FADE::In;
    }
private:
};

//==============================================================================
class DelayParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterFloat* Time;
    juce::AudioParameterFloat* LowFreq;
    juce::AudioParameterFloat* HighFreq;
    juce::AudioParameterFloat* Feedback;
    juce::AudioParameterFloat* Mix;
    DelayParams(juce::AudioParameterBool* enabled,
                juce::AudioParameterFloat* time,
                 juce::AudioParameterFloat* lowFreq,
                 juce::AudioParameterFloat* highFreq,
                 juce::AudioParameterFloat* feedback,
                 juce::AudioParameterFloat* mix);
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};
