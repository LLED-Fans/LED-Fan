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

    readConfig();
}

void Screen::readConfig() {
    setBrightness(StringRep::toFloat(TextFiles::readConf("brightness"), 1.0f));
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
            show();
            return;
        }

        // Behavior over, reset pointer
        behavior = nullptr;
    }

    if (!rotationSensor->isReliable()) {
        fill_solid(leds, ledCount, CRGB::Black);
        show();
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
//        Logger.print(ringIndex).ln();
//        Logger.print(x).ln();
//        Logger.print(y).ln();

                *pixel.color = buffer[x + y * cartesianResolution];
            }
        }
    }

    show();
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

    show();
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
        }
    }

    show();
}

void Screen::show() {
    for (int b = 0; b < bladeCount; ++b) {
        auto blade = blades[b];
        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];
            pixel.color->nscale8_video(pixel.correction);
        }
    }

    if (maxLightness > 0) {
        unsigned long lightness = 0;
        for (int i = 0; i < ledCount; i++) {
            CRGB &led = leds[i];
            lightness += led.r + led.g + led.b;
        }
        // Adjust for global correction
        lightness = lightness * FastLED.getBrightness() / 255;
        FastLED.setBrightness(brightness);

        if (lightness > maxLightness) {
            uint8_t scale = maxLightness * brightness / lightness;
            FastLED.setBrightness(scale);
        }
    }

    FastLED.show();
}

int Screen::noteInput(Mode mode) {
    inputTimestamps[mode] = lastUpdateTimestamp;
    if (this->_mode != mode)
        determineMode(lastUpdateTimestamp);
    return lastUpdateTimestamp;
}

void Screen::setCorrection(float correction) {
    this->correction = correction;
    _flushCorrection();
}

float Screen::getBrightness() const {
    return brightness / 255.0f;
}

void Screen::setBrightness(float brightness) {
    this->brightness = brightness * 255;
    TextFiles::writeConf("brightness", String(brightness));
}

void Screen::_flushCorrection() {
    fract8 maxCorrection = LED_CORRECTION_MIN_COLORS;

    for (int b = 0; b < bladeCount; ++b) {
        Blade *blade = blades[b];

        for (int p = 0; p < blade->pixelCount; ++p) {
            Blade::Pixel &pixel = blade->pixels[p];

            if (correction > 0)
                pixel.correction = fract8(std::min(pixel.radius / correction, 1.0f) * 255);
            else
                pixel.correction = 255;

            pixel.correction = std::max(maxCorrection, pixel.correction);
        }
    }
}

Screen::Mode Screen::getMode() const {
    return _mode;
}

void Screen::setMode(Mode mode) {
    if (this->_mode != mode && mode < Mode::count) {
        for (int i = 0; i < bufferSize; i++)
            buffer[i] = CRGB::Black;

        this->_mode = mode;
    }
}

Blade::Blade(int pixelCount, float rotationOffset) {
    this->pixelCount = pixelCount;
    pixels = new Pixel[pixelCount];
    this->rotationOffset = rotationOffset;
}
