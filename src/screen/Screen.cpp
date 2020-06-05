//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <util/Logger.h>
#include <util/Image.h>

#include <cmath>
#include <util/Profiler.h>
#include "Screen.h"
#include "ConcentricCoordinates.h"
#include "PolarCoordinates.h"

// FIXME This should definitely be per-instance

Screen::Screen(CLEDController *controller, int pin, int ledCount, int overflowWall, int cartesianResolution, IntRoller *concentricResolution)
: controller(controller), pin(pin), ledCount(ledCount), overflowWall(overflowWall),
cartesianResolution(cartesianResolution), concentricResolution(concentricResolution) {
    leds = new CRGB[ledCount + overflowWall]{CRGB::Black};
    FastLED.addLeds(controller, leds, ledCount + overflowWall)
            .setCorrection(TypicalLEDStrip);

    // Disable max refresh rates; we set this in Setup.h.
    FastLED.setMaxRefreshRate(0);

    int cartesianBufferSize = cartesianResolution * cartesianResolution;
    int concentricBufferSize = concentricResolution->sum();
    bufferSize = std::max(cartesianBufferSize, concentricBufferSize);
    buffer = new CRGB[bufferSize]{CRGB::Black};

    auto ringRadii = new float[ledCount];
    ConcentricCoordinates::ringRadii(ringRadii, ledCount);

    bladeCount = 2;
    blades = new Blade*[bladeCount];
    for (int b = 0; b < bladeCount; ++b) {
        Blade *blade = blades[b] = new Blade(
            ledCount / bladeCount,
            b / float(bladeCount)
        );
        int bladePolarity = b * 2 - 1;
        int bladeStartLED = ledCount / 2 - (bladePolarity < 0 ? 1 : 0);
        int bladeRingOffset = b == 0 ? 1 : 0;

        for (int p = 0; p < blade->pixelCount; ++p) {
            int ringIdx = bladeRingOffset + p * bladeCount;

            blade->pixels[p] = Blade::Pixel{
                ringRadii[ringIdx],
                ringIdx,
                1,
                leds + bladeStartLED + p * bladePolarity,
                (*concentricResolution)[ringIdx],
                buffer + std::accumulate(concentricResolution->data, concentricResolution->data + ringIdx, 0)
            };
        }
    }

    for (int i = 0; i < Mode::count; ++i)
        inputTimestamps[i] = 0;
}

void Screen::update(unsigned long delayMicros) {
    lastUpdateTimestamp = micros();
    draw(delayMicros);
}

void Screen::draw(unsigned long delayMicros) {
    for (int i = 0; i < overflowWall; ++i) {
        this->leds[ledCount + i] = CRGB::Black;
    }

    if (behavior != nullptr) {
        if (behavior->update(this, delayMicros)) {
            FastLED.show();
            return;
        }

        // Behavior over, reset pointer
        behavior = nullptr;
    }

    if (!rotationSensor->isReliable()) {
        fill_solid(leds, ledCount, CRGB::Black);
        FastLED.show();
        return;
    }

    switch (mode) {
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
    if (microseconds > inputTimestamps[mode] + MICROS_INPUT_ACTIVE) {
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
            mode = mostRecentInput;
        }
    }
}

void Screen::drawCartesian() {
    // Do not query this more than once since we only
    // show at FastLED.show() anyway
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
                int x = std::lround(relativeX * cartesianMax);
                int y = std::lround(relativeY * cartesianMax);

                // For plotting coords
//        Logger.println(ringIndex);
//        Logger.println(x);
//        Logger.println(y);

                *pixel.color = buffer[x + y * cartesianResolution];
            }

            pixel.color->nscale8_video(pixel.correction);
        }
    }

    FastLED.show();
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
                *pixel.color = CRGB::White;
            }
            else {
                fill_rainbow(
                    pixel.color, 1,
                    pixel.radius * 10 + hueShift,
                    0
                );
            }
        }
    }

    FastLED.show();
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
            CRGB leftPixel = pixel.concentricPointer[leftIndex];
            CRGB rightPixel = pixel.concentricPointer[rightIndex];

            auto rightPart = relativeIndex - leftIndex;
            auto leftPart = 1 - rightPart;

            *pixel.color = CRGB(
                leftPixel.r * leftPart + rightPixel.r * rightPart,
                leftPixel.g * leftPart + rightPixel.g * rightPart,
                leftPixel.b * leftPart + rightPixel.b * rightPart
            );
            pixel.color->nscale8_video(pixel.correction);
        }
    }

    FastLED.show();
}

int Screen::noteInput(Mode mode) {
    inputTimestamps[mode] = lastUpdateTimestamp;
    determineMode(lastUpdateTimestamp);
    return lastUpdateTimestamp;
}
void Screen::setCorrection(float correction) {
    for (int b = 0; b < bladeCount; ++b) {
        Blade *blade = blades[b];

        for (int p = 0; p < blade->pixelCount; ++p) {
            Blade::Pixel &pixel = blade->pixels[p];

            if (correction > 0) {
                pixel.correction = fract8(_min(pixel.radius / correction, 1) * 255);
            }
            else {
                pixel.correction = 255;
            }
        }
    }
}

Blade::Blade(int pixelCount, float rotationOffset) {
    this->pixelCount = pixelCount;
    pixels = new Pixel[pixelCount];
    this->rotationOffset = rotationOffset;
}
