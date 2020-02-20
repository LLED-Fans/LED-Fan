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
    double baselineX = NAN;
    double _slope = NAN;

    void adjust(std::vector<double> x, std::vector<double> y) override;
    double extrapolate(double x) override;
    double slope() override;
};


#endif //LED_FAN_STEPEXTRAPOLATOR_H
