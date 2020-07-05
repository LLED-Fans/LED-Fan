//
// Created by Lukas Tenbrink on 05.07.20.
//

#ifndef LED_FAN_FASTLEDRENDERER_H
#define LED_FAN_FASTLEDRENDERER_H

#include <FastLED.h>
#include "Renderer.h"

class FastLEDRenderer : public Renderer {
public:
    CLEDController *controller;
    CRGB *fastLEDOutput;

    FastLEDRenderer(size_t pixelCount, size_t overflowWall, CLEDController *controller);

    uint8_t getMaxDynamicColorRescale() const;
    void setMaxDynamicColorRescale(uint8_t maxDynamicColorRescale);
protected:
    uint32_t _maxDynamicColorRescale = 255;

    void _flush() override;
};


#endif //LED_FAN_FASTLEDRENDERER_H
