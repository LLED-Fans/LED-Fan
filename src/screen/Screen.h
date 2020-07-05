//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SCREEN_H
#define LED_FAN_SCREEN_H

static const int MICROS_INPUT_ACTIVE = 5000 * 1000;

#include <util/IntRoller.h>
#include <sensor/RotationSensor.h>
#include <screen/behavior/NativeBehavior.h>
#include "Renderer.h"

class Blade {
public:
    struct Pixel {
        float radius;
        int ringIndex;
        int pixelIndex;

        PRGB *color;

        // Cached for performance
        int concentricResolution;
        PRGB *concentricPointer;
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
    Renderer *renderer;

    int bladeCount;
    Blade **blades;

    unsigned long lastUpdateTimestamp;

    Mode getMode() const;
    void setMode(Mode mode);

    // Multi-purpose buffer for any input mode
    PRGB *buffer;
    int bufferSize;

    int cartesianResolution;
    CartesianSampling cartesianSampling = nearest;

    IntRoller *concentricResolution;

    NativeBehavior *behavior = nullptr;
    unsigned long inputTimestamps[Mode::count];

    Screen(Renderer *renderer, int cartesianResolution, IntRoller *concentricResolution);

    void readConfig();

    void update(unsigned long delayMicros);

    void draw(unsigned long delayMicros);
    void drawDemo();
    void drawCartesian();
    void drawConcentric();

    int noteInput(Mode mode);

    void determineMode(unsigned long microseconds);
    void setRadialCorrection(float ratio);

    int getPixelCount() {
        return renderer->pixelCount;
    }

    float getBrightness() const {
        return renderer->getBrightness();
    };
    void setBrightness(float brightness);

    float getResponse() const {
        return renderer->getResponse();
    };
    void setResponse(float response) {
        renderer->setResponse(response);
    };

protected:
    Mode _mode = demo;

    float _radialCorrection;
};


#endif //LED_FAN_SCREEN_H
