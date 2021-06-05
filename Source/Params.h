#pragma once

#include <JuceHeader.h>

namespace {

enum class VOICE_MODE { Mono, Poly };
const juce::StringArray VOICE_MODE_NAMES = juce::StringArray("Mono", "Poly");

enum class WAVEFORM { Sine, Triangle, SawUp, SawDown, Square, Random, Pink, White };
const juce::StringArray OSC_WAVEFORM_NAMES = juce::StringArray("Sine", "Triangle", "Saw", "Square", "Pink", "White");
const WAVEFORM OSC_WAVEFORM_VALUES[6] = { WAVEFORM::Sine, WAVEFORM::Triangle, WAVEFORM::SawDown, WAVEFORM::Square, WAVEFORM::Pink, WAVEFORM::White };

const juce::StringArray OSC_ENV_NAMES = juce::StringArray("1", "2");

const juce::StringArray FILTER_TARGET_NAMES = juce::StringArray("1", "2", "3", "All");

enum class FILTER_TYPE { Lowpass, Highpass, Bandpass1, Bandpass2, Notch, Peaking, LowShelf, HighShelf };
const juce::StringArray FILTER_TYPE_NAMES = juce::StringArray("Lowpass", "Highpass", "Bandpass1", "Bandpass2", "Notch", "Peaking", "LowShelf", "HighShelf" );

enum class FILTER_FREQ_TYPE { Absolute, Relative };
const juce::StringArray FILTER_FREQ_TYPE_NAMES = juce::StringArray("Abs", "Rel");

enum class LFO_TARGET_TYPE { OSC, Filter };
const juce::StringArray LFO_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter");

const juce::StringArray LFO_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray LFO_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");

enum class LFO_TARGET_OSC_PARAM { Vibrato, Tremolo, Edge, FM, AM, Pan };
const juce::StringArray LFO_TARGET_OSC_PARAM_NAMES = juce::StringArray("Vibrato", "Tremolo", "Edge", "FM", "AM", "Pan");

enum class LFO_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray LFO_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");

const juce::StringArray LFO_WAVEFORM_NAMES = juce::StringArray("Sine", "Triangle", "Saw-Up", "Saw-Down", "Square", "Random");
const WAVEFORM LFO_WAVEFORM_VALUES[7] = { WAVEFORM::Sine, WAVEFORM::Triangle, WAVEFORM::SawUp, WAVEFORM::SawDown, WAVEFORM::Square, WAVEFORM::Random };

enum class MODENV_TARGET_TYPE { OSC, Filter, LFO };
const juce::StringArray MODENV_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter", "LFO");

const juce::StringArray MODENV_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray MODENV_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");
const juce::StringArray MODENV_TARGET_LFO_NAMES = juce::StringArray("1", "2", "3", "All");

enum class MODENV_TARGET_OSC_PARAM { Freq, Edge, Detune, Spread };
const juce::StringArray MODENV_TARGET_OSC_PARAM_NAMES = juce::StringArray("Freq", "Edge", "Detune", "Spread");

enum class MODENV_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray MODENV_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");

enum class MODENV_TARGET_LFO_PARAM { Freq, Amount };
const juce::StringArray MODENV_TARGET_LFO_PARAM_NAMES = juce::StringArray("Freq", "Amount");

enum class MODENV_FADE { In, Out };
const juce::StringArray MODENV_FADE_NAMES = juce::StringArray("In", "Out");

enum class DELAY_TYPE { Parallel, PingPong };
const juce::StringArray DELAY_TYPE_NAMES = juce::StringArray("Parallel", "Ping-Pong");

const juce::StringArray DELAY_TIME_SYNC_NAMES = juce::StringArray(
    "1/1",   "1/2",   "1/4",   "1/8",   "1/16",   "1/32",
    "1/1 T", "1/2 T", "1/4 T", "1/8 T", "1/16 T", "1/32 T",
    "1/1 D", "1/2 D", "1/4 D", "1/8 D", "1/16 D", "1/32 D");
const double DELAY_TIME_SYNC_VALUES[18] = {
    1.0,       0.5,       0.25,       0.125,       0.0625,       0.03125,
    1.0 * 2/3, 0.5 * 2/3, 0.25 * 2/3, 0.125 * 2/3, 0.0625 * 2/3, 0.03125 * 2/3,
    1.0 * 3/2, 0.5 * 3/2, 0.25 * 3/2, 0.125 * 3/2, 0.0625 * 3/2, 0.03125 * 3/2 };

const juce::StringArray CONTROL_NUMBER_NAMES = juce::StringArray("None", "1: Modulation", "2: Breath", "4: Foot", "5: Portamento Time", "7: Main Volume", "10: Pan", "11: Expression", "71: Resonance", "74: Brightness", "75: Sound Control", "76: Sound Control", "77: Sound Control", "78: Sound Control", "79: Sound Control", "91: Reverb", "92: Tremolo", "93: Chorus", "94: Detune", "95: Phaser");
const int CONTROL_NUMBER_VALUES[20] { -1, 1, 2, 4, 5, 7, 10, 11, 71, 74, 75, 76, 77, 78, 79, 91, 92, 93, 94, 95 };

enum class CONTROL_TARGET_TYPE { OSC, Filter, LFO, Master };
const juce::StringArray CONTROL_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter", "LFO", "Misc");

const juce::StringArray CONTROL_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray CONTROL_TARGET_FILTER_NAMES = juce::StringArray("1", "2", "All");
const juce::StringArray CONTROL_TARGET_LFO_NAMES = juce::StringArray("1", "2", "3", "All");
const juce::StringArray CONTROL_TARGET_MODENV_NAMES = juce::StringArray("1", "2", "3", "All");

enum class CONTROL_TARGET_OSC_PARAM { Freq, Edge, Detune, Spread, Pan, Gain };
const juce::StringArray CONTROL_TARGET_OSC_PARAM_NAMES = juce::StringArray("Freq", "Edge", "Detune", "Spread", "Pan", "Gain");

enum class CONTROL_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray CONTROL_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");

enum class CONTROL_TARGET_LFO_PARAM { Freq, Amount };
const juce::StringArray CONTROL_TARGET_LFO_PARAM_NAMES = juce::StringArray("Freq", "Amount");

enum class CONTROL_TARGET_MISC_PARAM { PortamentoAmount, DelayAmount, MasterVolume };
const juce::StringArray CONTROL_TARGET_MISC_PARAM_NAMES = juce::StringArray("Portamento Amount", "Delay Amount", "Master Volume");

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
class VoiceParams : public SynthParametersBase
{
public:

