#include "Params.h"

#include "Voice.h"

//==============================================================================
GlobalParams::GlobalParams() {
    std::string idPrefix = "GLOBAL_";
    std::string namePrefix = "Global ";
    Pitch = new juce::AudioParameterFloat(idPrefix + "PITCH", namePrefix + "Pitch", -1.0f, 1.0f, 0.0f);
    Pan = new juce::AudioParameterFloat(idPrefix + "PAN", namePrefix + "Pan", -1.0f, 1.0f, 0.0f);
    Expression = new juce::AudioParameterFloat(idPrefix + "EXPRESSION", namePrefix + "Expression", 0.0f, 1.0f, 1.0f);
    MasterVolume =
        new juce::AudioParameterFloat(idPrefix + "MASTER_VOLUME", namePrefix + "Master Volume", 0.0f, 1.0f, 1.0f);
}
void GlobalParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Pitch);
    processor.addParameter(Pan);
    processor.addParameter(Expression);
    processor.addParameter(MasterVolume);
}
void GlobalParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Pitch->paramID, (double)Pitch->get());
    xml.setAttribute(Pan->paramID, (double)Pan->get());
    xml.setAttribute(Expression->paramID, (double)Expression->get());
    xml.setAttribute(MasterVolume->paramID, (double)MasterVolume->get());
}
void GlobalParams::loadParameters(juce::XmlElement& xml) {
    *Pitch = (float)xml.getDoubleAttribute(Pitch->paramID, 0);
    *Pan = (float)xml.getDoubleAttribute(Pan->paramID, 0);
    *Expression = (float)xml.getDoubleAttribute(Expression->paramID, 1.0);
    *MasterVolume = (float)xml.getDoubleAttribute(MasterVolume->paramID, 1.0);
}

//==============================================================================
VoiceParams::VoiceParams() {
    std::string idPrefix = "VOICE_";
    std::string namePrefix = "Voice ";
    Mode = new juce::AudioParameterChoice(
        idPrefix + "MODE", namePrefix + "Mode", VOICE_MODE_NAMES, VOICE_MODE_NAMES.indexOf("Poly"));
    {
        auto range = juce::NormalisableRange(0.001f, 1.0f, 0.001f);
        range.setSkewForCentre(0.1f);
        PortamentoTime =
            new juce::AudioParameterFloat(idPrefix + "PORTAMENTO_TIME", namePrefix + "Portamento Time", range, 0.02f);
    }
    PitchBendRange =
        new juce::AudioParameterInt(idPrefix + "PITCH_BEND_RANGE", namePrefix + "Pitch-Bend Range", 1, 12, 2);
}
void VoiceParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Mode);
    processor.addParameter(PortamentoTime);
    processor.addParameter(PitchBendRange);
}
void VoiceParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Mode->paramID, Mode->getIndex());
    xml.setAttribute(PortamentoTime->paramID, PortamentoTime->get());
    xml.setAttribute(PitchBendRange->paramID, PitchBendRange->get());
}
void VoiceParams::loadParameters(juce::XmlElement& xml) {
    *Mode = xml.getIntAttribute(Mode->paramID, 0);
    *PortamentoTime = xml.getDoubleAttribute(PortamentoTime->paramID, 0.001);
    *PitchBendRange = xml.getIntAttribute(PitchBendRange->paramID, 2);
}

