//
// Created by Lukas Tenbrink on 01.02.20.
//

#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/math.h"

float* ConcentricCoordinates::sampledCartesian(IntRoller *resolution) {
    float *result = new float[resolution->sum()];

    int i = 0;
    for (int ring = 0; ring < resolution->count; ring++) {
        int ringResolution = (*resolution)[ring];
        for (int pixel = 0; pixel < ringResolution; pixel++) {
            PolarCoordinates::asCartesian(
                (pixel / (float) ringResolution) * M_TWOPI,
                ring / (float) ringResolution,
                &result[i * 2],
                &result[i * 2 + 1]
            );
        }
    }

    return result;
}

IntRoller * ConcentricCoordinates::resolution(int ringCount) {
    int count = 0;
    for (int ring = 0; ring < ringCount; ring++) {
        count += ringResolution(ring);
    }

    IntRoller *resolution = new IntRoller(count);

    for (int ring = 0; ring < ringCount; ring++) {
        resolution->add(ringResolution(ring));
    }

    return resolution;
}

int ConcentricCoordinates::ringResolution(int ring) {
    return 2 + ring * 2;
}
