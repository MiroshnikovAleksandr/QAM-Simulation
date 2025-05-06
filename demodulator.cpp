#include "Demodulator.h"

Demodulator::Demodulator(Modulator::ModulationType type) : modType(type), modulator(type) {
    initConstellation();
}

vector<uint8_t> Demodulator::demodulate(const vector<complex<double>>& symbols) {
    vector<uint8_t> bits;
    int bits_per_symbol = modulator.getBitsPerSymbol();

    for (const auto& symbol : symbols) {
        uint32_t best_bits = 0;
        double min_dist = numeric_limits<double>::max();

        for (const auto& [bits, point] : constellation) {
            double dist = norm(symbol - point);
            if (dist < min_dist) {
                min_dist = dist;
                best_bits = bits;
            }
        }

        for (int i = bits_per_symbol - 1; i >= 0; i--) {
            bits.push_back((best_bits >> i) & 1);
        }
    }

    return bits;
}

double Demodulator::calculateBER(const vector<uint8_t>& original_bits, const vector<uint8_t>& received_bits) {
    int error_cnt = 0;
    for (size_t i = 0; i < original_bits.size(); i++) {
        if (original_bits[i] != received_bits[i]) {
            error_cnt++;
        }
    }

    return static_cast<double>(error_cnt) / original_bits.size();
}

void Demodulator::initConstellation() {
    int bits_per_symbol = modulator.getBitsPerSymbol();
    int numSymbols = 1 << bits_per_symbol;

    for (int i = 0; i < numSymbols; i++) {
        vector<uint8_t> bits;
        for (int j = bits_per_symbol - 1; j >= 0; --j) {
            bits.push_back((i >> j) & 1);
        }

        auto symbols = modulator.modulate(bits);
        if (!symbols.empty()) {
            constellation[i] = symbols[0];
        }
    }
}