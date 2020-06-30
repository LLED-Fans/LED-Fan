//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SCREEN_H
#define LED_FAN_SCREEN_H

static const int MICROS_INPUT_ACTIVE = 5000 * 1000;

#include <FastLED.h>
#include <util/IntRoller.h>
#include <sensor/RotationSensor.h>
#include <screen/behavior/NativeBehavior.h>

class Blade {
public:
    struct Pixel {
        float radius;
        int ringIndex;
        fract8 correction;

        CRGB *color;

        // Cached for performance
        int concentricResolution;
        CRGB *concentricPointer;
    };

    float rotationOffset;
    int pixelCount;
    Pixel *pixels;

    Blade(int pixelCount, float rotationOffset);
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

    Mode getMode() const;
    void setMode(Mode mode);

    // Multi-purpose buffer for any input mode
    CRGB *buffer;
    int bufferSize;

    int cartesianResolution;
    CartesianSampling cartesianSampling = bilinear;

    IntRoller *concentricResolution;

    NativeBehavior *behavior = nullptr;
    unsigned long inputTimestamps[Mode::count];

    unsigned long maxLightness;

    Screen(CLEDController *controller, int pin, int ledCount, int overflowWall, int cartesianResolution, IntRoller *concentricResolution);

    void readConfig();

    void update(unsigned long delayMicros);

    void draw(unsigned long delayMicros);
    void drawDemo();
    void drawCartesian();
    void drawConcentric();

    void show();

    int noteInput(Mode mode);

    void determineMode(unsigned long microseconds);
    void setCorrection(float ratio);

    float getBrightness() const;
    void setBrightness(float brightness);
private:
    Mode _mode = demo;
    float correction = 0.0f, brightness = 1.0f;
    void _flushCorrection();
};


#endif //LED_FAN_SCREEN_H
