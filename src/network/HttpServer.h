//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_HTTPSERVER_H
#define LED_FAN_HTTPSERVER_H


#include <util/VideoInterface.h>
#include <sensor/RotationSensor.h>
#include <util/ClockSynchronizer.h>

class HttpServer {
public:
    Screen *screen;
    VideoInterface *videoInterface;

    RotationSensor *rotationSensor;
    ClockSynchronizer *clockSynchronizer;

    HttpServer(VideoInterface *videoInterface, RotationSensor *rotationSensor, ClockSynchronizer *clockSynchronizer);

    void setupRoutes();
    String processTemplates(const String &var);
};


#endif //LED_FAN_HTTPSERVER_H
