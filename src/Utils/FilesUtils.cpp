#include "Utils\FilesUtils.h"
#include "Global.h"


void saveConfig() {
    writeFile("config.setup.json", configSetupJson);
}

int count(String str, String found) {
    if (str.indexOf(found) == -1) return 0;  
    str += found;                            
    uint8_t i = 0;                           
    while (str.length() != 0) {
        str = deleteBeforeDelimiter(str, found);  
        i++;                                      
    }
    return i; 
}

boolean isDigitStr(String str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!isDigit(str.charAt(i))) {
            return false;
        }
    }
    return str.length();
}

void safeDataToFile(String data, String Folder) {
    String fileName;
    fileName.toLowerCase();
    fileName = deleteBeforeDelimiter(fileName, " ");
    fileName.replace(" ", ".");
    fileName.replace("..", ".");
    fileName = Folder + "/" + fileName + ".txt";
    Serial.println(fileName);
    jsonWriteStr(configLiveJson, "test", fileName);
}

String readFile(String fileName, size_t len) {
    File configFile = LittleFS.open("/" + fileName, "r");
    if (!configFile) {
        return "Failed";
    }
    size_t size = configFile.size();
    if (size > len) {
        configFile.close();
        return "Large";
    }
    String temp = configFile.readString();
    configFile.close();
    return temp;
}

String sizeFile(String fileName) {
    File configFile = LittleFS.open("/" + fileName, "r");
    if (!configFile) {
        return "Failed";
    }
    size_t size = configFile.size();
    configFile.close();
    return String(size);
}

String writeFile(String fileName, String strings) {
    File configFile = LittleFS.open("/" + fileName, "w");
    if (!configFile) {
        return "Failed to open config file";
    }
    configFile.print(strings);
    configFile.close();
    return "Write sucsses";
}

String addFile(String fileName, String strings) {
    File configFile = LittleFS.open("/" + fileName, "a");
    if (!configFile) {
        return "Failed to open config file";
    }
    configFile.println(strings);
    configFile.close();
    return "Write sucsses";
}

String readFileString(const String& filename, const String& str_to_found) {
    String res = "failed";
    auto file = LittleFS.open("/" + filename, "r");
    if (file && file.find(str_to_found.c_str())) {
        res = file.readStringUntil('\n');
    }
    file.close();
    return res;
}
