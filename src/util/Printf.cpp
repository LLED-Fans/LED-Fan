//
// Created by Lukas Tenbrink on 21.01.20.
//

#include "Printf.h"

#include <cstdio>
#include <HardwareSerial.h>

void Printf::ln(char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    char buf[vsnprintf(NULL, 0, fmt, va) + 1];
    vsprintf(buf, fmt, va);
    Serial.println(buf);
    va_end(va);
}
