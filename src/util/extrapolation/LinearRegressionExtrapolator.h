//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_LINEARREGRESSIONEXTRAPOLATOR_H
#define LED_FAN_LINEARREGRESSIONEXTRAPOLATOR_H


#include <cmath>
#include "Extrapolator.h"

class LinearRegressionExtrapolator : public Extrapolator {
    double a = NAN, b = NAN;

    void adjust(std::vector<double> x, std::vector<double> y) override;
    double extrapolate(double x) override;
    double slope() override;
};


#endif //LED_FAN_LINEARREGRESSIONEXTRAPOLATOR_H
