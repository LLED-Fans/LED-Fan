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

## Debug

(macOS only) To debug a message, run

    ./decode_stacktrace.sh <STACKTRACE>

which creates and deletes a temporary __dmp.txt file.

## Update

Run 

    platformio run --target upload --upload-port IP-ADDRESS
    
as per http://docs.platformio.org/en/latest/platforms/espressif32.html#over-the-air-ota-update.


## Build

The chip expects a single LED strip to be layout down across the whole fan blade, with the center LED being offset by 0.25 LEDs away from data cable (end). This will place LEDs on each side at 0.5 offset from the other side.

#### Reasoning

**Why not just one blade?** Two fan blades will balance out the rotation at high speeds.

**Why not just on one blade?** By placing the LEDs across both blade edges and not just one, the rotation speed can be halved, which both conserves energy, noise and stabilizes across lower clock speeds.

**Why not mirrored across the center?** By offsetting the pixels, a larger area can be covered with light, eliminating holes in the picture. This also doubles the resolution, which makes for a higher-fidelity picture. If a lower resolution is desired for FPS or bandwidth purposes, target 2x or 4x API endpoints (WIP).
