//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_IMAGE_H
#define LED_FAN_IMAGE_H

#include <functional>

class Image {
public:
    static void bilinearSample(std::function<uint8_t*(int, int)> image, uint8_t *dest, int count, float x, float y);

    static inline uint8_t bilinearLerp(uint8_t q11, uint8_t q12, uint8_t q21, uint8_t q22, float xr, float yr, float xi, float yi);
};


#endif //LED_FAN_IMAGE_H
