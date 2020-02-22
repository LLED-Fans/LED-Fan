//
// Created by Lukas Tenbrink on 22.02.20.
//

#include "InterruptGPIOSwitch.h"
#include <esp32-hal.h>
#include <util/Interrupts.h>

InterruptGPIOSwitch::InterruptGPIOSwitch(const std::vector<int> &pins)
    : pins(pins) {
    checkpointCount = pins.size();
    attach();
}

void InterruptGPIOSwitch::attach() {
    for (int i = 0; i < pins.size(); i++) {
        attachInterruptFunction(pins[i], [this, i]() {
            lastCheckpoint = i;
            lastVisitTime = micros();
        }, FALLING, INPUT_PULLUP);
    }
}

void InterruptGPIOSwitch::update(unsigned long micros, int *visitCheckpoint, unsigned long *visitTime) {
    if (lastCheckpoint >= 0) {
        *visitCheckpoint = lastCheckpoint;
        *visitTime = lastVisitTime;

        lastCheckpoint = 0;
    }
}
