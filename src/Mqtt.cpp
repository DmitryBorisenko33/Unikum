#include "Mqtt.h"

#include "Global.h"
#include "WiFiUtilis.h"
#include "cert.h"

void MQTT_init() {
    pinMode(22, OUTPUT);

    server.on("/mqttSave", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("mqttServer")) {
            jsonWriteStr(configSetupJson, "mqttServer", request->getParam("mqttServer")->value());
        }
        if (request->hasArg("mqttPort")) {
            int port = (request->getParam("mqttPort")->value()).toInt();
            jsonWriteInt(configSetupJson, "mqttPort", port);
        }
        if (request->hasArg("mqttUser")) {
            jsonWriteStr(configSetupJson, "mqttUser", request->getParam("mqttUser")->value());
        }
        if (request->hasArg("mqttPass")) {
            jsonWriteStr(configSetupJson, "mqttPass", request->getParam("mqttPass")->value());
        }

        saveConfig();

        String tmp = "{}";
        jsonWriteStr(tmp, "title", "<button class=\"close\" onclick=\"toggle('my-block')\">×</button>" + stateMQTT());
        jsonWriteStr(tmp, "class", "pop-up");

        request->send(200, "text/text", tmp);
    });

    ts.add(
        WIFI_MQTT_CONNECTION_CHECK, wifi_mqtt_reconnecting, [&](void*) {
            if (WiFi.status() == WL_CONNECTED) {
#ifdef mqtt_wifi_result
                Serial.println("[WIFI][V] WiFi-ok");
#endif
                if (client_mqtt.connected()) {
#ifdef mqtt_wifi_result
                    Serial.println("[MQTT][V] MQTT-ok");
#endif
                    digitalWrite(22, HIGH);
                } else {
                    digitalWrite(22, LOW);
                    MQTT_Connecting();
                }
            } else {
                digitalWrite(22, LOW);
#ifdef mqtt_wifi_result
                Serial.println("[WIFI][E] Lost WiFi connection");
#endif
                ts.remove(WIFI_MQTT_CONNECTION_CHECK);
                StartAPMode();
            }
        },
        nullptr, true);
}

void handleMQTT() {
    if (WiFi.status() == WL_CONNECTED) {
        if (client_mqtt.connected()) {
            client_mqtt.loop();
        }
    }
}

void MQTT_Connecting() {
    String mqtt_server = jsonReadStr(configSetupJson, "mqttServer");

    if ((mqtt_server != "")) {
        Serial.println("[MQTT][E] Lost MQTT connection, start reconnecting");
#ifdef mqtts        
        espClient.setCACert(local_root_ca);
#endif        
        client_mqtt.setServer(mqtt_server.c_str(), jsonReadInt(configSetupJson, "mqttPort"));
        if (WiFi.status() == WL_CONNECTED) {
            if (!client_mqtt.connected()) {
                Serial.println("[MQTT][V] Connecting to MQTT server commenced");
                if (client_mqtt.connect(chipID.c_str(), jsonReadStr(configSetupJson, "mqttUser").c_str(), jsonReadStr(configSetupJson, "mqttPass").c_str())) {
                    Serial.println("[MQTT][V] MQTT connected");
                    digitalWrite(22, HIGH);
                    client_mqtt.setCallback(callback);
                    client_mqtt.subscribe((prefix + "/device/" + chipID + "/+/control").c_str());  // Подписываемся на топики control
                    client_mqtt.subscribe((prefix + "/device/" + chipID + "/+/debug").c_str());    // Подписываемся на топики debug
                    Serial.println("[MQTT][V] Callback set, subscribe done");
                } else {
                    Serial.println("[MQTT][E] try again in " + String(wifi_mqtt_reconnecting / 1000) + " sec");
                }
            }
        }
    } else {
        Serial.println("[MQTT][E] No date for MQTT connection");
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("[MQTT][=>] ");
    Serial.print(topic);
    String topic_str = String(topic);

    String str;
    for (int i = 0; i < length; i++) {
        str += (char)payload[i];
    }
    Serial.println(" => " + str);


    if (topic_str.indexOf("/core001/control") != -1) {
       
    }
}



boolean mqtt_send_parametr(String device_name, String data) {
    String topik = prefix + "/device/" + chipID + "/" + device_name + "/parameter";
    boolean send_status = client_mqtt.beginPublish(topik.c_str(), data.length(), false);
    client_mqtt.print(data);
    client_mqtt.endPublish();
    print_mqtt_send_status(send_status, device_name, topik);
    return send_status;
}

boolean mqtt_send_status(String device_name, String data) {
    String topik = prefix + "/device/" + chipID + "/" + device_name + "/status";
    boolean send_status = client_mqtt.beginPublish(topik.c_str(), data.length(), true);  //retained - true
    client_mqtt.print(data);
    client_mqtt.endPublish();
    print_mqtt_send_status(send_status, device_name, topik);
    return send_status;
}

boolean mqtt_send_debug(String data, String name) {
    String topik = "/debug/" + name;
    boolean send_status = client_mqtt.beginPublish(topik.c_str(), data.length(), false);
    client_mqtt.print(data);
    client_mqtt.endPublish();
    return send_status;
}

String stateMQTT() {
    int state = client_mqtt.state();

    switch (state) {
        case -4:
            return "the server didn't respond within the keepalive time";
            break;
        case -3:
            return "the network connection was broken";
            break;
        case -2:
            return "the network connection failed";
            break;
        case -1:
            return "the client_mqtt is disconnected cleanly";
            break;
        case 0:
            return "the client_mqtt is connected";
            break;
        case 1:
            return "the server doesn't support the requested version of MQTT";
            break;
        case 2:
            return "the server rejected the client_mqtt identifier";
            break;
        case 3:
            return "the server was unable to accept the connection";
            break;
        case 4:
            return "the username/password were rejected";
            break;
        case 5:
            return "the client_mqtt was not authorized to connect";
            break;
        default:
            return "always has default section in switch case and result in the function end";
    }
}
