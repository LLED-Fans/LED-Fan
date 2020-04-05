//
// Created by Lukas Tenbrink on 02.02.20.
//

#include "ArtnetServer.h"

using namespace std::placeholders;

ArtnetServer::ArtnetServer(Screen *screen)
        : screen(screen) {
    endpointCount = 2;
    endpoints = new ArtnetEndpoint[2];

    endpoints[1].net = 0;
    endpoints[1].mode = Screen::Mode::screen;

    endpoints[0].net = 1;
    endpoints[0].mode = Screen::Mode::concentric;

    artnet = new AsyncArtnet();
    artnet->artDmxCallback = std::bind(
            &ArtnetServer::acceptDMX, this,
            _1, _2, _3, _4, _5
    );
    artnet->artSyncCallback = std::bind(&ArtnetServer::acceptSync, this, _1);
    artnet->listen();
}

void ArtnetServer::acceptDMX(uint16_t universe_raw, uint16_t length, uint8_t sequence, uint8_t *data, IPAddress remoteIP) {
    uint16_t net = universe_raw >> 8;

    if (net >= endpointCount)
        return; // No endpoint for this net!

    uint16_t universe = universe_raw & 0xff;

    ArtnetEndpoint &endpoint = endpoints[net];
    screen->noteInput(endpoint.mode);

    if (screen->mode != endpoint.mode)
        return; // We're in another mode; don't jumble the buffer

    uint8_t *buffer = reinterpret_cast<uint8_t *>(screen->buffer);
    int bufferSize = screen->bufferSize * 3;

    unsigned int offset = (unsigned int) universe << (uint8_t) 9;
    int arrayCount = bufferSize - (int) offset;
    if (arrayCount <= 0) {
        return; // Out of scope
    }
    uint8_t *array = buffer + offset;

    memcpy(array, data, _min(arrayCount, length));
}

void ArtnetServer::acceptSync(IPAddress remoteIP) {
    Serial.print("Got Sync: ");
    Serial.print(remoteIP);
}
