//
// Created by Lukas Tenbrink on 21.01.20.
//

#ifndef LED_FAN_VIDEOINTERFACE_H
#define LED_FAN_VIDEOINTERFACE_H


#include <JPEGDecoder.h>
#include <Screen.h>

class VideoInterface {
public:
    JPEGDecoder *jpegDecoder;
    Screen *screen;

    VideoInterface(Screen *screen);

    void acceptJpeg(uint8_t *data, size_t len);
};


#endif //LED_FAN_VIDEOINTERFACE_H
