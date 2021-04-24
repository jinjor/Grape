#pragma once

#include <JuceHeader.h>

//==============================================================================
enum class TRANSITION_TYPE
{
    NONE = 0,
    LINEAR,
    EXPONENTIAL,
};
class TransitiveValue {
public:
    double value = 0;
    TransitiveValue() {};
    ~TransitiveValue() {};
    void init(double value) {
        this->value = value;
        type = TRANSITION_TYPE::NONE;
        targetValue = value;
        stepAmount = 0;
        steps = 0;
        endThreshold = 0;
    }
    void linear(double duration, double targetValue, double sampleRate) {
        this->targetValue = targetValue;
        if (duration == 0) {
            end();
            return;
        }
        type = TRANSITION_TYPE::LINEAR;
        stepAmount = (targetValue - value) / sampleRate / duration;
        steps = sampleRate * duration;
        endThreshold = 0;
    }
    void exponential(double duration, double targetValue, double sampleRate) {
        this->targetValue = targetValue;
        if (duration == 0) {
            end();
            return;
        }
        type = TRANSITION_TYPE::EXPONENTIAL;
        stepAmount = std::pow(0.37, 1.0 / sampleRate / duration);// 63% 減衰する時間とする
        steps = 0;
        endThreshold = std::abs((this->targetValue - value)) * 0.005;
    }
    bool step() {
        bool ended = false;
        switch (type) {
            case TRANSITION_TYPE::LINEAR: {
                value += stepAmount;
                if(steps <= 0) {
                    end();
                    ended = true;
                }
                steps--;
                break;
            }
            case TRANSITION_TYPE::EXPONENTIAL: {
                value = targetValue - (targetValue - value) * stepAmount;
                if (std::abs(value - targetValue) <= endThreshold) {
                    end();
                    ended = true;
                }
                break;
            }
            case TRANSITION_TYPE::NONE:
                ended = true;
                break;
        }
        return ended;
    }
    void end() {
        type = TRANSITION_TYPE::NONE;
        value = targetValue;
        stepAmount = 0;
        steps = 0;
        endThreshold = 0;
    }
private:
    TRANSITION_TYPE type = TRANSITION_TYPE::NONE;
    double targetValue = 0;
    double stepAmount = 0;
    int steps = 0;
    double endThreshold = 0;
    double setTargetAtTime(double initialValue, double targetValue, double pos);
};
//==============================================================================
/*
  p +     x---x
    |    /     \
    |   /       \
  s +  /         x------x
    | /                  \
    |/                    \
  b +-----+---+--+------+---
    |a    |h  |d |      |r |
*/
enum class ADSR_PHASE
{
    WAIT = 0,
    ATTACK,
    DECAY,
    HOLD,
    SUSTAIN,
    RELEASE,
};
class Adsr {
public:
    Adsr(){};
    ~Adsr(){};
    double getValue() {
        return tvalue.value;
    }
    bool isActive() {
        return phase != ADSR_PHASE::WAIT;
    }
    void setParams(double a, double h, double d, double s, double r) {
        base = 0;
        peak = 1;
        attack = a;
        hold = h;
        decay = d;
        sustain = s;
        release = r;
    }
    void doAttack(double sampleRate) {
        phase = ADSR_PHASE::ATTACK;
        tvalue.linear(attack, peak, sampleRate);
    }
    void doRelease(double sampleRate) {
        phase = ADSR_PHASE::RELEASE;
        tvalue.exponential(release, base, sampleRate);
    }
    void forceStop() {
        tvalue.init(0);
        phase = ADSR_PHASE::WAIT;
    }
    void step(double sampleRate) {
        // TODO: 途中でパラメータを変えた場合にアサーションに引っかかる
        switch (phase) {
            case ADSR_PHASE::ATTACK: {
                if (tvalue.step()) {
                    phase = ADSR_PHASE::HOLD;
                    tvalue.linear(hold, peak, sampleRate);
                }
//                jassert(tvalue.value <= peak);
                break;
            }
            case ADSR_PHASE::HOLD: {
                if (tvalue.step()) {
                    phase = ADSR_PHASE::DECAY;
                    tvalue.exponential(decay, sustain, sampleRate);
                }
//                jassert(tvalue.value == peak);
                break;
            }
            case ADSR_PHASE::DECAY: {
                if (tvalue.step()){
                    phase = ADSR_PHASE::SUSTAIN;
                }
//                jassert(tvalue.value >= sustain);
                break;
            }
            case ADSR_PHASE::SUSTAIN: {
//                jassert(tvalue.value == sustain);
                break;
            }
            case ADSR_PHASE::RELEASE: {
                if (tvalue.step()) {
                    phase = ADSR_PHASE::WAIT;
                }
//                jassert(tvalue.value >= base);
                break;
            }
            case ADSR_PHASE::WAIT: {
//                jassert(tvalue.value == base);
                break;
            }
        }
    }
private:
    double attack; // ms
    double hold; // ms
    double decay; // ms
    double sustain; // 0-1
    double release; // ms
    double base = 0; // 0-1
    double peak = 1; // 0-1
    ADSR_PHASE phase = phase = ADSR_PHASE::WAIT;
    TransitiveValue tvalue;
};

