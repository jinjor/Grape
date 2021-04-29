#pragma once

#include <JuceHeader.h>

namespace {
enum class WAVEFORM { Sine, Triangle, SawUp, SawDown, Square, Pulse, Random, Pink, White };
const juce::StringArray OSC_WAVEFORM_NAMES = juce::StringArray("Sine", "Triangle", "Saw", "Square", "Pulse", "Pink", "White");
const WAVEFORM OSC_WAVEFORM_Values[sizeof OSC_WAVEFORM_NAMES] = { WAVEFORM::Sine, WAVEFORM::Triangle, WAVEFORM::SawUp, WAVEFORM::Square, WAVEFORM::Pulse, WAVEFORM::Pink, WAVEFORM::White };

const juce::StringArray OSC_ENV_NAMES = juce::StringArray("1", "2");

const juce::StringArray FILTER_TARGET_NAMES = juce::StringArray("1", "2", "3", "All");

enum class FILTER_TYPE { Lowpass, Highpass };
const juce::StringArray FILTER_TYPE_NAMES = juce::StringArray("Lowpass", "Highpass");

enum class LFO_TARGET_TYPE { OSC, Filter };
const juce::StringArray LFO_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter");

const juce::StringArray LFO_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray LFO_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");

enum class LFO_TARGET_OSC_PARAM { Vibrato, Tremolo, FM, AM };
const juce::StringArray LFO_TARGET_OSC_PARAM_NAMES = juce::StringArray("Vibrato", "Tremolo", "FM", "AM");

enum class LFO_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray LFO_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");

const juce::StringArray LFO_WAVEFORM_NAMES = juce::StringArray("Sine", "Triangle", "Saw-Up", "Saw-Down", "Square", "Pulse", "Random");
const WAVEFORM LFO_WAVEFORM_Values[sizeof LFO_WAVEFORM_NAMES] = { WAVEFORM::Sine, WAVEFORM::Triangle, WAVEFORM::SawUp, WAVEFORM::SawDown, WAVEFORM::Square, WAVEFORM::Pulse, WAVEFORM::Random };

enum class MODENV_TARGET_TYPE { OSC, Filter, LFO };
const juce::StringArray MODENV_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter", "LFO");

const juce::StringArray MODENV_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray MODENV_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");
const juce::StringArray MODENV_TARGET_LFO_NAMES = juce::StringArray("1", "2", "3", "All");

enum class MODENV_TARGET_OSC_PARAM { Freq, Detune, Spread };
const juce::StringArray MODENV_TARGET_OSC_PARAM_NAMES = juce::StringArray("Freq", "Detune", "Spread");

enum class MODENV_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray MODENV_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");

enum class MODENV_TARGET_LFO_PARAM { Freq, Amount };
const juce::StringArray MODENV_TARGET_LFO_PARAM_NAMES = juce::StringArray("Freq", "Amount");

enum class MODENV_FADE { In, Out };
const juce::StringArray MODENV_FADE_NAMES = juce::StringArray("In", "Out");

enum class DELAY_TYPE { Parallel, PingPong };
const juce::StringArray DELAY_TYPE_NAMES = juce::StringArray("Parallel", "PingPong");
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
    juce::AudioParameterChoice* Waveform;
    juce::AudioParameterFloat* SlowFreq;
    juce::AudioParameterFloat* FastFreq;
    juce::AudioParameterFloat* Amount;
    LfoParams(juce::AudioParameterBool* enabled,
              juce::AudioParameterChoice* targetType,
              juce::AudioParameterChoice* targetOsc,
              juce::AudioParameterChoice* targetFilter,
              juce::AudioParameterChoice* targetOscParam,
              juce::AudioParameterChoice* targetFilterParam,
              juce::AudioParameterChoice* waveform,
              juce::AudioParameterFloat* slowFreq,
              juce::AudioParameterFloat* fastFreq,
              juce::AudioParameterFloat* amount);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool shouldUseFastFreq() {
        if(static_cast<LFO_TARGET_TYPE>(TargetType->getIndex()) == LFO_TARGET_TYPE::OSC) {
            auto targetOscParam = static_cast<LFO_TARGET_OSC_PARAM>(TargetOscParam->getIndex());
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
        auto targetType = static_cast<MODENV_TARGET_TYPE>(TargetType->getIndex());
        return (targetType == MODENV_TARGET_TYPE::OSC    && static_cast<MODENV_TARGET_OSC_PARAM>(TargetOscParam->getIndex()) == MODENV_TARGET_OSC_PARAM::Freq)
            || (targetType == MODENV_TARGET_TYPE::Filter && static_cast<MODENV_TARGET_FILTER_PARAM>(TargetFilterParam->getIndex()) == MODENV_TARGET_FILTER_PARAM::Freq)
            || (targetType == MODENV_TARGET_TYPE::LFO    && static_cast<MODENV_TARGET_LFO_PARAM>(TargetLfoParam->getIndex()) == MODENV_TARGET_LFO_PARAM::Freq);
    }
    bool shouldUseHold() {
        return !isTargetFreq() && static_cast<MODENV_FADE>(Fade->getIndex()) == MODENV_FADE::In;
    }
private:
};

//==============================================================================
class DelayParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* Type;
    juce::AudioParameterFloat* TimeL;
    juce::AudioParameterFloat* TimeR;
    juce::AudioParameterFloat* LowFreq;
    juce::AudioParameterFloat* HighFreq;
    juce::AudioParameterFloat* Feedback;
    juce::AudioParameterFloat* Mix;
    DelayParams(juce::AudioParameterBool* enabled,
                juce::AudioParameterChoice* type,
                juce::AudioParameterFloat* timeL,
                juce::AudioParameterFloat* timeR,
                juce::AudioParameterFloat* lowFreq,
                juce::AudioParameterFloat* highFreq,
                juce::AudioParameterFloat* feedback,
                juce::AudioParameterFloat* mix);
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};
