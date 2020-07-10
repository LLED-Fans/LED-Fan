//
// Created by Lukas Tenbrink on 02.02.20.
//

#include "Updater.h"
#include <ArduinoOTA.h>
#include <Update.h>
#include <util/Logger.h>

Updater::Updater() {
    // ArduinoOTA.setPort(3232);
    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("myesp32");
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                else // U_SPIFFS
                    type = "filesystem";

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                SerialLog.print("Start updating " + type).ln();
            })
            .onEnd([]() {
                SerialLog.print("\nEnd").ln();
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                SerialLog.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                SerialLog.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) SerialLog.print("Auth Failed").ln();
                else if (error == OTA_BEGIN_ERROR) SerialLog.print("Begin Failed").ln();
                else if (error == OTA_CONNECT_ERROR) SerialLog.print("Connect Failed").ln();
                else if (error == OTA_RECEIVE_ERROR) SerialLog.print("Receive Failed").ln();
                else if (error == OTA_END_ERROR) SerialLog.print("End Failed").ln();
            });
}

int Updater::check() {
    shouldCheckUpdate = true;
    return 60 * 1000 * 1000;
}

void Updater::handle() {
    if (shouldCheckUpdate) {
        checkUntilTimestamp = millis() + 10000;
        shouldCheckUpdate = false;
    }

    if (checkUntilTimestamp > 0) {
        if (millis() < checkUntilTimestamp) {
            // May run already but we can't check lol
            ArduinoOTA.begin();
        }
        else {
            ArduinoOTA.end();
            checkUntilTimestamp = 0;
        }
    }

    ArduinoOTA.handle();
}
