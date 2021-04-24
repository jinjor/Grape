#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"
#include "Params.h"

AnalyserState::AnalyserState() {}
AnalyserState::~AnalyserState() {}
void AnalyserState::pushFFTData(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() > 0)
    {
        auto* channelData = buffer.getReadPointer(0);
        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            pushNextSampleIntoFifo(channelData[i]);
        }
    }
}
void AnalyserState::pushNextSampleIntoFifo (float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }
        fifoIndex = 0;
    }
    fifo[fifoIndex++] = sample;
}

//==============================================================================
GrapeAudioProcessor::GrapeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
, oscParams {
    OscParams {
        new juce::AudioParameterBool("OSC0_ENABLED", "Enabled", true),
        new juce::AudioParameterChoice("OSC0_WAVEFOEM", "Waveform", OSC_WAVEFORM_NAMES, OSC_WAVEFORM_NAMES.indexOf("Sine")),
        new juce::AudioParameterInt("OSC0_OCTAVE", "Octave", -2, 2, 0),
        new juce::AudioParameterInt("OSC0_COARSE", "Coarse", -12, 12, 0),
        new juce::AudioParameterInt("OSC0_UNISON", "Unison", 1, 4, 1),
        new juce::AudioParameterFloat("OSC0_DETUNE", "Detune", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("OSC0_SPREAD", "Spread", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("OSC0_GAIN", "Gain", 0.0f, 1.0f, 0.5f),
        new juce::AudioParameterChoice("OSC0_ENVELOPE", "Envelope", OSC_ENV_NAMES, OSC_ENV_NAMES.indexOf("1")),
    },
    OscParams {
        new juce::AudioParameterBool("OSC1_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("OSC1_WAVEFOEM", "Waveform", OSC_WAVEFORM_NAMES, OSC_WAVEFORM_NAMES.indexOf("Sine")),
        new juce::AudioParameterInt("OSC1_OCTAVE", "Octave", -2, 2, 0),
        new juce::AudioParameterInt("OSC1_COARSE", "Coarse",  -12, 12, 0),
        new juce::AudioParameterInt("OSC1_UNISON", "Unison", 1, 4, 1),
        new juce::AudioParameterFloat("OSC1_DETUNE", "Detune", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("OSC1_SPREAD", "Spread", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("OSC1_GAIN", "Gain", 0.0f, 1.0f, 0.5f),
        new juce::AudioParameterChoice("OSC1_ENVELOPE", "Envelope", OSC_ENV_NAMES, OSC_ENV_NAMES.indexOf("2")),
    },
    OscParams {
        new juce::AudioParameterBool("OSC2_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("OSC2_WAVEFOEM", "Waveform", OSC_WAVEFORM_NAMES, OSC_WAVEFORM_NAMES.indexOf("Noise")),
        new juce::AudioParameterInt("OSC2_OCTAVE", "Octave", -2, 2, 0),
        new juce::AudioParameterInt("OSC2_COARSE", "Coarse",  -12, 12, 0),
        new juce::AudioParameterInt("OSC2_UNISON", "Unison", 1, 4, 1),
        new juce::AudioParameterFloat("OSC2_DETUNE", "Detune", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("OSC2_SPREAD", "Spread", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("OSC2_GAIN", "Gain", 0.0f, 1.0f, 0.5f),
        new juce::AudioParameterChoice("OSC2_ENVELOPE", "Envelope", OSC_ENV_NAMES, OSC_ENV_NAMES.indexOf("2")),
    },
}
, envelopeParams {
    EnvelopeParams {
        new juce::AudioParameterFloat("ENV0_ATTACK", "Attack", 0.001f, 1.0f, 0.1f),
        new juce::AudioParameterFloat("ENV0_DECAY", "Decay", 0.01f, 1.0f, 0.2f),
        new juce::AudioParameterFloat("ENV0_SUSTAIN", "Sustain", 0.0f, 1.0f, 0.7f),
        new juce::AudioParameterFloat("ENV0_RELEASE", "Release", 0.01f, 1.0f, 0.1f)
    },
    EnvelopeParams {
        new juce::AudioParameterFloat("ENV1_ATTACK", "Attack", 0.001f, 1.0f, 0.01f),
        new juce::AudioParameterFloat("ENV1_DECAY", "Decay", 0.01f, 1.0f, 0.1f),
        new juce::AudioParameterFloat("ENV1_SUSTAIN", "Sustain", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("ENV1_RELEASE", "Release", 0.01f, 1.0f, 0.1f)
    },
}
, filterParams {
    FilterParams {
        new juce::AudioParameterBool("FILTER0_ENABLED", "Enabled", true),
        new juce::AudioParameterChoice("FILTER0_TARGET", "Target", FILTER_TARGET_NAMES, FILTER_TARGET_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("FILTER0_TYPE", "Type", FILTER_TYPE_NAMES, FILTER_TYPE_NAMES.indexOf("Lowpass")),
        new juce::AudioParameterFloat("FILTER0_OCTAVE", "Octave", -4.0f, 4.0f, 0.0f),
        new juce::AudioParameterFloat("FILTER0_Q", "Q", 0.01f, 100.0f, 1.0f),
    },
    FilterParams {
        new juce::AudioParameterBool("FILTER1_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("FILTER1_TARGET", "Target", FILTER_TARGET_NAMES, FILTER_TARGET_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("FILTER1_TYPE", "Type", FILTER_TYPE_NAMES, FILTER_TYPE_NAMES.indexOf("Lowpass")),
        new juce::AudioParameterFloat("FILTER1_OCTAVE", "Octave", -4.0f, 4.0f, 0.0f),
        new juce::AudioParameterFloat("FILTER1_Q", "Q", 0.01f, 100.0f, 1.0f),
    },
}
, lfoParams {
    LfoParams {
        new juce::AudioParameterBool("LFO0_ENABLED", "Enabled", true),
        new juce::AudioParameterChoice("LFO0_TARGET_TYPE", "TargetType", LFO_TARGET_TYPE_NAMES, LFO_TARGET_TYPE_NAMES.indexOf("OSC")),
        new juce::AudioParameterChoice("LFO0_TARGET_OSC", "TargetOsc", LFO_TARGET_OSC_NAMES, LFO_TARGET_OSC_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("LFO0_TARGET_FILTER", "TargetFilter", LFO_TARGET_FILTER_NAMES, LFO_TARGET_FILTER_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("LFO0_TARGET_OSC_PARAM", "TargetOscParam", LFO_TARGET_OSC_PARAM_NAMES, LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato")),
        new juce::AudioParameterChoice("LFO0_TARGET_FILTER_PARAM", "TargetFilterParam", LFO_TARGET_FILTER_PARAM_NAMES, LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterFloat("LFO0_SLOW_FREQ", "SlowFreq", 0.0f, 100.0f, 0.0f),
        new juce::AudioParameterFloat("LFO0_FAST_FREQ", "FastFreq", 0.01f, 100.0f, 1.0f),
        new juce::AudioParameterFloat("LFO0_AMOUNT", "Amount", 0.0f, 1.0f, 0.0f),
    },
    LfoParams {
        new juce::AudioParameterBool("LFO1_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("LFO1_TARGET_TYPE", "TargetType", LFO_TARGET_TYPE_NAMES, LFO_TARGET_TYPE_NAMES.indexOf("OSC")),
        new juce::AudioParameterChoice("LFO1_TARGET_OSC", "TargetOsc", LFO_TARGET_OSC_NAMES, LFO_TARGET_OSC_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("LFO1_TARGET_FILTER", "TargetFilter", LFO_TARGET_FILTER_NAMES, LFO_TARGET_FILTER_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("LFO1_TARGET_OSC_PARAM", "TargetOscParam", LFO_TARGET_OSC_PARAM_NAMES, LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato")),
        new juce::AudioParameterChoice("LFO1_TARGET_FILTER_PARAM", "TargetFilterParam", LFO_TARGET_FILTER_PARAM_NAMES, LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterFloat("LFO1_SLOW_FREQ", "SlowFreq", 0.0f, 100.0f, 0.0f),
        new juce::AudioParameterFloat("LFO1_FAST_FREQ", "FastFreq", 0.01f, 100.0f, 1.0f),
        new juce::AudioParameterFloat("LFO1_AMOUNT", "Amount", 0.0f, 1.0f, 0.0f),
    },
    LfoParams {
        new juce::AudioParameterBool("LFO2_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("LFO2_TARGET_TYPE", "TargetType", LFO_TARGET_TYPE_NAMES, LFO_TARGET_TYPE_NAMES.indexOf("OSC")),
        new juce::AudioParameterChoice("LFO2_TARGET_OSC", "TargetOsc", LFO_TARGET_OSC_NAMES, LFO_TARGET_OSC_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("LFO2_TARGET_FILTER", "TargetFilter", LFO_TARGET_FILTER_NAMES, LFO_TARGET_FILTER_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("LFO2_TARGET_OSC_PARAM", "TargetOscParam", LFO_TARGET_OSC_PARAM_NAMES, LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato")),
        new juce::AudioParameterChoice("LFO2_TARGET_FILTER_PARAM", "TargetFilterParam", LFO_TARGET_FILTER_PARAM_NAMES, LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterFloat("LFO2_SLOW_FREQ", "SlowFreq", 0.0f, 100.0f, 0.0f),
        new juce::AudioParameterFloat("LFO2_FAST_FREQ", "FastFreq", 0.01f, 100.0f, 1.0f),
        new juce::AudioParameterFloat("LFO2_AMOUNT", "Amount", 0.0f, 1.0f, 0.0f),
    },
}
, modEnvParams {
    ModEnvParams {
        new juce::AudioParameterBool("MODENV0_ENABLED", "Enabled", true),
        new juce::AudioParameterChoice("MODENV0_TARGET_TYPE", "TargetType", MODENV_TARGET_TYPE_NAMES, MODENV_TARGET_TYPE_NAMES.indexOf("OSC")),
        new juce::AudioParameterChoice("MODENV0_TARGET_OSC", "TargetOsc", MODENV_TARGET_OSC_NAMES, MODENV_TARGET_OSC_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV0_TARGET_FILTER", "TargetFilter", MODENV_TARGET_FILTER_NAMES, MODENV_TARGET_FILTER_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV0_TARGET_LFO", "TargetLfo", MODENV_TARGET_LFO_NAMES, MODENV_TARGET_LFO_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV0_TARGET_OSC_PARAM", "TargetOscParam", MODENV_TARGET_OSC_PARAM_NAMES, MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV0_TARGET_FILTER_PARAM", "TargetFilterParam", MODENV_TARGET_FILTER_PARAM_NAMES, MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV0_TARGET_LFO_PARAM", "TargetLfoParam", MODENV_TARGET_LFO_PARAM_NAMES, MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV0_FADE", "Fade", MODENV_FADE_NAMES, MODENV_FADE_NAMES.indexOf("In")),
        new juce::AudioParameterFloat("MODENV0_START_FREQ", "StartFreq", -8.0f, 8.0, 0.0f),
        new juce::AudioParameterFloat("MODENV0_WAIT", "Wait", 0.0f, 1.0f, 0.5f),
        new juce::AudioParameterFloat("MODENV0_ATTACK", "Attack", 0.0f, 1.0f, 0.01f),
        new juce::AudioParameterFloat("MODENV0_DECAY", "Decay", 0.0f, 1.0f, 0.1f),
    },
    ModEnvParams {
        new juce::AudioParameterBool("MODENV1_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("MODENV1_TARGET_TYPE", "TargetType", MODENV_TARGET_TYPE_NAMES, MODENV_TARGET_TYPE_NAMES.indexOf("OSC")),
        new juce::AudioParameterChoice("MODENV1_TARGET_OSC", "TargetOsc", MODENV_TARGET_OSC_NAMES, MODENV_TARGET_OSC_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV1_TARGET_FILTER", "TargetFilter", MODENV_TARGET_FILTER_NAMES, MODENV_TARGET_FILTER_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV1_TARGET_LFO", "TargetLfo", MODENV_TARGET_LFO_NAMES, MODENV_TARGET_LFO_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV1_TARGET_OSC_PARAM", "TargetOscParam", MODENV_TARGET_OSC_PARAM_NAMES, MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV1_TARGET_FILTER_PARAM", "TargetFilterParam", MODENV_TARGET_FILTER_PARAM_NAMES, MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV1_TARGET_LFO_PARAM", "TargetLfoParam", MODENV_TARGET_LFO_PARAM_NAMES, MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV1_FADE", "Fade", MODENV_FADE_NAMES, MODENV_FADE_NAMES.indexOf("In")),
        new juce::AudioParameterFloat("MODENV1_START_FREQ", "StartFreq", -8.0f, 8.0, 0.0f),
        new juce::AudioParameterFloat("MODENV1_WAIT", "Wait", 0.0f, 1.0f, 0.5f),
        new juce::AudioParameterFloat("MODENV1_ATTACK", "Attack", 0.0f, 1.0f, 0.01f),
        new juce::AudioParameterFloat("MODENV1_DECAY", "Decay", 0.0f, 1.0f, 0.1f),
    },
    ModEnvParams {
        new juce::AudioParameterBool("MODENV2_ENABLED", "Enabled", false),
        new juce::AudioParameterChoice("MODENV2_TARGET_TYPE", "TargetType", MODENV_TARGET_TYPE_NAMES, MODENV_TARGET_TYPE_NAMES.indexOf("OSC")),
        new juce::AudioParameterChoice("MODENV2_TARGET_OSC", "TargetOsc", MODENV_TARGET_OSC_NAMES, MODENV_TARGET_OSC_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV2_TARGET_FILTER", "TargetFilter", MODENV_TARGET_FILTER_NAMES, MODENV_TARGET_FILTER_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV2_TARGET_LFO", "TargetLfo", MODENV_TARGET_LFO_NAMES, MODENV_TARGET_LFO_NAMES.indexOf("All")),
        new juce::AudioParameterChoice("MODENV2_TARGET_OSC_PARAM", "TargetOscParam", MODENV_TARGET_OSC_PARAM_NAMES, MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV2_TARGET_FILTER_PARAM", "TargetFilterParam", MODENV_TARGET_FILTER_PARAM_NAMES, MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV2_TARGET_LFO_PARAM", "TargetLfoParam", MODENV_TARGET_LFO_PARAM_NAMES, MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Freq")),
        new juce::AudioParameterChoice("MODENV2_FADE", "Fade", MODENV_FADE_NAMES, MODENV_FADE_NAMES.indexOf("In")),
        new juce::AudioParameterFloat("MODENV2_START_FREQ", "StartFreq", -8.0f, 8.0, 0.0f),
        new juce::AudioParameterFloat("MODENV2_WAIT", "Wait", 0.0f, 1.0f, 0.5f),
        new juce::AudioParameterFloat("MODENV2_ATTACK", "Attack", 0.0f, 1.0f, 0.01f),
        new juce::AudioParameterFloat("MODENV2_DECAY", "Decay", 0.0f, 1.0f, 0.1f),
    },
}
, delayParams {
    new juce::AudioParameterBool("DELAY_ENABLED", "Enabled", false),
    new juce::AudioParameterFloat("DELAY_TIME", "Time", 0.01f, 1.0f, 0.1f),
    new juce::AudioParameterFloat("DELAY_LOW_FREQ", "LowFfreq", 10.0f, 20000.0f, 10.0f),
    new juce::AudioParameterFloat("DELAY_HIGH_FREQ", "HighFreq", 10.0f, 20000.0f, 20000.0f),
    new juce::AudioParameterFloat("DELAY_FEEDBACK", "Feedback", 0.0f, 1.0f, 0.3f),
    new juce::AudioParameterFloat("DELAY_MIX", "Mix", 0.0f, 1.0f, 0.3f)
}
{
    int numVoices = 64;
    for (auto i = 0; i < numVoices; ++i) {
        synth.addVoice (new GrapeVoice(oscParams, envelopeParams, filterParams, lfoParams, modEnvParams));
    }
    synth.addSound (new GrapeSound());
    
    oscParams[0].addAllParameters(*this);
    oscParams[1].addAllParameters(*this);
    oscParams[2].addAllParameters(*this);
    envelopeParams[0].addAllParameters(*this);
    envelopeParams[1].addAllParameters(*this);
    filterParams[0].addAllParameters(*this);
    filterParams[1].addAllParameters(*this);
    lfoParams[0].addAllParameters(*this);
    lfoParams[1].addAllParameters(*this);
    lfoParams[2].addAllParameters(*this);
    modEnvParams[0].addAllParameters(*this);
    modEnvParams[1].addAllParameters(*this);
    modEnvParams[2].addAllParameters(*this);
    delayParams.addAllParameters(*this);
}

GrapeAudioProcessor::~GrapeAudioProcessor()
{
    DBG("GrapeAudioProcessor's destructor called.");
}

//==============================================================================
const juce::String GrapeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GrapeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GrapeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GrapeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GrapeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GrapeAudioProcessor::getNumPrograms()
{
    return 2;
}

int GrapeAudioProcessor::getCurrentProgram()
{
    return currentProgram;
}

void GrapeAudioProcessor::setCurrentProgram (int index)
{
    currentProgram = index;
}

const juce::String GrapeAudioProcessor::getProgramName (int index)
{
    switch(index)
    {
        case 0:
            return "program 0";
        case 1:
            return "program 1";
        default:
            return "";
    }
}

void GrapeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GrapeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    std::cout << "prepareToPlay" << std::endl;
    std::cout << "sampleRate: " << sampleRate << std::endl;
    std::cout << "totalNumInputChannels: " << getTotalNumInputChannels() << std::endl;
    std::cout << "totalNumOutputChannels: " << getTotalNumOutputChannels() << std::endl;
    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void GrapeAudioProcessor::releaseResources()
{
    std::cout << "releaseResources" << std::endl;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GrapeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GrapeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    midiMessages.clear();
    
    auto* leftIn = buffer.getReadPointer(0);
    auto* rightIn = buffer.getReadPointer(1);
    auto* leftOut = buffer.getWritePointer(0);
    auto* rightOut = buffer.getWritePointer(1);
    
    delayEffect.setParams(getSampleRate(),
                          false,
                          delayParams.Time->get(),
                          delayParams.LowFreq->get(),
                          delayParams.HighFreq->get(),
                          delayParams.Feedback->get(),
                          delayParams.Mix->get());
    if(delayParams.Enabled->get()) {
        for(int i = 0; i < buffer.getNumSamples(); ++i)
        {
            double sample[2] { leftIn[i], rightIn[i] };
            delayEffect.step(sample);
            jassert(sample[0] >= -1);
            jassert(sample[0] <= 1);
            jassert(sample[1] >= -1);
            jassert(sample[1] <= 1);
            leftOut[i] = sample[0];
            rightOut[i] = sample[1];
        }
    }
    analyserState.pushFFTData(buffer);
}

//==============================================================================
bool GrapeAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* GrapeAudioProcessor::createEditor()
{
    return new GrapeAudioProcessorEditor (*this);
}

//==============================================================================
void GrapeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // TODO: ValueTree でもできるらしいので調べる
    juce::XmlElement xml("GrapeInstrument");
    
    oscParams[0].saveParameters(xml);
    oscParams[1].saveParameters(xml);
    oscParams[2].saveParameters(xml);
    envelopeParams[0].saveParameters(xml);
    envelopeParams[1].saveParameters(xml);
    filterParams[0].saveParameters(xml);
    filterParams[1].saveParameters(xml);
    lfoParams[0].saveParameters(xml);
    lfoParams[1].saveParameters(xml);
    lfoParams[2].saveParameters(xml);
    modEnvParams[0].saveParameters(xml);
    modEnvParams[1].saveParameters(xml);
    modEnvParams[2].saveParameters(xml);
    delayParams.saveParameters(xml);
    
    copyXmlToBinary(xml, destData);
}

void GrapeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml.get() != nullptr)
    {
        if (xml->hasTagName ("GrapeInstrument"))
        {
            oscParams[0].loadParameters(*xml);
            oscParams[1].loadParameters(*xml);
            oscParams[2].loadParameters(*xml);
            envelopeParams[0].loadParameters(*xml);
            envelopeParams[1].loadParameters(*xml);
            filterParams[0].loadParameters(*xml);
            filterParams[1].loadParameters(*xml);
            lfoParams[0].loadParameters(*xml);
            lfoParams[1].loadParameters(*xml);
            lfoParams[2].loadParameters(*xml);
            modEnvParams[0].loadParameters(*xml);
            modEnvParams[1].loadParameters(*xml);
            modEnvParams[2].loadParameters(*xml);
            delayParams.loadParameters(*xml);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrapeAudioProcessor();
}
