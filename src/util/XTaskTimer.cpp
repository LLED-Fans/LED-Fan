//
// Created by Lukas Tenbrink on 19.02.20.
//

#include "XTaskTimer.h"

#include <utility>
#include <freertos/portmacro.h>

void runLLTimerTask(void *pvParameters)
{
    auto *timer = static_cast<XTaskTimer *>(pvParameters);

    auto xLastWakeTime = xTaskGetTickCount ();
    auto lastTimeMicros = micros();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for( ;; )
    {
        vTaskDelayUntil(&xLastWakeTime, 1);

        unsigned long microseconds = micros();
        timer->fun(microseconds);

        timer->frameTimes.push((int) (microseconds - lastTimeMicros));
        lastTimeMicros = microseconds;
    }
#pragma clang diagnostic pop
}

XTaskTimer::XTaskTimer(unsigned long frameTime, const char *name, int historySize, std::function<void(unsigned long)> fun)
        : frameTimeMS(frameTime), frameTimes(historySize), fun(std::move(fun)) {
    xTaskCreate( runLLTimerTask, name, 1024, this, 10, &handle );
    configASSERT( handle );
}
