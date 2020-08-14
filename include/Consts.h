#pragma once
#define USE_LittleFS
/*
* Здесь хранятся все настройки прошивки
*/

//===serial log management section===
//#define printRegisterReadingLog
//#define printRegisterWritingLog

//===mqtt log management section=====
//#define publishRegisterReadingLogToMqtt
//#define publishRegisterWritingLogToMqtt

#define wifi_mqtt_reconnecting 60000
//#define mqtt_only_final_line
#define mqtt_wifi_result

//#define OTA_enable
//#define MDNS_enable
//#define WS_enable

#define contactor_pin 26
#define start_dg_pin 18
#define alarm_pin 19

#define TIME_COMPILING String(__TIME__)
#define DATE_COMPILING String(__DATE__)
