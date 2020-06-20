//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <util/TextFiles.h>
#include <util/Logger.h>
#include "Network.h"

WifiMode Network::mode = WifiMode::accessPoint;
bool Network::needsReconnect = false;
WifiSetup *Network::stationSetup = new WifiSetup("", "");
WifiSetup *Network::softAPSetup = new WifiSetup("", "");

WifiSetup::WifiSetup(const String &ssid, const String &password) : ssid(ssid), password(password) {}

void WifiSetup::write(String file) {
    TextFiles::write(file, ssid + "\n" + password);
}

bool WifiSetup::read(String file) {
    String s = TextFiles::read(file);

    int newline = s.indexOf('\n');
    if (newline <= 0) {
        ssid = password = "";
        return false;
    }

    ssid = s.substring(0, newline);
    password = s.substring(newline + 1);

    return isComplete();
}

bool WifiSetup::isComplete() {
    if (ssid.isEmpty() || password.isEmpty())
        return false;

    String ssidCopy = ssid;
    ssidCopy.trim();
    if (ssidCopy.isEmpty())
        return false;

    String passwordCopy = password;
    passwordCopy.trim();
    if (passwordCopy.isEmpty())
        return false;

    return true;
}

bool WifiSetup::operator==(const WifiSetup &rhs) const {
    return ssid == rhs.ssid &&
           password == rhs.password;
}

bool WifiSetup::operator!=(const WifiSetup &rhs) const {
    return !(rhs == *this);
}

void Network::setHostname(String hostname) {
    WiFi.setHostname(hostname.begin());
}

bool Network::checkStatus() {
    switch (mode) {
        case WifiMode::station:
            if (needsReconnect || WiFi.getMode() != WIFI_MODE_STA || !WiFi.isConnected())
                connectToStation(1);

            break;
        case WifiMode::accessPoint:
            if (needsReconnect || WiFi.getMode() != WIFI_MODE_AP || WiFi.softAPBroadcastIP() == IPAddress())
                hostSoftAP();

            break;
        default:
            break;
    }

    return true;
}

bool Network::connectToStation(int tries) {
    if (tries < 0)
        return false;

    if (!stationSetup->isComplete()) {
        mode = WifiMode::accessPoint;
        Serial.println("Invalid Station Setup! Pairing!");
        return false;
    }

    Serial.println("Connecting to " + stationSetup->ssid + "...");

    needsReconnect = false;
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(stationSetup->ssid.begin(), stationSetup->password.begin());

    for (int i = tries; i > 0; --i) {
        if (WiFi.status() == WL_CONNECTED) {
            break;
        }
        
        if (i <= 1) {
            return false;
        }
        
        delay(1000);
    }

    return true;
}

IPAddress Network::address() {
    if (WiFi.getMode() == WIFI_MODE_STA)
        return WiFi.localIP();

    return WiFi.softAPIP();
}

void Network::pair() {
    Logger.print("Pairing…").ln();

    // Clear current pairing
    setStationSetup(new WifiSetup("", ""));
    mode = WifiMode::accessPoint;
}

void Network::readConfig() {
    if (stationSetup->read(STATION_SETUP_FILE)) {
        mode = WifiMode::station;
        connectToStation(1);
        return;
    }

    mode = WifiMode::accessPoint;
    hostSoftAP();
}

void Network::hostSoftAP() {
    if (!softAPSetup->isComplete()) {
        Serial.println("Failed to host access point!");
        return;
    }

    needsReconnect = false;

    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(softAPSetup->ssid.begin(), softAPSetup->password.begin());
}

WifiSetup *Network::getSoftApSetup() {
    return softAPSetup;
}

void Network::setSoftApSetup(WifiSetup *softAPSetup) {
    if (mode == WifiMode::accessPoint && Network::softAPSetup != softAPSetup)
        needsReconnect = true;

    Network::softAPSetup = softAPSetup;
}

WifiSetup *Network::getStationSetup() {
    return stationSetup;
}

void Network::setStationSetup(WifiSetup *stationSetup) {
    if (mode == WifiMode::station && Network::stationSetup != stationSetup)
        needsReconnect = true;

    Network::stationSetup = stationSetup;
    stationSetup->write(STATION_SETUP_FILE);
}
