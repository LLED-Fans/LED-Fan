//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_NETWORK_H
#define LED_FAN_NETWORK_H


class Network {
public:
    static void host(char *ssid, char *password);
    static bool connect(char *ssid, char *password);
};


#endif //LED_FAN_NETWORK_H
