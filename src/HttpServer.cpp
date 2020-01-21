//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "HttpServer.h"
#include "Network.h"

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <esp_wifi.h>

#define SERVE_HTML(uri, file) _server.on(uri, HTTP_GET, [template_processor](AsyncWebServerRequest *request){\
    request->send(SPIFFS, file, "text/html", false, template_processor);\
});

#define request_result(success) request->send(success ? 200 : 400, "text/plain", success ? "Success" : "Failure"); return

using namespace std::placeholders;

// FIXME This should be per-instance. Or something.
AsyncWebServer _server(80);

HttpServer::HttpServer(Screen *screen, RotationSensor *rotationSensor) : screen(screen),
                                                                         rotationSensor(rotationSensor) {
    auto template_processor = std::bind(&HttpServer::processTemplates, this, _1);

    _server.serveStatic("/material.min.js", SPIFFS, "/material.min.js");
    _server.serveStatic("/material.min.css", SPIFFS, "/material.min.css");

    _server.serveStatic("/images", SPIFFS, "/images");
    _server.serveStatic("/styles.css", SPIFFS, "/styles.css");

    SERVE_HTML("/", "/index.html")
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

    _server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "404 / Not Found");
    });

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
        return String(screen->pin());
    if (var == "MAGNET_PIN")
        return String(rotationSensor->sensorSwitch->pin);

    if (var == "S_MODE_DEMO") {
        return screen->mode == Screen::demo ? "mdl-button--accent" : "";
    }
    if (var == "S_MODE_SCREEN") {
        return screen->mode == Screen::screen ? "mdl-button--accent" : "";
    }

    return String("ERROR");
}
