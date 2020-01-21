//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_HTTPSERVER_H
#define LED_FAN_HTTPSERVER_H


#include "Screen.h"
#include "RotationSensor.h"

class HttpServer {
public:
    Screen *screen;
    RotationSensor *rotationSensor;

    HttpServer(Screen *screen, RotationSensor *rotationSensor);

    String processTemplates(const String &var);
};


#endif //LED_FAN_HTTPSERVER_H
