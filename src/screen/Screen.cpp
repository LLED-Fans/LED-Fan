//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <util/Logger.h>
#include <util/Image.h>

#include <cmath>
#include "Screen.h"
#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"

// FIXME This should definitely be per-instance

Screen::Screen(CLEDController *controller, int pin, int ledCount, int cartesianResolution, IntRoller *concentricResolution)
: controller(controller), pin(pin), ledCount(ledCount), cartesianResolution(cartesianResolution), concentricResolution(concentricResolution) {
    this->leds = new CRGB[ledCount];
    FastLED.addLeds(controller, leds, ledCount)
            .setCorrection(TypicalLEDStrip);

    // Disable max refresh rates; we set this in Setup.h.
    FastLED.setMaxRefreshRate(0);

    this->ringRadii = new float[ledCount];
    ConcentricCoordinates::ringRadii(this->ringRadii, ledCount);

    cartesianScreen = new CRGB[cartesianResolution * cartesianResolution];
    memset(cartesianScreen, 0x00, cartesianResolution * cartesianResolution * 3);

    concentricScreen = new CRGB[concentricResolution->sum()];
    memset(concentricScreen, 0x00, concentricResolution->sum() * 3);

    for (int i = 0; i < Mode::count; ++i)
        inputTimestamps[i] = 0;

    correction = new fract8[ledCount]{1};
}


void Screen::draw(unsigned long milliseconds, float rotation) {
    auto delay = milliseconds - lastUpdateTimestamp;
    lastUpdateTimestamp = milliseconds;

    determineMode(milliseconds);

    if (millisecondsPingLeft > 0) {
        drawRGB((((millisecondsPingLeft - 1) / 500) % 2) == 0 ? 0 : 1);
        millisecondsPingLeft = millisecondsPingLeft > delay
                ? millisecondsPingLeft - delay
                : 0;
        return;
    }

    if (fixedRotation >= 0) {
        rotation = fixedRotation;
    }

    if (std::isnan(rotation)) {
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
    for (int ledIndex = 0; ledIndex < ledCount; ledIndex++) {
        int ringIndex =  (int) abs((float) (ledIndex - (ledCount / 2)) * 2 + 0.5f);
        int ledPolarity = ledIndex < (ledCount / 2) ? -1 : 1;

        // 0 to 1
        float relativeX, relativeY;
        PolarCoordinates::asCartesian(
            rotation * M_TWOPI,
            ringRadii[ringIndex] * ledPolarity,
            &relativeX, &relativeY,
            true
        );

        if (cartesianSampling == bilinear) {
            Image::bilinearSample(
                    [this](int x, int y){
                    return reinterpret_cast<uint8_t*>(&cartesianScreen[x + y * cartesianResolution]);
                },
                    reinterpret_cast<uint8_t*>(&leds[ledIndex]), 3,
                relativeX * cartesianResolution,
                relativeY * cartesianResolution
            );
        }
        else {
            // 0 to cartesianSize - 1
            int x = std::lround(relativeX * (cartesianResolution - 1));
            int y = std::lround(relativeY * (cartesianResolution - 1));

            // For plotting coords
//        Logger::println(ringIndex);
//        Logger::println(x);
//        Logger::println(y);

            leds[ledIndex] = cartesianScreen[x + y * cartesianResolution];
        }

        leds[ledIndex].nscale8_video(this->correction[ledIndex]);
    }
    FastLED.show();
}

void Screen::drawDemo(unsigned long milliseconds, float rotation) {
    int centerLED = ledCount / 2;

    for (int ledIndex = 0; ledIndex < ledCount; ++ledIndex) {
        int distanceFromCenter = abs(ledIndex - centerLED);
        auto ledRotation = std::fmod(rotation + (ledIndex > centerLED ? 0.5f : 0.0f), 1.0f);

        fill_rainbow(&leds[ledIndex], 1,
        distanceFromCenter * 10 + milliseconds * 255 / 1000 / 10 + (int)(ledRotation * 255),
                     0
        );
        leds[ledIndex].nscale8_video(this->correction[ledIndex]);
    }
    FastLED.show();
}

void Screen::drawConcentric(unsigned long milliseconds, float rotation) {
    int centerLEDIndex = ledCount / 2;
    int ringArrayIndex = 0;
    for (int ring = 0; ring < concentricResolution->count; ++ring) {
        int polarity = (ring % 2) == 0 ? 1 : -1;
        int ledIndex = centerLEDIndex + ((ring + 1) / 2) * polarity;

        int ringResolution = (*concentricResolution)[ring];

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

        leds[ledIndex] = leftPixel * fract8(leftPart) + rightPixel * fract8(rightPart);
        leds[ledIndex].nscale8_video(this->correction[ledIndex]);

        ringArrayIndex += ringResolution;
    }

    FastLED.show();
}

int Screen::noteInput(Mode mode) {
    return inputTimestamps[mode] = lastUpdateTimestamp;
}

int Screen::ping() {
    millisecondsPingLeft = 2000;
    return 2000;
}

void Screen::setCorrection(float correction) {
    float baselineIBrightness = float(this->ledCount - 1 + 0.25) * correction;

    int centerLEDIndex = ledCount / 2;
    for (int ring = 0; ring < concentricResolution->count; ++ring) {
        int polarity = (ring % 2) == 0 ? 1 : -1;
        int ledIndex = centerLEDIndex + ((ring + 1) / 2) * polarity;

        float iBrightness = float(ring) + 0.25f;
        this->correction[ledIndex] = fract8(_min(iBrightness / baselineIBrightness, 1) * 255);
    }
}
