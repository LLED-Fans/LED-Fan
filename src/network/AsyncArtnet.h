//
// Created by Lukas Tenbrink on 02.02.20.
//

#ifndef LED_FAN_ASYNCARTNET_H
#define LED_FAN_ASYNCARTNET_H

// UDP specific
#define ART_NET_PORT 6454
// Opcodes
#define ART_POLL 0x2000
#define ART_POLL_REPLY 0x2100
#define ART_DMX 0x5000
#define ART_SYNC 0x5200
// Buffers
#define MAX_BUFFER_ARTNET 530
// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18

#include <vector>
#include <Stream.h> // Will fail without this explicit import
#include <AsyncUDP.h>


struct artnet_reply_s {
    uint8_t  id[8];
    uint16_t opCode;
    uint8_t  ip[4];
    uint16_t port;
    uint8_t  versionH;
    uint8_t  versionL;
    uint8_t  net;
    uint8_t  subNet;
    uint8_t  oemH;
    uint8_t  oem;
    uint8_t  ubea;
    uint8_t  status;
    uint8_t  estaman[2];
    uint8_t  shortname[18];
    uint8_t  longname[64];
    uint8_t  nodereport[64];
    uint8_t  numbportsH;
    uint8_t  numbports;
    uint8_t  porttypes[4];//max of 4 ports per node
    uint8_t  goodinput[4];
    uint8_t  goodoutput[4];
    uint8_t  swin[4];
    uint8_t  swout[4];
    uint8_t  swvideo;
    uint8_t  swmacro;
    uint8_t  swremote;
    uint8_t  sp1;
    uint8_t  sp2;
    uint8_t  sp3;
    uint8_t  style;
    uint8_t  mac[6];
    uint8_t  bindip[4];
    uint8_t  bindindex;
    uint8_t  status2;
    uint8_t  filler[26];
} __attribute__((packed));

class ArtnetChannel {
public:
    long start, length;
    String name;

    ArtnetChannel(long start, long length, const String &name);
};

template <typename T>
class ArtnetChannelPacket {
public:
    T *channel;
    int channelUniverse;

    uint8_t* data;
    uint16_t length;

    uint8_t sequence;

    IPAddress remoteIP;
};

template <typename T>
class AsyncArtnet {
public:
    AsyncUDP udp;
    uint8_t artnetPacket[MAX_BUFFER_ARTNET] = {};
    IPAddress broadcast;
    uint8_t  id[8] = {};

    std::function<void(ArtnetChannelPacket<T> *packet)> artDmxCallback;
    std::function<void(IPAddress *remoteIP)> artSyncCallback;

    std::vector<T*> *channels = new std::vector<T*>();

    bool listen(uint16_t port=ART_NET_PORT);
    bool accept(AsyncUDPPacket packet);

    bool print(AsyncUDPPacket packet);

    int activePort();
private:
    int port = 0;
    ArtnetChannelPacket<T> *channelPacket = new ArtnetChannelPacket<T>();

    struct artnet_reply_s artPollReply = {};
};

#endif //LED_FAN_ASYNCARTNET_H
