//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_POLYNOMIAL_H
#define LED_FAN_POLYNOMIAL_H


#include "IntRoller.h"
#include <vector>
#include <numeric>

class Polynomial {
public:
    template<typename T>
    static void linearRegression(std::vector<T> x, std::vector<T> y, T *a, T *b) {
        // f(x) = ax + b
        // a = Σ (x_i - x̄) * (y - ȳ) / Σ (x_i - x̄)^2

        T xMean = std::accumulate(x.begin(), x.end(), T(0)) / x.size();
        T yMean = std::accumulate(y.begin(), y.end(), T(0)) / y.size();

        T upper = 0;
        T lower = 0;
        for (int i = 0; i < x.size(); ++i) {
            T xDistance = x[i] - xMean;
            upper += xDistance * (y[i] - yMean);
            lower += xDistance * xDistance;
        }

        *a = upper / lower;
        *b = yMean - *a * xMean;
    }
};


#endif //LED_FAN_POLYNOMIAL_H