//==============================================================================
OscParams::OscParams(int index) {
    auto idPrefix = "OSC" + std::to_string(index) + "_";
    auto namePrefix = "OSC" + std::to_string(index) + " ";
    Enabled = new juce::AudioParameterBool(idPrefix + "ENABLED", namePrefix + "Enabled", false);
    Waveform = new juce::AudioParameterChoice(
        idPrefix + "WAVEFORM", namePrefix + "Waveform", OSC_WAVEFORM_NAMES, OSC_WAVEFORM_NAMES.indexOf("Sine"));
    Edge = new juce::AudioParameterFloat(idPrefix + "EDGE", namePrefix + "Edge", 0.0f, 1.0f, 0.0f);
    Octave = new juce::AudioParameterInt(idPrefix + "OCTAVE", namePrefix + "Octave", -2, 2, 0);
    Coarse = new juce::AudioParameterInt(idPrefix + "COARSE", namePrefix + "Coarse", -12, 12, 0);
    Unison = new juce::AudioParameterInt(idPrefix + "UNISON", namePrefix + "Unison", 1, 4, 1);
    Detune = new juce::AudioParameterFloat(idPrefix + "DETUNE", namePrefix + "Detune", 0.0f, 1.0f, 0.0f);
    Spread = new juce::AudioParameterFloat(idPrefix + "SPREAD", namePrefix + "Spread", 0.0f, 1.0f, 0.0f);
    {
        auto range = juce::NormalisableRange(0.0f, 4.0f, 0.01f);
        range.setSkewForCentre(1.0f);
        Gain = new juce::AudioParameterFloat(idPrefix + "GAIN", namePrefix + "Gain", range, 1.0f);
    }
    Envelope = new juce::AudioParameterChoice(
        idPrefix + "ENVELOPE", namePrefix + "Envelope", OSC_ENV_NAMES, OSC_ENV_NAMES.indexOf("1"));
}
void OscParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Enabled);
    processor.addParameter(Waveform);
    processor.addParameter(Edge);
    processor.addParameter(Octave);
    processor.addParameter(Coarse);
    processor.addParameter(Unison);
    processor.addParameter(Detune);
    processor.addParameter(Spread);
    processor.addParameter(Gain);
    processor.addParameter(Envelope);
}
void OscParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Enabled->paramID, Enabled->get());
    xml.setAttribute(Waveform->paramID, Waveform->getIndex());
    xml.setAttribute(Edge->paramID, Edge->get());
    xml.setAttribute(Octave->paramID, Octave->get());
    xml.setAttribute(Coarse->paramID, Coarse->get());
    xml.setAttribute(Unison->paramID, Unison->get());
    xml.setAttribute(Detune->paramID, (double)Detune->get());
    xml.setAttribute(Spread->paramID, (double)Spread->get());
    xml.setAttribute(Gain->paramID, (double)Gain->get());
    xml.setAttribute(Envelope->paramID, Envelope->getIndex());
}
void OscParams::loadParameters(juce::XmlElement& xml) {
    *Enabled = xml.getIntAttribute(Enabled->paramID, 0);
    *Waveform = xml.getIntAttribute(Waveform->paramID, 0);
    *Edge = xml.getIntAttribute(Edge->paramID, 0.0);
    *Octave = xml.getIntAttribute(Octave->paramID, 0);
    *Coarse = xml.getIntAttribute(Coarse->paramID, 0);
    *Unison = xml.getIntAttribute(Unison->paramID, 1);
    *Detune = (float)xml.getDoubleAttribute(Detune->paramID, 0);
    *Spread = (float)xml.getDoubleAttribute(Spread->paramID, 0);
    *Gain = (float)xml.getDoubleAttribute(Gain->paramID, 0);
    *Envelope = xml.getIntAttribute(Envelope->paramID, 0);
}

//==============================================================================
EnvelopeParams::EnvelopeParams(int index) {
    auto idPrefix = "ENV" + std::to_string(index) + "_";
    auto namePrefix = "Env" + std::to_string(index) + " ";
    Attack = new juce::AudioParameterFloat(idPrefix + "ATTACK", "Attack", {0.001f, 1.0f, 0.001f}, 0.05f);
    Decay = new juce::AudioParameterFloat(idPrefix + "DECAY", "Decay", 0.01f, 1.0f, 0.1f);
    Sustain = new juce::AudioParameterFloat(idPrefix + "SUSTAIN", "Sustain", 0.0f, 1.0f, 0.7f);
    Release = new juce::AudioParameterFloat(idPrefix + "RELEASE", "Release", 0.01f, 1.0f, 0.1f);
}
void EnvelopeParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Attack);
    processor.addParameter(Decay);
    processor.addParameter(Sustain);
    processor.addParameter(Release);
}
void EnvelopeParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Attack->paramID, (double)Attack->get());
    xml.setAttribute(Decay->paramID, (double)Decay->get());
    xml.setAttribute(Sustain->paramID, (double)Sustain->get());
    xml.setAttribute(Release->paramID, (double)Release->get());
}
void EnvelopeParams::loadParameters(juce::XmlElement& xml) {
    *Attack = (float)xml.getDoubleAttribute(Attack->paramID, 0.01);
    *Decay = (float)xml.getDoubleAttribute(Decay->paramID, 0.01);
    *Sustain = (float)xml.getDoubleAttribute(Sustain->paramID, 1.0);
    *Release = (float)xml.getDoubleAttribute(Release->paramID, 0.01);
}

