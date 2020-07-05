//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Image.h"
#include "Math.h"

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

    float r00 = xL * yL;
    float r01 = xL * yR;
    float r10 = xR * yL;
    float r11 = xR * yR;

    for (int i = 0; i < count; i++) {
        dest[i] = q00[i] * r00 +
                  q10[i] * r10 +
                  q01[i] * r01 +
                  q11[i] * r11;
    }
}

BilinearTraverser::BilinearTraverser(PRGB *image, int width, int height)
: image(image), width(width), height(height) {
}

void BilinearTraverser::begin(float vecX, float vecY) {
    this->vecX = vecX;
    this->vecY = vecY;
}

void BilinearTraverser::get(PRGB *dest, float distance) {
    float x = this->x + vecX * distance;
    float y = this->y + vecY * distance;

    int xIdxLow = int(x), yIdxLow = int(y);
    int imgIdx = xIdxLow + yIdxLow * width;

    const uint8_t *q00 = image[imgIdx].components;
    const uint8_t *q01 = image[imgIdx + width].components;
    const uint8_t *q10 = image[imgIdx + 1].components;
    const uint8_t *q11 = image[imgIdx + 1 + width].components;

    float xR = x - xIdxLow;
    float yR = y - yIdxLow;

    float xL = 1.f - xR;
    float yL = 1.f - yR;

    float r00 = xL * yL;
    float r01 = xL * yR;
    float r10 = xR * yL;
    float r11 = xR * yR;

    for (int i = 0; i < 3; i++) {
        dest->components[i] =
            float(q00[i]) * r00 +
            float(q10[i]) * r10 +
            float(q01[i]) * r01 +
            float(q11[i]) * r11;
    }
}
