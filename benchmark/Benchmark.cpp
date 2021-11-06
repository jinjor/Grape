#include <JuceHeader.h>
#include <benchmark/benchmark.h>

#include "../src/Params.h"
#include "../src/Voice.h"

class Params {
public:
    GlobalParams globalParams{};
    VoiceParams voiceParams{};
    MainParams mainParams{};
    std::array<ControlItemParams, NUM_CONTROL> controlItemParams = {ControlItemParams(0),
                                                                    ControlItemParams(1),
                                                                    ControlItemParams(2),
                                                                    ControlItemParams(3),
                                                                    ControlItemParams(4),
                                                                    ControlItemParams(5)};
};

static void doStepLoop(benchmark::State& state, Params& p) {
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo{};
    juce::AudioBuffer<float> buf{};

    GrapeVoice voice{&currentPositionInfo, p.globalParams, p.voiceParams, p.mainParams};

    auto numChannels = 2;
    auto sampleRate = 48000;
    double out[2]{0, 0};

    GrapeSound sound = GrapeSound();

    freezeParams(p.globalParams, p.voiceParams, p.mainParams, p.controlItemParams);
    voice.startNote(60, 1.0, &sound, 8192);
    voice.applyParamsBeforeLoop(sampleRate);
    for (auto _ : state) {
        voice.step(out, sampleRate, numChannels);
    }
}

static void BM_VoiceStep_empty(benchmark::State& state) {
    Params p{};
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_empty);

static void BM_VoiceStep_single_whitenoise(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("White");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_whitenoise);

static void BM_VoiceStep_single_pinknoise(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Pink");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_pinknoise);

static void BM_VoiceStep_single_sine(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_sine);

static void BM_VoiceStep_single_square(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Square");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_square);

static void BM_VoiceStep_single_sine_with_vibrato(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[0].Unison = 4;
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("OSC");
    *p.mainParams.lfoParams[0].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_sine_with_vibrato);

static void BM_VoiceStep_single_sine_with_tremolo(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[0].Unison = 4;
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("OSC");
    *p.mainParams.lfoParams[0].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("Tremolo");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_sine_with_tremolo);

static void BM_VoiceStep_single_sine_unison(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[0].Unison = 4;
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_sine_unison);

static void BM_VoiceStep_single_sine_unison_with_autopan(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[0].Unison = 4;
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("OSC");
    *p.mainParams.lfoParams[0].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("Pan");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_sine_unison_with_autopan);

static void BM_VoiceStep_multiple_sine(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[1].Enabled = true;
    *p.mainParams.oscParams[1].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[2].Enabled = true;
    *p.mainParams.oscParams[2].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_multiple_sine);

static void BM_VoiceStep_single_abs_filter(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_abs_filter);

static void BM_VoiceStep_single_abs_filter_with_lfo_filterfreq(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("Filter");
    *p.mainParams.lfoParams[0].TargetFilterParam = LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_abs_filter_with_lfo_filterfreq);

static void BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[0].Type = FILTER_TYPE_NAMES.indexOf("LowShelf");
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("Filter");
    *p.mainParams.lfoParams[0].TargetFilterParam = LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq);

static void BM_VoiceStep_single_abs_filter_with_lfo_filterq(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("Filter");
    *p.mainParams.lfoParams[0].TargetFilterParam = LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Q");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_abs_filter_with_lfo_filterq);

static void BM_VoiceStep_single_rel_filter(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[0].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_rel_filter);

static void BM_VoiceStep_single_rel_filter_with_vibrato(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[0].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("OSC");
    *p.mainParams.lfoParams[0].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_single_rel_filter_with_vibrato);

static void BM_VoiceStep_multiple_abs_filter(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[1].Enabled = true;
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_multiple_abs_filter);

static void BM_VoiceStep_multiple_rel_filter(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[0].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    *p.mainParams.filterParams[1].Enabled = true;
    *p.mainParams.filterParams[1].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_multiple_rel_filter);

static void BM_VoiceStep_multiple_rel_filter_with_vibrato(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[0].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    *p.mainParams.filterParams[1].Enabled = true;
    *p.mainParams.filterParams[1].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[0].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("OSC");
    *p.mainParams.lfoParams[0].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("Vibrato");
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_multiple_rel_filter_with_vibrato);

static void BM_VoiceStep_full(benchmark::State& state) {
    Params p{};
    *p.mainParams.oscParams[0].Enabled = true;
    *p.mainParams.oscParams[0].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[1].Enabled = true;
    *p.mainParams.oscParams[1].Waveform = OSC_WAVEFORM_NAMES.indexOf("Square");
    *p.mainParams.oscParams[1].Edge = 0.5;
    *p.mainParams.oscParams[2].Enabled = true;
    *p.mainParams.oscParams[2].Waveform = OSC_WAVEFORM_NAMES.indexOf("Sine");
    *p.mainParams.oscParams[2].Unison = 4;
    *p.mainParams.filterParams[0].Enabled = true;
    *p.mainParams.filterParams[1].Enabled = true;
    *p.mainParams.filterParams[1].FreqType = FILTER_FREQ_TYPE_NAMES.indexOf("Rel");
    *p.mainParams.lfoParams[0].Enabled = true;
    *p.mainParams.lfoParams[1].Enabled = true;
    *p.mainParams.lfoParams[1].TargetOscParam = LFO_TARGET_OSC_PARAM_NAMES.indexOf("FM");
    *p.mainParams.lfoParams[2].Enabled = true;
    *p.mainParams.lfoParams[2].TargetType = LFO_TARGET_TYPE_NAMES.indexOf("Filter");
    *p.mainParams.lfoParams[2].TargetFilterParam = LFO_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
    *p.mainParams.modEnvParams[0].Enabled = true;
    *p.mainParams.modEnvParams[0].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("OSC");
    *p.mainParams.modEnvParams[0].TargetOscParam = MODENV_TARGET_OSC_PARAM_NAMES.indexOf("Freq");
    *p.mainParams.modEnvParams[1].Enabled = true;
    *p.mainParams.modEnvParams[1].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("Filter");
    *p.mainParams.modEnvParams[1].TargetFilterParam = MODENV_TARGET_FILTER_PARAM_NAMES.indexOf("Freq");
    *p.mainParams.modEnvParams[2].Enabled = true;
    *p.mainParams.modEnvParams[2].TargetType = MODENV_TARGET_TYPE_NAMES.indexOf("LFO");
    *p.mainParams.modEnvParams[2].TargetLfoParam = MODENV_TARGET_LFO_PARAM_NAMES.indexOf("Freq");
    *p.mainParams.delayParams.Enabled = true;
    doStepLoop(state, p);
}
BENCHMARK(BM_VoiceStep_full);

static void BM_DelayStep(benchmark::State& state) {
    auto numChannels = 2;
    auto sampleRate = 48000;
    auto bpm = 120;
    StereoDelay stereoDelay{};

    stereoDelay.setParams(sampleRate,
                          bpm,
                          DELAY_TYPE::Parallel,
                          false,  // Sync
                          0.3,    // TimeL
                          0.3,    // TimeR
                          0.125,  // TimeSyncL
                          0.125,  // TimeSyncR
                          100,    // LowFreq
                          4000,   // HighFreq
                          0.3,    // Feedback
                          0.3);   // Mix
    juce::Random whiteNoise;
    for (auto _ : state) {
        double s = whiteNoise.nextDouble();
        double sample[2]{s, s};
        stereoDelay.step(sample);
    }
}
BENCHMARK(BM_DelayStep);

BENCHMARK_MAIN();