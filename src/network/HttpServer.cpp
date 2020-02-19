//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "HttpServer.h"
#include "Network.h"

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <esp_wifi.h>
#include <util/Logger.h>

#define SERVE_HTML(uri, file) _server.on(uri, HTTP_GET, [template_processor](AsyncWebServerRequest *request){\
    request->send(SPIFFS, file, "text/html", false, template_processor);\
});

#define request_result(success) request->send(success ? 200 : 400, "text/plain", success ? "Success" : "Failure"); return

using namespace std::placeholders;

// FIXME This should be per-instance. Or something.
AsyncWebServer _server(80);

HttpServer::HttpServer(VideoInterface *videoInterface, RotationSensor *rotationSensor, ClockSynchronizer *clockSynchronizer)
        : screen(videoInterface->screen),
          videoInterface(videoInterface),
          rotationSensor(rotationSensor),
          clockSynchronizer(clockSynchronizer) {
    setupRoutes();
    _server.begin();
}

String HttpServer::processTemplates(const String &var) {
    if (var == "AP_IP")
        return WiFi.softAPIP().toString();
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
        return String(screen->pin);
    if (var == "MAGNET_PIN") {
        String r = "";
        for (SensorSwitch *sensorSwitch : rotationSensor->switches) {
            r += String(sensorSwitch->pin) + ", ";
        }
        return r;
    }

    if (var == "S_MODE_DEMO") {
        return screen->mode == Screen::demo ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_SCREEN") {
        return screen->mode == Screen::screen ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_CONCENTRIC") {
        return screen->mode == Screen::concentric ? "mdl-button--accent" : "";
    }
    if (var == "VIRTUAL_SCREEN_SIZE") {
        return String(screen->cartesianResolution);
    }
    if (var == "MAGNET_VALUE") {
        return String(rotationSensor->switches[0]->peaks->lower)
            + " < " +  String(rotationSensor->switches[0]->rawValue())
            + " < " +  String(rotationSensor->switches[0]->peaks->upper);
    }
    if (var == "ROTATION_SPEED") {
        if (screen->fixedRotation >= 0)
            return "Fixed: " + String(screen->fixedRotation);

        if (!rotationSensor->isReliable)
            return "Unreliable";

        String result = String(rotationSensor->rotationsPerSecond()) + "r/s (";

        for (auto time : rotationSensor->history) {
            result += String(time) + "µs, ";
        }

        return result + ")";
    }
    if (var == "UPTIME") {
        return String((int) (esp_timer_get_time() / 1000 / 1000 / 60)) + " minutes";
    }
    if (var == "FPS") {
        float meanMicrosPerFrame = clockSynchronizer->frameTimeHistory->mean();

        return String(1000 * 1000 / _max(meanMicrosPerFrame, clockSynchronizer->microsecondsPerFrame))
            + " (slack: " + String(_max(0, clockSynchronizer->microsecondsPerFrame - meanMicrosPerFrame)) + "µs)";
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
    auto screen = this->screen;

    _server.serveStatic("/material.min.js", SPIFFS, "/material.min.js");
    _server.serveStatic("/material.min.css", SPIFFS, "/material.min.css");

    _server.serveStatic("/images", SPIFFS, "/images");
    _server.serveStatic("/styles.css", SPIFFS, "/styles.css");

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
            screen->mode = Screen::screen;
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

        auto result = Network::connect(ssid.begin(), password.begin());
        request_result(result);
    });

    // -----------------------------------------------
    // ------------------ Other ----------------------
    // -----------------------------------------------

    _server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "404 / Not Found");
    });

    _server.on("/ping", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(screen->ping()));
    });

    _server.on("/reboot", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "5000");
        ESP.restart();
    });

    _server.on("/log", HTTP_GET, [screen](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", Logger::string());
    });

    _server.on("/rotation/set", HTTP_POST, [screen](AsyncWebServerRequest *request) {
        if (!request->hasParam("rotation", true)) {
            request_result(false);
        }
        auto rotation = request->getParam("rotation", true)->value().toFloat();
        screen->fixedRotation = rotation;

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
