#include "loopPubMqttData.h"

#include "Global.h"
#include "Mqtt.h"

void pubMqttData() {
    inv001.trim();

    bat001.trim();
    bat002.trim();
    bat003.trim();
    bat004.trim();
    bat005.trim();

    core001.trim();
    
    cb001.trim();
    cb002.trim();
    cb003.trim();
    cb004.trim();

    if (!inv001.equals("{}")) mqtt_send_parametr("inv001", inv001);
    if (!bat001.equals("{}")) mqtt_send_parametr("bat001", bat001);
    if (!bat002.equals("{}")) mqtt_send_parametr("bat002", bat002);
    if (!bat003.equals("{}")) mqtt_send_parametr("bat003", bat003);
    if (!bat004.equals("{}")) mqtt_send_parametr("bat004", bat004);
    if (!bat005.equals("{}")) mqtt_send_parametr("bat005", bat005);
    if (!core001.equals("{}")) mqtt_send_parametr("core001", core001);
    if (!cb001.equals("{}")) mqtt_send_parametr("cb001", cb001);
    if (!cb002.equals("{}")) mqtt_send_parametr("cb002", cb002);
    if (!cb003.equals("{}")) mqtt_send_parametr("cb003", cb003);
    if (!cb004.equals("{}")) mqtt_send_parametr("cb004", cb004);

    if (modbusLog != "") mqtt_send_debug(modbusLog, "log");
    modbusLog = "";

    inv001 = "{}";
    bat001 = "{}";
    bat002 = "{}";
    bat003 = "{}";
    bat004 = "{}";
    bat005 = "{}";
    core001 = "{}";
    cb001 = "{}";
    cb002 = "{}";
    cb003 = "{}";
    cb004 = "{}";
}