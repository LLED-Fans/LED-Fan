#ifndef LED_FAN_DEFAULT_SETUP_H
#define LED_FAN_DEFAULT_SETUP_H

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

// ------------------------------------------
// ---- General
// ------------------------------------------

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

#define LED_DATA_PIN 13

// Only in use for clock-based LED strips (e.g. APA102)
#define LED_CLOCK_PIN 14
// Max 80, higher values need shorter wire length
#define LED_CLOCK_SPEED_MHZ 60

#define LED_COUNT 72
// How many additional LEDs we should set to black, just to be safe
#define LED_OVERFLOW_WALL 5

// 0 to 1, at 0.5 half of the pixels will be brightness-adjusted for the rest.
#define LED_BRIGHTNESS_CORRECTION 0.5

#define CARTESIAN_RESOLUTION_FACTOR 2

// RingResolution = add * x + min
#define CONCENTRIC_RESOLUTION_MIN 2
#define CONCENTRIC_RESOLUTION_ADD 1

// Values are "bilinear" and "nearest"
#define CARTESIAN_SAMPLING_MODE bilinear

// A value of 0 to 255 where 255 is no rescale, and 1 is "full" rescale.
// Uses LED's "global brightness" parameter to dynamically recompute pixels
// into a scale where we have better resolution.
#define MAX_DYNAMIC_COLOR_RESCALE 8

// Natural, or rather "minimum" response of LEDs.
#define NATURAL_COLOR_RESPONSE 2.2f

// ------------------------------------------
// ---- Rotation Sensor
// ------------------------------------------

// See ROTATION_SENSOR_TYPE_XXX
#define ROTATION_SENSOR_TYPE ROTATION_SENSOR_TYPE_INTERRUPT
// Comma-Separated list of pins in use
#define ROTATION_SENSOR_PINS 26,18,23,4
// If defined, only use this checkpoint for final position calculation.
#define ROTATION_SENSOR_CRITICAL_CHECKPOINT 0
// If true, calculate the time per checkpoint separately (for wobbly fans)
#define ROTATION_SENSOR_SEPARATE_CHECKPOINTS false
// If applicable, max framerate of rotation sensor task
#define ROTATION_SENSOR_MS 1
// How much offset the fan has for correct image rotation
#define ROTATION_SENSOR_OFFSET 0.53
// Define to mirror the picture
#define ROTATION_SENSOR_FLIP

// After this idle time, we're considered to be paused.
#define ROTATION_PAUSED_MS 2000

// See ROTATION_EXTRAPOLATION_XXX
#define ROTATION_EXTRAPOLATION ROTATION_EXTRAPOLATION_REGRESSION
// How many checkpoints to account for during extrapolation
// High numbers are stabler under stable conditions,
// low numbers are quicker to adjust
#define CHECKPOINT_HISTORY_COUNT 20

// ------------------------------------------
// ---- Motor Control
// ------------------------------------------

// Maximum rotation speed accepted.
// Setting this low helps with interpreting sensor activations correctly.
#define MAX_ROTATIONS_PER_SECOND 20
#define MOTOR_ROTATIONS_PER_SECOND 14

#define MOTOR_FORWARD_PIN 17
#define MOTOR_BACKWARD_PIN 16
#define MOTOR_PWM_FREQUENCY 30000
#define MOTOR_PWM_RESOLUTION 8

// ------------------------------------------
// ---- FastLED
// ------------------------------------------

// Define to use FastLED. If Apa102 is used, don't define.
// See https://github.com/FastLED/FastLED/blob/master/chipsets.h
// APA102Controller recommended, WS2013 etc. work too
//#define FastLED_LED_TYPE APA102Controller
#define COLOR_ORDER BGR

// In bytes
// 2 words per LED, + some boundary + buffer
#define SPI_BUFFER_SIZE (4 * LED_COUNT + 100)

// Set to a valid SPI host to route all SPI outputs to this
#define SPI_ESP32_HARDWARE_SPI_HOST HSPI_HOST

// ------------------------------------------
// ---- Other
// ------------------------------------------

// If the power supply is limited, define this to dynamically limit drawn power
#define MAX_AMPERE 3.9
#define AMPERE_PER_LED 0.06

#define PAIR_PIN 27

#define LUT_SIN_COUNT 1000

// Comment out to disable emergency brake
#define EMERGENCY_BRAKE_ENABLED

#endif //LED_FAN_DEFAULT_SETUP_H
