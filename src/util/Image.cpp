//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Image.h"

void Image::bilinearSample(std::function<uint8_t*(int, int)> image, uint8_t *dest, int count, float x, float y) {
    int x_l = int(x), y_l = int(y);

    const uint8_t *q11 = image(x_l, y_l);
    const uint8_t *q12 = image(x_l, y_l + 1);
    const uint8_t *q21 = image(x_l + 1, y_l);
    const uint8_t *q22 = image(x_l + 1, y_l + 1);

    float xr = x - x_l;
    float yr = y - y_l;

    float xi = 1.f - xr;
    float yi = 1.f - yr;

    for (int i = 0; i < count; i++) {
        dest[i] = bilinearLerp(q11[i], q12[i], q21[i], q22[i], xr, yr, xi, yi);
    }
}

uint8_t Image::bilinearLerp(uint8_t q11, uint8_t q12, uint8_t q21, uint8_t q22, float xr, float yr, float xi, float yi) {
    return q11 * xr * yr +
           q21 * xi * yr +
           q12 * xr * yi +
           q22 * xi * yi;
}
