#include "Main.h"
#include "FilesSystem.h"
#include "Global.h"
#include "Mqtt.h"
#include "Upgrade.h"
#include "Web.h"
#include "WebServer.h"
#include "WiFiUtilis.h"

void setup() {
    firmware_version = DATE_COMPILING + " " + TIME_COMPILING;
    file_system_init();
    Serial.println("file_system_init");
    //---------------------------------------------------------
    ROUTER_Connecting();
    Serial.println("ROUTER_Connecting");
    //---------------------------------------------------------
    Web_server_init();
    Serial.println("Web_server_init");
    //---------------------------------------------------------
    Time_Init();
    Serial.println("Time_Init");
    //---------------------------------------------------------
    MQTT_init();
    Serial.println("MQTT_init");
    //---------------------------------------------------------
    initUpgrade();
    Serial.println("initUpgrade");
    //---------------------------------------------------------
    web_init();
    Serial.println("web_init");
    //---------------------------------------------------------

    mqtt_send_debug("Loaded", "core001");
}

void loop() {
    ts.update();
    handleMQTT();
    handle_upgrade();
}