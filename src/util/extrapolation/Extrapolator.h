//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_EXTRAPOLATOR_H
#define LED_FAN_EXTRAPOLATOR_H

#include <vector>

class Extrapolator {
public:
    virtual void adjust(std::vector<float> x, std::vector<float> y) = 0;
    virtual float extrapolate(float x) = 0;
    virtual float slope() = 0;
};


#endif //LED_FAN_EXTRAPOLATOR_H
