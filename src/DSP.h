#pragma once

#include <JuceHeader.h>

//==============================================================================
class Wavetable {
public:
    const int* lookup = reinterpret_cast<const int*>(BinaryData::lookup);
    const float* sine = reinterpret_cast<const float*>(BinaryData::sine);
    const float* saw = reinterpret_cast<const float*>(BinaryData::saw);// 128 variations
    const float* parabola = reinterpret_cast<const float*>(BinaryData::parabola);// 128 variations
    Wavetable() {};
    ~Wavetable() {};
    double getSineValue(double angle) {
        angle = std::fmod(angle, juce::MathConstants<double>::twoPi);
        float pos = angle / juce::MathConstants<double>::twoPi;
        return getValue(sine, pos);
    }
    double getSawDownValue(double freq, double angle) {
        angle = std::fmod(angle, juce::MathConstants<double>::twoPi);
        float pos = angle / juce::MathConstants<double>::twoPi;
        const float* partial = getPartial(saw, freq);
        return getValue(partial, pos);
    }
    double getSawUpValue(double freq, double angle) {
        angle = std::fmod(angle, juce::MathConstants<double>::twoPi);
        float pos = angle / juce::MathConstants<double>::twoPi;
        const float* partial = getPartial(saw, freq);
        return getValueReverse(partial, pos);
    }
    double getSquareValue(double freq, double angle) {
        return getPulseValue(freq, angle, 0.5);
    }
    double getPulseValue(double freq, double angle, double edge) {
        double phaseShift = (1.0 - edge * 0.99) * 0.5;
        jassert(phaseShift > 0.0);
        jassert(phaseShift <= 0.5);
        angle = std::fmod(angle, juce::MathConstants<double>::twoPi);
        float pos1 = angle / juce::MathConstants<double>::twoPi;
        float pos2 = std::fmod(pos1 + phaseShift, 1.0f);
        const float* partial = getPartial(saw, freq);
        return getValue(partial, pos1) + getValueReverse(partial, pos2) + (1.0 - 2 * phaseShift);
    }
    double getTriangleValue(double freq, double angle) {
        return getSlopedVariableTriangleValue(freq, angle, 0.5);
    }
    double getSlopedVariableTriangleValue(double freq, double angle, double edge) {
        double phaseShift = (1.0 - edge * 0.99) * 0.5;
        jassert(phaseShift > 0.0);
        jassert(phaseShift <= 0.5);
        angle = std::fmod(angle, juce::MathConstants<double>::twoPi);
        float pos1 = angle / juce::MathConstants<double>::twoPi;
        float pos2 = std::fmod(pos1 + phaseShift, 1.0f);
        const float* partial = getPartial(parabola, freq);
//        return (getValue(partial, pos1) - getValue(partial, pos2)) / (8 * (duty - duty * duty));
        return (getValue(partial, pos1) - getValue(partial, pos2)) / (8 * (-0.8*(phaseShift-0.5)*(phaseShift-0.5)+0.25));// TODO: gibbs 試して再考
    }
private:
    double getValue(const float* partial, float normalizedAngle) {
        float indexFloat = normalizedAngle * 4095;
        int index = indexFloat;
        float fragment = indexFloat - index;
        return partial[index] * (1-fragment) + partial[index+1] * fragment;
    }
    double getValueReverse(const float* partial, float normalizedAngle) {
        float indexFloat = normalizedAngle * 4095;
        int index = indexFloat;
        float fragment = indexFloat - index;
        return partial[4095-index] * (1-fragment) + partial[4095-index-1] * fragment;
    }
    const float* getPartial(const float* data, double freq) {
        int partialIndex = lookup[freq >= 22000 ? 21999 : (int)freq];
        return &data[partialIndex * 4096];
    }
};

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
    }
    void exponential(double duration, double targetValue, double sampleRate) {
        this->targetValue = targetValue;
        if (duration == 0) {
            end();
            return;
        }
        type = TRANSITION_TYPE::EXPONENTIAL;
        stepAmount = std::pow(0.37, 1.0 / sampleRate / duration);// 63% 減衰する時間とする
        steps = std::log(0.005) / std::log(stepAmount);
    }
    bool step() {
        switch (type) {
            case TRANSITION_TYPE::LINEAR: {
                value += stepAmount;
                if(steps <= 0) {
                    end();
                    return true;
                }
                steps--;
                break;
            }
            case TRANSITION_TYPE::EXPONENTIAL: {
                value = targetValue - (targetValue - value) * stepAmount;
                if(steps <= 0) {
                    end();
                    return true;
                }
                steps--;
                break;
            }
            case TRANSITION_TYPE::NONE:
                return true;
        }
        return false;
    }
    void end() {
        type = TRANSITION_TYPE::NONE;
        value = targetValue;
        stepAmount = 0;
        steps = 0;
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
        if(this->sampleRate != sampleRate) {
            reciprocal_sampleRate = 1.0 / sampleRate;
        }
        this->sampleRate = sampleRate;
    }
    double step (FILTER_TYPE filterType, double freq, double q, double dbGain, int ch, double input) {
        jassert(ch < 2);
        jassert(sampleRate != 0.0);
        jassert(reciprocal_sampleRate > 0.0);
        setParams(filterType, freq, q, dbGain);
        
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
    double reciprocal_sampleRate = -1;
    FILTER_TYPE currentFilterType = FILTER_TYPE::Lowpass;
    double currentFreq = 0.0;
    double currentQ = 0.0;
    double currentDbGain = 0.0;
    void setParams (FILTER_TYPE filterType, double freq, double q, double dbGain) {
        freq = std::min(sampleRate * 0.5 - 10, freq);
        if(filterType == currentFilterType && freq == currentFreq && q == currentQ && dbGain == currentDbGain) {
            return;
        }
        switch(filterType) {
            case FILTER_TYPE::Lowpass:
            {
                setLowpassParams(freq, q);
                break;
            }
            case FILTER_TYPE::Highpass:
            {
                setHighpassParams(freq, q);
                break;
            }
            case FILTER_TYPE::Bandpass1:
            {
                setBandpass1Params(freq, q);
                break;
            }
            case FILTER_TYPE::Bandpass2:
            {
                setBandpass2Params(freq, q);
                break;
            }
            case FILTER_TYPE::Notch:
            {
                setNotchParams(freq, q);
                break;
            }
            case FILTER_TYPE::Peaking:
            {
                setPeakingParams(freq, q, dbGain);
                break;
            }
            case FILTER_TYPE::LowShelf:
            {
                setLowShelfParams(freq, q, dbGain);
                break;
            }
            case FILTER_TYPE::HighShelf:
            {
                setHighShelfParams(freq, q, dbGain);
                break;
            }
        }
        currentFilterType = filterType;
        currentFreq = freq;
        currentQ = q;
        currentDbGain = dbGain;
    }
    void setLowpassParams (double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = (1 - cosw0) * 0.5;
        auto b1 = (1 - cosw0);
        auto b2 = (1 - cosw0) * 0.5;
        auto a0 = 1 + alpha;
        auto a1 = -2 * cosw0;
        auto a2 = 1 - alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setHighpassParams (double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = (1 + cosw0) * 0.5;
        auto b1 = -(1 + cosw0);
        auto b2 = (1 + cosw0) * 0.5;
        auto a0 = 1 + alpha;
        auto a1 = -2 * cosw0;
        auto a2 = 1 - alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setBandpass1Params (double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto sinw0 = std::sin(w0);
        auto alpha = sinw0 / (2 * q);
        auto b0 = sinw0 * 0.5;
        auto b1 = 0.0;
        auto b2 = -sinw0 * 0.5;
        auto a0 = 1 + alpha;
        auto a1 = -2 * std::cos(w0);
        auto a2 = 1 - alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setBandpass2Params (double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = alpha;
        auto b1 = 0.0;
        auto b2 = -alpha;
        auto a0 = 1 + alpha;
        auto a1 = -2 * std::cos(w0);
        auto a2 = 1 - alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setNotchParams (double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = 1.0;
        auto b1 = -2 * cosw0;
        auto b2 = 1.0;
        auto a0 = 1 + alpha;
        auto a1 = -2 * cosw0;
        auto a2 = 1 - alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setAllPassParams (double freq, double q) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto b0 = 1 - alpha;
        auto b1 = -2 * cosw0;
        auto b2 = 1 + alpha;
        auto a0 = 1 + alpha;
        auto a1 = -2 * cosw0;
        auto a2 = 1 - alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setPeakingParams (double freq, double q, double dbGain) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto A = std::pow(10, dbGain/40);
        auto b0 = 1 + alpha * A;
        auto b1 = -2 * cosw0;
        auto b2 = 1 - alpha * A;
        auto a0 = 1 + alpha / A;
        auto a1 = -2 * cosw0;
        auto a2 = 1 - alpha / A;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setLowShelfParams (double freq, double q, double dbGain) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto A = std::pow(10, dbGain/40);
        auto sqrtA = std::sqrt(A);
        auto b0 =     A * ((A + 1) - (A - 1) * cosw0 + 2 * sqrtA * alpha);
        auto b1 = 2 * A * ((A - 1) - (A + 1) * cosw0);
        auto b2 =     A * ((A + 1) - (A - 1) * cosw0 - 2 * sqrtA * alpha);
        auto a0 =          (A + 1) + (A - 1) * cosw0 + 2 * sqrtA * alpha;
        auto a1 =    -2 * ((A - 1) + (A + 1) * cosw0);
        auto a2 =          (A + 1) + (A - 1) * cosw0 - 2 * sqrtA * alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
    }
    void setHighShelfParams (double freq, double q, double dbGain) {
        // from RBJ's cookbook
        auto fc = freq * reciprocal_sampleRate;
        auto w0 = 2 * juce::MathConstants<double>::pi * fc;
        auto cosw0 = std::cos(w0);
        auto alpha = std::sin(w0) / (2 * q);
        auto A = std::pow(10, dbGain/40);
        auto sqrtA = std::sqrt(A);
        auto b0 =      A * ((A + 1) + (A - 1) * cosw0 + 2 * sqrtA * alpha);
        auto b1 = -2 * A * ((A - 1) + (A + 1) * cosw0);
        auto b2 =      A * ((A + 1) + (A - 1) * cosw0 - 2 * sqrtA * alpha);
        auto a0 =           (A + 1) - (A - 1) * cosw0 + 2 * sqrtA * alpha;
        auto a1 =      2 * ((A - 1) - (A + 1) * cosw0);
        auto a2 =           (A + 1) - (A - 1) * cosw0 - 2 * sqrtA * alpha;
        auto _a0 = 1.0 / a0;
        feedforward[0] = b0 * _a0;
        feedforward[1] = b1 * _a0;
        feedforward[2] = b2 * _a0;
        feedback[0] = a1 * _a0;
        feedback[1] = a2 * _a0;
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
    void setWaveform (WAVEFORM waveform) {
        this->waveform = waveform;
    }
    void setSampleRate (double sampleRate) {
//        this->sampleRate = sampleRate;
        reciprocal_sampleRate = 1.0 / sampleRate;
    }
    void setAngle(double angle) {
        this->currentAngle = angle;
    }
    double step (double freq, double angleShift, double edge) {
        if (reciprocal_sampleRate <= 0.0)
        {
            return 0.0;
        }
        auto angleDelta = freq * juce::MathConstants<double>::twoPi * reciprocal_sampleRate;
        currentAngle += angleDelta;
        if(currentAngle > juce::MathConstants<double>::twoPi) {
            currentAngle -= juce::MathConstants<double>::twoPi;
            currentRandomValue = 0.0;
        }
        auto angle = currentAngle + angleShift;
        switch(waveform) {
            case WAVEFORM::Sine:
//                return std::sin(angle);
                return wavetable.getSineValue(angle);
            case WAVEFORM::Triangle:
//                return angle >= juce::MathConstants<double>::pi ?
//                    angle / juce::MathConstants<double>::twoPi * 4.0 - 1.0 :
//                    angle / juce::MathConstants<double>::twoPi - 4.0 + 3.0;
                return wavetable.getSlopedVariableTriangleValue(freq, angle, edge);
            case WAVEFORM::SawUp:
//                return angle / juce::MathConstants<double>::twoPi * 2.0 - 1.0;
                return wavetable.getSawUpValue(freq, angle);
            case WAVEFORM::SawDown:
//                return angle / juce::MathConstants<double>::twoPi * -2.0 + 1.0;
                return wavetable.getSawDownValue(freq, angle);
            case WAVEFORM::Square:
//                return angle < juce::MathConstants<double>::pi ? 1.0 : -1.0;
                return wavetable.getPulseValue(freq, angle, edge);
            case WAVEFORM::Random:
                if(currentRandomValue == 0.0) {
                    currentRandomValue = whiteNoise.nextDouble() * 2.0 - 1.0;
                }
                return currentRandomValue;
            case WAVEFORM::Pink: {
                auto white = (whiteNoise.nextDouble() * 2.0 - 1.0) * 0.5;
                bool eco = true;
                if(eco) {
                    pink[0] = 0.99765 * pink[0] + white * 0.0990460;
                    pink[1] = 0.96300 * pink[1] + white * 0.2965164;
                    pink[2] = 0.57000 * pink[2] + white * 1.0526913;
                    auto value = pink[0] + pink[1] + pink[2] + white * 0.1848;
                    return value;
                } else {
                    pink[0] = 0.99886 * pink[0] + white * 0.0555179;
                    pink[1] = 0.99332 * pink[1] + white * 0.0750759;
                    pink[2] = 0.96900 * pink[2] + white * 0.1538520;
                    pink[3] = 0.86650 * pink[3] + white * 0.3104856;
                    pink[4] = 0.55000 * pink[4] + white * 0.5329522;
                    pink[5] = -0.7616 * pink[5] - white * 0.0168980;
                    auto value = pink[0] + pink[1] + pink[2] + pink[3] + pink[4] + pink[5] + pink[6] + white * 0.5362;
                    pink[6] = white * 0.115926;
                    return value;
                }
            }
            case WAVEFORM::White:
                return whiteNoise.nextDouble() * 2.0 - 1.0;
        }
    }
private:
    Wavetable wavetable;
    double currentAngle = 0.0;
    double currentRandomValue = 0.0;
    double pink[7]{};
    juce::Random whiteNoise;
    WAVEFORM waveform = WAVEFORM::Sine;
//    double sampleRate = 0.0;
    double reciprocal_sampleRate = -1;
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
    void setWaveform (WAVEFORM waveform) {
        for(int i = 0; i < MAX_NUM_OSC; ++i) {
            oscs[i].setWaveform(waveform);
        }
    }
    void setSampleRate (double sampleRate) {
        for(int i = 0; i < MAX_NUM_OSC; ++i) {
            oscs[i].setSampleRate(sampleRate);
        }
    }
    void step(double numOsc, double pan, double detune, double spread, double freq, double angleShift, double edge, double* outout) {
//        if(numOsc == 1) {
//            outout[0] = outout[1] = oscs[0].step(freq, angleShift, edge) * GAIN_AT_CENTER;
//        } else {
            setUnison(numOsc, pan, detune, spread);
            outout[0] = 0;
            outout[1] = 0;
            for(int i = 0; i < currentNumOsc; ++i) {
                auto value = oscs[i].step(freq * detunes[i], angleShifts[i] + angleShift, edge);
                outout[0] += value * pans[i][0];
                outout[1] += value * pans[i][1];
            }
//        }
    }
private:
    Osc oscs[MAX_NUM_OSC];
    double pans[MAX_NUM_OSC][2]{};
    double detunes[MAX_NUM_OSC]{};
    double angleShifts[MAX_NUM_OSC]{};
    int currentNumOsc = 1;
    double currentPan = 0.0;
    double currentDetune = 1;
    double currentSpread = 1;
    void setUnison(int numOsc, double pan, double detune, double spread) {
        if(detune != currentDetune || numOsc != currentNumOsc) {
            for(int i = 0; i < numOsc; ++i) {
                double detuneValue = numOsc == 1 ? 0 : -detune + (detune * 2) / (numOsc - 1) * i;
                detunes[i] = std::pow(2, detuneValue / 20);// TODO: ?
            }
        }
        if(pan != currentPan || spread != currentSpread || numOsc != currentNumOsc) {
            auto panMax = pan + std::min(1.0 - pan, spread);
            auto panMin = pan - std::min(1.0 + pan, spread);
            for(int i = 0; i < numOsc; ++i) {
                double p = numOsc == 1
                    ? (panMin + panMax) * 0.5
                    : panMin * (numOsc - 1 - i) / (numOsc - 1) + panMax * i / (numOsc - 1);
                jassert(p >= -1);
                jassert(p <= 1);
                double theta = (p + 1) * 0.5 * juce::MathConstants<double>::halfPi;
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
        currentPan = pan;
        currentDetune = detune;
        currentSpread = spread;
    }
};

//==============================================================================
class StereoDelay
{
public:
    StereoDelay() {
    }
    ~StereoDelay() {
        DBG("DelayEffect's destructor called.");
    }
    void setParams(double sampleRate,
                   double bpm,
                   DELAY_TYPE type,
                   bool sync,
                   double delayTimeL,
                   double delayTimeR,
                   double delayTimeSyncL,
                   double delayTimeSyncR,
                   double lowFreq,
                   double highFreq,
                   double feedback,
                   double mix) {
        lowpass.setSampleRate(sampleRate);
        highpass.setSampleRate(sampleRate);
        if(sync) {
            auto timePerBar = 60 * 4 / bpm;
            delayTimeL = timePerBar * delayTimeSyncL;
            delayTimeR = timePerBar * delayTimeSyncR;
        }
        delayLength[0] = std::max(1.0, std::min(192000.0, sampleRate * delayTimeL));
        delayLength[1] = std::max(1.0, std::min(192000.0, sampleRate * delayTimeR));
        this->type = type;
        this->lowFreq = lowFreq;
        this->highFreq = highFreq;
        this->feedback = feedback;
        this->mix = mix;
    }
    void step(double* input) {
        double tmp[2] { past[0][cursor[0]], past[1][cursor[1]] };// loop の中で past を上書きするのでここに保持しておく
        for(int ch = 0; ch < 2; ch++) {
            auto dry = input[ch];
            auto wet = past[ch][cursor[ch]];
            input[ch] = dry * (1-mix) + wet * mix;
            
            auto newWet = dry + tmp[type == DELAY_TYPE::PingPong ? 1 - ch : ch] * feedback;
            newWet = lowpass.step(FILTER_TYPE::Lowpass, highFreq, 1.0, 0.0, ch, newWet);
            newWet = highpass.step(FILTER_TYPE::Highpass, lowFreq, 1.0, 0.0, ch, newWet);
            past[ch][cursor[ch]] = newWet;
        }
        for(int ch = 0; ch < 2; ch++) {
            cursor[ch]++;
            if(cursor[ch] >= delayLength[ch]) {
                cursor[ch] = 0;
            }
        }
    }
private:
    float past[2][192000]{};
    int delayLength[2] { 1, 1 };
    int cursor[2]{};
    Filter lowpass;
    Filter highpass;
    DELAY_TYPE type = DELAY_TYPE::Parallel;
    double lowFreq = 10;
    double highFreq = 20000;
    double feedback = 0;
    double mix = 0;
};
