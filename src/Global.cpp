#include "Global.h"

/*
* Objects.cpp(с данными) 
*/

TickerScheduler ts(10);

WiFiClientSecure espClient;
PubSubClient client_mqtt(espClient);

AsyncWebServer server(80);

HardwareSerial uart1(1);
ModbusMaster modbus1;

HardwareSerial uart2(2);
ModbusMaster modbus2;

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

String inv001 = "{}";
String bat001 = "{}";
String bat002 = "{}";
String bat003 = "{}";
String bat004 = "{}";
String bat005 = "{}";
String cb001 = "{}";
String cb002 = "{}";
String cb003 = "{}";
String cb004 = "{}";
String core001 = "{}";

String modbusLog;

String serialNumber = "SPC001-110810-PVL080-";

//NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
//SYSTEM
int sysmod;
String firmware_version;
int readingTimeSec;
int startMillis;
int endMillis;
bool firstStart = true;
//UPGRADE
boolean upgrade_flag;
