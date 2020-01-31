//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "Screen.h"

// FIXME This should definitely be per-instance
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LED_PIN 25

Screen::Screen(int ledCount, int virtualSize): count(ledCount), virtualSize(virtualSize) {
    this->leds = new CRGB[ledCount];
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, ledCount)
            .setCorrection(TypicalLEDStrip);

    virtualScreen = new CRGB[virtualSize * virtualSize];
}

void Screen::draw(unsigned long milliseconds, float rotation) {
    switch (mode) {
        default:
            drawScreen(milliseconds, rotation);
            break;
        case demo:
            drawDemo(milliseconds, rotation);
            break;
    }
}

void Screen::drawError() {
    drawValue(0);
}

void Screen::drawValue(float value, float value1, float value2) {
    fill_solid(leds, count, CRGB(
            (int)(value * 255),
            (int)(value1 * 255),
            (int)(value2 * 255)
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
        double dirX = sin(rotation * M_2_PI);
        double dirY = cos(rotation * M_2_PI);

        // 0 to 1
        double relativeX = 0.5 + dirX * pixelOffcenter;
        double relativeY = 0.5 + dirY * pixelOffcenter;

        int x = int(relativeX * virtualSize + 0.5);
        int y = int(relativeY * virtualSize + 0.5);

        leds[i] = virtualScreen[x * virtualSize + y];
    }
    FastLED.show();
}

void Screen::drawDemo(unsigned long milliseconds, float rotation) {
    fill_rainbow(leds, count, milliseconds * 255 / 1000 / 10 + (int)(rotation * 255), 7);
    FastLED.show();
}