//==============================================================================
namespace {
const int NUM_FEEDFORWARD = 3;
const int NUM_FEEDBACK = 2;
const int NUM_PAST = 2;
}
class Filter
{
public:
    Filter() {}
    ~Filter() {
        DBG("Filter's destructor called.");
    }
    void initializePastData() {
        past[0][0] = past[0][1] = past[1][0] = past[1][1] = 0;
    }
    void setSampleRate(double sampleRate) {
        this->sampleRate = sampleRate;
    }
    double step (FILTER_TYPE filterType, double freq, double q, int ch, double input) {
        jassert(ch < 2);
        jassert(sampleRate != 0.0);
        setParams(filterType, freq, q);
        
        double* p = past[ch];
        // apply b
        for (auto j = 0; j < NUM_FEEDBACK; j++) {
            input -= p[j] * feedback[j];
        }
        // apply a
        auto o = input * feedforward[0];
        for (auto j = 1; j < NUM_FEEDFORWARD; j++) {
            o += p[j-1] * feedforward[j];
        }
        // unshift f.past
        for (auto j = NUM_PAST - 2; j >= 0; j--) {
            p[j+1] = p[j];
        }
        p[0] = input;
        return o;
    }
private:
    double feedforward[NUM_FEEDFORWARD];
    double feedback[NUM_FEEDBACK];
    double past[2][NUM_PAST]{};
    double sampleRate = 0.0;
    FILTER_TYPE currentFilterType = FILTER_TYPE::Lowpass;
    double currentFreq = 0.0;
    double currentQ = 0.0;
    void setParams (FILTER_TYPE filterType, double freq, double q) {
        freq = std::min(sampleRate * 0.5 - 10, freq);
        if(filterType == currentFilterType && freq == currentFreq && q == currentQ) {
            return;
        }
        switch(filterType) {
            case FILTER_TYPE::Lowpass:
            {
                setLowpassParams(sampleRate, freq, q);
                break;
            }
            case FILTER_TYPE::Highpass:
            {
                setHighpassParams(sampleRate, freq, q);
                break;
            }
        }
        currentFilterType = filterType;
        currentFreq = freq;
        currentQ = q;
    }
    void setLowpassParams (double sampleRate, double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq / sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = (1 - std::cos(w0)) * 0.5;
        auto b1 = (1 - std::cos(w0));
        auto b2 = (1 - std::cos(w0)) * 0.5;
        auto a0 = 1 + alpha;
        auto a1 = -2 * std::cos(w0);
        auto a2 = 1 - alpha;
        feedforward[0] = b0/a0;
        feedforward[1] = b1/a0;
        feedforward[2] = b2/a0;
        feedback[0] = a1/a0;
        feedback[1] = a2/a0;
    }
    void setHighpassParams (double sampleRate, double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq / sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = (1 + std::cos(w0)) * 0.5;
        auto b1 = -(1 + std::cos(w0));
        auto b2 = (1 + std::cos(w0)) * 0.5;
        auto a0 = 1 + alpha;
        auto a1 = -2 * std::cos(w0);
        auto a2 = 1 - alpha;
        feedforward[0] = b0/a0;
        feedforward[1] = b1/a0;
        feedforward[2] = b2/a0;
        feedback[0] = a1/a0;
        feedback[1] = a2/a0;
    }
};

//==============================================================================
class Osc
{
public:
    Osc() {
//        currentAngle = whiteNoise.nextFloat() * juce::MathConstants<double>::twoPi;
    }
    ~Osc() {
        DBG("Osc's destructor called.");
    }
    void setWaveform (OSC_WAVEFORM waveform) {
        this->waveform = waveform;
    }
    void setSampleRate (double sampleRate) {
        this->sampleRate = sampleRate;
    }
    void setAngle(double angle) {
        this->currentAngle = angle;
    }
    double step (double freq, double angleShift) {
        if (sampleRate == 0.0)
        {
            return 0.0;
        }
        auto angleDelta = freq * juce::MathConstants<double>::twoPi / sampleRate;
        currentAngle += angleDelta;
        auto angle = currentAngle + angleShift;
        switch(waveform) {
            case OSC_WAVEFORM::Sine:
                return sin(angle);
            case OSC_WAVEFORM::Noise:
                return whiteNoise.nextDouble() * 2.0 - 1.0;
        }
    }
private:
    double currentAngle = 0.0;
    juce::Random whiteNoise;
    OSC_WAVEFORM waveform = OSC_WAVEFORM::Sine;
    double sampleRate = 0.0;
};

