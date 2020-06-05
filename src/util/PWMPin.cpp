//
// Created by Lukas Tenbrink on 18.04.20.
//

#include <esp32-hal-ledc.h>
#include <esp32-hal-gpio.h>
#include "PWMPin.h"
#include "Logger.h"

PWMPin::PWMPin(int pin, int pwmChannel) : pin(pin), pwmChannel(pwmChannel) {}

void PWMPin::setup(double frequency, uint8_t resolution) {
    ledcSetup(pwmChannel, frequency, resolution);

    this->frequency = frequency;
    this->resolution = resolution;
    this->maxValue = float((1u << resolution) - 1);
}

void PWMPin::setConstant(int mode) {
    if (isAttached) {
        isAttached = false;
        ledcDetachPin(pin);
        pinMode(pin, OUTPUT);
    }

    Logger.println(String(pin) + ": " + String(mode));
    digitalWrite(pin, mode);
}

void PWMPin::setRatio(float ratio) {
    if (ratio < SPEED_CONSTANT_THRESHOLD) {
        setConstant(0);
        return;
    }
    if (ratio > 1.0f - SPEED_CONSTANT_THRESHOLD) {
        setConstant(1);
        return;
    }

    if (!isAttached) {
        isAttached = true;
        ledcAttachPin(pin, pwmChannel);
    }

    Logger.println(String(pin) + ": " + String(ratio));
    ledcWrite(pwmChannel, uint32_t(ratio * maxValue));
}
