//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <util/Logger.h>
#include <util/Image.h>
#include "Screen.h"
#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"

// FIXME This should definitely be per-instance

Screen::Screen(CLEDController *controller, int pin, int ledCount, int cartesianSize)
: controller(controller), pin(pin) {
    this->leds = new CRGB[ledCount];
    FastLED.addLeds(controller, leds, ledCount)
            .setCorrection(TypicalLEDStrip);
    // WS2813 support 2000hz, but FastLED doesn't like it
    FastLED.setMaxRefreshRate(2000);

    this->ringRadii = new float[ledCount];
    ConcentricCoordinates::ringRadii(this->ringRadii, ledCount);

    cartesianScreen = new CRGB[cartesianSize * cartesianSize];
    memset(cartesianScreen, 0x00, cartesianSize * cartesianSize * 3);

    concentricResolution = ConcentricCoordinates::resolution(ledCount);
    concentricScreen = new CRGB[concentricResolution->sum()];
    memset(concentricScreen, 0x00, concentricResolution->sum() * 3);

    for (int i = 0; i < Mode::count; ++i)
        inputTimestamps[i] = 0;
}


void Screen::draw(unsigned long milliseconds, float rotation) {
    auto duration = milliseconds - lastFrameTime;
    frameDurations->append(duration);

    determineMode(milliseconds);

    if (millisecondsPingLeft > 0) {
        drawRGB(((millisecondsPingLeft / 500) % 2) == 0 ? 1 : 0);
        millisecondsPingLeft = millisecondsPingLeft > duration
                ? millisecondsPingLeft - duration
                : 0;
        return;
    }

    if (fixedRotation >= 0) {
        rotation = fixedRotation;
    }

    if (rotation < 0) {
        drawRGB(0);
        return;
    }

    switch (mode) {
        default:
            drawCartesian(milliseconds, rotation);
            break;
        case demo:
            drawDemo(milliseconds, rotation);
            break;
        case concentric:
            drawConcentric(milliseconds, rotation);
            break;
    }
}

void Screen::determineMode(unsigned long milliseconds) {
    if (milliseconds > inputTimestamps[mode] + 5000) {
        // No recent signal on input

        Mode mostRecentInput = Mode::count;
        unsigned long mostRecentInputTimestamp = milliseconds - 5000;
        for (int i = 0; i < Mode::count; i++) {
            if (inputTimestamps[i] > mostRecentInputTimestamp) {
                mostRecentInput = static_cast<Mode>(i);
                mostRecentInputTimestamp = inputTimestamps[i];
            }
        }
        if (mostRecentInput != Mode::count) {
            mode = mostRecentInput;
        }
    }
}

void Screen::drawError() {
    drawRGB(0);
}

void Screen::drawRGB(float red, float green, float blue) {
    fill_solid(leds, ledCount, CRGB(
            (int)(red * 255),
            (int)(green * 255),
            (int)(blue * 255)
    ));
    FastLED.show();
}

void Screen::drawCartesian(unsigned long milliseconds, float rotation) {
    for (int i = 0; i < ledCount; i++) {
        int ringIndex =  (int) abs((float) (i - (ledCount / 2)) * 2 + 0.5f);
        int ledPolarity = i < (ledCount / 2) ? -1 : 1;

        // 0 to 1
        float relativeX, relativeY;
        PolarCoordinates::asCartesian(
            rotation * M_TWOPI,
            ringRadii[ringIndex] * ledPolarity,
            &relativeX, &relativeY,
            true
        );

        if (cartesianSampling == bilinear) {
            Image::bilinearSample([this](int x, int y){
                return reinterpret_cast<uint8_t*>(&cartesianScreen[x + y * cartesianSize]);
            }, reinterpret_cast<uint8_t*>(&leds[i]), 3, relativeX, relativeY);
        }
        else {
            // 0 to cartesianSize - 1
            int x = int(relativeX * (cartesianSize - 1) + 0.5f);
            int y = int(relativeY * (cartesianSize - 1) + 0.5f);

            // For plotting coords
//        Logger::println(ringIndex);
//        Logger::println(x);
//        Logger::println(y);

            leds[i] = cartesianScreen[x + y * cartesianSize];
        }
    }
    FastLED.show();
}

void Screen::drawDemo(unsigned long milliseconds, float rotation) {
    int centerLED = ledCount / 2;

    for (int i = 0; i < ledCount; ++i) {
        int distanceFromCenter = abs(i - centerLED);
        auto ledRotation = std::fmod(rotation + (i > centerLED ? 0.5f : 0.0f), 1.0f);

        fill_rainbow(&leds[i], 1,
        distanceFromCenter * 10 + milliseconds * 255 / 1000 / 10 + (int)(ledRotation * 255),
        0
        );
    }
    FastLED.show();
}

void Screen::drawConcentric(unsigned long milliseconds, float rotation) {
    int centerLEDIndex = ledCount / 2;
    int ringArrayIndex = 0;
    for (int ring = 0; ring < concentricResolution->count; ++ring) {
        int ringResolution = (*concentricResolution)[ring];
        int polarity = (ring % 2) == 0 ? 1 : -1;

        float ledRotation = rotation;
        if (polarity < 0)
            ledRotation = std::fmod(ledRotation + 0.5f, 1.0f);

        float relativeIndex = ledRotation * (float) ringResolution;

        int leftIndex = (int) relativeIndex % ringResolution;
        int rightIndex = (int)(leftIndex + 1) % ringResolution;
        float rightPart = std::fmod(relativeIndex, 1.0f);
        float leftPart = 1.0f - rightPart;

        auto leftPixel = concentricScreen[leftIndex + ringArrayIndex];
        auto rightPixel = concentricScreen[rightIndex + ringArrayIndex];

        leds[centerLEDIndex + ((ring + 1) / 2) * polarity] = CRGB(
                leftPixel.r * leftPart + rightPixel.r * rightPart,
                leftPixel.g * leftPart + rightPixel.g * rightPart,
                leftPixel.b * leftPart + rightPixel.b * rightPart
        );

        ringArrayIndex += ringResolution;
    }

    FastLED.show();
}

int Screen::ping() {
    millisecondsPingLeft = 200;
    return 200;
}

