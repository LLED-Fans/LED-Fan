//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include <util/util.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <util/TextFiles.h>
#include "Network.h"

void Network::host(char* ssid, char* password) {
    printfln("Setting AP (Access Point)…");

    WiFi.mode(WIFI_AP_STA); // <<< Station AND Access Point
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    printfln("AP IP address: ");
    Serial.println(IP);
}

bool Network::connect(char *ssid, char *password, bool savePreset, int retries) {
    printfln("Connecting to Station: %s...", ssid);
    WiFi.begin(ssid, password);

    for (int i = retries; i >= 0; --i) {
        if (WiFi.status() == WL_CONNECTED) {
            break;
        }
        
        if (i == 0) {
            printfln("Failed to connect!");
            return false;
        }
        
        delay(1000);
    }

    if (savePreset) {
        TextFiles::write("/wifi/ssid", ssid);
        TextFiles::write("/wifi/password", password);
    }

    printfln("Successfully connected!");
    return true;
}

bool Network::connectToPreset() {
    String ssid = TextFiles::read("/wifi/ssid");
    String password = TextFiles::read("/wifi/password");

    if (ssid.isEmpty() || password.isEmpty()) {
        return false;
    }

    return connect(ssid.begin(), password.begin(), false, 2);
}

bool Network::checkStatus() {
    if (WiFi.status() != WL_CONNECTED) {
        return connectToPreset();
    }

    return true;
}
