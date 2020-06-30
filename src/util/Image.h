//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_IMAGE_H
#define LED_FAN_IMAGE_H

#include <functional>

class Image {
public:
    static void bilinearSample(std::function<uint8_t*(int, int)> image, uint8_t *dest, int count, float x, float y);
};


#endif //LED_FAN_IMAGE_H