//==============================================================================
namespace {
const int MAX_NUM_OSC = 4;
const double GAIN_AT_CENTER = std::cos(juce::MathConstants<double>::halfPi/2);
}
class MultiOsc
{
public:
    MultiOsc() {}
    ~MultiOsc() {
        DBG("MultiOsc's destructor called.");
    }
    void setWaveform (OSC_WAVEFORM waveform) {
        for(int i = 0; i < MAX_NUM_OSC; ++i) {
            oscs[i].setWaveform(waveform);
        }
    }
    void setSampleRate (double sampleRate) {
        for(int i = 0; i < MAX_NUM_OSC; ++i) {
            oscs[i].setSampleRate(sampleRate);
        }
    }
    void step(double numOsc, double detune, double spread, double freq, double angleShift, double* outout) {
        if(numOsc == 1) {
            outout[0] = outout[1] = oscs[0].step(freq, angleShift) * GAIN_AT_CENTER;
        } else {
            setUnison(numOsc, detune, spread);
            outout[0] = 0;
            outout[1] = 0;
            for(int i = 0; i < currentNumOsc; ++i) {
                auto value = oscs[i].step(freq * detunes[i], angleShifts[i] + angleShift);
                outout[0] += value * pans[i][0];
                outout[1] += value * pans[i][1];
            }
        }
    }
private:
    Osc oscs[MAX_NUM_OSC];
    double pans[MAX_NUM_OSC][2]{};
    double detunes[MAX_NUM_OSC]{};
    double angleShifts[MAX_NUM_OSC]{};
    double currentNumOsc = 1;
    double currentDetune = 1;
    double currentSpread = 1;
    void setUnison(double numOsc, double detune, double spread) {
        if(detune != currentDetune || numOsc != currentNumOsc) {
            for(int i = 0; i < numOsc; ++i) {
                double detuneValue = numOsc == 1 ? 0 : -detune + (detune * 2) / (numOsc - 1) * i;
                detunes[i] = std::pow(2, detuneValue / 20);
            }
        }
        if(spread != currentSpread || numOsc != currentNumOsc) {
            for(int i = 0; i < numOsc; ++i) {
                double pan = numOsc == 1 ? 0 : -spread + (spread * 2) / (numOsc - 1) * i;// [-1;1]
                double theta = (pan + 1) * 0.5 * juce::MathConstants<double>::halfPi;
                pans[i][0] = std::cos(theta);
                pans[i][1] = std::sin(theta);
            }
        }
        if(numOsc != currentNumOsc) {
            for(int i = 0; i < numOsc; ++i) {
                angleShifts[i] = juce::MathConstants<double>::pi * i / numOsc;
            }
        }
        currentNumOsc = numOsc;
        currentDetune = detune;
        currentSpread = spread;
    }
};

//==============================================================================
class DelayEffect
{
public:
    DelayEffect() {
    }
    ~DelayEffect() {
        DBG("DelayEffect's destructor called.");
    }
    void setParams(double sampleRate, bool pingpong, double delayTime, double lowFreq, double highFreq, double feedback, double mix) {
        lowpass.setSampleRate(sampleRate);
        highpass.setSampleRate(sampleRate);
        delayLength = std::max(1.0, std::min(48000.0, sampleRate * delayTime));
        this->pingpong = pingpong;
        this->lowFreq = lowFreq;
        this->highFreq = highFreq;
        this->feedback = feedback;
        this->mix = mix;
    }
    void step(double* input) {
        double tmp[2] { past[0][cursor], past[1][cursor] };
        for(int ch = 0; ch < 2; ch++) {
            auto dry = input[ch];
            auto wet = past[ch][cursor];
            input[ch] = dry * (1-mix) + wet * mix;
            
            auto newWet = dry + tmp[pingpong ? 1 - ch : ch] * feedback;
            newWet = lowpass.step(FILTER_TYPE::Lowpass, highFreq, 1.0, ch, newWet);
            newWet = highpass.step(FILTER_TYPE::Highpass, lowFreq, 1.0, ch, newWet);
            past[ch][cursor] = newWet;
        }
        cursor++;
        if(cursor >= delayLength) {
            cursor = 0;
        }
    }
private:
    double past[2][48000];
    int delayLength = 1;
    int cursor = 0;
    Filter lowpass;
    Filter highpass;
    bool pingpong = false;
    double lowFreq = 10;
    double highFreq = 20000;
    double feedback = 0;
    double mix = 0;
};
