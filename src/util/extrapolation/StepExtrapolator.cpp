//
// Created by Lukas Tenbrink on 20.02.20.
//

#include "StepExtrapolator.h"

#include <algorithm>

void StepExtrapolator::adjust(std::vector<float> x, std::vector<float> y) {
    int n = x.size();
    _slope = (y[n - 1] - y[0]) / (x[n - 1] - x[0]);
    baselineX = x[n - 1];
}

float StepExtrapolator::extrapolate(float x) {
    return _slope * (x - baselineX);
}

float StepExtrapolator::slope() {
    return _slope;
}
