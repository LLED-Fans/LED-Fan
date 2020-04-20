//
// Created by Lukas Tenbrink on 01.02.20.
//

#include "PolarCoordinates.h"

#include <cmath>
#include <util/LUT.h>
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/math.h"

void PolarCoordinates::asCartesian(float theta, float r, float *x, float *y, bool positive) {
    // -r to r
    *x = LUT::sin->lookup(theta) * r;
    *y = LUT::cos->lookup(theta) * r;

    if (positive) {
        // 0 to 1, assuming -1 < r < 1
        *x = *x * 0.5f + 0.5f;
        *y = *y * 0.5f + 0.5f;
    }
}
