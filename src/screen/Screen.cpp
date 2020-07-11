//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "Screen.h"

#include <util/Logger.h>
#include <util/Image.h>

#include <cmath>
#include <util/Profiler.h>
#include <Setup.h>
#include <util/TextFiles.h>
#include <util/StringRep.h>
#include <numeric>
#include <esp32-hal.h>
#include <screen/behavior/Demo.h>
#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"

// FIXME This should definitely be per-instance

Screen::Screen(Renderer *renderer, int cartesianResolution, IntRoller *concentricResolution)
: renderer(renderer), cartesianResolution(cartesianResolution), concentricResolution(concentricResolution) {
    int cartesianBufferSize = cartesianResolution * cartesianResolution;
    int concentricBufferSize = concentricResolution->sum();
    bufferSize = std::max(cartesianBufferSize, concentricBufferSize);
    buffer = new PRGB[bufferSize]{PRGB::black};

    auto pixelCount = renderer->pixelCount;
    auto ringRadii = new float[pixelCount];
    ConcentricCoordinates::ringRadii(ringRadii, pixelCount);

    bladeCount = 2;
    blades = new Blade*[bladeCount];
    pixels = new Blade::Pixel*[pixelCount];
    for (int b = 0; b < bladeCount; ++b) {
        Blade *blade = blades[b] = new Blade(
                pixelCount / bladeCount,
            b / float(bladeCount)
        );
        int bladePolarity = b * 2 - 1;
        int bladeStartLED = pixelCount / 2 - (bladePolarity < 0 ? 1 : 0);
        int bladeRingOffset = b == 0 ? 1 : 0;

        for (int p = 0; p < blade->pixelCount; ++p) {
            int ringIdx = bladeRingOffset + p * bladeCount;
            int pixelIdx = bladeStartLED + p * bladePolarity;

            blade->pixels[p] = Blade::Pixel {
                ringRadii[ringIdx],
                ringIdx,
                pixelIdx,
                b,
                renderer->rgb + pixelIdx,
                (*concentricResolution)[ringIdx],
                buffer + std::accumulate(concentricResolution->data, concentricResolution->data + ringIdx, 0)
            };
            pixels[ringIdx] = &blade->pixels[p];
        }
    }

    bilinearTraverser = new BilinearTraverser(buffer, cartesianResolution, cartesianResolution);
    cartesianCenter = float(cartesianResolution - 1) * 0.5f;
    bilinearTraverser->x = cartesianCenter;
    bilinearTraverser->y = cartesianCenter;

    for (int i = 0; i < Mode::count; ++i)
        inputTimestamps[i] = ULONG_MAX - MICROS_INPUT_ACTIVE;

    readConfig();
}

void Screen::readConfig() {
    renderer->setBrightness(StringRep::toFloat(TextFiles::readConf("brightness"), 1.0f));
    setResponse(StringRep::toFloat(TextFiles::readConf("response"), 1));
}

void Screen::update(unsigned long delayMicros) {
    lastUpdateTimestamp = micros();
    draw(delayMicros);
}

void Screen::draw(unsigned long delayMicros) {
    if (_mode != none && !hasSignal(micros()))
        _mode = none;  // Switch away from image

    if (_mode == none && behavior == nullptr) {
        // Nothing to show, let's switch back to Demo.
        behavior = new Demo();
    }

    if (behavior != nullptr) {
        auto status = behavior->update(this, delayMicros);

        if (status == NativeBehavior::alive || (status == NativeBehavior::purgatory && _mode == none)) {
            renderer->render();
            return;
        }

        // Behavior over, reset pointer
        behavior = nullptr;
    }

    if (!rotationSensor->isReliable() || _mode == none) {
        memset((void *)renderer->rgb, 0, renderer->pixelCount * 3); // Fill black
        renderer->render();
        return;
    }

    switch (_mode) {
        default:
        case cartesian:
            drawCartesian();
            break;
        case concentric:
            drawConcentric();
            break;
    }
}

void Screen::determineMode(unsigned long microseconds) {
    if (_mode != none && hasSignal(microseconds))
        return;

    // No recent signal on input

    Mode mostRecentInput = Mode::none;
    unsigned long leastDelay = MICROS_INPUT_ACTIVE;
    for (int i = 1; i < Mode::count; i++) {
        unsigned long delay = microseconds - inputTimestamps[i];

        if (delay < leastDelay) {
            mostRecentInput = static_cast<Mode>(i);
            leastDelay = delay;
        }
    }

    if (mostRecentInput != Mode::none) {
        this->setMode(mostRecentInput);
    }
}

