#include "Utils\TimeUtils.h"

#include "Global.h"
#include "Mqtt.h"
#include "Utils\StringUtils.h"

void Time_Init() {
    timeClient.begin();
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
        Serial.print("[V] Time synchronized");
    }
}

void reconfigTime() {
    if (WiFi.status() == WL_CONNECTED) {
        NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 30000);

        if (GetTimeUnix() != "failed") {
            Serial.print("[V] Time synchronized");
        } else {
            Serial.println("[E] Time server or internet connection error, will try again in 30 sec");
        }
    } else {
        Serial.println("[E] Get time impossible, no wifi connection");
    }
}

String GetTimeUnix() {
    unsigned long timestamp = timeClient.getEpochTime();
    if (timestamp < 30000) {
        return "failed";
    } else {
        return String(timestamp);
    }
}

String GetFormattedTimeLocal() {
    int hrs = timeClient.getHours();
    int min = timeClient.getMinutes();
    int sec = timeClient.getSeconds();
    int hrsLocal = hrs + jsonReadInt(configSetupJson, "timezone");
    hrsLocal = hrsLocal >= 24 ? hrsLocal - 24 : hrsLocal;
    String hrsLocalStr = hrsLocal < 10 ? "0" + String(hrsLocal) : String(hrsLocal);
    String minStr = min < 10 ? "0" + String(min) : String(min);
    String secStr = sec < 10 ? "0" + String(sec) : String(sec);
    String ret = hrsLocalStr + ":" + minStr + ":" + secStr;
    return ret;
}

String GetFormattedTimeGmt() {
    int hrs = timeClient.getHours();
    int min = timeClient.getMinutes();
    int sec = timeClient.getSeconds();
    String hrsStr = hrs < 10 ? "0" + String(hrs) : String(hrs);
    String minStr = min < 10 ? "0" + String(min) : String(min);
    String secStr = sec < 10 ? "0" + String(sec) : String(sec);
    String ret = hrsStr + ":" + minStr + ":" + secStr;
    return ret;
}

long timeToSec(String Time) {
    long sech = selectToMarker(Time, ":").toInt() * 60 * 60;
    Time = deleteBeforeDelimiter(Time, ":");
    long secm = selectToMarker(Time, ":").toInt() * 60;
    Time = deleteBeforeDelimiter(Time, ":");
    long sec = Time.toInt();
    long total = sech + secm + sec;
    return total;
}