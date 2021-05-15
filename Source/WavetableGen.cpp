#include <iostream>
#include <cmath>
#include <fstream>

namespace {
const int NUM_DIVISIONS = 4095;
const int NUM_SAMPLES = NUM_DIVISIONS+1;
const float BASE_FREQ = 440.0f;
const float MAX_FREQ = 22000.0f;
const float PI = 3.141592f;
const std::string LOOKUP_FILE = "data/lookup";
const std::string SAW_FILE = "data/saw";
const std::string PARABOLA_FILE = "data/parabola";
}

int main() {
    {
        std::cout << "generating lookup table..." << std::endl;
        int* lookupTable = new int[22000]();
        int freqIndex = 0;
        for(int noteNum = 0; noteNum < 128; noteNum++) {
            float freq = BASE_FREQ * std::pow(2.0, ((float)noteNum-69)/12);
            while(freqIndex < freq) {
                lookupTable[freqIndex] = noteNum;
                freqIndex++;
            }
        }
        int lastFreqIndex = freqIndex-1;
        while(freqIndex < 22000) {
            lookupTable[freqIndex] = lookupTable[lastFreqIndex];
            freqIndex++;
        }
        std::ofstream ofs(LOOKUP_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(lookupTable), 22000 * sizeof(int));
        delete[] lookupTable;
    }
    {
        std::cout << "generating saw table..." << std::endl;
        float* sawTable = new float[128 * NUM_SAMPLES]();
        for(int noteNum = 0; noteNum < 128; noteNum++) {
            float freq = BASE_FREQ * std::pow(2.0, ((float)noteNum-69)/12);
            for(int i = 0; i < NUM_SAMPLES; i++) {
                float angle = 2.0 * PI / NUM_DIVISIONS * i;
                float value = 0.0f;
                for(int n = 1; freq * n <= MAX_FREQ; n++) {
                    value += std::sin(n * angle) / n;
                }
                sawTable[noteNum * NUM_SAMPLES + i] = value;
            }
        }
        std::ofstream ofs(SAW_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(sawTable), 128 * NUM_SAMPLES * sizeof(float));
        delete[] sawTable;
    }
    {
        std::cout << "generating parabola table..." << std::endl;
        float* parabolaTable = new float[128 * NUM_SAMPLES]();
        for(int noteNum = 0; noteNum < 128; noteNum++) {
            float freq = BASE_FREQ * std::pow(2.0, ((float)noteNum-69)/12);
            for(int i = 0; i < NUM_SAMPLES; i++) {
                float angle = PI / NUM_DIVISIONS * i;
                float value = PI * PI / 3;
                int sign = -1;
                for(int n = 1; freq * n <= MAX_FREQ; n++) {
                    value += sign * 4 * std::cos(n * angle) / n / n;
                    sign = -sign;
                }
                parabolaTable[noteNum * NUM_SAMPLES + i] = value;
            }
        }
        float max = 0.0f;
        // normalize to richest waveform(0)
        for(int i = 0; i < NUM_SAMPLES; i++) {
            max = std::max(std::abs(parabolaTable[i]), max);
        }
        for(int i = 0; i < 128 * NUM_SAMPLES; i++) {
            parabolaTable[i] = parabolaTable[i] / max * 2.0f - 1.0f;
        }
        std::ofstream ofs(PARABOLA_FILE, std::ios::out | std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(parabolaTable), 128 * NUM_SAMPLES * sizeof(float));
        delete[] parabolaTable;
    }
    return 0;
}