bool Screen::hasSignal(unsigned long microseconds) const {
    return (microseconds - inputTimestamps[_mode]) < MICROS_INPUT_ACTIVE;
}

void Screen::drawCartesian() {
    // Do not query this more than once since we only
    // show at renderer->render(); anyway. No use being more "accurate"
    float rotation = rotationSensor->estimatedRotation(micros());

    for (int b = 0; b < bladeCount; ++b) {
        auto blade = blades[b];

        float vectorX, vectorY;
        PolarCoordinates::asCartesian(
            std::fmod((rotation + blade->rotationOffset), 1.0f) * float(M_TWOPI),
            cartesianCenter,
            &vectorX, &vectorY
        );

        if (cartesianSampling == bilinear) {
            bilinearTraverser->begin(vectorX, vectorY);

            for (int p = blade->pixelCount - 1; p >= 0; --p) {
                Blade::Pixel &pixel = blade->pixels[p];
                bilinearTraverser->get(pixel.color, pixel.radius);
            }
        }
        else {
            for (int p = blade->pixelCount - 1; p >= 0; --p) {
                Blade::Pixel &pixel = blade->pixels[p];
                int x = std::lroundf(cartesianCenter + vectorX * pixel.radius);
                int y = std::lroundf(cartesianCenter + vectorY * pixel.radius);

                *pixel.color = buffer[x + y * cartesianResolution];
            }
        }
    }

    renderer->render();
}

void Screen::drawConcentric() {
    float rotation = rotationSensor->estimatedRotation(micros());

    for (int b = 0; b < bladeCount; ++b) {
        auto blade = blades[b];
        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];

            int ringResolution = pixel.concentricResolution;
            float relativeIndex = bladeRotation * (float) ringResolution;

            int leftIndex = int(relativeIndex) % ringResolution;
            int rightIndex = (leftIndex + 1) % ringResolution;

            // Get / Copy pixels
            PRGB leftPixel = pixel.concentricPointer[leftIndex];
            PRGB rightPixel = pixel.concentricPointer[rightIndex];

            auto rightPart = relativeIndex - leftIndex;
            auto leftPart = 1 - rightPart;

            *pixel.color = PRGB(
                leftPixel.r * leftPart + rightPixel.r * rightPart,
                leftPixel.g * leftPart + rightPixel.g * rightPart,
                leftPixel.b * leftPart + rightPixel.b * rightPart
            );
        }
    }

    renderer->render();
}

int Screen::noteInput(Mode mode) {
    inputTimestamps[mode] = lastUpdateTimestamp;
    if (this->_mode != mode)
        determineMode(lastUpdateTimestamp);
    return lastUpdateTimestamp;
}

void Screen::setRadialCorrection(float correction) {
    _radialCorrection = correction;
    auto *localBrightness = new float[renderer->pixelCount];

    if (correction <= 0) {
        for (int i = 0; i < renderer->pixelCount; ++i) {
            localBrightness[i] = 1.0f;
        }
        renderer->setLocalBrightness(localBrightness);
        return;
    }

    // Practically, all LEDs in the middle will be less bright
    // until they match the pixel at 'correction' radius,
    // e.g. at 0.5 the pixel at the middle
    // Everything outside will be less bright.
    for (int b = 0; b < bladeCount; ++b) {
        Blade *blade = blades[b];

        for (int p = 0; p < blade->pixelCount; ++p) {
            Blade::Pixel &pixel = blade->pixels[p];
            localBrightness[pixel.pixelIndex] = float(pixel.radius) / correction;
        }
    }

    renderer->setLocalBrightness(localBrightness);
}

void Screen::setBrightness(float brightness) {
    // Let's not go overboard with the brightness
    renderer->setBrightness(std::min(brightness, 255.0f));
    TextFiles::writeConf("brightness", String(brightness));
}

float Screen::getResponse() const {
    return renderer->getResponse() - float(NATURAL_COLOR_RESPONSE) + 1;
}

void Screen::setResponse(float response) {
    response = std::max(1.0f, std::min(10.0f, response));
    TextFiles::writeConf("response", String(response));

    response += float(NATURAL_COLOR_RESPONSE) - 1;
    renderer->setResponse(response);
}

Screen::Mode Screen::getMode() const {
    return _mode;
}

void Screen::setMode(Mode mode) {
    if (this->_mode != mode && mode < Mode::count) {
        for (int i = 0; i < bufferSize; i++)
            buffer[i] = PRGB::black;

        this->_mode = mode;
    }
}

Blade::Blade(int pixelCount, float rotationOffset) {
    this->pixelCount = pixelCount;
    pixels = new Pixel[pixelCount];
    this->rotationOffset = rotationOffset;
}
