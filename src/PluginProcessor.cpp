#include "PluginProcessor.h"

#include "Params.h"
#include "PluginEditor.h"
#include "Voice.h"

//==============================================================================
GrapeAudioProcessor::GrapeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         )
#endif
      ,
      globalParams{},
      voiceParams{},
      oscParams{OscParams{0}, OscParams{1}, OscParams{2}},
      envelopeParams{EnvelopeParams{0}, EnvelopeParams{1}},
      filterParams{FilterParams{0}, FilterParams{1}},
      lfoParams{LfoParams{0}, LfoParams{1}, LfoParams{2}},
      modEnvParams{ModEnvParams{0}, ModEnvParams{1}, ModEnvParams{2}},
      distortionParams{},
      delayParams{},
      controlItemParams{ControlItemParams{0},
                        ControlItemParams{1},
                        ControlItemParams{2},
                        ControlItemParams{3},
                        ControlItemParams{4},
                        ControlItemParams{5}},
      synth(&currentPositionInfo,
            &monoStack,
            controlItemParams,
            globalParams,
            voiceParams,
            oscParams,
            envelopeParams,
            filterParams,
            lfoParams,
            modEnvParams,
            distortionParams,
            delayParams) {
    *oscParams[0].Enabled = true;

    *controlItemParams[0].Number = CONTROL_NUMBER_NAMES.indexOf("1: Modulation");
    *controlItemParams[0].TargetType = CONTROL_TARGET_TYPE_NAMES.indexOf("LFO");
    *controlItemParams[0].TargetLfoParam = CONTROL_TARGET_LFO_PARAM_NAMES.indexOf("Amount");

    globalParams.addAllParameters(*this);
    voiceParams.addAllParameters(*this);
    for (auto& params : envelopeParams) {
        params.addAllParameters(*this);
    }
    for (auto& params : oscParams) {
        params.addAllParameters(*this);
    }
    for (auto& params : filterParams) {
        params.addAllParameters(*this);
    }
    for (auto& params : lfoParams) {
        params.addAllParameters(*this);
    }
    for (auto& params : modEnvParams) {
        params.addAllParameters(*this);
    }
    distortionParams.addAllParameters(*this);
    delayParams.addAllParameters(*this);
    for (auto& params : controlItemParams) {
        params.addAllParameters(*this);
    }
}

GrapeAudioProcessor::~GrapeAudioProcessor() { DBG("GrapeAudioProcessor's destructor called."); }

//==============================================================================
const juce::String GrapeAudioProcessor::getName() const { return JucePlugin_Name; }

bool GrapeAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GrapeAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool GrapeAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double GrapeAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int GrapeAudioProcessor::getNumPrograms() { return 2; }

int GrapeAudioProcessor::getCurrentProgram() { return currentProgram; }

void GrapeAudioProcessor::setCurrentProgram(int index) { currentProgram = index; }

const juce::String GrapeAudioProcessor::getProgramName(int index) {
    switch (index) {
        case 0:
            return "program 0";
        case 1:
            return "program 1";
        default:
            return "";
    }
}

void GrapeAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

//==============================================================================
void GrapeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    std::cout << "prepareToPlay" << std::endl;
    std::cout << "sampleRate: " << sampleRate << std::endl;
    std::cout << "totalNumInputChannels: " << getTotalNumInputChannels() << std::endl;
    std::cout << "totalNumOutputChannels: " << getTotalNumOutputChannels() << std::endl;
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void GrapeAudioProcessor::releaseResources() { std::cout << "releaseResources" << std::endl; }

#ifndef JucePlugin_PreferredChannelConfigurations
bool GrapeAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) return false;
#endif

    return true;
#endif
}
#endif

void GrapeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (auto* playHead = getPlayHead()) {
        playHead->getCurrentPosition(currentPositionInfo);
    }
    auto voiceMode = static_cast<VOICE_MODE>(voiceParams.Mode->getIndex());
    int numVoices = 64;
    if (voiceMode == VOICE_MODE::Mono && synth.getNumVoices() != 1) {
        //        this->monoStack.reset();
        synth.clearVoices();
        synth.addVoice(new GrapeVoice(&currentPositionInfo,
                                      globalParams,
                                      voiceParams,
                                      oscParams,
                                      envelopeParams,
                                      filterParams,
                                      lfoParams,
                                      modEnvParams));
    } else if (voiceMode == VOICE_MODE::Poly && synth.getNumVoices() != numVoices) {
        synth.clearVoices();
        for (auto i = 0; i < numVoices; ++i) {
            synth.addVoice(new GrapeVoice(&currentPositionInfo,
                                          globalParams,
                                          voiceParams,
                                          oscParams,
                                          envelopeParams,
                                          filterParams,
                                          lfoParams,
                                          modEnvParams));
        }
    }
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    double startMillis = juce::Time::getMillisecondCounterHiRes();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());  // don't upcast
    double endMillis = juce::Time::getMillisecondCounterHiRes();
    timeConsumptionState.push(getSampleRate(), buffer.getNumSamples(), (endMillis - startMillis) / 1000);

    polyphony = 0;
    for (auto i = 0; i < synth.getNumVoices(); ++i) {
        if (synth.getVoice(i)->isVoiceActive()) {
            polyphony++;
        }
    }
    latestDataProvider.push(buffer);

    midiMessages.clear();
#if JUCE_DEBUG
//    auto* leftIn = buffer.getReadPointer(0);
//    auto* rightIn = buffer.getReadPointer(1);
//    for(int i = 0; i < buffer.getNumSamples(); ++i)
//    {
//        jassert(leftIn[i] >= -1);
//        jassert(leftIn[i] <= 1);
//        jassert(rightIn[i] >= -1);
//        jassert(rightIn[i] <= 1);
//    }
#endif
}

//==============================================================================
bool GrapeAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* GrapeAudioProcessor::createEditor() { return new GrapeAudioProcessorEditor(*this); }

//==============================================================================
void GrapeAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // TODO: ValueTree でもできるらしいので調べる
    juce::XmlElement xml("GrapeInstrument");

    globalParams.saveParameters(xml);
    voiceParams.saveParameters(xml);
    for (auto& param : envelopeParams) {
        param.saveParameters(xml);
    }
    for (auto& param : oscParams) {
        param.saveParameters(xml);
    }
    for (auto& param : filterParams) {
        param.saveParameters(xml);
    }
    for (auto& param : lfoParams) {
        param.saveParameters(xml);
    }
    for (auto& param : modEnvParams) {
        param.saveParameters(xml);
    }
    distortionParams.saveParameters(xml);
    delayParams.saveParameters(xml);
    for (auto& param : controlItemParams) {
        param.saveParameters(xml);
    }
    copyXmlToBinary(xml, destData);
}

void GrapeAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr) {
        if (xml->hasTagName("GrapeInstrument")) {
            globalParams.loadParameters(*xml);
            voiceParams.loadParameters(*xml);
            for (auto& param : envelopeParams) {
                param.loadParameters(*xml);
            }
            for (auto& param : oscParams) {
                param.loadParameters(*xml);
            }
            for (auto& param : filterParams) {
                param.loadParameters(*xml);
            }
            for (auto& param : lfoParams) {
                param.loadParameters(*xml);
            }
            for (auto& param : modEnvParams) {
                param.loadParameters(*xml);
            }
            distortionParams.loadParameters(*xml);
            delayParams.loadParameters(*xml);
            for (auto& param : controlItemParams) {
                param.loadParameters(*xml);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new GrapeAudioProcessor(); }
