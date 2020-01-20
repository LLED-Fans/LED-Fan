//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SCREEN_H
#define LED_FAN_SCREEN_H

#include <FastLED.h>

class Screen {
public:
    int count;
    CRGB *leds;

    int scrapMode = 0;

    Screen(int ledCount);

    void draw(float rotation);
    void drawError();
};


#endif //LED_FAN_SCREEN_H
