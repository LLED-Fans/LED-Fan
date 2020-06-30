
//
// Created by Lukas Tenbrink on 18.04.20.
//

#ifndef LED_FAN_PWMPIN_H
#define LED_FAN_PWMPIN_H

static const float SPEED_CONSTANT_THRESHOLD = 0.0001f;

#include <cstdint>

class PWMPin {
public:
    int pin;
    int pwmChannel;
    bool isAttached = false;

    PWMPin(int pin, int pwmChannel);

    void setup(double frequency, uint8_t resolution);

    void setConstant(int mode);
    void setRatio(float ratio);
private:
    double frequency;
    uint8_t resolution;
    float maxValue;
};


#endif //LED_FAN_PWMPIN_H
