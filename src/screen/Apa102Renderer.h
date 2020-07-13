//
// Created by Lukas Tenbrink on 05.07.20.
//

#ifndef LED_FAN_APA102RENDERER_H
#define LED_FAN_APA102RENDERER_H


#include <SPITools.h>
#include "Renderer.h"

struct Apa102Color {
    uint8_t brightness;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

class Apa102Renderer : public Renderer {
public:
    size_t bufferSize;
    uint8_t* buffer;

    SPI_settings_t SPI_settings = {};

    spi_transaction_t transactions[2];
    int currentTransaction = 0;

    Apa102Renderer(size_t pixelCount, size_t overflowWall);

    uint8_t getMaxDynamicColorRescale() const;
    void setMaxDynamicColorRescale(uint8_t maxDynamicColorRescale);
private:
    uint32_t _maxDynamicColorRescale = 255;

    size_t _startBoundary;
    size_t _endBoundary;

    void _prepareBuffer();
    void _flush() override;
};


#endif //LED_FAN_APA102RENDERER_H
