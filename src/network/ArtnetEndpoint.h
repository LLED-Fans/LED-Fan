//
// Created by Lukas Tenbrink on 02.02.20.
//

#ifndef LED_FAN_ARTNETENDPOINT_H
#define LED_FAN_ARTNETENDPOINT_H


#include <screen/Screen.h>
#include <network/AsyncArtnet.h>

class ArtnetEndpoint: public ArtnetChannel {
public:
    unsigned int net;
    Screen::Mode mode;

    ArtnetEndpoint(unsigned int net, long byteLength, const String &name, Screen::Mode mode)
    : ArtnetChannel(net << 8, (byteLength + 255) / 256, name), net(net), mode(mode) {}
};

#endif //LED_FAN_ARTNETENDPOINT_H