//==============================================================================
FilterParams::FilterParams(int index) {
    auto idPrefix = "FILTER" + std::to_string(index) + "_";
    auto namePrefix = "Filter" + std::to_string(index) + " ";
    Enabled = new juce::AudioParameterBool(idPrefix + "ENABLED", namePrefix + "Enabled", false);
    Target = new juce::AudioParameterChoice(
        idPrefix + "TARGET", namePrefix + "Target", FILTER_TARGET_NAMES, FILTER_TARGET_NAMES.indexOf("All"));
    Type = new juce::AudioParameterChoice(
        idPrefix + "TYPE", namePrefix + "Type", FILTER_TYPE_NAMES, FILTER_TYPE_NAMES.indexOf("Lowpass"));
    FreqType = new juce::AudioParameterChoice(idPrefix + "FREQ_TYPE",
                                              namePrefix + "Freq Type",
                                              FILTER_FREQ_TYPE_NAMES,
                                              FILTER_FREQ_TYPE_NAMES.indexOf("Abs"));
    {
        auto range = juce::NormalisableRange(30.0f, 20000.0f, 0.01f);
        range.setSkewForCentre(2000.0f);
        Hz = new juce::AudioParameterFloat(idPrefix + "HZ", namePrefix + "Hz", range, 4000.0f);
    }
    Semitone = new juce::AudioParameterInt(idPrefix + "SEMITONE", namePrefix + "Semitone", -48, 48, 0);
    Q = new juce::AudioParameterFloat(idPrefix + "Q", namePrefix + "Q", 0.01f, 100.0f, 1.0f);
    Gain = new juce::AudioParameterFloat(idPrefix + "GAIN", namePrefix + "Gain", -20.0f, 20.0f, 0.0f);
}
void FilterParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Enabled);
    processor.addParameter(Target);
    processor.addParameter(Type);
    processor.addParameter(FreqType);
    processor.addParameter(Hz);
    processor.addParameter(Semitone);
    processor.addParameter(Q);
    processor.addParameter(Gain);
}
void FilterParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Enabled->paramID, Enabled->get());
    xml.setAttribute(Target->paramID, Target->getIndex());
    xml.setAttribute(Type->paramID, Type->getIndex());
    xml.setAttribute(FreqType->paramID, FreqType->getIndex());
    xml.setAttribute(Hz->paramID, (double)Hz->get());
    xml.setAttribute(Semitone->paramID, Semitone->get());
    xml.setAttribute(Q->paramID, (double)Q->get());
    xml.setAttribute(Gain->paramID, (double)Gain->get());
}
void FilterParams::loadParameters(juce::XmlElement& xml) {
    *Enabled = xml.getIntAttribute(Enabled->paramID, 0);
    *Target = xml.getIntAttribute(Target->paramID, NUM_OSC);
    *Type = xml.getIntAttribute(Type->paramID, 0);
    *FreqType = xml.getIntAttribute(FreqType->paramID, 0);
    *Hz = (float)xml.getDoubleAttribute(Hz->paramID, 0);
    *Semitone = xml.getDoubleAttribute(Semitone->paramID, 0);
    *Q = (float)xml.getDoubleAttribute(Q->paramID, 1.0);
    *Gain = (float)xml.getDoubleAttribute(Gain->paramID, 0);
}

