//
// Created by Lukas Tenbrink on 22.02.20.
//

#include "SyncGPIOSwitch.h"
#include <util/PeakDetector.h>
#include <vector>
#include <esp32-hal.h>

SyncGPIOSwitch::SyncGPIOSwitch(const std::vector<int>& pins, double decay) {
    checkpointCount = pins.size();

    switches.reserve(pins.size());
    for (int magnetPin : pins) {
        switches.push_back(new SensorSwitch(magnetPin, new PeakDetector(decay)));
    }
}

void SyncGPIOSwitch::update(int *visitCheckpoint, unsigned long *visitTime) {
    for (int i = 0; i < switches.size(); ++i) {
        SensorSwitch *sensorSwitch = switches[i];

        // Test the switch
        if (sensorSwitch->test() && sensorSwitch->isReliable) {
            if (!sensorSwitch->isOn()) {
                *visitCheckpoint = i;
                *visitTime = micros();
            }
        }
    }
}
