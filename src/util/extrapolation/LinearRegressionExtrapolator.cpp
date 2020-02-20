//
// Created by Lukas Tenbrink on 20.02.20.
//

#include <util/Polynomial.h>
#include "LinearRegressionExtrapolator.h"

void LinearRegressionExtrapolator::adjust(std::vector<double> x, std::vector<double> y) {
    Polynomial::linearRegression(x, y, &a, &b);
}

double LinearRegressionExtrapolator::extrapolate(double x) {
    return a * x + b;
}

double LinearRegressionExtrapolator::slope() {
    return a;
}
