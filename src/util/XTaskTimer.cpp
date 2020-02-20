//
// Created by Lukas Tenbrink on 19.02.20.
//

#include "XTaskTimer.h"

#include <utility>
#include <freertos/portmacro.h>

void runLLTimerTask(void *pvParameters)
{
    auto *timer = static_cast<XTaskTimer *>(pvParameters);
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount ();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for( ;; )
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(timer->frameTimeMS));

        unsigned long time = micros();
        timer->fun(time);
        timer->frameTimes.append((int) time);
    }
#pragma clang diagnostic pop
}

XTaskTimer::XTaskTimer(unsigned long frameTime, const char *name, int historySize, std::function<void(unsigned long)> fun)
        : frameTimeMS(frameTime), frameTimes(historySize), fun(std::move(fun)) {
    xTaskCreate( runLLTimerTask, name, 1024, NULL, 10, &handle );
    configASSERT( handle );
}
