#include "Upgrade.h"

void initUpgrade() {
    server.on("/check", HTTP_GET, [](AsyncWebServerRequest* request) {
        String tmp = "{}";
        jsonWriteStr(tmp, "title", "<button class=\"close\" onclick=\"toggle('my-block')\">×</button>{{ServSetUpdateNew}}<a href=\"#\" class=\"btn btn-block btn-danger\" onclick=\"send_request(this, '/upgrade');setTimeout(function(){ location.href='/?set.service'; }, 80000);html('my-block','<span class=loader></span>{{ServSetUpdateProgress}}')\">{{ServSetUpdateInstall}}</a>");
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

    t_httpUpdate_return ret = httpUpdate.updateSpiffs(client_for_upgrade, "http://91.204.228.124:1100/enfinergy/update/spiffs.bin");

    if (ret == HTTP_UPDATE_OK) {  //если FS обновилась успешно

        Serial.println("!=!=!LittleFS upgrade done!");

        Serial.println("Start upgrade BUILD, please wait...");

        httpUpdate.rebootOnUpdate(false);
        t_httpUpdate_return ret = httpUpdate.update(client_for_upgrade, "http://91.204.228.124:1100/enfinergy/update/firmware.bin");

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

//===========>WRITING OF: inv001
//W; name: inv001; addr: 4; reg: 20104; val: 1; ok;
//W; name: inv001; addr: 4; reg: 10102; val: 2200; ok;
//W; name: inv001; addr: 4; reg: 20103; val: 5000; ok;
//W; name: inv001; addr: 4; reg: 20111; val: 0; ok;=============================
//W; name: inv001; addr: 4; reg: 20108; val: 0; ok;=============================
//W; name: inv001; addr: 4; reg: 10110; val: 2; ok;
//W; name: inv001; addr: 4; reg: 10111; val: 210; ok;
//W; name: inv001; addr: 4; reg: 20113; val: 217; ok;
//W; name: inv001; addr: 4; reg: 20125; val: 400; ok;
//W; name: inv001; addr: 4; reg: 10108; val: 100; ok;
//W; name: inv001; addr: 4; reg: 20132; val: 800; ok;
//W; name: inv001; addr: 4; reg: 20119; val: 545; ok;
//W; name: inv001; addr: 4; reg: 20118; val: 480; ok;
//W; name: inv001; addr: 4; reg: 10104; val: 545; ok;
//W; name: inv001; addr: 4; reg: 10103; val: 540; ok;
//W; name: inv001; addr: 4; reg: 20127; val: 480; ok;
//W; name: inv001; addr: 4; reg: 20128; val: 600; ok;
//W; name: inv001; addr: 4; reg: 20144; val: 0; ok;
//W; name: inv001; addr: 4; reg: 20109; val: 3; error;
//W; name: inv001; addr: 4; reg: 20109; val: 3; ok;
//W; name: inv001; addr: 4; reg: 20111; val: 0; error;
//W; name: inv001; addr: 4; reg: 20111; val: 0; ok;
//W; name: inv001; addr: 4; reg: 20112; val: 1; ok;
//W; name: inv001; addr: 4; reg: 20108; val: 0; ok;
//W; name: inv001; addr: 4; reg: 20143; val: 3; ok;
//===========>WRITING end=========
