# Setup

Clion Recommended:

    platformio init --ide clion --board esp32dev

Relies on the flash file system for storage / static html files.
Run uploadfs as well as upload to flash the ESP32.

## Debug

To debug a message, run

    java -jar EspStackTraceDecoder.jar ~/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-addr2line .pio/build/esp32dev/firmware.elf dump.txt

whereas dump.txt contains the stack trace.

## Update

Run 

    platformio run --target upload --upload-port IP-ADDRESS
    
as per http://docs.platformio.org/en/latest/platforms/espressif32.html#over-the-air-ota-update.
