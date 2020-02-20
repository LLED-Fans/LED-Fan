//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_EXTRAPOLATOR_H
#define LED_FAN_EXTRAPOLATOR_H

#include <vector>

class Extrapolator {
public:
    virtual void adjust(std::vector<double> x, std::vector<double> y) = 0;
    virtual double extrapolate(double x) = 0;
    virtual double slope() = 0;
};


#endif //LED_FAN_EXTRAPOLATOR_H
