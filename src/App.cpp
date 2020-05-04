//
// Created by Lukas Tenbrink on 27.04.20.
//

#include "App.h"

#include <SPIFFS.h>
#include <screen/ConcentricCoordinates.h>
#include <network/Network.h>

#include <util/Logger.h>
#include <util/LUT.h>

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
#endif

#define MICROSECONDS_PER_FRAME (1000 * 1000 / MAX_FRAMES_PER_SECOND)

App::App() {
    // Enable Monitoring
    Serial.begin(9600);

    Serial.println("Booting LLED Fan Firmware");

    LUT::initSin(LUT_SIN_COUNT);

    // Mount file system
    SPIFFS.begin(false);

    // Clock Synchronizer
    regularClock = new RegularClock(
            MICROSECONDS_PER_FRAME, 50
    );

    // Initialize Screen

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
            CHECKPOINT_HISTORY_COUNT * (ROTATION_SENSOR_SEPARATE_CHECKPOINTS ? rotationSensorPins.size() : 1),
#if ROTATION_EXTRAPOLATION == ROTATION_EXTRAPOLATION_STEP
            new StepExtrapolator()
#elif ROTATION_EXTRAPOLATION == ROTATION_EXTRAPOLATION_REGRESSION
            new LinearRegressionExtrapolator()
#endif
    );
    rotationSensor->minCheckpointTime = 1000 * 1000 / (MAX_ROTATIONS_PER_SECOND * rotationSensorPins.size());
    rotationSensor->pauseInterval = ROTATION_PAUSED_MS * 1000;
    rotationSensor->separateCheckpoints = ROTATION_SENSOR_SEPARATE_CHECKPOINTS;

    screen = new Screen(
#if LED_TYPE == APA102Controller
            new LED_TYPE<LED_DATA_PIN, LED_CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(LED_CLOCK_SPEED_MHZ)>(),
#else
            new LED_TYPE<LED_DATA_PIN, COLOR_ORDER>(),
#endif
            LED_DATA_PIN,
            LED_COUNT,
            LED_OVERFLOW_WALL,
            LED_COUNT,
            ConcentricCoordinates::resolution(LED_COUNT, CONCENTRIC_RESOLUTION_ADD, CONCENTRIC_RESOLUTION_MIN)
    );
    screen->setCorrection(LED_BRIGHTNESS_CORRECTION);
    screen->rotationSensor = rotationSensor;

    auto motorForwardPin = new PWMPin(MOTOR_FORWARD_PIN, 0);
    auto motorBackwardPin = new PWMPin(MOTOR_BACKWARD_PIN, 1);
    motorForwardPin->setup(MOTOR_PWM_FREQUENCY, MOTOR_PWM_RESOLUTION);
    motorBackwardPin->setup(MOTOR_PWM_FREQUENCY, MOTOR_PWM_RESOLUTION);
    speedControl = new SpeedControl(
            motorForwardPin,
            motorBackwardPin,
            rotationSensor,
            MOTOR_ROTATIONS_PER_SECOND
    );

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
    server->hallTimer = ((XTaskGPIOSwitch *) rotationSensor->visitor)->timer;
#endif

    // Initialize Server
    Network::host(HOST_NETWORK_SSID, HOST_NETWORK_PASSWORD);
    Network::connectToPreset();
    Network::setHostname(WIFI_HOSTNAME);

    artnetServer = new ArtnetServer(screen, speedControl);
    updater = new Updater();

    server = new HttpServer(this);
}

void App::run() {
    auto delayMicros = regularClock->sync();
    rotationSensor->update();

    screen->update(delayMicros);
    speedControl->update(delayMicros);

    EVERY_N_SECONDS(2) {
        Network::checkStatus();
    }
    updater->handle();
}
