//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_NETWORK_H
#define LED_FAN_NETWORK_H


static const int CONNECT_RETRIES = 5;

enum ConnectStatus {
    accessPoint, station, invalidNetwork
};

class Network {
public:
    static ConnectStatus status;

    static bool host(char *ssid, char *password);
    static void setHostname(char *hostname);

    static bool connectToPreset();
    static bool connect(char *ssid, char *password, bool savePreset=true, int retries=CONNECT_RETRIES);

    static void pair(char *ssid, char *password);

    static bool checkStatus();
    static IPAddress address();
};


#endif //LED_FAN_NETWORK_H
