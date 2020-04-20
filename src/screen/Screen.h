//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SCREEN_H
#define LED_FAN_SCREEN_H

#include <FastLED.h>
#include <util/IntRoller.h>
#include <sensor/RotationSensor.h>

class Blade {
public:
    struct Pixel {
        float radius;
        int ringIndex;
        fract8 correction;

        CRGB *color;
        CRGB *concentricPointer;
    };

    float rotationOffset;
    int pixelCount;
    Pixel *pixels;

    Blade(int pixelCoun, float rotationOffset);
};

class Screen {
public:
    enum Mode {
        demo, cartesian, concentric, count
    };

    enum CartesianSampling {
        nearest, bilinear
    };

    RotationSensor *rotationSensor;

    CLEDController *controller;
    const int pin; // Just for output
    int ledCount;
    CRGB *leds;

    int bladeCount;
    Blade **blades;

    int overflowWall;

    unsigned long lastUpdateTimestamp;

    Mode mode = demo;

    // Multi-purpose buffer for any input mode
    CRGB *buffer;
    int bufferSize;

    int cartesianResolution;
    CartesianSampling cartesianSampling = bilinear;

    IntRoller *concentricResolution;

    unsigned long millisecondsPingLeft = 0;
    unsigned long inputTimestamps[Mode::count];

    Screen(CLEDController *controller, int pin, int ledCount, int overflowWall, int cartesianResolution, IntRoller *concentricResolution);

    void draw();

    void drawDemo();
    void drawCartesian();
    void drawConcentric();
    void drawError();

    void drawRGB(float red, float green = 0, float blue = 0);

    int noteInput(Mode mode);
    int ping();

    void determineMode(unsigned long milliseconds);

    void setCorrection(float ratio);
};


#endif //LED_FAN_SCREEN_H
