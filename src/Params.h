#pragma once

#include <JuceHeader.h>

namespace {

enum class VOICE_MODE { Mono, Poly };
const juce::StringArray VOICE_MODE_NAMES = juce::StringArray("Mono", "Poly");

enum class WAVEFORM { Sine, Triangle, SawUp, SawDown, Square, Random, Pink, White };
const juce::StringArray OSC_WAVEFORM_NAMES = juce::StringArray("Sine", "Triangle", "Saw", "Square", "Pink", "White");
const WAVEFORM OSC_WAVEFORM_VALUES[6] = {
    WAVEFORM::Sine, WAVEFORM::Triangle, WAVEFORM::SawDown, WAVEFORM::Square, WAVEFORM::Pink, WAVEFORM::White};

const juce::StringArray OSC_ENV_NAMES = juce::StringArray("1", "2");

const juce::StringArray FILTER_TARGET_NAMES = juce::StringArray("1", "2", "3", "All");

enum class FILTER_TYPE { Lowpass, Highpass, Bandpass1, Bandpass2, Notch, Peaking, LowShelf, HighShelf };
const juce::StringArray FILTER_TYPE_NAMES =
    juce::StringArray("Lowpass", "Highpass", "Bandpass1", "Bandpass2", "Notch", "Peaking", "LowShelf", "HighShelf");

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

const juce::StringArray LFO_WAVEFORM_NAMES =
    juce::StringArray("Sine", "Triangle", "Saw-Up", "Saw-Down", "Square", "Random");
const WAVEFORM LFO_WAVEFORM_VALUES[7] = {
    WAVEFORM::Sine, WAVEFORM::Triangle, WAVEFORM::SawUp, WAVEFORM::SawDown, WAVEFORM::Square, WAVEFORM::Random};

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

const juce::StringArray DELAY_TIME_SYNC_NAMES = juce::StringArray("1/1",
                                                                  "1/2",
                                                                  "1/4",
                                                                  "1/8",
                                                                  "1/16",
                                                                  "1/32",
                                                                  "1/1 T",
                                                                  "1/2 T",
                                                                  "1/4 T",
                                                                  "1/8 T",
                                                                  "1/16 T",
                                                                  "1/32 T",
                                                                  "1/1 D",
                                                                  "1/2 D",
                                                                  "1/4 D",
                                                                  "1/8 D",
                                                                  "1/16 D",
                                                                  "1/32 D");
const double DELAY_TIME_SYNC_VALUES[18] = {1.0,
                                           0.5,
                                           0.25,
                                           0.125,
                                           0.0625,
                                           0.03125,
                                           1.0 * 2 / 3,
                                           0.5 * 2 / 3,
                                           0.25 * 2 / 3,
                                           0.125 * 2 / 3,
                                           0.0625 * 2 / 3,
                                           0.03125 * 2 / 3,
                                           1.0 * 3 / 2,
                                           0.5 * 3 / 2,
                                           0.25 * 3 / 2,
                                           0.125 * 3 / 2,
                                           0.0625 * 3 / 2,
                                           0.03125 * 3 / 2};

const juce::StringArray CONTROL_NUMBER_NAMES = juce::StringArray("None",
                                                                 "1: Modulation",
                                                                 "2: Breath",
                                                                 "4: Foot",
                                                                 "5: Portamento Time",
                                                                 "71: Resonance",
                                                                 "74: Brightness",
                                                                 "75: Sound Control",
                                                                 "76: Sound Control",
                                                                 "77: Sound Control",
                                                                 "78: Sound Control",
                                                                 "79: Sound Control",
                                                                 "91: Reverb",
                                                                 "92: Tremolo",
                                                                 "93: Chorus",
                                                                 "94: Detune",
                                                                 "95: Phaser");
const int CONTROL_NUMBER_VALUES[17]{-1, 1, 2, 4, 5, 71, 74, 75, 76, 77, 78, 79, 91, 92, 93, 94, 95};

enum class CONTROL_TARGET_TYPE { OSC, Filter, LFO, Master };
const juce::StringArray CONTROL_TARGET_TYPE_NAMES = juce::StringArray("OSC", "Filter", "LFO", "Misc");

const juce::StringArray CONTROL_TARGET_OSC_NAMES = juce::StringArray("1", "2", "3");
const juce::StringArray CONTROL_TARGET_FILTER_NAMES = juce::StringArray("1", "2");
const juce::StringArray CONTROL_TARGET_LFO_NAMES = juce::StringArray("1", "2", "3");
const juce::StringArray CONTROL_TARGET_MODENV_NAMES = juce::StringArray("1", "2", "3");

enum class CONTROL_TARGET_OSC_PARAM { Edge, Detune, Spread, /*Pan,*/ Gain };
const juce::StringArray CONTROL_TARGET_OSC_PARAM_NAMES =
    juce::StringArray("Edge", "Detune", "Spread", /*"Pan",*/ "Gain");

enum class CONTROL_TARGET_FILTER_PARAM { Freq, Q };
const juce::StringArray CONTROL_TARGET_FILTER_PARAM_NAMES = juce::StringArray("Freq", "Q");

enum class CONTROL_TARGET_LFO_PARAM { Freq, Amount };
const juce::StringArray CONTROL_TARGET_LFO_PARAM_NAMES = juce::StringArray("Freq", "Amount");

enum class CONTROL_TARGET_MISC_PARAM { PortamentoTime, DelayMix };
const juce::StringArray CONTROL_TARGET_MISC_PARAM_NAMES = juce::StringArray("Portamento Time", "Delay Mix");

}  // namespace
//==============================================================================
class SynthParametersBase {
public:
    virtual ~SynthParametersBase() {}
    virtual void addAllParameters(juce::AudioProcessor& processor) = 0;
    virtual void saveParameters(juce::XmlElement& xml) = 0;
    virtual void loadParameters(juce::XmlElement& xml) = 0;
};

