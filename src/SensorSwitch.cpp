//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <esp32-hal-adc.h>
#include "SensorSwitch.h"

SensorSwitch::SensorSwitch(int pin): pin(pin) {
    peaks = new PeakDetector(1.0f / 50.0f);
    thresholdSwitch = new ThresholdSwitch(0, 0);
}

bool SensorSwitch::test() {
    auto magnetValue = analogRead(pin);

    // Compute magnet value thresholds
    if (peaks->accept(magnetValue)) {
        thresholdSwitch->upper = ((peaks->lower * 1 / 5) + (peaks->upper * 4 / 5));
        thresholdSwitch->lower = ((peaks->lower * 3 / 5) + (peaks->upper * 2 / 5));
        isReliable = thresholdSwitch->upper > thresholdSwitch->lower + 100;
        //printfln("Boundaries! %d", thresholdSwitch.lower, thresholdSwitch.upper);
    }

    return thresholdSwitch->accept(magnetValue);
}