//==============================================================================
LfoParams::LfoParams(int index) {
    auto idPrefix = "LFO" + std::to_string(index) + "_";
    auto namePrefix = "LFO" + std::to_string(index) + " ";
    Enabled = new juce::AudioParameterBool(idPrefix + "ENABLED", namePrefix + "Enabled", false);
    TargetType = new juce::AudioParameterChoice(idPrefix + "TARGET_TYPE",
                                                namePrefix + "Target Type",
                                                LFO_TARGET_TYPE_NAMES,
                                                LFO_TARGET_TYPE_NAMES.indexOf("OSC"));
    TargetOsc = new juce::AudioParameterChoice(
        idPrefix + "TARGET_OSC", namePrefix + "Target OSC", LFO_TARGET_OSC_NAMES, LFO_TARGET_OSC_NAMES.indexOf("All"));
    TargetFilter = new juce::AudioParameterChoice(idPrefix + "TARGET_FILTER",
                                                  namePrefix + "Target Filter",
                                                  LFO_TARGET_FILTER_NAMES,
                                                  LFO_TARGET_FILTER_NAMES.indexOf("All"));
    TargetOscParam = new juce::AudioParameterChoice(idPrefix + "TARGET_OSC_PARAM",
                                                    namePrefix + "Target OSC Param",
                                                    LFO_TARGET_OSC_PARAM_NAMES,
                                                    LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato"));
    TargetFilterParam = new juce::AudioParameterChoice(idPrefix + "TARGET_FILTER_PARAM",
                                                       namePrefix + "Target Filter Param",
                                                       LFO_TARGET_FILTER_PARAM_NAMES,
                                                       LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq"));
    Waveform = new juce::AudioParameterChoice(
        idPrefix + "WAVEFORM", namePrefix + "Waveform", LFO_WAVEFORM_NAMES, LFO_WAVEFORM_NAMES.indexOf("Sine"));
    {
        auto range = juce::NormalisableRange(0.0f, 100.0f, 0.01f);
        range.setSkewForCentre(4.0f);
        SlowFreq = new juce::AudioParameterFloat(idPrefix + "SLOW_FREQ", namePrefix + "Slow Freq", range, 4.0f);
    }
    {
        auto range = juce::NormalisableRange(0.01f, 100.0f, 0.01f);
        range.setSkewForCentre(1.0f);
        FastFreq = new juce::AudioParameterFloat(idPrefix + "FAST_FREQ", namePrefix + "Fast Freq", range, 1.0f);
    }
    Amount = new juce::AudioParameterFloat(idPrefix + "AMOUNT", namePrefix + "Amount", 0.0f, 1.0f, 0.2f);
}
void LfoParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Enabled);
    processor.addParameter(TargetType);
    processor.addParameter(TargetOsc);
    processor.addParameter(TargetFilter);
    processor.addParameter(TargetOscParam);
    processor.addParameter(TargetFilterParam);
    processor.addParameter(Waveform);
    processor.addParameter(SlowFreq);
    processor.addParameter(FastFreq);
    processor.addParameter(Amount);
}
void LfoParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Enabled->paramID, Enabled->get());
    xml.setAttribute(TargetType->paramID, TargetType->getIndex());
    xml.setAttribute(TargetOsc->paramID, TargetOsc->getIndex());
    xml.setAttribute(TargetFilter->paramID, TargetFilter->getIndex());
    xml.setAttribute(TargetOscParam->paramID, TargetOscParam->getIndex());
    xml.setAttribute(TargetFilterParam->paramID, TargetFilterParam->getIndex());
    xml.setAttribute(SlowFreq->paramID, (double)SlowFreq->get());
    xml.setAttribute(FastFreq->paramID, (double)FastFreq->get());
    xml.setAttribute(Amount->paramID, (double)Amount->get());
}
void LfoParams::loadParameters(juce::XmlElement& xml) {
    *Enabled = xml.getIntAttribute(Enabled->paramID, 0);
    *TargetType = xml.getIntAttribute(TargetType->paramID, 0);
    *TargetOsc = xml.getIntAttribute(TargetOsc->paramID, NUM_OSC);
    *TargetFilter = xml.getIntAttribute(TargetOsc->paramID, NUM_FILTER);
    *TargetOscParam = xml.getIntAttribute(TargetOscParam->paramID, 0);
    *TargetFilterParam = xml.getIntAttribute(TargetFilterParam->paramID, 0);
    *SlowFreq = (float)xml.getDoubleAttribute(SlowFreq->paramID, 0);
    *FastFreq = (float)xml.getDoubleAttribute(FastFreq->paramID, 0);
    *Amount = (float)xml.getDoubleAttribute(Amount->paramID, 1.0);
}

