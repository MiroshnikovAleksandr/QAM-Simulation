#pragma once
#include "Modulator.h"
#include <vector>
#include <complex>
#include <map>
#include <limits>

class Demodulator {
public:
    Demodulator(Modulator::ModulationType type);
    vector<uint8_t> demodulate(const vector<complex<double>>& symbols);
    double calculateBER(const vector<uint8_t>& original_bits, const vector<uint8_t>& received_bits);

private:
    Modulator::ModulationType modType;
    Modulator modulator;
    map<uint32_t, complex<double>> constellation;

    void initConstellation();
};
