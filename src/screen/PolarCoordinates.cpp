//
// Created by Lukas Tenbrink on 01.02.20.
//

#include "PolarCoordinates.h"
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/math.h"

// Theta
void PolarCoordinates::asCartesian(float theta, float r, float *x, float *y) {
    // -1 to 1
    *x = sin(theta) * r;
    *y = cos(theta) * r;
}
