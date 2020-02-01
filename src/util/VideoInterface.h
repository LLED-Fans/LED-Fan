//
// Created by Lukas Tenbrink on 21.01.20.
//

#ifndef LED_FAN_VIDEOINTERFACE_H
#define LED_FAN_VIDEOINTERFACE_H

#include <JPEGDecoder.h>
#include <screen/Screen.h>

class VideoInterface {
public:
    JPEGDecoder *jpegDecoder;
    Screen *screen;

    VideoInterface(Screen *screen);

    bool acceptRawRGB(File file);
    bool acceptJpeg(File file);
};


#endif //LED_FAN_VIDEOINTERFACE_H
