cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#list(APPEND EXTRA_COMPONENT_DIRS .pio/libdeps/esp32dev/FastLED-idf/components)
# Not a normal libdep, but we can add its "components" folder to scan
project(led-fan)
