//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_IMAGE_H
#define LED_FAN_IMAGE_H

#include <functional>
#include <screen/Pixels.h>

class Image {
public:
    static void bilinearSample(std::function<uint8_t*(int, int)> image, uint8_t *dest, int count, float x, float y);
};

class BilinearTraverser {
public:
    PRGB *image;
    int width, height;

    float vecX, vecY;
    float x, y;

    BilinearTraverser(PRGB *image, int width, int height);

    void begin(float vecX, float vecY);
    void get(PRGB *dest, float distance);
};


#endif //LED_FAN_IMAGE_H
