//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_RANDOM_H
#define LED_FAN_RANDOM_H

#include <esp_system.h>

void randomSeed(unsigned long seed)
{
    if(seed != 0) {
        srand(seed);
    }
}

uint32_t random(uint32_t max) {
    return esp_random() % max;
}

uint32_t random(uint32_t min, uint32_t max) {
    return random(max - min) + min;
}

float randomf(float min, float max, uint32_t step=1000) {
    return float(random(uint32_t((max - min) * step) + 1)) / float(step) + min;
}

#endif //LED_FAN_RANDOM_H
