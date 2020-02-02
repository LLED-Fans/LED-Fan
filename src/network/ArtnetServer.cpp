//
// Created by Lukas Tenbrink on 02.02.20.
//

#include "ArtnetServer.h"

using namespace std::placeholders;

ArtnetServer::ArtnetServer(VideoInterface *videoInterface, AsyncArtnet *artnet) : videoInterface(videoInterface),
                                                                                  artnet(artnet) {
    artnet->artDmxCallback = std::bind(&ArtnetServer::acceptDMX, this, _1, _2, _3, _4, _5);
    artnet->artSyncCallback = std::bind(&ArtnetServer::acceptSync, this, _1);

    artnet->listen(1234);
}

void ArtnetServer::acceptDMX(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data, IPAddress remoteIP) {
    memcpy(data, videoInterface->screen->concentricScreen, length);
}

void ArtnetServer::acceptSync(IPAddress remoteIP) {
    Serial.print("Got Sync: ");
    Serial.print(remoteIP);
}

