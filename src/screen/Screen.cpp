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
#include <HardwareSerial.h>
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
            int ledIndex = bladeStartLED + p * bladePolarity;

            blade->pixels[p] = Blade::Pixel{
                ringRadii[ringIdx],
                ringIdx,
                ledIndex,
                renderer->rgb + ledIndex,
                (*concentricResolution)[ringIdx],
                buffer + std::accumulate(concentricResolution->data, concentricResolution->data + ringIdx, 0)
            };
        }
    }

    for (int i = 0; i < Mode::count; ++i)
        inputTimestamps[i] = 0;

    readConfig();
}

void Screen::readConfig() {
    renderer->setBrightness(StringRep::toFloat(TextFiles::readConf("brightness"), 1.0f));
    setResponse(StringRep::toInt(TextFiles::readConf("response"), 3));
}

void Screen::update(unsigned long delayMicros) {
    lastUpdateTimestamp = micros();
    draw(delayMicros);
}

void Screen::draw(unsigned long delayMicros) {
    if (behavior != nullptr) {
        if (behavior->update(this, delayMicros)) {
            renderer->render();;
            return;
        }

        // Behavior over, reset pointer
        behavior = nullptr;
    }

    if (!rotationSensor->isReliable()) {
        memset((void *)renderer->rgb, 0, renderer->pixelCount * 3); // Fill black
        renderer->render();
        return;
    }

    switch (_mode) {
        default:
            drawCartesian();
            break;
        case demo:
            drawDemo();
            break;
        case concentric:
            drawConcentric();
            break;
    }
}

void Screen::determineMode(unsigned long microseconds) {
    if (microseconds > inputTimestamps[_mode] + MICROS_INPUT_ACTIVE) {
        // No recent signal on input

        Mode mostRecentInput = Mode::count;
        unsigned long mostRecentInputTimestamp = microseconds - MICROS_INPUT_ACTIVE;
        for (int i = 0; i < Mode::count; i++) {
            if (inputTimestamps[i] > mostRecentInputTimestamp) {
                mostRecentInput = static_cast<Mode>(i);
                mostRecentInputTimestamp = inputTimestamps[i];
            }
        }
        if (mostRecentInput != Mode::count) {
            this->setMode(mostRecentInput);
        }
    }
}

void Screen::drawCartesian() {
    // Do not query this more than once since we only
    // show at renderer->render(); anyway. No use being more "accurate"
    float rotation = rotationSensor->estimatedRotation(micros());
    int cartesianMax = cartesianResolution - 1;

    for (int b = 0; b < bladeCount; ++b) {
        auto blade = blades[b];

        float vectorX, vectorY;
        PolarCoordinates::asCartesian(
                std::fmod((rotation + blade->rotationOffset), 1.0f) * float(M_TWOPI),
                0.5f,
                &vectorX, &vectorY
        );

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];

            // 0 to 1
            float relativeX = 0.5f + vectorX * pixel.radius;
            float relativeY = 0.5f + vectorY * pixel.radius;

            if (cartesianSampling == bilinear) {
                Image::bilinearSample(
                        [this](int x, int y){
                            return reinterpret_cast<uint8_t*>(&buffer[x + y * cartesianResolution]);
                        },
                        reinterpret_cast<uint8_t*>(pixel.color), 3,
                        relativeX * cartesianMax,
                        relativeY * cartesianMax
                );
            }
            else {
                // 0 to cartesianSize - 1
                int x = std::lroundf(relativeX * cartesianMax);
                int y = std::lroundf(relativeY * cartesianMax);

                // For plotting coords
//        Logger.print(ringIndex).ln();
//        Logger.print(x).ln();
//        Logger.print(y).ln();

                *pixel.color = buffer[x + y * cartesianResolution];
            }
        }
    }

    renderer->render();
}

void Screen::drawDemo() {
    float rotation = rotationSensor->estimatedRotation(micros());
    auto milliseconds = millis();

    for (int b = 0; b < bladeCount; ++b) {
        auto blade = blades[b];
        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);

        auto hueShift = milliseconds * 255 / 1000 / 10  + (unsigned long)(bladeRotation * 255);

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];

            if (bladeRotation > 0.75f) {
                *pixel.color = PRGB::white;
            }
            else {
                // TODO Fix
//                fill_rainbow(
//                    pixel.color, 1,
//                    pixel.radius * 10 + hueShift,
//                    0
//                );
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
