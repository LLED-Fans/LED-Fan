//
// Created by Lukas Tenbrink on 02.02.20.
//

#include <util/Logger.h>
#include "ArtnetServer.h"

using namespace std::placeholders;

ArtnetServer::ArtnetServer(Screen *screen, SpeedControl *speedControl)
: screen(screen), speedControl(speedControl) {
    artnet = new AsyncArtnet<ArtnetEndpoint>();

#ifdef RTTI_SUPPORTED
#define VISUAL_CLASS VisualEndpoint
#define SPEED_CLASS SpeedEndpoint
#else
#define VISUAL_CLASS ArtnetEndpoint
#define SPEED_CLASS ArtnetEndpoint
#endif

    artnet->channels->push_back(new VISUAL_CLASS(
        0,
        screen->cartesianResolution * screen->cartesianResolution * 3,
        "Cartesian",
        Screen::Mode::cartesian
    ));

    auto *concentric = new VISUAL_CLASS(
            1,
            screen->concentricResolution->sum() * 3,
            "Concentric",
            Screen::Mode::concentric
    );
    concentric->isAdvertised = false;
    artnet->channels->push_back(concentric);

    artnet->channels->push_back(new SPEED_CLASS(
        2,
        1,
        "Rotation Speed"
#ifndef RTTI_SUPPORTED
        , Screen::Mode::count
#endif
    ));

    artnet->artDmxCallback = std::bind(&ArtnetServer::acceptDMX, this, _1);
    artnet->artSyncCallback = std::bind(&ArtnetServer::acceptSync, this, _1);
    artnet->listen(ART_NET_PORT);
}

void ArtnetServer::acceptDMX(ArtnetChannelPacket<ArtnetEndpoint> *packet) {
    ArtnetEndpoint *rawEndpoint = packet->channel;

#ifdef RTTI_SUPPORTED
    if (SpeedEndpoint *endpoint = dynamic_cast<SpeedEndpoint*>(rawEndpoint)) {
#else
    if (rawEndpoint->mode >= Screen::Mode::count) {
#endif
        speedControl->setDesiredSpeed((float(packet->data[0]) - 128) / 128);
        return;
    }

#ifdef RTTI_SUPPORTED
    auto endpoint = dynamic_cast<VisualEndpoint*>(rawEndpoint);
#else
    auto endpoint = rawEndpoint;
#endif

    if (!endpoint) {
        WifiLog.print("Error: Unknown Endpoint").ln();
        return;
    }

    screen->noteInput(endpoint->mode);

    if (screen->getMode() != endpoint->mode)
        return; // We're in another mode; don't jumble the buffer

    uint8_t *buffer = reinterpret_cast<uint8_t *>(screen->buffer);
    int bufferSize = screen->bufferSize * 3;

    unsigned int offset = (unsigned int) packet->channelUniverse << (uint8_t) 9;
    if (offset >= bufferSize) {
        return; // Out of scope
    }
    uint16_t arrayCount = bufferSize - (int) offset;
    uint8_t *array = buffer + offset;

    memcpy(array, packet->data, std::min(arrayCount, packet->length));
}

void ArtnetServer::acceptSync(IPAddress *remoteIP) {
    SerialLog.print("Got Sync: ");
    SerialLog.print(*remoteIP).ln();
}

std::vector<ArtnetEndpoint *> *ArtnetServer::endpoints() {
    return artnet->channels;
}
