//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_LINEARREGRESSIONEXTRAPOLATOR_H
#define LED_FAN_LINEARREGRESSIONEXTRAPOLATOR_H


#include <cmath>
#include "Extrapolator.h"

class LinearRegressionExtrapolator : public Extrapolator {
    float a = NAN, b = NAN;

    void adjust(std::vector<float> x, std::vector<float> y) override;
    float extrapolate(float x) override;
    float slope() override;
};


#endif //LED_FAN_LINEARREGRESSIONEXTRAPOLATOR_H
