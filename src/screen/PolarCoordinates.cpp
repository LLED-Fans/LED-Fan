//
// Created by Lukas Tenbrink on 01.02.20.
//

#include "PolarCoordinates.h"

#include <cmath>
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/math.h"

void PolarCoordinates::asCartesian(float theta, float r, float *x, float *y, bool positive) {
    // -1 to 1
    *x = std::sin(theta) * r;
    *y = std::cos(theta) * r;

    if (positive) {
        // 0 to 1
        *x = *x * 0.5f + 0.5f;
        *y = *y * 0.5f + 0.5f;
    }
}