    juce::AudioParameterChoice* Mode;
    juce::AudioParameterFloat* PortamentoTime;
    juce::AudioParameterInt* PitchBendRange;
    
    VoiceParams(juce::AudioParameterChoice* mode,
                juce::AudioParameterFloat* portamentoTime,
                juce::AudioParameterInt* pitchBendRange);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};

//==============================================================================
class OscParams : public SynthParametersBase
{
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* Waveform;
    juce::AudioParameterFloat* Edge;
    juce::AudioParameterInt* Octave;
    juce::AudioParameterInt* Coarse;
    juce::AudioParameterInt* Unison;
    juce::AudioParameterFloat* Detune;
    juce::AudioParameterFloat* Spread;
    juce::AudioParameterFloat* Gain;
    juce::AudioParameterChoice* Envelope;
    
    OscParams(juce::AudioParameterBool* enabled,
              juce::AudioParameterChoice* waveform,
              juce::AudioParameterFloat* edge,
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
    juce::AudioParameterChoice* FreqType;
    juce::AudioParameterFloat* Hz;
    juce::AudioParameterFloat* Octave;
    juce::AudioParameterFloat* Q;
    juce::AudioParameterFloat* Gain;
    FilterParams(juce::AudioParameterBool* enabled,
                 juce::AudioParameterChoice* target,
                 juce::AudioParameterChoice* type,
                 juce::AudioParameterChoice* freqType,
                 juce::AudioParameterFloat* hz,
                 juce::AudioParameterFloat* octave,
                 juce::AudioParameterFloat* q,
                 juce::AudioParameterFloat* gain);
    
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool hasGain() {
        switch(static_cast<FILTER_TYPE>(Type->getIndex())) {
            case FILTER_TYPE::Peaking:
            case FILTER_TYPE::LowShelf:
            case FILTER_TYPE::HighShelf:
                return true;
            default:
                return false;
        }
    }
    
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
    juce::AudioParameterBool* Sync;
    juce::AudioParameterFloat* TimeL;
    juce::AudioParameterFloat* TimeR;
    juce::AudioParameterChoice* TimeSyncL;
    juce::AudioParameterChoice* TimeSyncR;
    juce::AudioParameterFloat* LowFreq;
    juce::AudioParameterFloat* HighFreq;
    juce::AudioParameterFloat* Feedback;
    juce::AudioParameterFloat* Mix;
    DelayParams(juce::AudioParameterBool* enabled,
                juce::AudioParameterChoice* type,
                juce::AudioParameterBool* sync,
                juce::AudioParameterFloat* timeL,
                juce::AudioParameterFloat* timeR,
                juce::AudioParameterChoice* timeSyncL,
                juce::AudioParameterChoice* timeSyncR,
                juce::AudioParameterFloat* lowFreq,
                juce::AudioParameterFloat* highFreq,
                juce::AudioParameterFloat* feedback,
                juce::AudioParameterFloat* mix);
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};

//==============================================================================
class ControlItemParams : public SynthParametersBase
{
public:
    juce::AudioParameterChoice* Number;
    juce::AudioParameterChoice* TargetType;
    juce::AudioParameterChoice* TargetOsc;
    juce::AudioParameterChoice* TargetFilter;
    juce::AudioParameterChoice* TargetLfo;
    juce::AudioParameterChoice* TargetOscParam;
    juce::AudioParameterChoice* TargetFilterParam;
    juce::AudioParameterChoice* TargetLfoParam;
    juce::AudioParameterChoice* TargetMiscParam;
    ControlItemParams(juce::AudioParameterChoice* number,
                      juce::AudioParameterChoice* targetType,
                      juce::AudioParameterChoice* targetOsc,
                      juce::AudioParameterChoice* targetFilter,
                      juce::AudioParameterChoice* targetLfo,
                      juce::AudioParameterChoice* targetOscParam,
                      juce::AudioParameterChoice* targetFilterParam,
                      juce::AudioParameterChoice* targetLfoParam,
                      juce::AudioParameterChoice* targetMiscParam);
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    
private:
};
