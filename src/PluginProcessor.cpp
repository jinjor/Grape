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
                         .withOutput("Output 1", juce::AudioChannelSet::stereo(), false)
                         .withOutput("Output 2", juce::AudioChannelSet::stereo(), false)
                         .withOutput("Output 3", juce::AudioChannelSet::stereo(), false)
                         .withOutput("Output 4", juce::AudioChannelSet::stereo(), false)
#endif
                         )
#endif
      ,
      allParams{},
      synth(&currentPositionInfo, &monoStack, buffers, busBuffers, allParams) {

    buffers.reserve(129);
    for (auto i = 0; i < 129; i++) {
        buffers.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 0));
    }
    *allParams.mainParamList[128].oscParams[0].Enabled = true;
    *allParams.controlItemParams[0].Number = CONTROL_NUMBER_NAMES.indexOf("1: Modulation");
    *allParams.controlItemParams[0].TargetType = CONTROL_TARGET_TYPE_NAMES.indexOf("LFO");
    *allParams.controlItemParams[0].TargetLfoParam = CONTROL_TARGET_LFO_PARAM_NAMES.indexOf("Amount");

    allParams.addAllParameters(*this);
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

void GrapeAudioProcessor::processBlock(juce::AudioBuffer<float>& _buffer, juce::MidiBuffer& midiMessages) {
    auto busCount = getBusCount(false);
    busBuffers.clear();
    for (int i = 0; i < busCount; i++) {
        auto enabled = getBus(false, i)->isEnabled();
        if (enabled) {
            busBuffers.push_back(getBusBuffer(_buffer, false, i));
            busBuffers[i]->clear();
        } else {
            busBuffers.push_back(nullopt);
        }
    }
    if (auto* playHead = getPlayHead()) {
        if (auto positionInfo = playHead->getPosition()) {
            currentPositionInfo.bpm = *positionInfo->getBpm();
        }
    }
    auto voiceMode = static_cast<VOICE_MODE>(allParams.voiceParams.Mode->getIndex());
    int numVoices = 64;
    if (voiceMode == VOICE_MODE::Mono && synth.getNumVoices() != 1) {
        //        this->monoStack.reset();
        synth.clearVoices();
        synth.addVoice(new GrapeVoice(
            &currentPositionInfo, buffers, allParams.globalParams, allParams.voiceParams, allParams.mainParamList));
    } else if ((voiceMode == VOICE_MODE::Poly || voiceMode == VOICE_MODE::Drum) && synth.getNumVoices() != numVoices) {
        synth.clearVoices();
        for (auto i = 0; i < numVoices; ++i) {
            synth.addVoice(new GrapeVoice(
                &currentPositionInfo, buffers, allParams.globalParams, allParams.voiceParams, allParams.mainParamList));
        }
    }
    auto buffer = *busBuffers[0];
    auto numSamples = buffer.getNumSamples();

    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    double startMillis = juce::Time::getMillisecondCounterHiRes();
    synth.renderNextBlock(buffer, midiMessages, 0, numSamples);  // don't upcast
    double endMillis = juce::Time::getMillisecondCounterHiRes();
    timeConsumptionState.push(getSampleRate(), numSamples, (endMillis - startMillis) / 1000);

    polyphony = 0;
    for (auto i = 0; i < synth.getNumVoices(); ++i) {
        if (synth.getVoice(i)->isVoiceActive()) {
            polyphony++;
        }
    }
    latestDataProvider.push(buffer);  // TODO: パラアウトした時は最初の bus しか表示されない

    midiMessages.clear();
#if JUCE_DEBUG
    // auto* leftIn = buffer.getReadPointer(0);
    // auto* rightIn = buffer.getReadPointer(1);
    // for (int i = 0; i < buffer.getNumSamples(); ++i) {
    //     jassert(leftIn[i] >= -1);
    //     jassert(leftIn[i] <= 1);
    //     jassert(rightIn[i] >= -1);
    //     jassert(rightIn[i] <= 1);
    // }
#endif
}

//==============================================================================
bool GrapeAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* GrapeAudioProcessor::createEditor() { return new GrapeAudioProcessorEditor(*this); }

//==============================================================================
void GrapeAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // TODO: ValueTree でもできるらしいので調べる
    juce::XmlElement xml("GrapeInstrument");
    allParams.saveParameters(xml);
    copyXmlToBinary(xml, destData);
}
void GrapeAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName("GrapeInstrument")) {
        allParams.loadParameters(*xml);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new GrapeAudioProcessor(); }

//==============================================================================
void GrapeAudioProcessor::copyToClipboard() {
    // TODO: ValueTree でもできるらしいので調べる
    juce::XmlElement xml("GrapeInstrumentClipboard");
    allParams.saveParametersToClipboard(xml);
    juce::SystemClipboard::copyTextToClipboard(xml.toString());
}
void GrapeAudioProcessor::pasteFromClipboard() {
    auto text = juce::SystemClipboard::getTextFromClipboard();
    DBG(text);
    auto xml = juce::parseXML(text);
    if (xml && xml->hasTagName("GrapeInstrumentClipboard")) {
        allParams.loadParametersFromClipboard(*xml);
    }
}