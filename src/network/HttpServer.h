//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_HTTPSERVER_H
#define LED_FAN_HTTPSERVER_H


#include "screen/Screen.h"
#include <util/VideoInterface.h>
#include "sensor/RotationSensor.h"

class HttpServer {
public:
    Screen *screen;
    VideoInterface *videoInterface;
    RotationSensor *rotationSensor;

    HttpServer(Screen *screen, RotationSensor *rotationSensor);

    void setupRoutes();
    String processTemplates(const String &var);
};


#endif //LED_FAN_HTTPSERVER_H
