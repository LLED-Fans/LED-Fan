#include "Setup.h"

#include <SPIFFS.h>
#include <network/ArtnetServer.h>
#include <network/Updater.h>
#include <network/HttpServer.h>
#include <network/Network.h>
#include <screen/ConcentricCoordinates.h>
#include <util/ClockSynchronizer.h>

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_SYNC
#include <sensor/SyncGPIOSwitch.h>
#elif  ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
#include <sensor/XTaskGPIOSwitch.h>
#elif  ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_INTERRUPT
#include <sensor/InterruptGPIOSwitch.h>
#endif

#if ROTATION_EXTRAPOLATION == ROTATION_EXTRAPOLATION_STEP
#include <util/extrapolation/StepExtrapolator.h>
#elif ROTATION_EXTRAPOLATION == ROTATION_EXTRAPOLATION_REGRESSION
#include <util/extrapolation/LinearRegressionExtrapolator.h>
#include <util/Logger.h>

#endif

#define MICROSECONDS_PER_FRAME (1000 * 1000 / MAX_FRAMES_PER_SECOND)

RotationSensor *rotationSensor;
Screen *screen;

HttpServer *server;
ArtnetServer *artnetServer;

Updater *updater;

ClockSynchronizer *clockSynchronizer;

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

    std::vector<int> rotationSensorPins = {ROTATION_SENSOR_PINS};
    rotationSensor = new RotationSensor(
#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_SYNC
    new SyncGPIOSwitch(rotationSensorPins, MICROSECONDS_PER_FRAME / 1000.0 / 1000.0 / 10.0),
#elif  ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
    new XTaskGPIOSwitch(rotationSensorPins, MICROSECONDS_PER_FRAME / 1000.0 / 1000.0 / 10.0, ROTATION_SENSOR_MS),
#elif  ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_INTERRUPT
            new InterruptGPIOSwitch(rotationSensorPins),
#endif
        // if separate checkpoints, each n
        5 * (ROTATION_SENSOR_SEPARATE_CHECKPOINTS ? rotationSensorPins.size() : 1),
#if ROTATION_EXTRAPOLATION == ROTATION_EXTRAPOLATION_STEP
        new StepExtrapolator()
#elif ROTATION_EXTRAPOLATION == ROTATION_EXTRAPOLATION_REGRESSION
            new LinearRegressionExtrapolator()
#endif
    );
    rotationSensor->minCheckpointTime = 1000 * 1000 / (MAX_ROTATIONS_PER_SECOND * rotationSensorPins.size());
    rotationSensor->pauseInterval = ROTATION_PAUSED_MS * 1000;
    rotationSensor->separateCheckpoints = ROTATION_SENSOR_SEPARATE_CHECKPOINTS;

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
    server->hallTimer = ((XTaskGPIOSwitch *) rotationSensor->visitor)->timer;
#endif

    // Initialize Server
    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    Network::connectToPreset();
    Network::setHostname(WIFI_HOSTNAME);

    artnetServer = new ArtnetServer(screen);

    server = new HttpServer(
        new VideoInterface(screen, artnetServer),
        rotationSensor,
        clockSynchronizer
    );

    // Updater
    updater = new Updater();

    Logger::init();
    Logger::println("Starting up...");
}

void loop() {
    unsigned long microseconds = clockSynchronizer->sync();
    auto milliseconds = microseconds / 1000;

    rotationSensor->update(microseconds);

    screen->draw(
        milliseconds,
        rotationSensor->estimatedRotation(microseconds)
    );

    EVERY_N_SECONDS(2) {
        Network::checkStatus();
    }
    updater->check();
}