#include "Web.h"
#include "Global.h"


void web_init() {
    //====================================Main Settings====================================

    server.on("/web", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("WebUser")) {
            jsonWriteStr(configSetupJson, "WebUser", request->getParam("WebUser")->value());
        }
        if (request->hasArg("WebPass")) {
            jsonWriteStr(configSetupJson, "WebPass", request->getParam("WebPass")->value());
        }
        server.serveStatic("/", LittleFS, "/").setDefaultFile("index.htm").setAuthentication(jsonReadStr(configSetupJson,"WebUser").c_str(), jsonReadStr(configSetupJson,"WebPass").c_str());
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //====================================WiFi Settings====================================

    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("wifissid")) {
            jsonWriteStr(configSetupJson, "wifissid", request->getParam("wifissid")->value());
        }
        if (request->hasArg("wifipass")) {
            jsonWriteStr(configSetupJson, "wifipass", request->getParam("wifipass")->value());
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("device")) {
            if (request->getParam("device")->value() == "ok") ESP.restart();
        }
        request->send(200, "text/text", "OK");
    });
}