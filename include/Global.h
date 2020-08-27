#pragma once

/*
* Libraries
*/

#include <FS.h>

#include "Consts.h"
#include "Utils\FilesUtils.h"
#include "Utils\JsonUtils.h"
#include "Utils\StringUtils.h"
#include "Utils\TimeUtils.h"

#ifdef littleFileSystem
#include "LITTLEFS.h"
#define LittleFS LITTLEFS
#endif

#ifndef littleFileSystem
#include "SPIFFS.h"
#define LittleFS SPIFFS
#endif

#include "esp_log.h"

#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

#include <PubSubClient.h>
#include <TickerScheduler.h>
#include <WiFi.h>

#include "../lib/dependencies/WiFiClientSecure/WiFiClientSecure.h"
#include "GyverFilters.h"
#include "time.h"

/*
* Objects.h(без данных)
*/

extern TickerScheduler ts;

#ifdef mqtts
extern WiFiClientSecure espClient;
#endif

extern PubSubClient client_mqtt;

extern AsyncWebServer server;


/*
* Global vars.h (без данных)
*/

//MQTT
extern String prefix;
extern String chipID;
//WEB
extern const char* hostName;
//JSON
extern String configSetupJson;
extern String configLiveJson;
extern String configOptionJson;
extern String inv001;
extern String bat001;
extern String bat002;
extern String bat003;
extern String bat004;
extern String bat005;
extern String cb001;
extern String cb002;
extern String cb003;
extern String cb004;
extern String core001;

extern String modbusLog;

extern String serialNumber;

//NTP
extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;
//SYSTEM
extern int sysmod;
extern String firmware_version;
extern int readingTimeSec;
extern int startMillis;
extern int endMillis;
extern bool firstStart;
//UPGRADE
extern boolean upgrade_flag;
//SAFETY
extern const char* local_root_ca;

enum { READING_REGISTER_EXECUTION,
       ROUTER_SEARCHING,
       WIFI_MQTT_CONNECTION_CHECK,
       DG1,
       DG2,
       TEST,
       TIME_SYNC };

