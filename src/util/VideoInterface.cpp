//
// Created by Lukas Tenbrink on 21.01.20.
//

#include "VideoInterface.h"
#include "Printf.h"

VideoInterface::VideoInterface(Screen *screen) : screen(screen) {
    jpegDecoder = new JPEGDecoder();
}

void VideoInterface::acceptJpeg(uint8_t *data, size_t len) {
    if (!jpegDecoder->decodeArray(data, len)) {
        Printf::ln("Failed to decode JPEG.");
        return;
    }

    if (jpegDecoder->width != screen->virtualSize || jpegDecoder->height != screen->virtualSize) {
        Printf::ln("Got image of invalid size; resizing unsupported! (%d, %d)");
        return;
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
}
