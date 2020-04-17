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

#ifdef RTTI_SUPPORTED
    virtual ~ArtnetEndpoint() = default;
#else
    Screen::Mode mode;
#endif

    ArtnetEndpoint(unsigned int net, long byteLength, const String &name
#ifndef RTTI_SUPPORTED
    , Screen::Mode mode
#endif
    )
    : ArtnetChannel(net << 8, (byteLength + 255) / 256, name), net(net)
#ifndef RTTI_SUPPORTED
    , mode(mode)
#endif
    {}
};

#ifdef RTTI_SUPPORTED
class VisualEndpoint: public ArtnetEndpoint {
public:
    Screen::Mode mode;

    VisualEndpoint(unsigned int net, long byteLength, const String &name)
            : ArtnetEndpoint(net, byteLength, name) {}
};

class SpeedEndpoint: public ArtnetEndpoint {};
#endif

#endif //LED_FAN_ARTNETENDPOINT_H
