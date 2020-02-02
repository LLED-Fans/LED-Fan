//
// Created by Lukas Tenbrink on 01.02.20.
//

#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/math.h"

float *ConcentricCoordinates::sampledCartesian(IntRoller *resolution, int* count) {
    *count = resolution->sum();
    auto result = new float[*count * 2];

    int i = 0;
    for (int ring = 0; ring < resolution->count; ring++) {
        int ringResolution = (*resolution)[ring];
        for (int pixel = 0; pixel < ringResolution; pixel++) {
            PolarCoordinates::asCartesian(
                (pixel / (float) ringResolution) * M_TWOPI,
                (ring + 0.5) / (float) (ringResolution - 0.5),
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
