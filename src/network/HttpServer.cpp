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
#include <util/Profiler.h>

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
        return Network::address().toString();
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
        return app->screen->getMode() == Screen::demo ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_SCREEN") {
        return app->screen->getMode() == Screen::cartesian ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_CONCENTRIC") {
        return app->screen->getMode() == Screen::concentric ? "mdl-button--accent" : "";
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
        return Profiler::readableTime(esp_timer_get_time(), 2);
    }
    if (var == "FPS") {
        unsigned long meanMicrosPerFrame = app->regularClock->frameTimeHistory->mean();

        auto fpsString = String(1000 * 1000 / std::max(meanMicrosPerFrame, app->regularClock->microsecondsPerFrame))
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

void registerREST(const char* url, String param, const std::function<String(String)>& set, const std::function<String()>& get) {
    _server.on(url, HTTP_POST, [param, set](AsyncWebServerRequest *request) {
        if (!request->hasParam(param, true)) {
            request_result(false);
        }

        auto value = request->getParam(param, true)->value();
        auto result = set(value);
        if (result.isEmpty())
            request->send(400, "text/plain", "Failure");
        else
            request->send(200, "text/plain", result);
    });

    _server.on(url, HTTP_GET, [param, get](AsyncWebServerRequest *request) {
        if (!request->hasParam(param)) {
            request_result(false);
        }

        auto value = request->getParam(param)->value();
        request->send(200, "text/plain", get());
    });
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

    _server.on("/wifi/connect", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("ssid", true) || !request->hasParam("password", true)) {
            request_result(false);
        }

        auto ssid = request->getParam("ssid", true)->value();
        auto password = request->getParam("password", true)->value();

        Network::setStationSetup(new WifiSetup(ssid, password));
        Network::mode = WifiMode::station;

        // We may connect later, but it's deferred
        request_result(true);
    });

    _server.on("/wifi/pair", HTTP_POST, [](AsyncWebServerRequest *request) {
        Network::pair();
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
        Logger.print("Pong").ln();
        request->send(200, "text/plain", String(time));
    });

    registerREST("/behavior", "id", [screen](String id) {
        if (id == "none") {
            screen ->behavior = nullptr;
        }
        else if (id == "strobe") {
            screen->behavior = new StrobeDemo();
        }
        else if (id == "dotted")
            screen -> behavior = new Dotted();
        else {
            return String();
        }

        return String(2000 * 1000);
    }, [](){ return ""; });

    _server.on("/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(5000 * 1000));
        ESP.restart();
    });

    _server.on("/checkupdate", HTTP_POST, [updater](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(updater->check()));
    });

    _server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", Logger.string());
    });

    registerREST("/rotation", "rotation", [rotationSensor](String value) {
        auto rotation = value.toFloat();
        rotationSensor->fixedRotation = rotation;
        return "Success";
    }, [rotationSensor]() { return String(rotationSensor->fixedRotation); });

    registerREST("/speed", "speed-control", [videoInterface](String value) {
        auto speed = value.toFloat();
        videoInterface->artnetServer->speedControl->setDesiredSpeed(speed);
        return "Success";
    }, [videoInterface]() { return String(videoInterface->artnetServer->speedControl->getDesiredSpeed()); });

    registerREST("/brightness", "brightness", [screen](String value) {
        auto brightness = value.toFloat();
        screen->setBrightness(brightness);
        return "Success";
    }, [screen]() { return String(screen->getBrightness()); });

    // -----------------------------------------------
    // ------------------- Data ----------------------
    // -----------------------------------------------

    _server.on("/i", HTTP_GET,[videoInterface](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        auto json = videoInterface->info();
        serializeJsonPretty(json, *response);
        request->send(response);
    });
}
