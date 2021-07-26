#include <benchmark/benchmark.h>

#include <JuceHeader.h>
#include "../src/Params.h"
#include "../src/Voice.h"

static void BM_VoiceStep(benchmark::State& state) {
  GlobalParams globalParams {};
  VoiceParams voiceParams {};
  OscParams oscParams[3] = { OscParams(0), OscParams(1), OscParams(2) };
  EnvelopeParams envelopeParams[2] = { EnvelopeParams(0), EnvelopeParams(1) };
  FilterParams filterParams[2] = { FilterParams(0), FilterParams(1) };
  LfoParams lfoParams[3] = { LfoParams(0), LfoParams(1), LfoParams(2) };
  ModEnvParams modEnvParams[3] = { ModEnvParams(0), ModEnvParams(1), ModEnvParams(2) };
  DelayParams delayParams {};
  ControlItemParams controlItemParams[6] = { ControlItemParams(0), ControlItemParams(1), ControlItemParams(2), ControlItemParams(3), ControlItemParams(4), ControlItemParams(5) };

  juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo {};
  juce::AudioBuffer<float> buf {};
  
  GrapeVoice voice { &currentPositionInfo, &globalParams, &voiceParams, oscParams, envelopeParams, filterParams, lfoParams, modEnvParams };
  
  auto numChannels = 2;
  auto sampleRate = 48000;
  double out[2] {0, 0};

  voice.startNote(60, 1.0, nullptr, 8192);
  voice.step(out, sampleRate, numChannels);

  for (auto _ : state) {
    voice.step (out, sampleRate, numChannels);
  }
}
BENCHMARK(BM_VoiceStep);

static void BM_VoiceStep2(benchmark::State& state) {
  GlobalParams globalParams {};
  VoiceParams voiceParams {};
  OscParams oscParams[3] = { OscParams(0), OscParams(1), OscParams(2) };
  EnvelopeParams envelopeParams[2] = { EnvelopeParams(0), EnvelopeParams(1) };
  FilterParams filterParams[2] = { FilterParams(0), FilterParams(1) };
  LfoParams lfoParams[3] = { LfoParams(0), LfoParams(1), LfoParams(2) };
  ModEnvParams modEnvParams[3] = { ModEnvParams(0), ModEnvParams(1), ModEnvParams(2) };
  DelayParams delayParams {};
  ControlItemParams controlItemParams[6] = { ControlItemParams(0), ControlItemParams(1), ControlItemParams(2), ControlItemParams(3), ControlItemParams(4), ControlItemParams(5) };

  *oscParams[0].Enabled = true;
  *oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
  *oscParams[1].Enabled = true;
  *oscParams[1].Waveform = OSC_WAVEFORM_NAMES.indexOf("Square");
  *oscParams[1].Edge = 0.5;
  *oscParams[2].Enabled = true;
  *oscParams[2].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
  *oscParams[2].Unison = 4;
  *filterParams[0].Enabled = true;
  *filterParams[1].Enabled = true;
  *filterParams[1].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
  *lfoParams[0].Enabled = true;
  *lfoParams[1].Enabled = true;
  *lfoParams[1].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("FM");
  *lfoParams[2].Enabled = true;
  *lfoParams[2].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("Filter");
  *lfoParams[2].TargetFilterParam = LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
  *modEnvParams[0].Enabled = true;
  *modEnvParams[0].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("OSC");
  *modEnvParams[0].TargetOscParam = MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq");
  *modEnvParams[1].Enabled = true;
  *modEnvParams[1].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("Filter");
  *modEnvParams[1].TargetFilterParam = MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
  *modEnvParams[2].Enabled = true;
  *modEnvParams[2].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("LFO");
  *modEnvParams[2].TargetLfoParam = MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Freq");
  *delayParams.Enabled = true;

  juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo {};
  juce::AudioBuffer<float> buf {};
  
  GrapeVoice voice { &currentPositionInfo, &globalParams, &voiceParams, oscParams, envelopeParams, filterParams, lfoParams, modEnvParams };
  
  auto numChannels = 2;
  auto sampleRate = 48000;
  double out[2] {0, 0};

  voice.startNote(60, 1.0, nullptr, 8192);
  voice.step(out, sampleRate, numChannels);

  for (auto _ : state) {
    voice.step (out, sampleRate, numChannels);
  }
}
BENCHMARK(BM_VoiceStep2);

BENCHMARK_MAIN();