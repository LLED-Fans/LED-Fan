//
// Created by Lukas Tenbrink on 02.02.20.
//

#include "ArtnetServer.h"

using namespace std::placeholders;

ArtnetServer::ArtnetServer(Screen *screen)
        : screen(screen) {
    endpointCount = 2;
    endpoints = new ArtnetEndpoint[2];

    endpoints[0].port = 1200;
    endpoints[0].array = reinterpret_cast<uint8_t *>(screen->concentricScreen);
    endpoints[0].arraySize = screen->concentricResolution->sum() * 3;
    endpoints[0].mode = Screen::Mode::concentric;

    endpoints[1].port = 1201;
    endpoints[1].array = reinterpret_cast<uint8_t *>(screen->cartesianScreen);
    endpoints[1].arraySize = screen->cartesianSize * screen->cartesianSize * 3;
    endpoints[1].mode = Screen::Mode::screen;

    artnets = new AsyncArtnet*[endpointCount];
    for (int i = 0; i < endpointCount; i++) {
        artnets[i] = new AsyncArtnet();

        artnets[i]->artDmxCallback = std::bind(
                &ArtnetServer::acceptDMX, this,
                i, _1, _2, _3, _4, _5
        );
        artnets[i]->artSyncCallback = std::bind(&ArtnetServer::acceptSync, this, i, _1);

        artnets[i]->listen(endpoints[i].port);
    }
}

void ArtnetServer::acceptDMX(int endpointIndex, uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data, IPAddress remoteIP) {
    ArtnetEndpoint &endpoint = endpoints[endpointIndex];

    unsigned int offset = (unsigned int) universe << (uint8_t) 9;
    int arrayCount = endpoint.arraySize - (int) offset;
    if (arrayCount <= 0) {
        return; // Out of scope
    }
    uint8_t *array = endpoint.array + offset;

    memcpy(array, data, _min(arrayCount, length));
    screen->inputTimestamps[endpoint.mode] = screen->lastFrameTime;
}

void ArtnetServer::acceptSync(int endpoint, IPAddress remoteIP) {
    Serial.print("Got Sync: ");
    Serial.print(remoteIP);
}
