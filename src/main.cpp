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

#define MAGNET_PIN 33

#define HOST_NETWORK_SSID "LLED Fan"
#define HOST_NETWORK_PASSWORD "We love LED"

#define MICROSECONDS_PER_FRAME 10000

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
    rotationSensor = new RotationSensor(
            new SensorSwitch(MAGNET_PIN, new PeakDetector(MICROSECONDS_PER_FRAME / 1000.0 / 1000.0 / 2.0))
    );

    // Initialize Server
    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    Network::connectToPreset();

    server = new HttpServer(screen, rotationSensor);
}

void loop() {
    unsigned long microseconds = micros();
    auto milliseconds = microseconds / 1000;

    float currentRotation = rotationSensor->update(milliseconds);
    if (rotationSensor->isReliable() && currentRotation > 0 && currentRotation < 5) {
        screen->draw(milliseconds, fmod(currentRotation, 1.0f));
    }
    else {
        // Something is wrong, I can feel it
        screen->drawValue(
            rotationSensor->sensorSwitch->rawValue() ? 0 : 1
        );
    }

    EVERY_N_SECONDS(10) {
        Network::checkStatus();
    }

    unsigned long frameTime = (micros() - microseconds);
    if (frameTime < MICROSECONDS_PER_FRAME) {
        delayMicroseconds(MICROSECONDS_PER_FRAME - frameTime);
    }
    // Else we just run with lower framerate
}