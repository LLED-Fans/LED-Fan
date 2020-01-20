//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cstdio>
#include <HardwareSerial.h>

void printfln(char * fmt, ...) {
    va_list va;
    va_start(va, fmt);
    char buf[vsnprintf(NULL, 0, fmt, va) + 1];
    vsprintf(buf, fmt, va);
    Serial.println(buf);
    va_end(va);
}
