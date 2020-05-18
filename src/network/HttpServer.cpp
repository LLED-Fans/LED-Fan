//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "HttpServer.h"
#include "Network.h"

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <esp_wifi.h>
#include <util/Logger.h>
#include <screen/behavior/Ping.h>
#include <screen/behavior/StrobeDemo.h>
#include <App.h>
#include <screen/behavior/Dotted.h>

#define SERVE_HTML(uri, file) _server.on(uri, HTTP_GET, [template_processor](AsyncWebServerRequest *request){\
    request->send(SPIFFS, file, "text/html", false, template_processor);\
});

#define request_result(success) request->send(success ? 200 : 400, "text/plain", success ? "Success" : "Failure"); return

using namespace std::placeholders;

// FIXME This should be per-instance. Or something.
AsyncWebServer _server(80);

HttpServer::HttpServer(App *app) : app(app), videoInterface(new VideoInterface(app->screen, app->artnetServer)) {
    setupRoutes();
    _server.begin();
}

String HttpServer::processTemplates(const String &var) {
    if (var == "AP_IP")
        return Network::status == ConnectStatus::accessPoint
        ? WiFi.softAPIP().toString()
        : "Not running";
    if (var == "AP_SSID") {
        wifi_config_t conf_current;
        esp_wifi_get_config(WIFI_IF_AP, &conf_current);
        return String(reinterpret_cast<char*>(conf_current.ap.ssid));;
    }

    if (var == "LOCAL_IP")
        return WiFi.localIP().toString();
    if (var == "LOCAL_SSID")
        return String(WiFi.SSID());

    if (var == "LED_PIN")
#if LED_TYPE == APA102Controller
        return String(app->screen->pin) + ", Clock: " + String(LED_CLOCK_PIN);
#else
        return String(app->screen->pin);
#endif
    if (var == "MAGNET_PIN") {
        String r = "";
        for (auto pin : {ROTATION_SENSOR_PINS}) {
            r += String(pin) + ", ";
        }
        return r;
    }

    if (var == "S_MODE_DEMO") {
        return app->screen->mode == Screen::demo ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_SCREEN") {
        return app->screen->mode == Screen::cartesian ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_CONCENTRIC") {
        return app->screen->mode == Screen::concentric ? "mdl-button--accent" : "";
    }
    if (var == "VIRTUAL_SCREEN_SIZE") {
        return String(app->screen->cartesianResolution);
    }
    if (var == "MAGNET_VALUE") {
        return app->rotationSensor->stateDescription();
    }
    if (var == "ROTATION_SPEED") {
        if (app->rotationSensor->fixedRotation >= 0)
            return "Fixed: " + String(app->rotationSensor->fixedRotation);

        IntRoller *timestamps = app->rotationSensor->checkpointTimestamps;
        IntRoller *indices = app->rotationSensor->checkpointIndices;
        String history = "";
        for (int i = 1; i < timestamps->count; ++i) {
            int diffMS = ((*timestamps)[i] - (*timestamps)[i - 1]) / 1000;
            history += String(diffMS) + "ms (" + (*indices)[i] + "), ";
        }

        if (app->rotationSensor->isPaused) {
            return "Paused";
        }

        if (!app->rotationSensor->isReliable())
            return "Unreliable (" + history + ")";

        return String(int(app->rotationSensor->rotationsPerSecond())) + "r/s (" + history + ")";
    }
    if (var == "MOTOR_SPEED") {
        return String(app->speedControl->getDesiredSpeed());
    }
    if (var == "UPTIME") {
        return String((int) (esp_timer_get_time() / 1000 / 1000 / 60)) + " minutes";
    }
    if (var == "FPS") {
        float meanMicrosPerFrame = app->regularClock->frameTimeHistory->mean();

        auto fpsString = String(1000 * 1000 / _max(meanMicrosPerFrame, app->regularClock->microsecondsPerFrame))
            + " (slack: " + String(_max(0, (int) (app->regularClock->microsecondsPerFrame - meanMicrosPerFrame))) + "µs)";

#if ROTATION_SENSOR_TYPE == ROTATION_SENSOR_TYPE_HALL_XTASK
        float meanHallMicrosPerFrame = hallTimer->frameTimes.mean();
        fpsString += " / Sensor: " + String(1000 * 1000 / meanHallMicrosPerFrame);
#endif

        return fpsString;
    }

    return String("ERROR");
}

