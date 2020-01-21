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
    fill_solid(leds, count, CRGB::Red);
    FastLED.show();
}

int Screen::pin() {
    return LED_PIN;
}

void Screen::drawScreen(unsigned long milliseconds, float rotation) {
    for (int i = 0; i < count; i++) {
        double relative_x = sin(rotation * M_2_PI);
        double relative_y = cos(rotation * M_2_PI);

        int x = int((relative_x + 1.0) * (double) virtualSize / 2 + 0.5);
        int y = int((relative_y + 1.0) * (double) virtualSize / 2 + 0.5);

        leds[i] = virtualScreen[x * virtualSize + y];
    }
    FastLED.show();
}

void Screen::drawDemo(unsigned long milliseconds, float rotation) {
    fill_rainbow(leds, count, milliseconds / 10 + (int)(rotation * 255), 7);
    FastLED.show();
}
