#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace {
const int NUM_DIVISIONS = 4095;
const int NUM_SAMPLES = NUM_DIVISIONS + 1;
const float BASE_FREQ = 440.0f;
const float MAX_FREQ = 22000.0f;
const float PI = 3.141592f;
const std::string LOOKUP_FILE = "lookup";
const std::string SINE_FILE = "sine";
const std::string SAW_FILE = "saw";
const std::string PARABOLA_FILE = "parabola";
const std::string WHITE_FILE = "white";
const std::string PINK_FILE = "pink";
}  // namespace

int main() {
    {
        std::cout << "generating lookup table..." << std::endl;
        int* lookupTable = new int[22000]();
        int freqIndex = 0;
        for (int noteNum = 0; noteNum < 128; noteNum++) {
            float freq = BASE_FREQ * std::pow(2.0, ((float)noteNum - 69) / 12);
            while (freqIndex < freq) {
                lookupTable[freqIndex] = noteNum;
                freqIndex++;
            }
        }
        int lastFreqIndex = freqIndex - 1;
        while (freqIndex < 22000) {
            lookupTable[freqIndex] = lookupTable[lastFreqIndex];
            freqIndex++;
        }
        std::ofstream ofs(LOOKUP_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(lookupTable), 22000 * sizeof(int));
        delete[] lookupTable;
    }
    {
        std::cout << "generating sine table..." << std::endl;
        float* sineTable = new float[NUM_SAMPLES]();
        for (int i = 0; i < NUM_SAMPLES; i++) {
            float angle = 2.0 * PI / NUM_DIVISIONS * i;
            float value = std::sin(angle);
            sineTable[i] = value;
        }
        std::ofstream ofs(SINE_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(sineTable), NUM_SAMPLES * sizeof(float));
        delete[] sineTable;
    }
    {
        std::cout << "generating white-noise table..." << std::endl;
        std::random_device rnd;
        std::mt19937 mt(rnd());
        float* noiseTable = new float[NUM_SAMPLES]();
        for (int i = 0; i < NUM_SAMPLES; i++) {
            auto value = (double)mt() / 0xffffffff * 2 - 1.0;
            noiseTable[i] = (float)value;
        }
        std::ofstream ofs(WHITE_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(noiseTable), NUM_SAMPLES * sizeof(float));
        delete[] noiseTable;
    }
    {
        std::cout << "generating pink-noise table..." << std::endl;
        std::random_device rnd;
        std::mt19937 mt(rnd());
        float* noiseTable = new float[NUM_SAMPLES]();
        double pink[7]{};
        for (int i = 0; i < NUM_SAMPLES; i++) {
            auto white = ((double)mt() / 0xffffffff * 2 - 1.0) * 0.5;
            pink[0] = 0.99886 * pink[0] + white * 0.0555179;
            pink[1] = 0.99332 * pink[1] + white * 0.0750759;
            pink[2] = 0.96900 * pink[2] + white * 0.1538520;
            pink[3] = 0.86650 * pink[3] + white * 0.3104856;
            pink[4] = 0.55000 * pink[4] + white * 0.5329522;
            pink[5] = -0.7616 * pink[5] - white * 0.0168980;
            auto value = pink[0] + pink[1] + pink[2] + pink[3] + pink[4] + pink[5] + pink[6] + white * 0.5362;
            pink[6] = white * 0.115926;
            noiseTable[i] = (float)value;
        }
        std::ofstream ofs(PINK_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(noiseTable), NUM_SAMPLES * sizeof(float));
        delete[] noiseTable;
    }
    {
        std::cout << "generating saw table..." << std::endl;
        float* sawTable = new float[128 * NUM_SAMPLES]();
        for (int noteNum = 0; noteNum < 128; noteNum++) {
            float freq = BASE_FREQ * std::pow(2.0, ((float)noteNum - 69) / 12);
            for (int i = 0; i < NUM_SAMPLES; i++) {
                float angle = 2.0 * PI / NUM_DIVISIONS * i;
                float value = 0.0f;
                int numPartials = MAX_FREQ / freq;
                for (int n = 1; n <= numPartials; n++) {
                    float k = (PI / 2) / numPartials;
                    float gibbsFix = std::pow(std::cos((n - 1) * k), 2);
                    value += std::sin(n * angle) / n * gibbsFix;
                }
                sawTable[noteNum * NUM_SAMPLES + i] = value;
            }
        }
        float max = 0.0f;
        // normalize to richest waveform(0)
        for (int i = 0; i < NUM_SAMPLES; i++) {
            max = std::max(std::abs(sawTable[i]), max);
        }
        //        std::cout << "max=" << max << std::endl;
        for (int i = 0; i < 128 * NUM_SAMPLES; i++) {
            sawTable[i] = sawTable[i] / max;
        }
        std::ofstream ofs(SAW_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(sawTable), 128 * NUM_SAMPLES * sizeof(float));
        delete[] sawTable;
    }
    {
        std::cout << "generating parabola table..." << std::endl;
        float* parabolaTable = new float[128 * NUM_SAMPLES]();
        for (int noteNum = 0; noteNum < 128; noteNum++) {
            float freq = BASE_FREQ * std::pow(2.0, ((float)noteNum - 69) / 12);
            for (int i = 0; i < NUM_SAMPLES; i++) {
                float angle = PI / NUM_DIVISIONS * i;
                float value = PI * PI / 3;
                int sign = -1;
                int numPartials = MAX_FREQ / freq;
                for (int n = 1; n <= numPartials; n++) {
                    float k = (PI / 2) / numPartials;
                    float gibbsFix = std::pow(std::cos((n - 1) * k), 2);
                    value += sign * 4 * std::cos(n * angle) / n / n * gibbsFix;
                    sign = -sign;
                }
                parabolaTable[noteNum * NUM_SAMPLES + i] = value;
            }
        }
        float max = 0.0f;
        // normalize to richest waveform(0)
        for (int i = 0; i < NUM_SAMPLES; i++) {
            max = std::max(std::abs(parabolaTable[i]), max);
        }
        //        std::cout << "max=" << max << std::endl;
        for (int i = 0; i < 128 * NUM_SAMPLES; i++) {
            parabolaTable[i] = parabolaTable[i] / max * 2.0f - 1.0f;
        }
        std::ofstream ofs(PARABOLA_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(parabolaTable), 128 * NUM_SAMPLES * sizeof(float));
        delete[] parabolaTable;
    }
    return 0;
}

// ref: https://www.musicdsp.org/en/latest/Synthesis/17-bandlimited-waveform-generation.html
