//
// Created by Lukas Tenbrink on 05.07.20.
//

#include <algorithm>
#include "FastLEDRenderer.h"

FastLEDRenderer::FastLEDRenderer(size_t pixelCount, size_t overflowWall, CLEDController *controller)
: Renderer(pixelCount, overflowWall), controller(controller) {
    fastLEDOutput = new CRGB[pixelCount+ overflowWall]{CRGB::Black};
    FastLED.addLeds(controller, fastLEDOutput, pixelCount + overflowWall);

    // Disable max refresh rates; we set this in Setup.h.
    FastLED.setMaxRefreshRate(0);
}

void FastLEDRenderer::_flush() {
#if FASTLED_USE_GLOBAL_BRIGHTNESS == 1
    // If this is set, we can make use of global brightness
    // which may broaden our color resolution

    uint32_t peakBrightness;
    for (int i = 0; i < pixelCount * 3; ++i) {
        peakBrightness = std::max(_rgbOutput[i], peakBrightness);
    }

    // 0 to 255
    uint32_t dynamicBrightness = std::max(
        _maxDynamicColorRescale,
        peakBrightness / (255 * 255 * 255)
    );

    auto *rgbComponents = reinterpret_cast<uint8_t *>(fastLEDOutput);
    for (int i = 0; i < pixelCount * 3; ++i) {
        rgbComponents[i] = std::min(
            _rgbOutput[i] / (dynamicBrightness * 255 * 255),
            uint32_t(255) // Since we round, some components may just barely reach 256
        );
    }

    FastLED.setBrightness(dynamicBrightness);
    FastLED.show();
#else
    // No way to increase color resolution;
    // FastLED brightness only rescales our pixels on load.
    // We can just render what we have.

    auto *rgbComponents = reinterpret_cast<uint8_t *>(fastLEDOutput);
    for (int i = 0; i < pixelCount * 3; ++i) {
        rgbComponents[i] = std::min(
            _rgbOutput[i] / (255 * 255 * 255),
            uint32_t(255) // Since we round, some components may just barely reach 256
        );
    }

    FastLED.show();
#endif
}

uint8_t FastLEDRenderer::getMaxDynamicColorRescale() const {
    return _maxDynamicColorRescale;
}

void FastLEDRenderer::setMaxDynamicColorRescale(uint8_t maxDynamicColorRescale) {
    _maxDynamicColorRescale = std::max(uint8_t(1), maxDynamicColorRescale);
}
