//
// Created by Lukas Tenbrink on 17.04.20.
//

#include "SpeedControl.h"

void SpeedControl::setDesiredSpeed(float speed) {
    currentSpeed = speed;
    // TODO Update outputs etc.
}

SpeedControl::SpeedControl() {
    setDesiredSpeed(currentSpeed);
}
