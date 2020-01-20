//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "HttpServer.h"

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// FIXME This should be per-instance. Or something.
AsyncWebServer _server(80);

HttpServer::HttpServer(Screen *screen) : screen(screen) {
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/server/index.html");
    });

    _server.onNotFound([](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "404 / Not Found");
    });

    _server.begin();
}
