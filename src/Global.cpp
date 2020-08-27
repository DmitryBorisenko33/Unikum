#include "Global.h"
/*
* Objects.cpp(с данными) 
*/
TickerScheduler ts(10);
#ifdef mqtts
WiFiClientSecure espClient;
#endif
#ifndef mqtts
WiFiClient espClient;
#endif
PubSubClient client_mqtt(espClient);
AsyncWebServer server(80);
/*
* Global vars.cpp (с данными)
*/
//MQTT
String prefix = "/ru";
String chipID = "";
//WEB
const char* hostName = "esp-async";
//JSON
String configSetupJson = "{}";
String configLiveJson = "{}";
String configOptionJson = "{}";
//NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
//SYSTEM
String firmware_version;
//UPGRADE
boolean upgrade_flag;
