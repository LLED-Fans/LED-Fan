//
// Created by Lukas Tenbrink on 27.04.20.
//

#include "Profiler.h"

#include "Logger.h"
#include <esp32-hal.h>

void Profiler::start(String section) {
    Logger::println("----------");

    timestamp = micros();
    currentSection = std::move(section);
}

void Profiler::mark(String section) {
    if (!currentSection.isEmpty())
        end();

    timestamp = micros();
    currentSection = std::move(section);
}

void Profiler::end() {
    Logger::println(currentSection + String(": ") + readableTime(micros() - timestamp));
    currentSection = String();
}

String Profiler::readableTime(unsigned long micros, unsigned long turnover) {
    if (micros < 1000 * turnover)
        return String(micros) + String("us");
    micros /= 1000;

    if (micros < 1000 * turnover)
        return String(micros) + String("ms");
    micros /= 1000;

    if (micros < 60 * turnover)
        return String(micros) + String("s");
    micros /= 60;

    if (micros < 60 * turnover)
        return String(micros) + String("m");
    micros /= 60;

    return String(micros) + String("h");
}

