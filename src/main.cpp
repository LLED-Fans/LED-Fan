// ==================================================================
// ======================  Setup  ===================================
// ==================================================================

#define MAGNET_PINS 33

#define HOST_NETWORK_SSID "LLED Fan"
#define HOST_NETWORK_PASSWORD "We love LED"

#define MAX_FRAMES_PER_SECOND 2000

#define LED_TYPE WS2813
#define COLOR_ORDER GRB
#define LED_PIN 25
#define LED_COUNT 18

#define VIRTUAL_SCREEN_SIZE 64
#define CONCENTRIC_RESOLUTION_MIN 4
#define CONCENTRIC_RESOLUTION_ADD 4

// hall_synchronized or hall_xtask
#define ROTATION_SENSOR_TYPE hall_xtask
#define ROTATION_SENSOR_TICKS 10000

// ==================================================================
// ==================================================================

#include <SPIFFS.h>
#include <network/ArtnetServer.h>
#include <network/Updater.h>
#include <network/HttpServer.h>
#include <network/Network.h>
#include <screen/ConcentricCoordinates.h>
#include <util/ClockSynchronizer.h>

#define MICROSECONDS_PER_FRAME (1000 * 1000 / MAX_FRAMES_PER_SECOND)

RotationSensor *rotationSensor;
CLEDController *ledChipset;
Screen *screen;

HttpServer *server;
ArtnetServer *artnetServer;

Updater *updater;

ClockSynchronizer *clockSynchronizer;

#if ROTATION_SENSOR_TYPE == hall_xtask
TaskHandle_t hallXTaskHandle = NULL;

void runHallXTask(void * pvParameters)
{
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount ();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for( ;; )
    {
        vTaskDelayUntil( &xLastWakeTime, ROTATION_SENSOR_TICKS );
        rotationSensor->update(micros());
    }
#pragma clang diagnostic pop
}
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

#if ROTATION_SENSOR_TYPE == hall_xtask
    xTaskCreate( runHallXTask, "RSENSOR", 1024, nullptr, 10, &hallXTaskHandle );
    configASSERT( hallXTaskHandle );
#endif
}

void loop() {
    unsigned long microseconds = clockSynchronizer->sync();
    auto milliseconds = microseconds / 1000;

#if ROTATION_SENSOR_TYPE == hall_synchronized
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