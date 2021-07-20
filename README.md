# About

This software is intended to be run on an ESP32 attached to a spinning blade of an LED Fan. Detailed build instructions are not provided at this point.

It controls I/O for the fan, has a web interface and is controllable via ArtNet. 

A python script is attached which is capable of capturing screen or images and sending them to the fan.

# Setup

Clion Recommended:

    platformio init --ide clion --board esp32dev

Relies on the flash file system for storage / static html files.
Run uploadfs as well as upload to flash the ESP32.

Take a look at Setup.h to see if it fits your build.

# Hacks

Since some stuff currently works... sub-bar, do the following steps too:

    #include <Arduino.h>  // On top of FastLED.h
    // Merge this: 
    https://github.com/5chmidti/FastLED/commit/1550a942ba69272e10e4ca56fd51dd9f074e1671
    // Set task priority to 20 in AsyncUDP.cpp


# First Run

Check the correct partition file to use and select it in the platformio.ini.

Remember to hold boot and press "EN" on the ESP to put it to flash mode. Run:

	SETUP_FILE="" bash -c "platformio run -t upload"
	SETUP_FILE="" bash -c "platformio run -t uploadfs"

It should boot up with a new WiFi. When connected, its local IP is `192.168.4.1`.

## Debug

(macOS only) To debug a message, run

    ./decode_stacktrace.sh <STACKTRACE>

which creates and deletes a temporary __dmp.txt file.

## Update

Run 

    platformio run --target upload --upload-port IP-ADDRESS

To update using a non-default setup file, create SomeSetup.h in builds/. The file will "inherit" from the default setup, so it can focus on overriding variables.

    SETUP_FILE="SomeSetup" bash -c "platformio run --target upload --upload-port IP-ADDRESS"
    
as per http://docs.platformio.org/en/latest/platforms/espressif32.html#over-the-air-ota-update.

## Factory

If reversion to factory is required, run `./otatool_proxy.py --port "/dev/ttyUSB1" erase_otadata`. This will erase OTA partitions and revert back to what was uploaded via serial.
The script is sub-par, especially with the parts provided from esp-idf, and may need be checked. It is recommended to set output from parttool not to "None" since several restarts of the device are required during running of scripts.


## Build

The chip expects a single LED strip to be layout down across the whole fan blade, with the center LED being offset by 0.25 LEDs away from data cable (end). This will place LEDs on each side at 0.5 offset from the other side.

#### Reasoning

**Why not just one blade?** Two fan blades will balance out the rotation at high speeds.

**Why not just on one blade?** By placing the LEDs across both blade edges and not just one, the rotation speed can be halved, which both conserves energy, noise and stabilizes across lower clock speeds.

**Why not mirrored across the center?** By offsetting the pixels, a larger area can be covered with light, eliminating holes in the picture. This also doubles the resolution, which makes for a higher-fidelity picture. When placing the strips, make sure the internal light sources, not the pixels themselves, are placed correctly.

**Why a single critical checkpoint?** Reed switches are, among one another, quite inexact (+-20%). One reed switch however functions in an analogue way and switch on very reliably.

## Performance Considerations

Some quick tests gave the following numbers:

    nop                       : 0.004 us
    digitalRead               : 0.143 us
    digitalWrite              : 0.125 us
    pinMode                   : 2.708 us
    multiply byte             : 0.038 us
    divide byte               : 0.053 us
    add byte                  : 0.034 us
    multiply integer          : 0.054 us
    divide integer            : 0.058 us
    add integer               : 0.054 us
    mod integer               : 0.062 us
    or/and integer            : 0.054 us
    shift integer             : 0.054 us
    multiply long             : 0.055 us
    divide long               : 0.073 us
    add long                  : 0.054 us
    multiply float            : 0.055 us
    divide float              : 0.248 us
    add float                 : 0.058 us
    itoa()                    : 0.703 us
    ltoa()                    : 0.898 us
    dtostrf()                 : 11.323 us
    y |= (1<<x)               : 0.046 us
    bitSet()                  : 0.046 us
    analogRead()              : 22.648 us
    
This suggests that besides float division and byte operations, no operator can be considered noteworthy and has to be preferred / avoided.
