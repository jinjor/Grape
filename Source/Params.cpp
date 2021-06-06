#include "Params.h"
#include "Voice.h"

//==============================================================================
VoiceParams::VoiceParams(juce::AudioParameterChoice* mode,
                         juce::AudioParameterFloat* portamentoTime,
                         juce::AudioParameterInt* pitchBendRange)
: Mode(mode)
, PortamentoTime(portamentoTime)
, PitchBendRange(pitchBendRange)
{}
void VoiceParams::addAllParameters(juce::AudioProcessor& processor)
{
    processor.addParameter(Mode);
    processor.addParameter(PortamentoTime);
    processor.addParameter(PitchBendRange);
}
void VoiceParams::saveParameters(juce::XmlElement& xml)
{
    xml.setAttribute(Mode->paramID, Mode->getIndex());
    xml.setAttribute(PortamentoTime->paramID, PortamentoTime->get());
    xml.setAttribute(PitchBendRange->paramID, PitchBendRange->get());
}
void VoiceParams::loadParameters(juce::XmlElement& xml)
{
    *Mode = xml.getIntAttribute(Mode->paramID, 0);
    *PortamentoTime = xml.getIntAttribute(PortamentoTime->paramID, 0.001);
    *PitchBendRange = xml.getIntAttribute(PitchBendRange->paramID, 2);
}

//==============================================================================
OscParams::OscParams(juce::AudioParameterBool* enabled,
                     juce::AudioParameterChoice* waveform,
                     juce::AudioParameterFloat* edge,
                     juce::AudioParameterInt* octave,
                     juce::AudioParameterInt* coarse,
                     juce::AudioParameterInt* unison,
                     juce::AudioParameterFloat* detune,
                     juce::AudioParameterFloat* spread,
                     juce::AudioParameterFloat* gain,
                     juce::AudioParameterChoice* envelope)
