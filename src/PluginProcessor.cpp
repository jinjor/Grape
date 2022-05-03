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
      controlItemParams{ControlItemParams{0}, ControlItemParams{1}, ControlItemParams{2}},
      synth(&currentPositionInfo, &monoStack, buffers, controlItemParams, globalParams, voiceParams, mainParamList) {

    mainParamList.reserve(129);
    for (int i = 0; i < 129; i++) {
        mainParamList.push_back(MainParams{i});
    }
    buffers.reserve(129);
    for (auto i = 0; i < 129; i++) {
        buffers.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 0));
    }
    *mainParamList[128].oscParams[0].Enabled = true;

    *controlItemParams[0].Number = CONTROL_NUMBER_NAMES.indexOf("1: Modulation");
    *controlItemParams[0].TargetType = CONTROL_TARGET_TYPE_NAMES.indexOf("LFO");
    *controlItemParams[0].TargetLfoParam = CONTROL_TARGET_LFO_PARAM_NAMES.indexOf("Amount");

    globalParams.addAllParameters(*this);
    voiceParams.addAllParameters(*this);
    for (int i = 0; i < 129; i++) {
        mainParamList[i].addAllParameters(*this);
    }
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
        synth.addVoice(new GrapeVoice(&currentPositionInfo, buffers, globalParams, voiceParams, mainParamList));
    } else if (voiceMode == VOICE_MODE::Poly && synth.getNumVoices() != numVoices) {
        synth.clearVoices();
        for (auto i = 0; i < numVoices; ++i) {
            synth.addVoice(new GrapeVoice(&currentPositionInfo, buffers, globalParams, voiceParams, mainParamList));
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
    for (int i = 0; i < 129; i++) {
        auto enabled = mainParamList[i].isEnabled();
        xml.setAttribute(juce::String("MAIN_PARAMS_" + std::to_string(i) + "_ENABLED"), enabled);
        if (enabled) {
            mainParamList[i].saveParameters(xml);
        }
    }
    for (auto& param : controlItemParams) {
        param.saveParameters(xml);
    }
    copyXmlToBinary(xml, destData);
}
void GrapeAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml) {
        if (xml->hasTagName("GrapeInstrument")) {
            globalParams.loadParameters(*xml);
            voiceParams.loadParameters(*xml);
            for (int i = 0; i < 129; i++) {
                auto enabled =
                    xml->getBoolAttribute(juce::String("MAIN_PARAMS_" + std::to_string(i) + "_ENABLED"), i == 128);
                if (enabled) {
                    mainParamList[i].loadParameters(*xml);
                }
            }
            for (auto& param : controlItemParams) {
                param.loadParameters(*xml);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new GrapeAudioProcessor(); }

//==============================================================================
void replaceAttributeNames(juce::XmlElement& xml, juce::StringRef before, juce::StringRef after) {
    auto newAttrs = std::map<juce::String, juce::String>{};
    for (auto i = 0; i < xml.getNumAttributes(); i++) {
        auto attrName = xml.getAttributeName(i);
        attrName = attrName.replace(before, after);
        newAttrs[attrName] = xml.getAttributeValue(i);
    }
    xml.removeAllAttributes();
    for (auto const& [attrName, value] : newAttrs) {
        xml.setAttribute(attrName, value);
    }
}

void GrapeAudioProcessor::copyToClipboard() {
    // TODO: ValueTree でもできるらしいので調べる
    juce::XmlElement xml("GrapeInstrumentClipboard");

    // TODO: portamento, pitchbend range
    auto index = voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128;
    xml.setAttribute("DRUM_MODE", voiceParams.isDrumMode());
    mainParamList[index].saveParameters(xml);
    if (!voiceParams.isDrumMode()) {
        for (auto& param : controlItemParams) {
            param.saveParameters(xml);
        }
    }

    replaceAttributeNames(xml, "G" + std::to_string(index) + "_", "GROUP_");

    juce::SystemClipboard::copyTextToClipboard(xml.toString());
}
void GrapeAudioProcessor::pasteFromClipboard() {
    auto text = juce::SystemClipboard::getTextFromClipboard();
    std::cout << text << std::endl;
    auto xml = juce::parseXML(text);
    if (xml) {
        if (xml->hasTagName("GrapeInstrumentClipboard")) {
            auto index = voiceParams.isDrumMode() ? voiceParams.getTargetNote() : 128;

            replaceAttributeNames(*xml, "GROUP_", "G" + std::to_string(index) + "_");

            auto wasDrumMode = xml->getBoolAttribute("DRUM_MODE", false);
            mainParamList[index].loadParameters(*xml);
            if (!wasDrumMode && !voiceParams.isDrumMode()) {
                for (auto& param : controlItemParams) {
                    param.loadParameters(*xml);
                }
            }
        }
    }
}