//
// Created by Lukas Tenbrink on 18.04.20.
//

#include <esp32-hal-ledc.h>
#include "PWMPin.h"

PWMPin::PWMPin(int pin, int pwmChannel) : pin(pin), pwmChannel(pwmChannel) {}

void PWMPin::setup(double frequency, uint8_t resolution) {
    ledcSetup(pwmChannel, frequency, resolution);
    ledcAttachPin(pin, pwmChannel);

    this->frequency = frequency;
    this->resolution = resolution;
}

void PWMPin::setConstant(int mode) {
    setRatio(mode);
}

void PWMPin::setRatio(float ratio) {
    ledcWrite(pwmChannel, uint32_t(ratio * frequency));
}
