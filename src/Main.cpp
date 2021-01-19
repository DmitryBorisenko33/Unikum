#include "Main.h"

#include "FilesSystem.h"
#include "Global.h"
#include "Mqtt.h"
#include "Upgrade.h"
#include "Web.h"
#include "WebServer.h"
#include "WiFiUtilis.h"

void setup() {
    firmware_version = "0.0.3";  //DATE_COMPILING + " " + TIME_COMPILING;
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

    //server.POST("?id=12345×tamp=1563129868&lat=-30.100675&lon=-51.262190&altitude=44");  //this alone wont update data on server
    //server.POST("");                                                                      //this second POST makes the data to be processed (you don't need to connect as "keep-alive" for that to work)
}

void loop() {
    ts.update();
    handleMQTT();
    handle_upgrade();
}