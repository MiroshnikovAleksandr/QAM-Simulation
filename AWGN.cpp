#include "AWGN.h"

AWGNChannel::AWGNChannel() {
    random_device rd;
    gen = mt19937(rd());
}

vector<complex<double>> AWGNChannel::addNoise(const vector<complex<double>>& symbols) {
    vector<complex<double>> noisy_symbols;
    noisy_symbols.reserve(symbols.size());

    for (const auto& symbol : symbols) {
        double noise_I = dist(gen);
        double noise_Q = dist(gen);
        noisy_symbols.emplace_back(symbol.real() + noise_I, symbol.imag() + noise_Q);
    }

    return noisy_symbols;
}

void AWGNChannel::setNoiseVariance(double noise_var) {
    dist = normal_distribution<double>(0.0, sqrt(noise_var));
}