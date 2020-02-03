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

    int endpointCount;
    ArtnetEndpoint *endpoints;
    AsyncArtnet **artnets;

    ArtnetServer(Screen *screen);

    void acceptDMX(int endpointIndex, uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP);
    void acceptSync(int endpoint, IPAddress remoteIP);
};


#endif //LED_FAN_ARTNETSERVER_H
