//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_IMAGE_H
#define LED_FAN_IMAGE_H

#include <functional>

class Image {
public:
    static void bilinearSample(std::function<uint8_t*(int, int)> image, uint8_t *dest, int count, float x, float y);

    static inline uint8_t bilinearLerp(uint8_t q00, uint8_t q01, uint8_t q10, uint8_t q11, float xL, float yL, float xR, float yR);
};


#endif //LED_FAN_IMAGE_H
