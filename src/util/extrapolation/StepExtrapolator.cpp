//
// Created by Lukas Tenbrink on 20.02.20.
//

#include "StepExtrapolator.h"

#include <algorithm>

void StepExtrapolator::adjust(std::vector<double> x, std::vector<double> y) {
    double slopeSum = 0.0;

    for (int i = 1; i < x.size(); ++i) {
        slopeSum += (x[i] - x[i - 1]) / (y[i] - y[i - 1]);
    }

    _slope = 1.0 / (slopeSum / (x.size() - 1));
    baselineX = x[x.size() - 1];
}

double StepExtrapolator::extrapolate(double x) {
    return _slope * (x - baselineX);
}

double StepExtrapolator::slope() {
    return _slope;
}
