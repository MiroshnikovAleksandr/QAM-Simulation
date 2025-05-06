#include <iostream>
#include <fstream>
#include <random>
#include "Modulator.h"
#include "AWGN.h"
#include "Demodulator.h"

using namespace std;

void simulateQAMSystem(int num_bits, Modulator::ModulationType modType) {
    vector<uint8_t> original_bits(num_bits);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);
    for (auto& bit : original_bits) {
        bit = dis(gen);
    }

    Modulator modulator(modType);
    AWGNChannel channel;
    Demodulator demodulator(modType);

    vector<double> noise_vars;
    vector<int> snrdbs;
    vector<double> ber_results;

    for (int snrdb = 0; snrdb <= 20; snrdb += 2) {
        double snr = pow(10.0, snrdb / 10.0);
        double noise_var = 1.0 / (2.0 * snr * modulator.getBitsPerSymbol());
        channel.setNoiseVariance(noise_var);

        auto symbols = modulator.modulate(original_bits);
        auto noisy_symbols = channel.addNoise(symbols);
        auto received_bits = demodulator.demodulate(noisy_symbols);
        double ber = demodulator.calculateBER(original_bits, received_bits);

        noise_vars.push_back(noise_var);
        snrdbs.push_back(snrdb);
        ber_results.push_back(ber);

        cout << "SNR: " << snrdb << " dB, BER: " << ber << endl;
    }

    ofstream res_file((modType == Modulator::QPSK) ? "ber_vs_noise_var_QPSK.csv" :
                           (modType == Modulator::QAM16) ? "ber_vs_noise_var_QAM16.csv" : "ber_vs_noise_var_QAM64.csv");
    res_file << "noise_variance,BER\n";
    for (size_t i = 0; i < noise_vars.size(); i++) {
        res_file << noise_vars[i] << "," << ber_results[i] << "\n";
    }
    res_file.close();

    ofstream res_file_snr((modType == Modulator::QPSK) ? "ber_vs_snr_QPSK.csv" :
                               (modType == Modulator::QAM16) ? "ber_vs_snr_QAM16.csv" : "ber_vs_snr_QAM64.csv");
    res_file_snr << "SNR,BER\n";
    for (size_t i = 0; i < snrdbs.size(); i++) {
        res_file_snr << snrdbs[i] << "," << ber_results[i] << "\n";
    }
    res_file_snr.close();
}

int main() {
    try {
        simulateQAMSystem(120000000, Modulator::QPSK);
        cout << "Simulation of QPSK completed successfully. Results saved to ber_vs_noise_var_QPSK.csv" << endl;
        simulateQAMSystem(12000000, Modulator::QAM16);
        cout << "Simulation of QAM16 completed successfully. Results saved to ber_vs_noise_var_QAM16.csv" << endl;
        simulateQAMSystem(6000000, Modulator::QAM64);
        cout << "Simulation of QAM64 completed successfully. Results saved to ber_vs_noise_var_QAM64.csv" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}