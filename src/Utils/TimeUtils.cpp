#include "Utils\TimeUtils.h"

#include "Global.h"
#include "Mqtt.h"
#include "Utils\StringUtils.h"

void Time_Init() {
    ts.add(
        TIME_SYNC, 30000, [&](void*) {
            time_check();
        },
        nullptr, true);
}

void time_check() {
    if (GetTimeUnix() == "failed") {
        Serial.println("[i] Time is not synchronized, start synchronization");
        reconfigTime();
    } else {
        Serial.print("[V] Time = ");
        Serial.print(GetDataDigital());
        Serial.print(" ");
        Serial.println(GetTime());
    }
}

void reconfigTime() {
    if (WiFi.status() == WL_CONNECTED) {
        //String ntp = jsonReadStr(configSetupJson, "ntp");
        configTime(0, 0, "pool.ntp.org", "ru.pool.ntp.org", "ntp2.stratum2.ru");
        int i = 0;
        Serial.println("[i] Awaiting for time ");

        struct tm timeinfo;
        while (!getLocalTime(&timeinfo) && i <= 4) {
            Serial.print(".");
            i++;
            delay(1000);
        }

        if (GetTimeUnix() != "failed") {
            Serial.print("[V] Time synchronized = ");
            Serial.print(GetDataDigital());
            Serial.print(" ");
            Serial.println(GetTime());
        } else {
            Serial.println("[E] Time server or internet connection error, will try again in 30 sec");
        }
    } else {
        Serial.println("[E] Get time impossible, no wifi connection");
    }
}

//Получаем время в формате linux gmt
String GetTimeUnix() {
    time_t now = time(nullptr);
    if (now < 30000) {
        return "failed";
    } else {
        return String(now);
    }
}

//12:00:06
String GetTime() {
    time_t now = time(nullptr);
    int zone = 3600 * jsonReadStr(configSetupJson, "timezone").toInt();
    now = now + zone;
    String Time = "";
    Time += ctime(&now);
    int i = Time.indexOf(":");
    Time = Time.substring(i - 2, i + 6);
    return Time;
}

//12:00:06
String GetTimeGMT() {
    time_t now = time(nullptr);
    String Time = "";
    Time += ctime(&now);
    int i = Time.indexOf(":");
    Time = Time.substring(i - 2, i + 6);
    return Time;
}

//12:00
String GetTimeWOsec() {
    time_t now = time(nullptr);
    int zone = 3600 * jsonReadStr(configSetupJson, "timezone").toInt();
    now = now + zone;
    String Time = "";
    Time += ctime(&now);
    int i = Time.indexOf(":");
    Time = Time.substring(i - 2, i + 3);
    return Time;
}

// Получение даты
String GetDate() {
    time_t now = time(nullptr);
    int zone = 3600 * jsonReadStr(configSetupJson, "timezone").toInt();
    now = now + zone;
    String Data = "";
    Data += ctime(&now);
    Data.replace("\n", "");
    uint8_t i = Data.lastIndexOf(" ");
    String Time = Data.substring(i - 8, i + 1);
    Data.replace(Time, "");
    return Data;
}

String GetDataDigital() {
    String date = GetDate();

    date = deleteBeforeDelimiter(date, " ");

    date.replace("Jan", "01");
    date.replace("Feb", "02");
    date.replace("Mar", "03");
    date.replace("Apr", "04");
    date.replace("May", "05");
    date.replace("Jun", "06");
    date.replace("Jul", "07");
    date.replace("Aug", "08");
    date.replace("Sep", "09");
    date.replace("Oct", "10");
    date.replace("Nov", "11");
    date.replace("Dec", "12");

    String month = date.substring(0, 2);
    String day = date.substring(3, 5);
    String year = date.substring(8, 10);

    String out = day;
    out += ".";
    out += month;
    out += ".";
    out += year;

    return out;
}

//00:00:00
long timeToSec(String Time) {
    long sech = selectToMarker(Time, ":").toInt() * 60 * 60;
    Time = deleteBeforeDelimiter(Time, ":");
    long secm = selectToMarker(Time, ":").toInt() * 60;
    Time = deleteBeforeDelimiter(Time, ":");
    long sec = Time.toInt();
    long total = sech + secm + sec;
    return total;
}