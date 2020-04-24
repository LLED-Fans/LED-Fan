//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <util/Logger.h>
#include <util/Image.h>

#include <cmath>
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

        for (int p = 0; p < blade->pixelCount; ++p) {
            int ringIdx = b + p * bladeCount;

            blade->pixels[p] = Blade::Pixel{
                ringRadii[ringIdx],
                ringIdx,
                1,
                leds + bladeStartLED + p * bladePolarity,
                buffer + std::accumulate((*concentricResolution).data, (*concentricResolution).data + ringIdx, 0)
            };
        }
    }

    int cartesianBufferSize = cartesianResolution * cartesianResolution;
    int concentricBufferSize = concentricResolution->sum();
    bufferSize = _max(cartesianBufferSize, concentricBufferSize);
    buffer = new CRGB[bufferSize]{CRGB::Black};

    for (int i = 0; i < Mode::count; ++i)
        inputTimestamps[i] = 0;
}


void Screen::draw() {
    auto milliseconds = millis();
    auto delay = milliseconds - lastUpdateTimestamp;
    lastUpdateTimestamp = milliseconds;

    for (int i = 0; i < overflowWall; ++i) {
        this->leds[ledCount + i] = CRGB::Black;
    }

    if (behavior != nullptr) {
        if (behavior->update(leds, ledCount, delay)) {
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

void Screen::determineMode(unsigned long milliseconds) {
    if (milliseconds > inputTimestamps[mode] + 5000) {
        // No recent signal on input

        Mode mostRecentInput = Mode::count;
        unsigned long mostRecentInputTimestamp = milliseconds - 5000;
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
    for (int b = 0; b < bladeCount; ++b) {
        auto blade = blades[b];

        float rotation = rotationSensor->estimatedRotation(micros());

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
                        relativeX * cartesianResolution,
                        relativeY * cartesianResolution
                );
            }
            else {
                // 0 to cartesianSize - 1
                int x = std::lround(relativeX * (cartesianResolution - 1));
                int y = std::lround(relativeY * (cartesianResolution - 1));

                // For plotting coords
//        Logger::println(ringIndex);
//        Logger::println(x);
//        Logger::println(y);

                *pixel.color = buffer[x + y * cartesianResolution];
            }

            pixel.color->nscale8_video(pixel.correction);
        }
    }

    FastLED.show();
}

void Screen::drawDemo() {
    for (int b = 0; b < bladeCount; ++b) {
        float rotation = rotationSensor->estimatedRotation(micros());
        auto milliseconds = millis();

        auto blade = blades[b];
        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];

            if (bladeRotation > 0.75f) {
                *pixel.color = CRGB::White;
            }
            else {
                fill_rainbow(
                    pixel.color, 1,
                    pixel.radius * 10 + milliseconds * 255 / 1000 / 10 + (int)(bladeRotation * 255),
                    0
                );
            }
        }
    }

    FastLED.show();
}

void Screen::drawConcentric() {
    for (int b = 0; b < bladeCount; ++b) {
        float rotation = rotationSensor->estimatedRotation(micros());

        auto blade = blades[b];
        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];

            int ringResolution = (*concentricResolution)[pixel.ringIndex];

            float relativeIndex = bladeRotation * (float) ringResolution;

            int leftIndex = (int) relativeIndex % ringResolution;
            int rightIndex = (int)(leftIndex + 1) % ringResolution;
            float rightPart = std::fmod(relativeIndex, 1.0f);
            float leftPart = 1.0f - rightPart;

            auto leftPixel = pixel.concentricPointer[leftIndex];
            auto rightPixel = pixel.concentricPointer[rightIndex];

            *pixel.color = leftPixel * fract8(leftPart) + rightPixel * fract8(rightPart);
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
