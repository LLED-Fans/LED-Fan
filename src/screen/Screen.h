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

    enum CartesianSampling {
        nearest, bilinear
    };

    CLEDController *controller;
    int pin; // Just for output
    int ledCount;
    CRGB *leds;
    float *ringRadii; // -1 to 1

    Mode mode = demo;

    unsigned long lastFrameTime = 0;
    IntRoller *frameDurations = new IntRoller(50);

    int cartesianSize;
    CRGB *cartesianScreen;
    CartesianSampling cartesianSampling = bilinear;

    IntRoller *concentricResolution;
    CRGB *concentricScreen;

    float fixedRotation = -1;
    unsigned long millisecondsPingLeft = 0;
    unsigned long inputTimestamps[Mode::count];

    Screen(CLEDController *controller, int pin, int ledCount, int cartesianSize);

    void draw(unsigned long milliseconds, float rotation);

    void drawDemo(unsigned long milliseconds, float rotation);
    void drawCartesian(unsigned long milliseconds, float rotation);
    void drawConcentric(unsigned long milliseconds, float rotation);
    void drawError();

    int pin();

    void drawRGB(float red, float green = 0, float blue = 0);

    int ping();

    void determineMode(unsigned long milliseconds);
};


#endif //LED_FAN_SCREEN_H