//==============================================================================
class GlobalParams : public SynthParametersBase {
public:
    //    TODO: portamento time?, pitch bend range?, velocity sense

    juce::AudioParameterFloat* Pitch;
    juce::AudioParameterFloat* Pan;
    juce::AudioParameterFloat* Expression;
    juce::AudioParameterFloat* MasterVolume;
    juce::AudioParameterFloat* MidiVolume;

    GlobalParams();
    GlobalParams(const GlobalParams&) = delete;
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;

    void setMidiVolumeFromControl(double normalizedValue) { *MidiVolume = normalizedValue; }
    void setPanFromControl(double normalizedValue) { *Pan = Pan->range.convertFrom0to1(normalizedValue); }
    void setExpressionFromControl(double normalizedValue) { *Expression = normalizedValue; }

    float pitch;
    float pan;
    float expression;
    float masterVolume;
    float midiVolume;
    void freeze() {
        pitch = Pitch->get();
        pan = Pan->get();
        expression = Expression->get();
        masterVolume = MasterVolume->get();
        midiVolume = MidiVolume->get();
    }

private:
};

//==============================================================================
class VoiceParams : public SynthParametersBase {
public:
    juce::AudioParameterChoice* Mode;
    juce::AudioParameterFloat* PortamentoTime;
    juce::AudioParameterInt* PitchBendRange;

    VoiceParams();
    VoiceParams(const VoiceParams&) = delete;
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;

    void setPortamentoTimeFromControl(double normalizedValue) {
        *PortamentoTime = PortamentoTime->range.convertFrom0to1(normalizedValue);
    }

    bool isMonoMode;
    float portamentoTime;
    int pitchBendRange;
    void freeze() {
        isMonoMode = static_cast<VOICE_MODE>(Mode->getIndex()) == VOICE_MODE::Mono;
        portamentoTime = PortamentoTime->get();
        pitchBendRange = PitchBendRange->get();
    }

private:
};

//==============================================================================
class OscParams : public SynthParametersBase {
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

    OscParams(int index);
    OscParams(const OscParams&) = delete;
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;

    void setEdgeFromControl(double normalizedValue) { *Edge = normalizedValue; }
    void setDetuneFromControl(double normalizedValue) { *Detune = normalizedValue; }
    void setSpreadFromControl(double normalizedValue) { *Spread = normalizedValue; }
    void setGainFromControl(double normalizedValue) { *Gain = Gain->range.convertFrom0to1(normalizedValue); }

    bool enabled;
    WAVEFORM waveform;
    float edge;
    int octave;
    int coarse;
    int unison;
    float detune;
    float spread;
    float gain;
    int envelope;
    void freeze() {
        enabled = Enabled->get();
        waveform = OSC_WAVEFORM_VALUES[Waveform->getIndex()];
        edge = Edge->get();
        octave = Octave->get();
        coarse = Coarse->get();
        unison = Unison->get();
        detune = Detune->get();
        spread = Spread->get();
        gain = Gain->get();
        envelope = Envelope->getIndex();
    }

private:
    OscParams(){};
};

//==============================================================================
class EnvelopeParams : public SynthParametersBase {
public:
    juce::AudioParameterFloat* Attack;
    juce::AudioParameterFloat* Decay;
    juce::AudioParameterFloat* Sustain;
    juce::AudioParameterFloat* Release;

    EnvelopeParams(int index);
    EnvelopeParams(const EnvelopeParams&) = delete;

    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;

