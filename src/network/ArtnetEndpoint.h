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
    virtual ~ArtnetEndpoint() = default;

    ArtnetEndpoint(unsigned int net, long byteLength, const String &name)
    : ArtnetChannel(net << 8, (byteLength + 255) / 256, name), net(net) {}
};

class VisualEndpoint: public ArtnetEndpoint {
public:
    Screen::Mode mode;

    VisualEndpoint(unsigned int net, long byteLength, const String &name, Screen::Mode mode)
            : ArtnetEndpoint(net, byteLength, name), mode(mode) {}
};

class SpeedEndpoint: public ArtnetEndpoint {};

#endif //LED_FAN_ARTNETENDPOINT_H
