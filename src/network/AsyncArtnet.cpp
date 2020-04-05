//
// Created by Lukas Tenbrink on 02.02.20.
//

#include "AsyncArtnet.h"

#include "HardwareSerial.h"
#include <WiFi.h>

using namespace std::placeholders;

bool AsyncArtnet::listen(uint16_t port) {
    if (!udp.listen(port)) {
        Serial.println("UDP Listen failed!");
        return false;
    }

    Serial.print("UDP Listening on Port: ");
    Serial.println(port);
    udp.onPacket(std::bind(&AsyncArtnet::accept, this, _1));

    return true;
}

bool AsyncArtnet::accept(AsyncUDPPacket packet) {
    auto remoteIP = packet.remoteIP();
    auto packetSize = packet.length();

    if (packetSize > MAX_BUFFER_ARTNET || packetSize <= 0) {
        Serial.print("Got packet too large to read: ");
        Serial.println(packetSize);
        return 0;
    }

    packet.read(artnetPacket, MAX_BUFFER_ARTNET);

    // Check that packetID is "Art-Net" else ignore
    for (byte i = 0; i < 8; i++) {
        if (artnetPacket[i] != ART_NET_ID[i])
            return 0;
    }

    auto opcode = artnetPacket[8] | artnetPacket[9] << 8;

    if (opcode == ART_DMX) {
        auto sequence = artnetPacket[12];
        auto incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
        auto dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;

        if (artDmxCallback)
            artDmxCallback(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START, remoteIP);
        return ART_DMX;
    }
    if (opcode == ART_POLL) {
        //fill the reply struct, and then send it to the network's broadcast address
        Serial.print("POLL from ");
        Serial.print(remoteIP);
        Serial.print(" broadcast addr: ");
        Serial.println(broadcast);

#if !defined(ARDUINO_SAMD_ZERO) && !defined(ESP8266) && !defined(ESP32)
        IPAddress local_ip = Ethernet.localIP();
#else
        IPAddress local_ip = WiFi.localIP();
#endif
        uint8_t node_ip_address[4];
        node_ip_address[0] = local_ip[0];
        node_ip_address[1] = local_ip[1];
        node_ip_address[2] = local_ip[2];
        node_ip_address[3] = local_ip[3];

        sprintf((char *) id, "Art-Net");
        memcpy(ArtPollReply.id, id, sizeof(ArtPollReply.id));
        memcpy(ArtPollReply.ip, node_ip_address, sizeof(ArtPollReply.ip));

        ArtPollReply.opCode = ART_POLL_REPLY;
        ArtPollReply.port = ART_NET_PORT;

        memset(ArtPollReply.goodinput, 0x08, 4);
        memset(ArtPollReply.goodoutput, 0x80, 4);
        memset(ArtPollReply.porttypes, 0xc0, 4);

        uint8_t shortname[18];
        uint8_t longname[64];
        sprintf((char *) shortname, "Small LLED Fan");
        sprintf((char *) longname, "Small LLED Fan - Cartesian");
        memcpy(ArtPollReply.shortname, shortname, sizeof(shortname));
        memcpy(ArtPollReply.longname, longname, sizeof(longname));

        ArtPollReply.etsaman[0] = 0;
        ArtPollReply.etsaman[1] = 0;
        ArtPollReply.verH = 1;
        ArtPollReply.ver = 0;
        ArtPollReply.subH = 0;
        ArtPollReply.sub = 0;
        ArtPollReply.oemH = 0;
        ArtPollReply.oem = 0xFF;
        ArtPollReply.ubea = 0;
        ArtPollReply.status = 0xd2;
        ArtPollReply.swvideo = 0;
        ArtPollReply.swmacro = 0;
        ArtPollReply.swremote = 0;
        ArtPollReply.style = 0;

        ArtPollReply.numbportsH = 0;
        ArtPollReply.numbports = 4;
        ArtPollReply.status2 = 0x08;

        ArtPollReply.bindip[0] = node_ip_address[0];
        ArtPollReply.bindip[1] = node_ip_address[1];
        ArtPollReply.bindip[2] = node_ip_address[2];
        ArtPollReply.bindip[3] = node_ip_address[3];

        uint8_t swin[4] = {0x00, 0x01, 0x02, 0x03};
        uint8_t swout[4] = {0x00, 0x01, 0x02, 0x03};
        for (uint8_t i = 0; i < 4; i++) {
            ArtPollReply.swout[i] = swout[i];
            ArtPollReply.swin[i] = swin[i];
        }
        sprintf((char *) ArtPollReply.nodereport, "%i DMX output universes active.", ArtPollReply.numbports);

        udp.broadcastTo((uint8_t *)&ArtPollReply, sizeof(ArtPollReply), ART_NET_PORT);
//            udp.send(new AsyncUDPPacket(&udp, (uint8_t *)&ArtPollReply, ))
//            udp.beginPacket(broadcast, ART_NET_PORT);//send the packet to the broadcast address
//            Udp.write((uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
//            Udp.endPacket();

        return ART_POLL;
    }
    if (opcode == ART_SYNC) {
        if (artSyncCallback) artSyncCallback(remoteIP);
        return ART_SYNC;
    }

    Serial.print("Got unrecognized packet with opcode: ");
    Serial.println(opcode);

    return 0;
}

bool AsyncArtnet::print(AsyncUDPPacket packet) {
    Serial.print("UDP Packet Type: ");
    Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
    Serial.print(", From: ");
    Serial.print(packet.remoteIP());
    Serial.print(":");
    Serial.print(packet.remotePort());
    Serial.print(", To: ");
    Serial.print(packet.localIP());
    Serial.print(":");
    Serial.print(packet.localPort());
    Serial.print(", Length: ");
    Serial.print(packet.length());
    Serial.print(", Data: ");
    Serial.write(packet.data(), packet.length());
    Serial.println();
    //reply to the client
//    packet.printf("Got %u bytes of data", packet.length());

    return false;
}

int AsyncArtnet::activePort() {
    return udp.connected() ? port : -1;
}
