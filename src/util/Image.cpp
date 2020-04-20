//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Image.h"

void Image::bilinearSample(std::function<uint8_t*(int, int)> image, uint8_t *dest, int count, float x, float y) {
    int xIdxLow = int(x), yIdxLow = int(y);

    const uint8_t *q00 = image(xIdxLow, yIdxLow);
    const uint8_t *q01 = image(xIdxLow, yIdxLow + 1);
    const uint8_t *q10 = image(xIdxLow + 1, yIdxLow);
    const uint8_t *q11 = image(xIdxLow + 1, yIdxLow + 1);

    float xR = x - xIdxLow;
    float yR = y - yIdxLow;

    float xL = 1.f - xR;
    float yL = 1.f - yR;

    for (int i = 0; i < count; i++) {
        dest[i] = bilinearLerp(q00[i], q01[i], q10[i], q11[i], xL, yL, xR, yR);
    }
}

uint8_t Image::bilinearLerp(uint8_t q00, uint8_t q01, uint8_t q10, uint8_t q11, float xL, float yL, float xR, float yR) {
    return q00 * xL * yL +
           q10 * xR * yL +
           q01 * xL * yR +
           q11 * xR * yR;
}
