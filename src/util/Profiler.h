//
// Created by Lukas Tenbrink on 27.04.20.
//

#ifndef LED_FAN_PROFILER_H
#define LED_FAN_PROFILER_H

#include <WString.h>

class Profiler {
public:
    String currentSection;
    unsigned long timestamp;

    void start(String section);
    void mark(String section);
    void end();

    static String readableTime(unsigned long micros, unsigned long turnover = 5);
};


#endif //LED_FAN_PROFILER_H
