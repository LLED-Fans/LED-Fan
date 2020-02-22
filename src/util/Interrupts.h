//
// Created by Lukas Tenbrink on 22.02.20.
//

#ifndef LED_FAN_INTERRUPTS_H
#define LED_FAN_INTERRUPTS_H

#include <functional>

#include <esp_attr.h>
#include <esp32-hal-gpio.h>
#include <pins_arduino.h>

std::function<void()> *_interrupt_functions = new std::function<void()>[64];

void IRAM_ATTR _shared_interrupt(void *arg) {
    (*static_cast<std::function<void(void)> *>(arg))();
}

void attachInterruptFunction(uint8_t gpio, std::function<void()> function, int mode) {
    _interrupt_functions[gpio] = function;
    pinMode(gpio, INPUT_PULLUP);
    attachInterruptArg(digitalPinToInterrupt(gpio), _shared_interrupt, &_interrupt_functions[gpio], mode);
}

#endif //LED_FAN_INTERRUPTS_H
