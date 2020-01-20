//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include <util/util.h>
#include <WiFi.h>
#include "Network.h"

void Network::host(char* ssid, char* password) {
    printfln("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    printfln("AP IP address: ");
    Serial.println(IP);
}

bool Network::connect(char *ssid, char *password) {
    WiFi.begin(ssid, password);

    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }

    printfln("Connected to the WiFi network: %s", ssid);
    return true;
}
