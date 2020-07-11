//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_STEPEXTRAPOLATOR_H
#define LED_FAN_STEPEXTRAPOLATOR_H


#include <util/IntRoller.h>
#include <cmath>
#include "Extrapolator.h"

class StepExtrapolator : public Extrapolator {
public:
    float baselineX = NAN;
    float _slope = NAN;

    void adjust(std::vector<float> x, std::vector<float> y) override;
    float extrapolate(float x) override;
    float slope() override;
};


#endif //LED_FAN_STEPEXTRAPOLATOR_H
