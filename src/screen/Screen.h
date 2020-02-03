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
        demo, screen, concentric, count
    };

    int ledCount;
    CRGB *leds;

    Mode mode = demo;

    unsigned long lastFrameTime = 0;

    int cartesianSize;
    CRGB *cartesianScreen;

    IntRoller *concentricResolution;
    CRGB *concentricScreen;

    float fixedRotation = -1;
    unsigned long millisecondsPingLeft = 0;
    unsigned long inputTimestamps[Mode::count];

    Screen(int ledCount, int cartesianSize);

    void draw(unsigned long milliseconds, float rotation);

    void drawDemo(unsigned long milliseconds, float rotation);
    void drawScreen(unsigned long milliseconds, float rotation);
    void drawConcentric(unsigned long milliseconds, float rotation);
    void drawError();

    int pin();

    void drawRGB(float red, float green = 0, float blue = 0);

    int ping();

    void determineMode(unsigned long milliseconds);
};


#endif //LED_FAN_SCREEN_H
