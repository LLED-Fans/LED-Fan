//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "ThresholdSwitch.h"

bool ThresholdSwitch::accept(int value) {
    if (isOn && value < lower) {
        isOn = false;
        return true;
    }
    else if (!isOn  && value > upper) {
        isOn = true;
        return true;
    }

    return false;
}
