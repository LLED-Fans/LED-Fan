//
// Created by Lukas Tenbrink on 22.02.20.
//

#include "InterruptGPIOSwitch.h"
#include <malloc.h>

#include <esp_attr.h>
#include <esp32-hal-gpio.h>
#include <pins_arduino.h>
#include "util/Logger.h"

int __interruptSwitchLastCheckpoint;
unsigned long __interruptSwitchLastVisitTime;

InterruptGPIOSwitch::InterruptGPIOSwitch(const std::vector<int> &pins)
    : pins(pins) {
    checkpointCount = pins.size();

    attach();
}

void IRAM_ATTR __interrupt_switch(void *args) {
    __interruptSwitchLastVisitTime = micros();
    __interruptSwitchLastCheckpoint = (int) args;
}

void InterruptGPIOSwitch::attach() {
    for (auto pin : pins) {
        pinMode(pin, INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(pin), __interrupt_switch, (void *) pin, FALLING);
    }
}

void InterruptGPIOSwitch::update(unsigned long micros, int *visitCheckpoint, unsigned long *visitTime) {
    if (__interruptSwitchLastCheckpoint >= 0) {
        Logger::println("Update");
        Logger::println(__interruptSwitchLastVisitTime);

        *visitCheckpoint = __interruptSwitchLastCheckpoint;
        *visitTime = __interruptSwitchLastVisitTime;

        __interruptSwitchLastCheckpoint = -1;
    }
}
