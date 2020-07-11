//
// Created by Lukas Tenbrink on 20.02.20.
//

#include <util/Polynomial.h>
#include "LinearRegressionExtrapolator.h"

void LinearRegressionExtrapolator::adjust(std::vector<float> x, std::vector<float> y) {
    Polynomial::linearRegression(x, y, &a, &b);
}

float LinearRegressionExtrapolator::extrapolate(float x) {
    return a * x + b;
}

float LinearRegressionExtrapolator::slope() {
    return a;
}