//==============================================================================
ModEnvParams::ModEnvParams(int index) {
    auto idPrefix = "MODENV" + std::to_string(index) + "_";
    auto namePrefix = "ModEnv" + std::to_string(index) + " ";
    Enabled = new juce::AudioParameterBool(idPrefix + "ENABLED", namePrefix + "Enabled", false);
    TargetType = new juce::AudioParameterChoice(idPrefix + "TARGET_TYPE",
                                                namePrefix + "Target Type",
                                                MODENV_TARGET_TYPE_NAMES,
                                                MODENV_TARGET_TYPE_NAMES.indexOf("LFO"));
    TargetOsc = new juce::AudioParameterChoice(idPrefix + "TARGET_OSC",
                                               namePrefix + "Target OSC",
                                               MODENV_TARGET_OSC_NAMES,
                                               MODENV_TARGET_OSC_NAMES.indexOf("All"));
    TargetFilter = new juce::AudioParameterChoice(idPrefix + "TARGET_FILTER",
                                                  namePrefix + "Target Filter",
                                                  MODENV_TARGET_FILTER_NAMES,
                                                  MODENV_TARGET_FILTER_NAMES.indexOf("All"));
    TargetLfo = new juce::AudioParameterChoice(idPrefix + "TARGET_LFO",
                                               namePrefix + "Target LFO",
                                               MODENV_TARGET_LFO_NAMES,
                                               MODENV_TARGET_LFO_NAMES.indexOf("All"));
    TargetOscParam = new juce::AudioParameterChoice(idPrefix + "TARGET_OSC_PARAM",
                                                    namePrefix + "Target OSC Param",
                                                    MODENV_TARGET_OSC_PARAM_NAMES,
                                                    MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq"));
    TargetFilterParam = new juce::AudioParameterChoice(idPrefix + "TARGET_FILTER_PARAM",
                                                       namePrefix + "Target Filter Param",
                                                       MODENV_TARGET_FILTER_PARAM_NAMES,
                                                       MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq"));
    TargetLfoParam = new juce::AudioParameterChoice(idPrefix + "TARGET_LFO_PARAM",
                                                    namePrefix + "Target LFO Param",
                                                    MODENV_TARGET_LFO_PARAM_NAMES,
                                                    MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Amount"));
    Fade = new juce::AudioParameterChoice(
        idPrefix + "FADE", namePrefix + "Fade", MODENV_FADE_NAMES, MODENV_FADE_NAMES.indexOf("In"));
    PeakFreq = new juce::AudioParameterFloat(idPrefix + "PEAK_FREQ", namePrefix + "Peak Freq", -8.0f, 8.0, 2.0f);
    Wait = new juce::AudioParameterFloat(idPrefix + "WAIT", namePrefix + "Wait", 0.0f, 1.0f, 0.5f);
    Attack = new juce::AudioParameterFloat(idPrefix + "ATTACK", namePrefix + "Attack", {0.0f, 1.0f, 0.001f}, 0.0f);
    Decay = new juce::AudioParameterFloat(idPrefix + "DECAY", namePrefix + "Decay", 0.0f, 1.0f, 0.2f);
}
void ModEnvParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Enabled);
    processor.addParameter(TargetType);
    processor.addParameter(TargetOsc);
    processor.addParameter(TargetFilter);
    processor.addParameter(TargetLfo);
    processor.addParameter(TargetOscParam);
    processor.addParameter(TargetFilterParam);
    processor.addParameter(TargetLfoParam);
    processor.addParameter(Fade);
    processor.addParameter(PeakFreq);
    processor.addParameter(Wait);
    processor.addParameter(Attack);
    processor.addParameter(Decay);
}
void ModEnvParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Enabled->paramID, Enabled->get());
    xml.setAttribute(TargetType->paramID, TargetType->getIndex());
    xml.setAttribute(TargetOsc->paramID, TargetOsc->getIndex());
    xml.setAttribute(TargetFilter->paramID, TargetFilter->getIndex());
    xml.setAttribute(TargetLfo->paramID, TargetLfo->getIndex());
    xml.setAttribute(TargetOscParam->paramID, TargetOscParam->getIndex());
    xml.setAttribute(TargetFilterParam->paramID, TargetFilterParam->getIndex());
    xml.setAttribute(TargetLfoParam->paramID, TargetLfoParam->getIndex());
    xml.setAttribute(Fade->paramID, Fade->getIndex());
    xml.setAttribute(PeakFreq->paramID, (double)PeakFreq->get());
    xml.setAttribute(Wait->paramID, (double)Wait->get());
    xml.setAttribute(Attack->paramID, (double)Attack->get());
    xml.setAttribute(Decay->paramID, (double)Decay->get());
}
void ModEnvParams::loadParameters(juce::XmlElement& xml) {
    *Enabled = xml.getIntAttribute(Enabled->paramID, 0);
    *TargetType = xml.getIntAttribute(TargetType->paramID, 0);
    *TargetOsc = xml.getIntAttribute(TargetOsc->paramID, NUM_OSC);
    *TargetFilter = xml.getIntAttribute(TargetOsc->paramID, NUM_FILTER);
    *TargetLfo = xml.getIntAttribute(TargetOsc->paramID, NUM_LFO);
    *TargetOscParam = xml.getIntAttribute(TargetOscParam->paramID, 0);
    *TargetFilterParam = xml.getIntAttribute(TargetFilterParam->paramID, 0);
    *TargetLfoParam = xml.getIntAttribute(TargetLfoParam->paramID, 0);
    *Fade = xml.getIntAttribute(Fade->paramID, 0);
    *PeakFreq = (float)xml.getDoubleAttribute(PeakFreq->paramID, 0);
    *Wait = (float)xml.getDoubleAttribute(Wait->paramID, 0);
    *Attack = (float)xml.getDoubleAttribute(Attack->paramID, 0.01);
    *Decay = (float)xml.getDoubleAttribute(Decay->paramID, 0.1);
}

