//
// Created by Lukas Tenbrink on 21.01.20.
//

#ifndef LED_FAN_VIDEOINTERFACE_H
#define LED_FAN_VIDEOINTERFACE_H

#include <JPEGDecoder.h>
#include <ArduinoJson.h>
#include <screen/Screen.h>
#include <network/ArtnetServer.h>

class VideoInterface {
public:
    JPEGDecoder *jpegDecoder;
    Screen *screen;
    ArtnetServer *artnetServer;

    VideoInterface(Screen *screen, ArtnetServer *artnetServer);

    DynamicJsonDocument info();

    bool acceptJpeg(File file);
};


#endif //LED_FAN_VIDEOINTERFACE_H
