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
        demo, cartesian, concentric, count
    };

    enum CartesianSampling {
        nearest, bilinear
    };

    CLEDController *controller;
    const int pin; // Just for output
    int ledCount;
    CRGB *leds;
    float *ringRadii; // -1 to 1

    int overflowWall;

    unsigned long lastUpdateTimestamp;

    Mode mode = demo;

    // Multi-purpose buffer for any input mode
    CRGB *buffer;
    int bufferSize;

    int cartesianResolution;
    CartesianSampling cartesianSampling = bilinear;

    IntRoller *concentricResolution;

    float fixedRotation = -1;
    unsigned long millisecondsPingLeft = 0;
    unsigned long inputTimestamps[Mode::count];

    Screen(CLEDController *controller, int pin, int ledCount, int overflowWall, int cartesianResolution, IntRoller *concentricResolution);

    void draw(unsigned long milliseconds, float rotation);

    void drawDemo(unsigned long milliseconds, float rotation);
    void drawCartesian(unsigned long milliseconds, float rotation);
    void drawConcentric(unsigned long milliseconds, float rotation);
    void drawError();

    void drawRGB(float red, float green = 0, float blue = 0);

    int noteInput(Mode mode);
    int ping();

    void determineMode(unsigned long milliseconds);

    void setCorrection(float ratio);
private:
    fract8 *correction;
};


#endif //LED_FAN_SCREEN_H