//==============================================================================
DelayParams::DelayParams() {
    std::string idPrefix = "DELAY_";
    std::string namePrefix = "Delay ";
    Enabled = new juce::AudioParameterBool(idPrefix + "ENABLED", namePrefix + "Enabled", false);
    Type = new juce::AudioParameterChoice(
        idPrefix + "TYPE", namePrefix + "Type", DELAY_TYPE_NAMES, DELAY_TYPE_NAMES.indexOf("Parallel"));
    Sync = new juce::AudioParameterBool(idPrefix + "SYNC", namePrefix + "Sync", false);
    TimeL = new juce::AudioParameterFloat(idPrefix + "TIME_L", namePrefix + "TimeL", 0.01f, 1.0f, 0.3f);
    TimeR = new juce::AudioParameterFloat(idPrefix + "TIME_R", namePrefix + "TimeR", 0.01f, 1.0f, 0.4f);
    TimeSyncL = new juce::AudioParameterChoice(idPrefix + "TIME_SYNC_L",
                                               namePrefix + "TimeSyncL",
                                               DELAY_TIME_SYNC_NAMES,
                                               DELAY_TIME_SYNC_NAMES.indexOf("1/8"));
    TimeSyncR = new juce::AudioParameterChoice(idPrefix + "TIME_SYNC_R",
                                               namePrefix + "TimeSyncR",
                                               DELAY_TIME_SYNC_NAMES,
                                               DELAY_TIME_SYNC_NAMES.indexOf("1/8"));
    LowFreq = new juce::AudioParameterFloat(idPrefix + "LOW_FREQ", namePrefix + "LowFfreq", 10.0f, 20000.0f, 10.0f);
    HighFreq =
        new juce::AudioParameterFloat(idPrefix + "HIGH_FREQ", namePrefix + "HighFreq", 10.0f, 20000.0f, 20000.0f);
    Feedback = new juce::AudioParameterFloat(idPrefix + "FEEDBACK", namePrefix + "Feedback", 0.0f, 1.0f, 0.3f);
    Mix = new juce::AudioParameterFloat(idPrefix + "MIX", namePrefix + "Mix", 0.0f, 1.0f, 0.3f);
}
void DelayParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Enabled);
    processor.addParameter(Type);
    processor.addParameter(Sync);
    processor.addParameter(TimeL);
    processor.addParameter(TimeR);
    processor.addParameter(TimeSyncL);
    processor.addParameter(TimeSyncR);
    processor.addParameter(LowFreq);
    processor.addParameter(HighFreq);
    processor.addParameter(Feedback);
    processor.addParameter(Mix);
}
void DelayParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Enabled->paramID, Enabled->get());
    xml.setAttribute(Type->paramID, Type->getIndex());
    xml.setAttribute(Sync->paramID, Sync->get());
    xml.setAttribute(TimeL->paramID, (double)TimeL->get());
    xml.setAttribute(TimeR->paramID, (double)TimeR->get());
    xml.setAttribute(TimeSyncL->paramID, TimeSyncL->getIndex());
    xml.setAttribute(TimeSyncR->paramID, TimeSyncR->getIndex());
    xml.setAttribute(LowFreq->paramID, (double)LowFreq->get());
    xml.setAttribute(HighFreq->paramID, (double)HighFreq->get());
    xml.setAttribute(Feedback->paramID, (double)Feedback->get());
    xml.setAttribute(Mix->paramID, (double)Mix->get());
}
void DelayParams::loadParameters(juce::XmlElement& xml) {
    *Enabled = xml.getBoolAttribute(Enabled->paramID, false);
    *Type = xml.getIntAttribute(Type->paramID, 0);
    *Sync = xml.getBoolAttribute(Sync->paramID, false);
    *TimeL = (float)xml.getDoubleAttribute(TimeL->paramID, 0.01);
    *TimeR = (float)xml.getDoubleAttribute(TimeR->paramID, 0.01);
    *TimeSyncL = xml.getIntAttribute(TimeSyncL->paramID, 0);
    *TimeSyncR = xml.getIntAttribute(TimeSyncR->paramID, 0);
    *LowFreq = (float)xml.getDoubleAttribute(LowFreq->paramID, 10);
    *HighFreq = (float)xml.getDoubleAttribute(HighFreq->paramID, 20000);
    *Feedback = (float)xml.getDoubleAttribute(Feedback->paramID, 0);
    *Mix = (float)xml.getDoubleAttribute(Mix->paramID, 0);
}

