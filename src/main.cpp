//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
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
    Serial.begin(9600);

    screen = new Screen(18);
    rotationSensor = new RotationSensor(new SensorSwitch(MAGNET_PIN));

    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    server = new HttpServer(screen);
}

void loop() {
    unsigned long currentTime = millis();

    float currentRotation = rotationSensor->update(currentTime);
    if (currentRotation > 0 && currentRotation < 5) {
        screen->draw(fmod(currentRotation, 1.0f));
    }
    else {
        // Something is wrong, I can feel it
        screen->drawError();
    }
}