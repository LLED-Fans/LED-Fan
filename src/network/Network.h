//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_NETWORK_H
#define LED_FAN_NETWORK_H


static const int CONNECT_RETRIES = 5;

static const char *const STATION_SETUP_FILE = "/wifi/station";

enum WifiMode {
    accessPoint, station
};

class WifiSetup {
public:
    String ssid;
    String password;

    WifiSetup(const String &ssid, const String &password);

    void write(String file);
    bool read(String file);

    bool isComplete();

    bool operator==(const WifiSetup &rhs) const;

    bool operator!=(const WifiSetup &rhs) const;
};

class Network {
public:
    static WifiMode mode;

    static WifiSetup *getSoftApSetup();

    static void setSoftApSetup(WifiSetup *softAPSetup);

    static WifiSetup *getStationSetup();

    static void setStationSetup(WifiSetup *stationSetup);

    static void setHostname(String hostname);
    static void pair();

    static void readConfig();

    static bool connectToStation(int tries=CONNECT_RETRIES);
    static void hostSoftAP();

    static bool checkStatus();
    static IPAddress address();

private:
    static WifiSetup *softAPSetup;
    static WifiSetup *stationSetup;

    static bool needsReconnect;
};


#endif //LED_FAN_NETWORK_H
