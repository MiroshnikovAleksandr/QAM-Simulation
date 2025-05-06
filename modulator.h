#pragma once
#include <vector>
#include <complex>
#include <map>
#include <cmath>
#include <stdexcept>

using namespace std;

class Modulator {
public:
    enum ModulationType { QPSK, QAM16, QAM64 };

    Modulator(ModulationType type);
    vector<complex<double>> modulate(const vector<uint8_t>& bits);
    int getBitsPerSymbol() const;

private:
    ModulationType modType;
    map<uint32_t, complex<double>> constellation;

    void initConstellation();
    void initQPSK();
    void initQAM16();
    void initQAM64();
};
