// ==================================================================
// ======================  Constants  ===================================
// ==================================================================

#define ROTATION_SENSOR_TYPE_HALL_SYNC 1
#define ROTATION_SENSOR_TYPE_HALL_XTASK 2

#include "Setup.h"

#include <SPIFFS.h>
#include <network/ArtnetServer.h>
#include <network/Updater.h>
#include <network/HttpServer.h>
#include <network/Network.h>
#include <screen/ConcentricCoordinates.h>
#include <util/ClockSynchronizer.h>

using namespace std::placeholders;

#define MICROSECONDS_PER_FRAME (1000 * 1000 / MAX_FRAMES_PER_SECOND)

RotationSensor *rotationSensor;
Screen *screen;

HttpServer *server;
ArtnetServer *artnetServer;

Updater *updater;

ClockSynchronizer *clockSynchronizer;

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
#include <util/XTaskTimer.h>
XTaskTimer *hallTimer;
#endif

void setup() {
    // Enable Monitoring
    Serial.begin(9600);

    Serial.println("Booting LLED Fan Firmware");

    // Mount file system
    SPIFFS.begin(false);

    // Clock Synchronizer
    clockSynchronizer = new ClockSynchronizer(
        MICROSECONDS_PER_FRAME, 50
    );

    // Initialize Screen

    screen = new Screen(
        new LED_TYPE<LED_PIN, COLOR_ORDER>(),
        LED_PIN,
        LED_COUNT,
        VIRTUAL_SCREEN_SIZE,
        ConcentricCoordinates::resolution(LED_COUNT, CONCENTRIC_RESOLUTION_ADD, CONCENTRIC_RESOLUTION_MIN)
    );

    std::vector<SensorSwitch *> switches = {};
    for (int magnetPin : {MAGNET_PINS}) {
        switches.push_back(new SensorSwitch(magnetPin, new PeakDetector(MICROSECONDS_PER_FRAME / 1000.0 / 1000.0 / 10.0)));
    }
    rotationSensor = new RotationSensor(switches);

    // Initialize Server
    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    Network::connectToPreset();

    artnetServer = new ArtnetServer(screen);

    server = new HttpServer(
        new VideoInterface(screen, artnetServer),
        rotationSensor,
        clockSynchronizer
    );

    // Updater
    updater = new Updater();

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
    hallTimer = new XTaskTimer(
        ROTATION_SENSOR_MS,
        "RSENSOR",
        10,
        std::bind(&RotationSensor::update, rotationSensor, _1)
    );
    server->hallTimer = hallTimer;
#endif
}

void loop() {
    unsigned long microseconds = clockSynchronizer->sync();
    auto milliseconds = microseconds / 1000;

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_SYNC
    rotationSensor->update(microseconds);
#endif

    screen->draw(
        milliseconds,
        rotationSensor->estimatedRotation(microseconds)
    );

    EVERY_N_SECONDS(2) {
        Network::checkStatus();
    }
    updater->check();
}