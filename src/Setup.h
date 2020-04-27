// ==================================================================
// ===================== Constants  =================================
// ==================================================================

// Do the sensor check on the main thread.
#define ROTATION_SENSOR_TYPE_HALL_SYNC 1
// Do the sensor check as an async xtask.
#define ROTATION_SENSOR_TYPE_HALL_XTASK 2
// Let the sensors interrupt the CPU on activation.
#define ROTATION_SENSOR_TYPE_INTERRUPT 3

// Good if measurements are accurate, but speed may vary
#define ROTATION_EXTRAPOLATION_STEP 1
// Good if speed is relatively constant, but measurements vary
#define ROTATION_EXTRAPOLATION_REGRESSION 2

// ==================================================================
// ======================  Setup  ===================================
// ==================================================================

#ifndef LED_FAN_SETUP_H
#define LED_FAN_SETUP_H

// ------------------------------------------
// ---- General
// ------------------------------------------

// Upper cap
#define MAX_FRAMES_PER_SECOND 10000

// ------------------------------------------
// ---- Wifi
// ------------------------------------------

#define HOST_NETWORK_SSID "LLED Fan"
#define HOST_NETWORK_PASSWORD "We love LED"
#define WIFI_HOSTNAME "lled.fan"

// ------------------------------------------
// ---- Screen
// ------------------------------------------

// See https://github.com/FastLED/FastLED/blob/master/chipsets.h
// APA102Controller recommended, WS2013 etc. work too
#define LED_TYPE APA102Controller
#define COLOR_ORDER BGR
#define LED_DATA_PIN 26

// Only in use for clock-based LED strips (e.g. APA102)
#define LED_CLOCK_PIN 27
// Max 80, higher values need shorter wire length
#define LED_CLOCK_SPEED_MHZ 40

#define LED_COUNT 42
// How many additional LEDs we should set to black, just to be safe
#define LED_OVERFLOW_WALL 5

// 0 to 1, at 0.5 half of the pixels will be brightness-adjusted for the rest.
#define LED_BRIGHTNESS_CORRECTION 0.15

// RingResolution = add * x + min
#define CONCENTRIC_RESOLUTION_MIN 2
#define CONCENTRIC_RESOLUTION_ADD 1

// ------------------------------------------
// ---- Rotation Sensor
// ------------------------------------------

// See ROTATION_SENSOR_TYPE_XXX
#define ROTATION_SENSOR_TYPE ROTATION_SENSOR_TYPE_INTERRUPT
// See ROTATION_EXTRAPOLATION_XXX
#define ROTATION_EXTRAPOLATION ROTATION_EXTRAPOLATION_REGRESSION
// Comma-Separated list of pins in use
#define ROTATION_SENSOR_PINS 14,18,23,4
// If true, calculate the time per checkpoint separately (for wobbly fans)
#define ROTATION_SENSOR_SEPARATE_CHECKPOINTS false
// If applicable, max framerate of rotation sensor task
#define ROTATION_SENSOR_MS 1

// After this idle time, we're considered to be paused.
#define ROTATION_PAUSED_MS 2000

// How many checkpoints to account for during extrapolation
// High numbers are stabler under stable conditions,
// low numbers are quicker to adjust
#define CHECKPOINT_HISTORY_COUNT 12

// ------------------------------------------
// ---- Motor Control
// ------------------------------------------

// Maximum rotation speed accepted.
// Setting this low helps with interpreting sensor activations correctly.
#define MAX_ROTATIONS_PER_SECOND 20
#define MOTOR_ROTATIONS_PER_SECOND 14

#define MOTOR_FORWARD_PIN 16
#define MOTOR_BACKWARD_PIN 17
#define MOTOR_PWM_FREQUENCY 30000
#define MOTOR_PWM_RESOLUTION 8

// ------------------------------------------
// ---- Accuracy
// ------------------------------------------

#define LUT_SIN_COUNT 1000

#endif //LED_FAN_SETUP_H
