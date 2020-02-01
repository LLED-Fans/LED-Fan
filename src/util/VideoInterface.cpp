//
// Created by Lukas Tenbrink on 21.01.20.
//

#include <screen/ConcentricCoordinates.h>
#include "VideoInterface.h"
#include "Printf.h"

VideoInterface::VideoInterface(Screen *screen) : screen(screen) {
    jpegDecoder = new JPEGDecoder();
}

bool VideoInterface::acceptRawRGB(File file) {
    if (file.size() != 3 * screen->virtualSize * screen->virtualSize) {
        Printf::ln("Got image of invalid size; resizing unsupported! (%d)", file.size());
        return false;
    }

    auto *buffer = new char[3 * screen->virtualSize];
    for (int y = 0; y < screen->virtualSize; y++) {
        file.readBytes(buffer, 3 * screen->virtualSize);

        for (int x = 0; x < screen->virtualSize; x++) {
            screen->virtualScreen[x * screen->virtualSize + y] =
                CRGB(buffer[x * 3], buffer[x * 3 + 1], buffer[x * 3 + 2]);
        }
    }

    return true;
}

bool VideoInterface::acceptJpeg(File file) {
    auto status = jpegDecoder->decodeFsFile(file);
    if (!status) {
        Printf::ln("Failed to decode JPEG: %d", status);
        return false;
    }

    if (jpegDecoder->width != screen->virtualSize || jpegDecoder->height != screen->virtualSize) {
        Printf::ln("Got image of invalid size; resizing unsupported! (%d, %d)", jpegDecoder->width, jpegDecoder->height);
        return false;
    }

    // See https://github.com/Bodmer/JPEGDecoder/blob/master/examples/Other%20libraries/SPIFFS_Jpeg/JPEG_functions.ino
    auto pImg = JpegDec.pImage;

    int16_t mcu_w = JpegDec.MCUWidth;
    int16_t mcu_h = JpegDec.MCUHeight;
    int32_t max_x = JpegDec.width;
    int32_t max_y = JpegDec.height;

    while ( JpegDec.read()) {
        // Position of image block
        int mcu_x = JpegDec.MCUx * mcu_w;
        int mcu_y = JpegDec.MCUy * mcu_h;

        for (int x = 0; x < max_x, x++;) {
            for (int y = 0; y < max_y, y++;) {
                uint16_t pixel = pImg[x * max_x + y];

                auto pixelX = mcu_x + x;
                auto pixelY = mcu_y + y;

                screen->virtualScreen[pixelX * screen->virtualSize + pixelY] =
                        CRGB(pixel, pixel + 1, pixel + 2);
            }
        }
    }

    Printf::ln("Successfully applied image");

    return true;
}

DynamicJsonDocument *VideoInterface::info() {
    IntRoller *concentricResolution = ConcentricCoordinates::resolution(screen->count / 2);
    int pixelCount = concentricResolution->sum();
    float *rawPixels = ConcentricCoordinates::sampledCartesian(concentricResolution);

    auto doc = new DynamicJsonDocument(
        JSON_OBJECT_SIZE(1)
        + JSON_ARRAY_SIZE(1)
        + pixelCount
    );

    JsonArray pixels = doc->createNestedArray("pixels");
    for (int i = 0; i < pixelCount; ++i) {
        pixels.add(rawPixels[i]);
    }

    return doc;
}
