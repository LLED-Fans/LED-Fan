//
// Created by Lukas Tenbrink on 05.07.20.
//

#include <algorithm>
#include <cmath>
#include "Renderer.h"

Renderer::Renderer(size_t pixelCount, size_t overflowWall)
: pixelCount(pixelCount), _overflowWall(overflowWall) {
    rgb = new PRGB[pixelCount]{PRGB::black};
    _rgbOutput = new uint32_t [pixelCount * 3]{0};
    _localBrightness = new float[pixelCount]{1};

    _brightnessLUT = new uint32_t[256];
    _componentLUT = new uint32_t[pixelCount * 3];
}

void Renderer::_flushLUT() {
    for (int c = 0; c < 256; ++c) {
        float desiredValue = powf(float(c), _response) * powf(255.0f, 3 - _response);
        desiredValue *= _brightness;

        // If this is > 255^3, we will get overflows later.
        // Theoretically, someone can set some value > 100
        // And thus upscale the whole picture. We need to cap off.
        _brightnessLUT[c] = std::min(
            uint32_t(desiredValue),
            uint32_t(255 * 255 * 255)
        );
    }

    for (int i = 0; i < pixelCount * 3; ++i) {
        int pIndex = i / 3;
        float desiredValue = _localBrightness[pIndex] * _colorCorrection.components[i % 3];
        _componentLUT[i] = std::min(
            uint32_t(desiredValue),
            uint32_t(255)
        );
    }
}

void Renderer::render() {
    uint64_t totalLightness = 0;

    auto *rgbComponents = reinterpret_cast<uint8_t *>(rgb);
    for (int i = 0; i < pixelCount * 3; ++i) {
        // 0 to 255^4
        uint32_t color = _brightnessLUT[rgbComponents[i]] * _componentLUT[i];
        _rgbOutput[i] = color;
        totalLightness += color;
    }

    if (_maxLightness > 0) {
        float lightnessRatio = _maxLightness / (float(totalLightness) / (255.0f * 255.0f * 255.0f * 255.0f));
        auto lightnessRescale = uint32_t(lroundf(lightnessRatio * 255));

        if (lightnessRescale < 255) {
            // Too much power used, need to rescale :(
            for (int i = 0; i < pixelCount * 3; ++i) {
                _rgbOutput[i] = _rgbOutput[i] / 255 * lightnessRescale;
            }
        }
    }

    _flush();
}

float *Renderer::getLocalBrightness() {
    return _localBrightness;
}

void Renderer::setLocalBrightness(float *brightness) {
    free(_localBrightness);
    _localBrightness = brightness;
    _flushLUT();
}


void Renderer::setResponse(float response) {
    _response = response;
    _flushLUT();
}

float Renderer::getResponse() {
    return _response;
}

void Renderer::setColorCorrection(PRGB correction) {
    _colorCorrection = correction;
    _flushLUT();
}

PRGB Renderer::getColorCorrection() {
    return _colorCorrection;
}

void Renderer::setBrightness(float brightness) {
    _brightness = brightness;
    _flushLUT();
}

float Renderer::getBrightness() {
    return _brightness;
}

float Renderer::getMaxLightness() {
    return _maxLightness;
}

void Renderer::setMaxLightness(float lightness) {
    _maxLightness = lightness;
}
