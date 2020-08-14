#include "Web.h"

#include "Global.h"
#include "Modbus.h"
#include "ModbusToMqtt.h"
#include "SystemModes.h"

void web_init() {
    //====================================Main Settings====================================

    server.on("/web", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("WebUser")) {
            jsonWriteStr(configSetupJson, "WebUser", request->getParam("WebUser")->value());
        }
        if (request->hasArg("WebPass")) {
            jsonWriteStr(configSetupJson, "WebPass", request->getParam("WebPass")->value());
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //====================================WiFi Settings====================================

    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("wifissid")) {
            jsonWriteStr(configSetupJson, "wifissid", request->getParam("wifissid")->value());
        }
        if (request->hasArg("wifipass")) {
            jsonWriteStr(configSetupJson, "wifipass", request->getParam("wifipass")->value());
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("device")) {
            if (request->getParam("device")->value() == "ok") ESP.restart();
        }
        request->send(200, "text/text", "OK");
    });

    //====================================Grid Settings====================================

    server.on("/grid", HTTP_GET, [](AsyncWebServerRequest* request) {
        //Grid Voltage
        if (request->hasArg("vsi")) {
            int vsi = request->getParam("vsi")->value().toInt();
            jsonWriteInt(configSetupJson, "vsi", vsi);
            writeSingleRegInt("modbus115200", "inv001", "0x04", 10102, vsi, 10);
        }

        //Grid Frequency
        if (request->hasArg("fsi")) {
            int fsi = request->getParam("fsi")->value().toInt();
            jsonWriteInt(configSetupJson, "fsi", fsi);
            writeSingleRegInt("modbus115200", "inv001", "0x04", 20103, fsi, 100);
        }

        if (request->hasArg("vgmin")) {
            int vgmin = request->getParam("vgmin")->value().toInt();
            jsonWriteInt(configSetupJson, "vgmin", vgmin);
        }

        if (request->hasArg("vgmax")) {
            int vgmax = request->getParam("vgmax")->value().toInt();
            jsonWriteInt(configSetupJson, "vgmax", vgmax);
        }

        if (request->hasArg("pt")) {
            jsonWriteStr(configSetupJson, "pt", request->getParam("pt")->value());
        }

        if (request->hasArg("opt")) {
            jsonWriteStr(configSetupJson, "opt", request->getParam("opt")->value());
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //Grid protection standart (drop down)
    server.on("/gpsi", HTTP_GET, [](AsyncWebServerRequest* request) {
        int argument;
        if (request->hasArg("gpsi")) {
            argument = request->getParam("gpsi")->value().toInt();
            jsonWriteInt(configSetupJson, "gpsi", argument);
            saveConfig();
            writeSingleRegInt("modbus115200", "inv001", "0x04", 20111, argument, 0);
            request->send(200, "text/text", "ok");
        }
    });

    //User Defined Grid protection (checkbox)
    server.on("/pud", HTTP_GET, [](AsyncWebServerRequest* request) {
        int pud;
        if (request->hasArg("pud")) {
            pud = request->getParam("pud")->value().toInt();
            jsonWriteInt(configSetupJson, "pud", pud);
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //Export to grid allowed (checkbox)
    server.on("/gtei", HTTP_GET, [](AsyncWebServerRequest* request) {
        int gtei;
        if (request->hasArg("gtei")) {
            gtei = request->getParam("gtei")->value().toInt();
            jsonWriteInt(configSetupJson, "gtei", gtei);
            writeSingleRegInt("modbus115200", "inv001", "0x04", 20108, gtei, 0);
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //DSO control allowed (checkbox)
    server.on("/dsoc", HTTP_GET, [](AsyncWebServerRequest* request) {
        int dsoc;
        if (request->hasArg("dsoc")) {
            dsoc = request->getParam("dsoc")->value().toInt();
            jsonWriteInt(configSetupJson, "dsoc", dsoc);
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //====================================Battery Settings====================================
    //SOC
    server.on("/soc", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("socf")) {
            jsonWriteInt(configSetupJson, "socf", request->getParam("socf")->value().toInt());
        }
        if (request->hasArg("socm")) {
            jsonWriteInt(configSetupJson, "socm", request->getParam("socm")->value().toInt());
        }
        if (request->hasArg("socl")) {
            jsonWriteInt(configSetupJson, "socl", request->getParam("socl")->value().toInt());
        }
        saveConfig();
        request->send(200, "text/text", "OK");
    });

    //=============================PV Settings====================================
    server.on("/pv", HTTP_GET, [](AsyncWebServerRequest* request) {
        //checkbox
        if (request->hasArg("stpv")) {
            jsonWriteInt(configSetupJson, "stpv", request->getParam("stpv")->value().toInt());
        }
        //checkbox
        if (request->hasArg("papv")) {
            jsonWriteInt(configSetupJson, "papv", request->getParam("papv")->value().toInt());
        }
        //Array nominal power
        if (request->hasArg("pnpv")) {
            jsonWriteStr(configSetupJson, "pnpv", request->getParam("pnpv")->value());
        }
        //Array open curcuit voltage
        if (request->hasArg("vocpv")) {
            jsonWriteStr(configSetupJson, "vocpv", request->getParam("vocpv")->value());
        }
        //Array max. power voltage
        if (request->hasArg("vmppv")) {
            jsonWriteStr(configSetupJson, "vmppv", request->getParam("vmppv")->value());
        }
        //Array max. power current 
        if (request->hasArg("imppv")) {
            jsonWriteStr(configSetupJson, "imppv", request->getParam("imppv")->value());
        }
        //Array inclination angle
        if (request->hasArg("iscpv")) {
            jsonWriteStr(configSetupJson, "iscpv", request->getParam("iscpv")->value());
        }
        //
        if (request->hasArg("angpv")) {
            jsonWriteStr(configSetupJson, "angpv", request->getParam("angpv")->value());
        }
        //Array azimuth
        if (request->hasArg("azpv")) {
            jsonWriteStr(configSetupJson, "azpv", request->getParam("azpv")->value());
        }
        saveConfig();
        request->send(200, "text/text", "OK");  // отправляем ответ о выполнении
    });

    //===========================Generator Settings======================================

    //==============================IOT Settings=========================================

    //=============================Service Settings======================================
    //energy saving mode switch
    server.on("/esvi", HTTP_GET, [](AsyncWebServerRequest* request) {
        int argument;
        if (request->hasArg("esvi")) {
            argument = request->getParam("esvi")->value().toInt();
            jsonWriteInt(configSetupJson, "esvi", argument);
            saveConfig();
            writeSingleRegInt("modbus115200", "inv001", "0x04", 20104, argument, 0);
            request->send(200, "text/text", "ok");
        }
    });

    //battery type switch
    server.on("/bti", HTTP_GET, [](AsyncWebServerRequest* request) {
        int argument;
        if (request->hasArg("bti")) {
            argument = request->getParam("bti")->value().toInt();
            jsonWriteInt(configSetupJson, "bti", argument);
            saveConfig();
            writeSingleRegInt("modbus115200", "inv001", "0x04", 10110, argument, 0);
            request->send(200, "text/text", "ok");
        }
    });

    //system mode switch
    server.on("/setmode", HTTP_GET, [](AsyncWebServerRequest* request) {
        int argument;
        if (request->hasArg("setmode")) {
            argument = request->getParam("setmode")->value().toInt();
        }
        if (argument == 1) {
            inverter_mode_1();
        }
        if (argument == 2) {
            inverter_mode_2();
        }
        if (argument == 3) {
            inverter_mode_3();
        }
        if (argument == 4) {
            inverter_mode_4();
        }
        if (argument == 5) {
            inverter_mode_5();
        }
        if (argument == 6) {
            inverter_mode_6();
        }
        if (argument == 7) {
            inverter_mode_7();
        }
        if (argument == 8) {
            inverter_mode_8();
        }
        
        
        request->send(200, "text/text", "ok");  // отправляем ответ о выполнении
    });

    server.on("/service", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasArg("manual")) {
            int manual = request->getParam("manual")->value().toInt();
            jsonWriteInt(configSetupJson, "manual", manual);
        }
        //Test value of itpv
        if (request->hasArg("itpv")) {
            int itpv = request->getParam("itpv")->value().toInt();
            jsonWriteInt(configSetupJson, "itpv", itpv);
        }
        //Number of batteries
        if (request->hasArg("bnset")) {
            int bnset = request->getParam("bnset")->value().toInt();
            jsonWriteInt(configSetupJson, "bnset", bnset);
        }
        //========================int==============================================
        //Batteries Total Capacity
        if (request->hasArg("cbti")) {
            int cbti = request->getParam("cbti")->value().toInt();
            jsonWriteInt(configSetupJson, "cbti", cbti);
            writeSingleRegInt("modbus115200", "inv001", "0x04", 10111, cbti, 0);
        }
        //=========================float===========================================
        //Max Invertor current
        if (request->hasArg("idsi")) {
            float idsi = request->getParam("idsi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "idsi", idsi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20113, idsi, 10);
        }
        //Max Grid charge current
        if (request->hasArg("igchmi")) {
            float igchmi = request->getParam("igchmi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "igchmi", igchmi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20125, igchmi, 10);
        }
        //Max PV Charger current
        if (request->hasArg("ipvmi")) {
            float ipvmi = request->getParam("ipvmi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "ipvmi", ipvmi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 10108, ipvmi, 10);
        }
        //Max Combined Charge current
        if (request->hasArg("ichmi")) {
            float ichmi = request->getParam("ichmi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "ichmi", ichmi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20132, ichmi, 10);
        }
        //Battery Stop charging Voltage
        if (request->hasArg("vbsci")) {
            float vbsci = request->getParam("vbsci")->value().toFloat();
            jsonWriteFloat(configSetupJson, "vbsci", vbsci);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20119, vbsci, 10);
        }
        //Battery Stop disharging Voltage
        if (request->hasArg("vbsdi")) {
            float vbsdi = request->getParam("vbsdi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "vbsdi", vbsdi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20118, vbsdi, 10);
        }
        //CV Voltage
        if (request->hasArg("vabi")) {
            float vabi = request->getParam("vabi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "vabi", vabi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 10104, vabi, 10);
        }
        //Float Voltage
        if (request->hasArg("vfli")) {
            float vfli = request->getParam("vfli")->value().toFloat();
            jsonWriteFloat(configSetupJson, "vfli", vfli);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 10103, vfli, 10);
        }
        //Battery Low Voltage
        if (request->hasArg("vbli")) {
            float vbli = request->getParam("vbli")->value().toFloat();
            jsonWriteFloat(configSetupJson, "vbli", vbli);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20127, vbli, 10);
        }
        //Battery High Voltage
        if (request->hasArg("vbhi")) {
            float vbhi = request->getParam("vbhi")->value().toFloat();
            jsonWriteFloat(configSetupJson, "vbhi", vbhi);
            writeSingleRegFloat("modbus115200", "inv001", "0x04", 20128, vbhi, 10);
        }
        //===========================================================================
        //Solar power balance switch
        server.on("/spbi", HTTP_GET, [](AsyncWebServerRequest* request) {
            int argument = request->getParam("spbi")->value().toInt();
            if (request->hasArg("spbi")) {
                jsonWriteInt(configSetupJson, "spbi", argument);
                saveConfig();
                writeSingleRegInt("modbus115200", "inv001", "0x04", 20144, argument, 0);
                request->send(200, "text/text", "ok");
            }
        });

        saveConfig();

        request->send(200, "text/text", "OK");
    });
}