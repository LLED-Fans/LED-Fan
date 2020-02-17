//
// Created by Lukas Tenbrink on 01.02.20.
//

#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/math.h"

float *ConcentricCoordinates::ringRadii(float *result, int count) {
    // a * 0 + b = (1 / 4) / (count - 1)
    // a * count + b = 1

    float b = 0.25f / (float) (count - 1);
    float a = (1 - b) / (float) count;

    for (int i = 0; i < count; i++) {
        // -1 to 1
        result[i] = a * (float) i + b;
    }
}

float *ConcentricCoordinates::sampledCartesian(IntRoller *resolution, float *radii, int* count) {
    *count = resolution->sum();
    auto result = new float[*count * 2];

    int i = 0;
    for (int ring = 0; ring < resolution->count; ring++) {
        int ringResolution = (*resolution)[ring];
        for (int pixel = 0; pixel < ringResolution; pixel++) {
            PolarCoordinates::asCartesian(
                (pixel / (float) ringResolution) * M_TWOPI,
                radii[ring],
                &result[i],
                &result[i + 1],
                true
            );
            i += 2;
        }
    }

    return result;
}

IntRoller * ConcentricCoordinates::resolution(int ringCount) {
    auto *resolution = new IntRoller(ringCount);

    for (int ring = 0; ring < ringCount; ring++) {
        resolution->append(ringResolution(ring));
    }

    return resolution;
}

int ConcentricCoordinates::ringResolution(int ring) {
    return 2 + ring * 2;
}
