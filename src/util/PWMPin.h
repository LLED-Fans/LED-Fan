
//
// Created by Lukas Tenbrink on 18.04.20.
//

#ifndef LED_FAN_PWMPIN_H
#define LED_FAN_PWMPIN_H


#include <stdint.h>

class PWMPin {
public:
    int pin;
    int pwmChannel;

    PWMPin(int pin, int pwmChannel);

    void setup(double frequency, uint8_t resolution);

    void setConstant(int mode);
    void setRatio(float ratio);
private:
    double frequency;
    uint8_t resolution;
};


#endif //LED_FAN_PWMPIN_H
