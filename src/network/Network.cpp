//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <util/TextFiles.h>
#include <util/Printf.h>
#include "Network.h"

ConnectStatus Network::status = ConnectStatus::invalidNetwork;

bool Network::host(char *ssid, char *password) {
    Printf::ln("Setting AP (Access Point)…");

    status = ConnectStatus::accessPoint;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Printf::ln("AP IP address: ");
    Serial.println(IP);

    return true;
}

void Network::setHostname(char *hostname) {
    WiFi.setHostname(hostname);
}

bool Network::connect(char *ssid, char *password, bool savePreset, int retries) {
    Printf::ln("Connecting to Station: %s...", ssid);

    status = ConnectStatus::station;

    if (savePreset) {
        TextFiles::write("/wifi/ssid", ssid);
        TextFiles::write("/wifi/password", password);
    }

    if (retries < 0)
        return false; // Defer run to update

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    for (int i = retries; i >= 0; --i) {
        if (WiFi.status() == WL_CONNECTED) {
            break;
        }
        
        if (i == 0) {
            Printf::ln("Failed to connect!");
            return false;
        }
        
        delay(1000);
    }

    Printf::ln("Successfully connected!");
    return true;
}

bool Network::connectToPreset() {
    String ssid = TextFiles::read("/wifi/ssid");
    String password = TextFiles::read("/wifi/password");

    if (ssid.isEmpty() || password.isEmpty()) {
        status = ConnectStatus::invalidNetwork;
        return false;
    }

    return connect(ssid.begin(), password.begin(), false, 0);
}

bool Network::checkStatus() {
    if (WiFi.status() != WL_CONNECTED && status == ConnectStatus::station) {
        return connectToPreset();
    }

    return true;
}

void Network::pair(char *ssid, char *password) {
    if (status == ConnectStatus::accessPoint)
        return; // Already done

    Printf::ln("Pairing…");

    WiFi.disconnect();
    WiFi.softAPdisconnect();

    TextFiles::write("/wifi/ssid", "");
    TextFiles::write("/wifi/password", "");

    host(ssid, password);
}

IPAddress Network::address() {
    if (WiFi.getMode() == WIFI_MODE_STA)
        return WiFi.localIP();

    return WiFi.softAPIP();
}
