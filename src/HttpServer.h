//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_HTTPSERVER_H
#define LED_FAN_HTTPSERVER_H


#include "Screen.h"

class HttpServer {
public:
    Screen *screen;

    HttpServer(Screen *screen);
};


#endif //LED_FAN_HTTPSERVER_H
