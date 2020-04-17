//
// Created by Lukas Tenbrink on 02.02.20.
//

#include "ArtnetServer.h"

using namespace std::placeholders;

ArtnetServer::ArtnetServer(Screen *screen)
        : screen(screen) {
    artnet = new AsyncArtnet<ArtnetEndpoint>();

    artnet->channels->push_back(new ArtnetEndpoint(
        0,
        screen->cartesianResolution * screen->cartesianResolution,
        "Cartesian",
        Screen::Mode::cartesian
    ));

    artnet->channels->push_back(new ArtnetEndpoint(
        1,
        screen->concentricResolution->sum(),
        "Concentric",
        Screen::Mode::concentric
    ));

    artnet->artDmxCallback = std::bind(&ArtnetServer::acceptDMX, this, _1);
    artnet->artSyncCallback = std::bind(&ArtnetServer::acceptSync, this, _1);
    artnet->listen(ART_NET_PORT);
}

void ArtnetServer::acceptDMX(ArtnetChannelPacket<ArtnetEndpoint> *packet) {
    ArtnetEndpoint *endpoint = packet->channel;
    screen->noteInput(endpoint->mode);

    if (screen->mode != endpoint->mode)
        return; // We're in another mode; don't jumble the buffer

    uint8_t *buffer = reinterpret_cast<uint8_t *>(screen->buffer);
    int bufferSize = screen->bufferSize * 3;

    unsigned int offset = (unsigned int) packet->channelUniverse << (uint8_t) 9;
    int arrayCount = bufferSize - (int) offset;
    if (arrayCount <= 0) {
        return; // Out of scope
    }
    uint8_t *array = buffer + offset;

    memcpy(array, packet->data, _min(arrayCount, packet->length));
}

void ArtnetServer::acceptSync(IPAddress *remoteIP) {
    Serial.print("Got Sync: ");
    Serial.print(*remoteIP);
}

std::vector<ArtnetEndpoint *> *ArtnetServer::endpoints() {
    return artnet->channels;
}