    float attack;
    float decay;
    float sustain;
    float release;
    void freeze() {
        attack = Attack->get();
        decay = Decay->get();
        sustain = Sustain->get();
        release = Release->get();
    }

private:
    EnvelopeParams(){};
};

//==============================================================================
class FilterParams : public SynthParametersBase {
public:
    juce::AudioParameterBool* Enabled;
    juce::AudioParameterChoice* Target;
    juce::AudioParameterChoice* Type;
    juce::AudioParameterChoice* FreqType;
    juce::AudioParameterFloat* Hz;
    juce::AudioParameterInt* Semitone;
    juce::AudioParameterFloat* Q;
    juce::AudioParameterFloat* Gain;

    FilterParams(int index);
    FilterParams(const FilterParams&) = delete;

    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool hasGain() {
        switch (static_cast<FILTER_TYPE>(Type->getIndex())) {
            case FILTER_TYPE::Peaking:
            case FILTER_TYPE::LowShelf:
            case FILTER_TYPE::HighShelf:
                return true;
            default:
                return false;
        }
    }
    bool isFreqAbsolute() { return static_cast<FILTER_FREQ_TYPE>(FreqType->getIndex()) == FILTER_FREQ_TYPE::Absolute; }

    void setHzFromControl(double normalizedValue) { *Hz = Hz->range.convertFrom0to1(normalizedValue); }
    void setSemitoneFromControl(double normalizedValue) {
        *Semitone = Semitone->getNormalisableRange().convertFrom0to1(normalizedValue);
    }
    void setQFromControl(double normalizedValue) { *Q = Q->range.convertFrom0to1(normalizedValue); }

    bool enabled;
    int target;
    FILTER_TYPE type;
    bool isFreqAbsoluteFreezed;
    float hz;
    int semitone;
    float q;
    float gain;
    void freeze() {
        enabled = Enabled->get();
        target = Target->getIndex();
        type = static_cast<FILTER_TYPE>(Type->getIndex());
        isFreqAbsoluteFreezed = isFreqAbsolute();
        hz = Hz->get();
        semitone = Semitone->get();
        q = Q->get();
        gain = Gain->get();
    }

private:
    FilterParams(){};
};

//==============================================================================
class LfoParams : public SynthParametersBase {
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

