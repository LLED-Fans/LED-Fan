echo "$1" > __dmp.txt
java -jar /Applications/EspStackTraceDecoder.jar ~/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-addr2line .pio/build/esp32dev/firmware.elf __dmp.txt
rm __dmp.txt
