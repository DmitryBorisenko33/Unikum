#include "FilesSystem.h"

#include "Global.h"

void file_system_init() {
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    Serial.println("--------------started----------------");

    LittleFS.begin(false,"/littlefs", 10);
    //LittleFS.begin();
    
    configSetupJson = readFile("config.setup.json", 4096);
    configSetupJson.replace(" ", "");
    configSetupJson.replace("\r\n", "");
    Serial.println(configSetupJson);
    Serial.println("SPIFFS_init");
    //---------------------------------------------------------
    uint32_t chipID_u = ESP.getEfuseMac();
    chipID = String(chipID_u);
    serialNumber = serialNumber + chipID;
    jsonWriteStr(configSetupJson, "serialNumber", serialNumber);
    jsonWriteStr(configSetupJson, "chipID", chipID);
    jsonWriteStr(configSetupJson, "firmware_version", firmware_version);

    //esp_log_level_set("esp_littlefs", ESP_LOG_NONE);
}