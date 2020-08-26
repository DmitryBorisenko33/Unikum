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
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
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
//UPGRADE
boolean upgrade_flag;
//SYSTEM
String firmware_version;
