//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SCREEN_H
#define LED_FAN_SCREEN_H

#include <FastLED.h>

class Screen {
public:
    enum Mode {
        demo, screen
    };

    int count;
    CRGB *leds;

    Mode mode = demo;

    int virtualSize;
    CRGB *virtualScreen;

    Screen(int ledCount, int virtualSize);

    void draw(unsigned long milliseconds, float rotation);

    void drawDemo(unsigned long milliseconds, float rotation);
    void drawScreen(unsigned long milliseconds, float rotation);
    void drawError();

    int pin();
};


#endif //LED_FAN_SCREEN_H
