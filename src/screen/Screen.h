//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SCREEN_H
#define LED_FAN_SCREEN_H

#include <FastLED.h>
#include <util/IntRoller.h>

class Screen {
public:
    enum Mode {
        demo, screen, concentric
    };

    int count;
    CRGB *leds;

    Mode mode = demo;

    int virtualSize;
    CRGB *virtualScreen;

    IntRoller *concentricResolution;
    CRGB *concentricScreen;

    Screen(int ledCount, int virtualSize);

    void draw(unsigned long milliseconds, float rotation);

    void drawDemo(unsigned long milliseconds, float rotation);
    void drawScreen(unsigned long milliseconds, float rotation);
    void drawConcentric(unsigned long milliseconds, float rotation);
    void drawError();

    int pin();

    void drawValue(float value, float value1 = 0, float value2 = 0);
};


#endif //LED_FAN_SCREEN_H
