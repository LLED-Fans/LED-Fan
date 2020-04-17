//
// Created by Lukas Tenbrink on 02.02.20.
//

#ifndef LED_FAN_ARTNETSERVER_H
#define LED_FAN_ARTNETSERVER_H


#include <screen/Screen.h>
#include "AsyncArtnet.h"
#include "ArtnetEndpoint.h"

class ArtnetServer {
public:
    Screen *screen;
    AsyncArtnet<ArtnetEndpoint> *artnet;

    ArtnetServer(Screen *screen);

    void acceptDMX(ArtnetChannelPacket<ArtnetEndpoint> *);
    void acceptSync(IPAddress *remoteIP);

    std::vector<ArtnetEndpoint *> *endpoints();
};


#endif //LED_FAN_ARTNETSERVER_H