: Enabled(enabled)
, Waveform(waveform)
, Edge(edge)
, Octave(octave)
, Coarse(coarse)
, Unison(unison)
, Detune(detune)
, Spread(spread)
, Gain(gain)
, Envelope(envelope)
{}
void OscParams::addAllParameters(juce::AudioProcessor& processor)
{
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
void OscParams::saveParameters(juce::XmlElement& xml)
{
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
void OscParams::loadParameters(juce::XmlElement& xml)
{
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
EnvelopeParams::EnvelopeParams(juce::AudioParameterFloat* attack,
                               juce::AudioParameterFloat* decay,
                               juce::AudioParameterFloat* sustain,
                               juce::AudioParameterFloat* release)
: Attack(attack)
, Decay(decay)
, Sustain(sustain)
, Release(release)
{}
void EnvelopeParams::addAllParameters(juce::AudioProcessor& processor)
{
    processor.addParameter(Attack);
    processor.addParameter(Decay);
    processor.addParameter(Sustain);
    processor.addParameter(Release);
}
void EnvelopeParams::saveParameters(juce::XmlElement& xml)
{
    xml.setAttribute(Attack->paramID, (double)Attack->get());
    xml.setAttribute(Decay->paramID, (double)Decay->get());
    xml.setAttribute(Sustain->paramID, (double)Sustain->get());
    xml.setAttribute(Release->paramID, (double)Release->get());
}
void EnvelopeParams::loadParameters(juce::XmlElement& xml)
{
    *Attack = (float)xml.getDoubleAttribute(Attack->paramID, 0.01);
    *Decay = (float)xml.getDoubleAttribute(Decay->paramID, 0.01);
    *Sustain = (float)xml.getDoubleAttribute(Sustain->paramID, 1.0);
    *Release = (float)xml.getDoubleAttribute(Release->paramID, 0.01);
}

//==============================================================================
FilterParams::FilterParams(juce::AudioParameterBool* enabled,
                           juce::AudioParameterChoice* target,
                           juce::AudioParameterChoice* type,
                           juce::AudioParameterChoice* freqType,
                           juce::AudioParameterFloat* hz,
                           juce::AudioParameterInt* cent,
                           juce::AudioParameterFloat* q,
                           juce::AudioParameterFloat* gain)
: Enabled(enabled)
, Target(target)
, Type(type)
, FreqType(freqType)
, Hz(hz)
, Cent(cent)
, Q(q)
, Gain(gain)
{}
void FilterParams::addAllParameters(juce::AudioProcessor& processor)
{
    processor.addParameter(Enabled);
    processor.addParameter(Target);
    processor.addParameter(Type);
    processor.addParameter(FreqType);
    processor.addParameter(Hz);
    processor.addParameter(Cent);
    processor.addParameter(Q);
    processor.addParameter(Gain);
}
void FilterParams::saveParameters(juce::XmlElement& xml)
{
    xml.setAttribute(Enabled->paramID, Enabled->get());
    xml.setAttribute(Target->paramID, Target->getIndex());
    xml.setAttribute(Type->paramID, Type->getIndex());
    xml.setAttribute(FreqType->paramID, FreqType->getIndex());
    xml.setAttribute(Hz->paramID, (double)Hz->get());
    xml.setAttribute(Cent->paramID, Cent->get());
    xml.setAttribute(Q->paramID, (double)Q->get());
    xml.setAttribute(Gain->paramID, (double)Gain->get());
}
void FilterParams::loadParameters(juce::XmlElement& xml)
{
    *Enabled = xml.getIntAttribute(Enabled->paramID, 0);
    *Target = xml.getIntAttribute(Target->paramID, NUM_OSC);
    *Type = xml.getIntAttribute(Type->paramID, 0);
    *FreqType = xml.getIntAttribute(FreqType->paramID, 0);
    *Hz = (float)xml.getDoubleAttribute(Hz->paramID, 0);
    *Cent = xml.getDoubleAttribute(Cent->paramID, 0);
    *Q = (float)xml.getDoubleAttribute(Q->paramID, 1.0);
    *Gain = (float)xml.getDoubleAttribute(Gain->paramID, 0);
}

//==============================================================================
LfoParams::LfoParams(juce::AudioParameterBool* enabled,
                     juce::AudioParameterChoice* targetType,
                     juce::AudioParameterChoice* targetOsc,
                     juce::AudioParameterChoice* targetFilter,
                     juce::AudioParameterChoice* targetOscParam,
                     juce::AudioParameterChoice* targetFilterParam,
                     juce::AudioParameterChoice* waveform,
                     juce::AudioParameterFloat* slowFreq,
                     juce::AudioParameterFloat* fastFreq,
                     juce::AudioParameterFloat* amount)
: Enabled(enabled)
, TargetType(targetType)
, TargetOsc(targetOsc)
, TargetFilter(targetFilter)
, TargetOscParam(targetOscParam)
, TargetFilterParam(targetFilterParam)
, Waveform(waveform)
, SlowFreq(slowFreq)
, FastFreq(fastFreq)
, Amount(amount)
{}
void LfoParams::addAllParameters(juce::AudioProcessor& processor)
{
    processor.addParameter(Enabled);
    processor.addParameter(TargetType);
    processor.addParameter(TargetOsc);
    processor.addParameter(TargetFilter);
    processor.addParameter(TargetOscParam);
    processor.addParameter(TargetFilterParam);
    processor.addParameter(SlowFreq);
    processor.addParameter(FastFreq);
    processor.addParameter(Amount);
}
void LfoParams::saveParameters(juce::XmlElement& xml)
{
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
void LfoParams::loadParameters(juce::XmlElement& xml)
{
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
ModEnvParams::ModEnvParams(juce::AudioParameterBool* enabled,
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
                     juce::AudioParameterFloat* decay)
: Enabled(enabled)
, TargetType(targetType)
, TargetOsc(targetOsc)
, TargetFilter(targetFilter)
, TargetLfo(targetLfo)
, TargetOscParam(targetOscParam)
, TargetFilterParam(targetFilterParam)
, TargetLfoParam(targetLfoParam)
, Fade(fade)
, PeakFreq(peakFreq)
, Wait(wait)
, Attack(attack)
, Decay(decay)
{}
void ModEnvParams::addAllParameters(juce::AudioProcessor& processor)
{
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
void ModEnvParams::saveParameters(juce::XmlElement& xml)
{
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
void ModEnvParams::loadParameters(juce::XmlElement& xml)
{
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
DelayParams::DelayParams(juce::AudioParameterBool* enabled,
                         juce::AudioParameterChoice* type,
                         juce::AudioParameterBool* sync,
                         juce::AudioParameterFloat* timeL,
                         juce::AudioParameterFloat* timeR,
                         juce::AudioParameterChoice* timeSyncL,
                         juce::AudioParameterChoice* timeSyncR,
                         juce::AudioParameterFloat* lowFreq,
                         juce::AudioParameterFloat* highFreq,
                         juce::AudioParameterFloat* feedback,
                         juce::AudioParameterFloat* mix)
: Enabled(enabled)
, Type(type)
, Sync(sync)
, TimeL(timeL)
, TimeR(timeR)
, TimeSyncL(timeSyncL)
, TimeSyncR(timeSyncR)
, LowFreq(lowFreq)
, HighFreq(highFreq)
, Feedback(feedback)
, Mix(mix)
{}
void DelayParams::addAllParameters(juce::AudioProcessor& processor)
{
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
void DelayParams::saveParameters(juce::XmlElement& xml)
{
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
void DelayParams::loadParameters(juce::XmlElement& xml)
{
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
ControlItemParams::ControlItemParams(juce::AudioParameterChoice* number,
                                     juce::AudioParameterChoice* targetType,
                                     juce::AudioParameterChoice* targetOsc,
                                     juce::AudioParameterChoice* targetFilter,
                                     juce::AudioParameterChoice* targetLfo,
                                     juce::AudioParameterChoice* targetOscParam,
                                     juce::AudioParameterChoice* targetFilterParam,
                                     juce::AudioParameterChoice* targetLfoParam,
                                     juce::AudioParameterChoice* targetMiscParam)
: Number(number)
, TargetType(targetType)
, TargetOsc(targetOsc)
, TargetFilter(targetFilter)
, TargetLfo(targetLfo)
, TargetOscParam(targetOscParam)
, TargetFilterParam(targetFilterParam)
, TargetLfoParam(targetLfoParam)
, TargetMiscParam(targetMiscParam)
{}
void ControlItemParams::addAllParameters(juce::AudioProcessor& processor)
{
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
void ControlItemParams::saveParameters(juce::XmlElement& xml)
{
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
void ControlItemParams::loadParameters(juce::XmlElement& xml)
{
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
