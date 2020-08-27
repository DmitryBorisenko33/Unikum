#include "Upgrade.h"

void initUpgrade() {
    server.on("/check", HTTP_GET, [](AsyncWebServerRequest* request) {
        String tmp = "{}";
        jsonWriteStr(tmp, "title", "<button class=\"close\" onclick=\"toggle('my-block')\">×</button>{{ServSetUpdateNew}}<a href=\"#\" class=\"btn btn-block btn-danger\" onclick=\"send_request(this, '/upgrade');setTimeout(function(){ location.href='/?set.main'; }, 80000);html('my-block','<span class=loader></span>{{ServSetUpdateProgress}}')\">{{ServSetUpdateInstall}}</a>");
        jsonWriteStr(tmp, "class", "pop-up");

        request->send(200, "text/text", tmp);
    });

    server.on("/upgrade", HTTP_GET, [](AsyncWebServerRequest* request) {
        upgrade_flag = true;
        request->send(200, "text/text", "ok");
    });
}

void upgrade_firmware() {
    
    Serial.println("Start upgrade LittleFS, please wait...");
 

    WiFiClient client_for_upgrade;

    httpUpdate.rebootOnUpdate(false);

    t_httpUpdate_return ret = httpUpdate.updateSpiffs(client_for_upgrade, "http://91.204.228.124:1100/unikum/update/spiffs.bin");

    if (ret == HTTP_UPDATE_OK) {  //если FS обновилась успешно

        Serial.println("!=!=!LittleFS upgrade done!");

        Serial.println("Start upgrade BUILD, please wait...");

        httpUpdate.rebootOnUpdate(false);
        t_httpUpdate_return ret = httpUpdate.update(client_for_upgrade, "http://91.204.228.124:1100/unikum/update/firmware.bin");

        if (ret == HTTP_UPDATE_OK) {  //если BUILD обновился успешно

            Serial.println("!=!=!BUILD upgrade done!");

            Serial.println("Restart ESP....");
            delay(1000);
            ESP.restart();
        }
    }
}

void handle_upgrade() {
    if (upgrade_flag) {
        upgrade_flag = false;
        upgrade_firmware();
    }
}


