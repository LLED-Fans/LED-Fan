//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "Screen.h"
#include "ConcentricCoordinates.h"

// FIXME This should definitely be per-instance
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LED_PIN 25

Screen::Screen(int ledCount, int virtualSize): count(ledCount), virtualSize(virtualSize) {
    this->leds = new CRGB[ledCount];
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, ledCount)
            .setCorrection(TypicalLEDStrip);

    virtualScreen = new CRGB[virtualSize * virtualSize];

    concentricResolution = ConcentricCoordinates::resolution(ledCount / 2);
    concentricScreen = new CRGB[concentricResolution->sum()];
}

void Screen::draw(unsigned long milliseconds, float rotation) {
    if (millisecondsPingLeft > 0) {
        drawRGB(((millisecondsPingLeft / 500) % 2) == 0 ? 1 : 0);
        millisecondsPingLeft -= milliseconds - lastFrameTime;
        return;
    }

    if (rotation < 0) {
        drawRGB(0);
        return;
    }

    switch (mode) {
        default:
            drawScreen(milliseconds, rotation);
            break;
        case demo:
            drawDemo(milliseconds, rotation);
            break;
        case concentric:
            drawConcentric(milliseconds, rotation);
            break;
    }
}

void Screen::drawError() {
    drawRGB(0);
}

void Screen::drawRGB(float red, float green, float blue) {
    fill_solid(leds, count, CRGB(
            (int)(red * 255),
            (int)(green * 255),
            (int)(blue * 255)
    ));
    FastLED.show();
}

int Screen::pin() {
    return LED_PIN;
}

void Screen::drawScreen(unsigned long milliseconds, float rotation) {
    for (int i = 0; i < count; i++) {
        // -0.5 to 0.5
        double pixelOffcenter = ((double) i / (double) (count - 1)) - 0.5;
        // -1 to 1
        double dirX = sin(rotation * M_TWOPI);
        double dirY = cos(rotation * M_TWOPI);

        // 0 to 1
        double relativeX = 0.5 + dirX * pixelOffcenter;
        double relativeY = 0.5 + dirY * pixelOffcenter;

        // 0 to virtualSize - 1
        int x = int(relativeX * (virtualSize - 1) + 0.5);
        int y = int(relativeY * (virtualSize - 1) + 0.5);

        leds[i] = virtualScreen[x * virtualSize + y];
    }
    FastLED.show();
}

void Screen::drawDemo(unsigned long milliseconds, float rotation) {
    fill_rainbow(leds, count, milliseconds * 255 / 1000 / 10 + (int)(rotation * 255), 7);
    FastLED.show();
}

void Screen::drawConcentric(unsigned long milliseconds, float rotation) {
    int ringIndex = 0;
    for (int ring = 0; ring < concentricResolution->count; ++ring) {
        int ringResolution = (*concentricResolution)[ring];

        for (int polarity = 0; polarity <= 1; ++polarity) {
            float ledRotation = rotation + polarity * 0.5;
            int pDirection = polarity * 2 - 1;

            float index = ledRotation * ringResolution;

            int leftIndex = ((int)index) % ringResolution;
            int rightIndex = (int)(index + 1) % ringResolution;
            float lerp = fmodf(ledRotation, 1.0);
            float ilerp = 1.0 - lerp;

            auto leftPixel = concentricScreen[leftIndex + ringIndex];
            auto rightPixel = concentricScreen[rightIndex + ringIndex];

            leds[(count - 1) * (1 - polarity) + ring * pDirection] = CRGB(
                leftPixel.r * ilerp + rightPixel.r * lerp,
                leftPixel.g * ilerp + rightPixel.g * lerp,
                leftPixel.b * ilerp + rightPixel.b * lerp
            );

            ringIndex += ringResolution;
        }
    }
}

int Screen::ping() {
    millisecondsPingLeft = 1500;
    return 1500;
}