//==============================================================================
ControlItemParams::ControlItemParams(int index) {
    auto idPrefix = "CONTROL" + std::to_string(index) + "_";
    auto namePrefix = "Control" + std::to_string(index) + " ";
    Number = new juce::AudioParameterChoice(
        idPrefix + "NUMBER", namePrefix + "Number", CONTROL_NUMBER_NAMES, CONTROL_NUMBER_NAMES.indexOf("None"));
    TargetType = new juce::AudioParameterChoice(idPrefix + "TARGET_TYPE",
                                                namePrefix + "TargetType",
                                                CONTROL_TARGET_TYPE_NAMES,
                                                CONTROL_TARGET_TYPE_NAMES.indexOf("OSC"));
    TargetOsc = new juce::AudioParameterChoice(idPrefix + "TARGET_OSC",
                                               namePrefix + "TargetOsc",
                                               CONTROL_TARGET_OSC_NAMES,
                                               CONTROL_TARGET_OSC_NAMES.indexOf("1"));
    TargetFilter = new juce::AudioParameterChoice(idPrefix + "TARGET_FILTER",
                                                  namePrefix + "TargetFilter",
                                                  CONTROL_TARGET_FILTER_NAMES,
                                                  CONTROL_TARGET_FILTER_NAMES.indexOf("1"));
    TargetLfo = new juce::AudioParameterChoice(idPrefix + "TARGET_LFO",
                                               namePrefix + "TargetLfo",
                                               CONTROL_TARGET_LFO_NAMES,
                                               CONTROL_TARGET_LFO_NAMES.indexOf("1"));
    TargetOscParam = new juce::AudioParameterChoice(idPrefix + "TARGET_OSC_PARAM",
                                                    namePrefix + "TargetOscParam",
                                                    CONTROL_TARGET_OSC_PARAM_NAMES,
                                                    CONTROL_TARGET_OSC_PARAM_NAMES.indexOf("Gain"));
    TargetFilterParam = new juce::AudioParameterChoice(idPrefix + "TARGET_FILTER_PARAM",
                                                       namePrefix + "TargetFilterParam",
                                                       CONTROL_TARGET_FILTER_PARAM_NAMES,
                                                       CONTROL_TARGET_FILTER_PARAM_NAMES.indexOf("Freq"));
    TargetLfoParam = new juce::AudioParameterChoice(idPrefix + "TARGET_LFO_PARAM",
                                                    namePrefix + "TargetLfoParam",
                                                    CONTROL_TARGET_LFO_PARAM_NAMES,
                                                    CONTROL_TARGET_LFO_PARAM_NAMES.indexOf("Amount"));
    TargetMiscParam = new juce::AudioParameterChoice(idPrefix + "TARGET_MISC_PARAM",
                                                     namePrefix + "TargetMiscParam",
                                                     CONTROL_TARGET_MISC_PARAM_NAMES,
                                                     CONTROL_TARGET_MISC_PARAM_NAMES.indexOf("Master Volume"));
}
void ControlItemParams::addAllParameters(juce::AudioProcessor& processor) {
    processor.addParameter(Number);
    processor.addParameter(TargetType);
    processor.addParameter(TargetOsc);
    processor.addParameter(TargetFilter);
    processor.addParameter(TargetLfo);
    processor.addParameter(TargetOscParam);
    processor.addParameter(TargetFilterParam);
    processor.addParameter(TargetLfoParam);
    processor.addParameter(TargetMiscParam);
}
void ControlItemParams::saveParameters(juce::XmlElement& xml) {
    xml.setAttribute(Number->paramID, Number->getIndex());
    xml.setAttribute(TargetType->paramID, TargetType->getIndex());
    xml.setAttribute(TargetOsc->paramID, TargetOsc->getIndex());
    xml.setAttribute(TargetFilter->paramID, TargetFilter->getIndex());
    xml.setAttribute(TargetLfo->paramID, TargetLfo->getIndex());
    xml.setAttribute(TargetOscParam->paramID, TargetOscParam->getIndex());
    xml.setAttribute(TargetFilterParam->paramID, TargetFilterParam->getIndex());
    xml.setAttribute(TargetLfoParam->paramID, TargetLfoParam->getIndex());
    xml.setAttribute(TargetMiscParam->paramID, TargetMiscParam->getIndex());
}
void ControlItemParams::loadParameters(juce::XmlElement& xml) {
    *Number = xml.getIntAttribute(Number->paramID, 0);
    *TargetType = xml.getIntAttribute(TargetType->paramID, 0);
    *TargetOsc = xml.getIntAttribute(TargetOsc->paramID, NUM_OSC);
    *TargetFilter = xml.getIntAttribute(TargetOsc->paramID, NUM_FILTER);
    *TargetLfo = xml.getIntAttribute(TargetOsc->paramID, NUM_LFO);
    *TargetOscParam = xml.getIntAttribute(TargetOscParam->paramID, 0);
    *TargetFilterParam = xml.getIntAttribute(TargetFilterParam->paramID, 0);
    *TargetLfoParam = xml.getIntAttribute(TargetLfoParam->paramID, 0);
    *TargetMiscParam = xml.getIntAttribute(TargetMiscParam->paramID, 0);
}
