#pragma once
#define USE_LittleFS
/*
* Здесь хранятся все настройки прошивки
*/

#define mqtts //включчение mqtt с шифрованием

#define wifi_mqtt_reconnecting 60000
#define mqtt_wifi_result

//#define OTA_enable
//#define MDNS_enable
//#define WS_enable

#define TIME_COMPILING String(__TIME__)
#define DATE_COMPILING String(__DATE__)
