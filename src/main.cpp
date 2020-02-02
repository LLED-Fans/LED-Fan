//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include <SPIFFS.h>
#include <network/ArtnetServer.h>
#include <network/Updater.h>
#include <network/HttpServer.h>
#include <network/Network.h>

#define MAGNET_PIN 33

#define HOST_NETWORK_SSID "LLED Fan"
#define HOST_NETWORK_PASSWORD "We love LED"

#define MICROSECONDS_PER_FRAME 1000

RotationSensor *rotationSensor;
Screen *screen;

HttpServer *server;
ArtnetServer *artnetServer;

Updater *updater;

void setup() {
    // Enable Monitoring
    Serial.begin(9600);

    Serial.println("Booting LLED Fan Firmware");

    // Mount file system
    SPIFFS.begin(false);

    // Initialize Screen
    screen = new Screen(18, 64);
    rotationSensor = new RotationSensor(
            new SensorSwitch(MAGNET_PIN, new PeakDetector(MICROSECONDS_PER_FRAME / 1000.0 / 1000.0 / 10.0))
    );

    // Initialize Server
    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    Network::connectToPreset();

    artnetServer = new ArtnetServer(screen);

    server = new HttpServer(
        new VideoInterface(screen, artnetServer),
        rotationSensor
    );

    // Updater
    updater = new Updater();
}

void loop() {
    unsigned long microseconds = micros();
    auto milliseconds = microseconds / 1000;

    rotationSensor->update(milliseconds);

    screen->draw(
        milliseconds,
        rotationSensor->isReliable ? rotationSensor->rotation : -1
    );
    screen->lastFrameTime = milliseconds;

    EVERY_N_SECONDS(2) {
        Network::checkStatus();
    }
    updater->check();

    unsigned long frameTime = (micros() - microseconds);
    if (frameTime < MICROSECONDS_PER_FRAME) {
        delayMicroseconds(MICROSECONDS_PER_FRAME - frameTime);
    }
    // Else we just run with lower framerate
}