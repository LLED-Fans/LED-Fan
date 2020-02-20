//
// Created by Lukas Tenbrink on 20.02.20.
//

#include <numeric>
#include "Polynomial.h"

void Polynomial::linearRegression(std::vector<double> x, std::vector<double> y, double *a, double *b) {
    // f(x) = ax + b
    // a = Σ (x_i - x̄) * (y - ȳ) / Σ (x_i - x̄)^2

    double xMean = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
    double yMean = std::accumulate(y.begin(), y.end(), 0.0) / y.size();

    double upper = 0;
    double lower = 0;
    for (int i = 0; i < x.size(); ++i) {
        double xDistance = x[i] - xMean;
        upper += xDistance * (y[i] - yMean);
        lower += xDistance * xDistance;
    }

    *a = upper / lower;
    *b = yMean - *a * xMean;
}