bool handlePartialFile(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (!index) {
        request->_tempFile = SPIFFS.open("/img", FILE_WRITE);
    }
    request->_tempFile.write(data, len);

    if(index + len == total) {
        // Final Packet
        request->_tempFile.close();
        request->_tempFile = SPIFFS.open("/img", FILE_READ);

        return true;
    }

    return false;
}

void HttpServer::setupRoutes() {
    auto template_processor = std::bind(&HttpServer::processTemplates, this, _1);
    auto videoInterface = this->videoInterface;
    auto screen = app->screen;
    auto rotationSensor = app->rotationSensor;
    auto updater = app->updater;

    _server.serveStatic("/material.min.js", SPIFFS, "/material.min.js");
    _server.serveStatic("/material.min.css", SPIFFS, "/material.min.css");

    _server.serveStatic("/images", SPIFFS, "/images");
    _server.serveStatic("/styles.css", SPIFFS, "/styles.css");
    _server.serveStatic("/scripts.js", SPIFFS, "/scripts.js");

    SERVE_HTML("/", "/index.html")

    // -----------------------------------------------
    // ---------------- Settings ---------------------
    // -----------------------------------------------

    SERVE_HTML("/settings", "/settings.html")

    _server.on("/mode/set", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        if (!request->hasParam("mode", true)) {
            request_result(false);
        }

        auto mode = request->getParam("mode", true)->value();
        if (mode == "demo") {
            screen->mode = Screen::demo;
        } else if (mode == "screen") {
            screen->mode = Screen::cartesian;
        } else if (mode == "concentric") {
            screen->mode = Screen::concentric;
        }

        request_result(true);
    });

    _server.on("/wifi/connect", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        if (!request->hasParam("ssid", true) || !request->hasParam("password", true)) {
            request_result(false);
        }

        auto ssid = request->getParam("ssid", true)->value();
        auto password = request->getParam("password", true)->value();

        Network::connect(ssid.begin(), password.begin(), true, -1);
        // We may connect later, but it's deferred
        request_result(true);
    });

    // -----------------------------------------------
    // ------------------ Other ----------------------
    // -----------------------------------------------

    _server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "404 / Not Found");
    });

    _server.on("/ping", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        unsigned long time = 2000 * 1000;

        screen->behavior = new Ping(time);
        request->send(200, "text/plain", String(time));
    });

    _server.on("/behavior/set", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        if (!request->hasParam("id")) {
            request_result(false);
        }

        auto id = request->getParam("id")->value();

        if (id == "none") {
            screen ->behavior = nullptr;
        }
        else if (id == "strobe") {
            screen->behavior = new StrobeDemo();
        }
        else if (id == "dotted")
            screen -> behavior = new Dotted();
        else {
            request_result(false);
        }

        request->send(200, "text/plain", String(2000));
    });

    _server.on("/reboot", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "5000");
        ESP.restart();
    });

    _server.on("/checkupdate", HTTP_POST, [updater](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(updater->check()));
    });

    _server.on("/log", HTTP_GET, [screen](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", Logger::string());
    });

    _server.on("/rotation/set", HTTP_POST, [rotationSensor](AsyncWebServerRequest *request) {
        if (!request->hasParam("rotation", true)) {
            request_result(false);
        }
        auto rotation = request->getParam("rotation", true)->value().toFloat();
        rotationSensor->fixedRotation = rotation;

        request_result(true);
    });

    _server.on("/speed/set", HTTP_POST, [videoInterface](AsyncWebServerRequest *request) {
        if (!request->hasParam("speed-control", true)) {
            request_result(false);
        }
        auto speed = request->getParam("speed-control", true)->value().toFloat();
        videoInterface->artnetServer->speedControl->setDesiredSpeed(speed);

        request_result(true);
    });

    // -----------------------------------------------
    // ------------------- Data ----------------------
    // -----------------------------------------------

    _server.on("/i", HTTP_GET,[videoInterface](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        auto json = videoInterface->info();
        serializeJsonPretty(json, *response);
        request->send(response);
    });

    _server.on("/i/img/jpg", HTTP_POST,[videoInterface](AsyncWebServerRequest *request) {
                   request->send(200);
               }, nullptr, [videoInterface](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
                   if (handlePartialFile(request, data, len, index, total)) {
                       bool accepted = videoInterface->acceptJpeg(request->_tempFile);
                       request_result(accepted);
                   }
               }
    );
}
