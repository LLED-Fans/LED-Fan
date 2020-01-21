//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_NETWORK_H
#define LED_FAN_NETWORK_H


static const int CONNECT_RETRIES = 5;

class Network {
public:
    static void host(char *ssid, char *password);

    static bool connectToPreset();
    static bool connect(char *ssid, char *password, bool savePreset=true, int retries=CONNECT_RETRIES);

    static bool checkStatus();
};


#endif //LED_FAN_NETWORK_H