    LfoParams(int index);
    LfoParams(const LfoParams&) = delete;

    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool shouldUseFastFreq() {
        if (static_cast<LFO_TARGET_TYPE>(TargetType->getIndex()) == LFO_TARGET_TYPE::OSC) {
            auto targetOscParam = static_cast<LFO_TARGET_OSC_PARAM>(TargetOscParam->getIndex());
            switch (targetOscParam) {
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

    void setFastFreqFromControl(double normalizedValue) {
        *FastFreq = FastFreq->range.convertFrom0to1(normalizedValue);
    }
    void setSlowFreqFromControl(double normalizedValue) {
        *SlowFreq = SlowFreq->range.convertFrom0to1(normalizedValue);
    }
    void setAmountFromControl(double normalizedValue) { *Amount = normalizedValue; }

    bool enabled;
    LFO_TARGET_TYPE targetType;
    int targetOsc;
    int targetFilter;
    LFO_TARGET_OSC_PARAM targetOscParam;
    LFO_TARGET_FILTER_PARAM targetFilterParam;
    WAVEFORM waveform;
    float slowFreq;
    float fastFreq;
    float amount;
    bool shouldUseFastFreqFreezed;
    void freeze() {
        enabled = Enabled->get();
        targetType = static_cast<LFO_TARGET_TYPE>(TargetType->getIndex());
        targetOsc = TargetOsc->getIndex();
        targetFilter = TargetFilter->getIndex();
        targetOscParam = static_cast<LFO_TARGET_OSC_PARAM>(TargetOscParam->getIndex());
        targetFilterParam = static_cast<LFO_TARGET_FILTER_PARAM>(TargetFilterParam->getIndex());
        waveform = LFO_WAVEFORM_VALUES[Waveform->getIndex()];
        slowFreq = SlowFreq->get();
        fastFreq = FastFreq->get();
        amount = Amount->get();
        shouldUseFastFreqFreezed = shouldUseFastFreq();
    }

private:
    LfoParams(){};
};

//==============================================================================
class ModEnvParams : public SynthParametersBase {
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

    ModEnvParams(int index);
    ModEnvParams(const ModEnvParams&) = delete;

    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;
    bool isTargetFreq() {
        auto targetType = static_cast<MODENV_TARGET_TYPE>(TargetType->getIndex());
        return (targetType == MODENV_TARGET_TYPE::OSC &&
                static_cast<MODENV_TARGET_OSC_PARAM>(TargetOscParam->getIndex()) == MODENV_TARGET_OSC_PARAM::Freq) ||
               (targetType == MODENV_TARGET_TYPE::Filter &&
                static_cast<MODENV_TARGET_FILTER_PARAM>(TargetFilterParam->getIndex()) ==
                    MODENV_TARGET_FILTER_PARAM::Freq) ||
               (targetType == MODENV_TARGET_TYPE::LFO &&
                static_cast<MODENV_TARGET_LFO_PARAM>(TargetLfoParam->getIndex()) == MODENV_TARGET_LFO_PARAM::Freq);
    }
    bool shouldUseHold() { return !isTargetFreq() && isFadeIn(); }
    bool isFadeIn() { return static_cast<MODENV_FADE>(Fade->getIndex()) == MODENV_FADE::In; }

    bool enabled;
    MODENV_TARGET_TYPE targetType;
    int targetOsc;
    int targetFilter;
    int targetLfo;
    MODENV_TARGET_OSC_PARAM targetOscParam;
    MODENV_TARGET_FILTER_PARAM targetFilterParam;
    MODENV_TARGET_LFO_PARAM targetLfoParam;
    bool fadeIn;
    float peakFreq;
    float wait;
    float attack;
    float decay;
    void freeze() {
        enabled = Enabled->get();
        targetType = static_cast<MODENV_TARGET_TYPE>(TargetType->getIndex());
        targetOsc = TargetOsc->getIndex();
        targetFilter = TargetFilter->getIndex();
        targetLfo = TargetLfo->getIndex();
        targetOscParam = static_cast<MODENV_TARGET_OSC_PARAM>(TargetOscParam->getIndex());
        targetFilterParam = static_cast<MODENV_TARGET_FILTER_PARAM>(TargetFilterParam->getIndex());
        targetLfoParam = static_cast<MODENV_TARGET_LFO_PARAM>(TargetLfoParam->getIndex());
        fadeIn = isFadeIn();
        peakFreq = PeakFreq->get();
        wait = Wait->get();
        attack = Attack->get();
        decay = Decay->get();
    }

private:
    ModEnvParams(){};
};

//==============================================================================
class DelayParams : public SynthParametersBase {
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
    DelayParams();
    DelayParams(const DelayParams&) = delete;
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;

    void setMixFromControl(double normalizedValue) { *Mix = normalizedValue; }

    bool enabled;
    DELAY_TYPE type;
    bool sync;
    float timeL;
    float timeR;
    double timeSyncL;
    double timeSyncR;
    float lowFreq;
    float highFreq;
    float feedback;
    float mix;
    void freeze() {
        enabled = Enabled->get();
        type = static_cast<DELAY_TYPE>(Type->getIndex());
        sync = Sync->get();
        timeL = TimeL->get();
        timeR = TimeR->get();
        timeSyncL = DELAY_TIME_SYNC_VALUES[TimeSyncL->getIndex()];
        timeSyncR = DELAY_TIME_SYNC_VALUES[TimeSyncR->getIndex()];
        lowFreq = LowFreq->get();
        highFreq = HighFreq->get();
        feedback = Feedback->get();
        mix = Mix->get();
    }

private:
};

//==============================================================================
class ControlItemParams : public SynthParametersBase {
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
    ControlItemParams(int index);
    ControlItemParams(const ControlItemParams&) = delete;
    virtual void addAllParameters(juce::AudioProcessor& processor) override;
    virtual void saveParameters(juce::XmlElement& xml) override;
    virtual void loadParameters(juce::XmlElement& xml) override;

    int number;
    CONTROL_TARGET_TYPE targetType;
    int targetOsc;
    int targetFilter;
    int targetLfo;
    CONTROL_TARGET_OSC_PARAM targetOscParam;
    CONTROL_TARGET_FILTER_PARAM targetFilterParam;
    CONTROL_TARGET_LFO_PARAM targetLfoParam;
    CONTROL_TARGET_MISC_PARAM targetMiscParam;
    void freeze() {
        number = CONTROL_NUMBER_VALUES[Number->getIndex()];
        targetType = static_cast<CONTROL_TARGET_TYPE>(TargetType->getIndex());
        targetOsc = TargetOsc->getIndex();
        targetFilter = TargetFilter->getIndex();
        targetLfo = TargetLfo->getIndex();
        targetOscParam = static_cast<CONTROL_TARGET_OSC_PARAM>(TargetOscParam->getIndex());
        targetFilterParam = static_cast<CONTROL_TARGET_FILTER_PARAM>(TargetFilterParam->getIndex());
        targetLfoParam = static_cast<CONTROL_TARGET_LFO_PARAM>(TargetLfoParam->getIndex());
        targetMiscParam = static_cast<CONTROL_TARGET_MISC_PARAM>(TargetMiscParam->getIndex());
    }

private:
    ControlItemParams(){};
};
