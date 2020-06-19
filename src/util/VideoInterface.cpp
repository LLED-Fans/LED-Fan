//
// Created by Lukas Tenbrink on 21.01.20.
//

#include <screen/ConcentricCoordinates.h>
#include <Setup.h>
#include "VideoInterface.h"
#include "Printf.h"

VideoInterface::VideoInterface(Screen *screen, ArtnetServer *artnetServer) : screen(screen),
                                                                             artnetServer(artnetServer) {
    jpegDecoder = new JPEGDecoder();
}

bool VideoInterface::acceptJpeg(File file) {
    auto status = jpegDecoder->decodeFsFile(file);
    if (!status) {
        Printf::ln("Failed to decode JPEG: %d", status);
        return false;
    }

    if (jpegDecoder->width != screen->cartesianResolution || jpegDecoder->height != screen->cartesianResolution) {
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

        for (int x = 0; x < max_x; x++) {
            for (int y = 0; y < max_y; y++) {
                uint16_t pixel = pImg[x * max_x + y];

                auto pixelX = mcu_x + x;
                auto pixelY = mcu_y + y;

                screen->buffer[pixelX * screen->cartesianResolution + pixelY] =
                        CRGB(pixel, pixel + 1, pixel + 2);
            }
        }
    }

    Printf::ln("Successfully applied image");

    return true;
}

DynamicJsonDocument VideoInterface::info() {
    IntRoller *concentricResolution = screen->concentricResolution;

    const std::vector<ArtnetEndpoint *> *endpoints = artnetServer->endpoints();

    DynamicJsonDocument doc(
            JSON_OBJECT_SIZE(2)
            // Cartesian Screen
            + JSON_OBJECT_SIZE(3)
            // Concentric Screen
            + JSON_OBJECT_SIZE(3)
            + JSON_ARRAY_SIZE(8)
            + JSON_ARRAY_SIZE(8)
            + 512 // Capacity for strings
    );

    // ==============================================
    // ================Cartesian=====================
    // ==============================================

    {
        auto object = doc.createNestedObject("cartesian");

        object["net"] = (*endpoints)[0]->net;
        object["width"] = screen->cartesianResolution;
        object["height"] = screen->cartesianResolution;
    }

    // ==============================================
    // ================Concentric=====================
    // ==============================================

    {
        auto object = doc.createNestedObject("concentric");

        object["net"] = (*endpoints)[1]->net;

        auto names = object.createNestedArray("names");
        auto equations = object.createNestedArray("equations");

        names.add("rings");
        equations.add(String(screen->concentricResolution->count));

        names.add("|ring");
        equations.add("rings");

        names.add("radius");
        equations.add(ConcentricCoordinates::radiusExpression(screen->concentricResolution->count));

        names.add("resolution");
        equations.add(String(CONCENTRIC_RESOLUTION_ADD) + "*ring+" + String(CONCENTRIC_RESOLUTION_MIN));

        names.add("|i");
        equations.add("resolution");

        names.add("theta");
        equations.add("(i/resolution)*2*pi");

        names.add("x");
        equations.add("sin(theta)*0.5*radius+0.5");
        names.add("y");
        equations.add("cos(theta)*0.5*radius+0.5");
    }

    return doc;
}

