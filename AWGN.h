#pragma once
#include <vector>
#include <complex>
#include <random>

using namespace std;

class AWGNChannel {
public:
    AWGNChannel();
    vector<complex<double>> addNoise(const vector<complex<double>>& symbols);
    void setNoiseVariance(double noise_var);

private:
    mt19937 gen;
    normal_distribution<double> dist;
};
