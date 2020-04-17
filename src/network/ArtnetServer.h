//
// Created by Lukas Tenbrink on 02.02.20.
//

#ifndef LED_FAN_ARTNETSERVER_H
#define LED_FAN_ARTNETSERVER_H


#include <screen/Screen.h>
#include <speed/SpeedControl.h>
#include "AsyncArtnet.h"
#include "ArtnetEndpoint.h"

class ArtnetServer {
public:
    AsyncArtnet<ArtnetEndpoint> *artnet;

    Screen *screen;
    SpeedControl *speedControl;

    ArtnetServer(Screen *screen, SpeedControl *speedControl);

    void acceptDMX(ArtnetChannelPacket<ArtnetEndpoint> *);
    void acceptSync(IPAddress *remoteIP);

    std::vector<ArtnetEndpoint *> *endpoints();
};


#endif //LED_FAN_ARTNETSERVER_H
