//
// Created by Lukas Tenbrink on 01.08.20.
//

#ifndef LED_FAN_UUID_H
#define LED_FAN_UUID_H

#pragma once

#include <stdint.h>

typedef uint8_t uuid_t[16];

#define UUID_STR_LEN 37

void uuid_generate(uuid_t out);
int uuid_parse(const char *in, uuid_t uu);
void uuid_unparse(const uuid_t uu, char *out);

#endif //LED_FAN_UUID_H
