#include "Modulator.h"

Modulator::Modulator(ModulationType type) : modType(type) {
    initConstellation();
}

vector<complex<double>> Modulator::modulate(const vector<uint8_t>& bits) {
    vector<complex<double>> symbols;
    int bits_per_symbol = getBitsPerSymbol();

    if (bits.size() % bits_per_symbol != 0) {
        throw runtime_error("Number of bits must be divisible by bits per symbol");
    }

    for (size_t i = 0; i < bits.size(); i += bits_per_symbol) {
        uint32_t symbol = 0;
        for (int j = 0; j < bits_per_symbol; j++) {
            symbol = (symbol << 1) | bits[i + j];
        }

        symbols.push_back(constellation.at(symbol));
    }

    return symbols;
}

int Modulator::getBitsPerSymbol() const {
    switch (modType) {
        case QPSK:
            return 2;
        case QAM16:
            return 4;
        case QAM64:
            return 6;
        default:
            return 2;
    }
}

void Modulator::initConstellation() {
    switch (modType) {
        case QPSK:
            initQPSK(); 
            break;

        case QAM16:
            initQAM16();
            break;

        case QAM64:
            initQAM64();
            break;
        default:
            initQPSK();
    }
}

void Modulator::initQPSK() {
    const double norm = 1.0 / sqrt(2.0);
    constellation[0b00] = complex<double>(norm, norm);
    constellation[0b01] = complex<double>(norm, -norm);
    constellation[0b10] = complex<double>(-norm, norm);
    constellation[0b11] = complex<double>(-norm, -norm);
}

void Modulator::initQAM16() {
    const double norm = 1.0 / sqrt(10.0);
    for (int i = 0; i < 16; i++) {
        // ??????? ?? 3GPP TS 38.211
        int I = (1 - 2 * (i & 0b1)) * (2 - (1 - 2 * ((i >> 2) & 0b1)));
        int Q = (1 - 2 * ((i >> 1) & 0b1)) * (2 - (1 - 2 * (i >> 3)));

        constellation[i] = complex<double>(I * norm, Q * norm);
    }
}

void Modulator::initQAM64() {
    const double norm = 1.0 / sqrt(42.0);
    for (int i = 0; i < 64; i++) {
        // ??????? ?? 3GPP TS 38.211
        int I = (1 - 2 * (i & 0b1)) * (4 - ((1 - 2 * ((i >> 2) & 0b1)) * (2 - (1 - 2 * ((i >> 4) & 0b1)))));
        int Q = (1 - 2 * ((i >> 1) & 0b1)) * (4 - ((1 - 2 * ((i >> 3) & 0b1)) * (2 - (1 - 2 * (i >> 5)))));

        constellation[i] = complex<double>(I * norm, Q * norm);
    }
}