#include <iostream>
#include <tuple>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

double normCDF(double x) {
    return 0.5 * (1 + erf(x / sqrt(2.0)));
}

tuple<double, double, double> asianOptionMC(int n, double S0, double K, int T_days, double sigma, double r) {
    double T = T_days / 255.0;
    double dt = 1.0 / 255.0;
    int steps = T_days;

    mt19937 rng(random_device{}());
    normal_distribution<> norm(0.0, 1.0);

    double sumPayoff_base = 0.0;
    double sumPayoff_delta = 0.0;
    double sumPayoff_vega = 0.0;

    double h = 1e-4;
    double S0_delta = S0 + h;
    double sigma_vega = sigma + h;

    for (int i = 0; i < n; ++i) {
        vector<double> Zs(steps);
        for (int j = 0; j < steps; ++j) {
            Zs[j] = norm(rng);
        }

        // Base path
        double S_base = S0;
        double sum_base = S0;
        for (int j = 1; j < steps; ++j) {
            S_base *= exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Zs[j]);
            sum_base += S_base;
        }
        double avg_base = sum_base / steps;
        sumPayoff_base += max(avg_base - K, 0.0);

        // Delta path (bumped S0)
        double S_delta = S0_delta;
        double sum_delta = S0_delta;
        for (int j = 1; j < steps; ++j) {
            S_delta *= exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Zs[j]);
            sum_delta += S_delta;
        }
        double avg_delta = sum_delta / steps;
        sumPayoff_delta += max(avg_delta - K, 0.0);

        // Vega path (bumped sigma)
        double S_vega = S0;
        double sum_vega = S0;
        for (int j = 1; j < steps; ++j) {
            S_vega *= exp((r - 0.5 * sigma_vega * sigma_vega) * dt + sigma_vega * sqrt(dt) * Zs[j]);
            sum_vega += S_vega;
        }
        double avg_vega = sum_vega / steps;
        sumPayoff_vega += max(avg_vega - K, 0.0);
    }

    double discountFactor = exp(-r * T);
    double npv_base = sumPayoff_base / n * discountFactor;
    double npv_delta = sumPayoff_delta / n * discountFactor;
    double npv_vega = sumPayoff_vega / n * discountFactor;

    double delta = (npv_delta - npv_base) / h;
    double vega = (npv_vega - npv_base) / h;

    return make_tuple(npv_base, delta, vega);
}

double europeanCallPrice(double S0, double K, double T, double sigma, double r) {
    double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S0 * normCDF(d1) - K * exp(-r * T) * normCDF(d2);
}

double europeanCallDelta(double S0, double K, double T, double sigma, double r) {
    double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    return normCDF(d1);
}

int main() {
    int n = 1000000; // Number of Monte Carlo samples
    double S0 = 100.0;
    double K = 103.0;
    int T_days = 255;
    double sigma = 0.1;
    double r = 0.01;

    auto [asianNPV, asianDelta, asianVega] = asianOptionMC(n, S0, K, T_days, sigma, r);

    double T = T_days / 255.0;
    double europeanNPV = europeanCallPrice(S0, K, T, sigma, r);
    double europeanDelta = europeanCallDelta(S0, K, T, sigma, r);

    cout << fixed << setprecision(4);
    cout << "Asian Option NPV: " << asianNPV << endl;
    cout << "Asian Delta: " << asianDelta << endl;
    cout << "Asian Vega: " << asianVega << endl;

    cout << "European Option NPV: " << europeanNPV << endl;
    cout << "European Delta: " << europeanDelta << endl;

    return 0;
}