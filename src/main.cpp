//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include <SPIFFS.h>
#include "Screen.h"
#include "SensorSwitch.h"
#include "RotationSensor.h"
#include "Network.h"
#include "HttpServer.h"

#define MAGNET_PIN 13

#define HOST_NETWORK_SSID "LLED Fan"
#define HOST_NETWORK_PASSWORD "We love LED"

Screen *screen;
RotationSensor *rotationSensor;
HttpServer *server;

void setup() {
    // Enable Monitoring
    Serial.begin(9600);

    // Mount file system
    SPIFFS.begin(false);

    // Initialize Screen
    screen = new Screen(18, 64);
    rotationSensor = new RotationSensor(new SensorSwitch(MAGNET_PIN));

    // Initialize Server
    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    Network::connectToPreset();

    server = new HttpServer(screen, rotationSensor);
}

void loop() {
    unsigned long milliseconds = millis();

    float currentRotation = rotationSensor->update(milliseconds);
    if (currentRotation > 0 && currentRotation < 5) {
        screen->draw(milliseconds, fmod(currentRotation, 1.0f));
    }
    else {
        // Something is wrong, I can feel it
        screen->drawError();
    }

    EVERY_N_SECONDS(10) {
        Network::checkStatus();
    }
}