#pragma once

#include <Arduino.h>

extern void MQTT_init();
extern void handleMQTT();
extern void MQTT_Connecting();
extern void callback(char* topic, byte* payload, unsigned int length);
extern void send_user_mode();
extern boolean mqtt_send_parametr(String device_name, String data);
extern boolean mqtt_send_status(String device_name, String data);
extern boolean mqtt_send_debug(String data, String name);
extern void print_mqtt_send_status(boolean send_status, String device_name, String topik);
extern String stateMQTT();