//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_HTTPSERVER_H
#define LED_FAN_HTTPSERVER_H


#include <util/VideoInterface.h>
#include <sensor/RotationSensor.h>
#include <util/ClockSynchronizer.h>

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
#include <util/XTaskTimer.h>
#endif

class HttpServer {
public:
    Screen *screen;
    VideoInterface *videoInterface;

    RotationSensor *rotationSensor;
    ClockSynchronizer *clockSynchronizer;

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
    XTaskTimer *hallTimer;
#endif

    HttpServer(VideoInterface *videoInterface, RotationSensor *rotationSensor, ClockSynchronizer *clockSynchronizer);

    void setupRoutes();
    String processTemplates(const String &var);
};


#endif //LED_FAN_HTTPSERVER_H
