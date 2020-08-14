#include "Modbus.h"

#include "Global.h"
#include "SystemModes.h"
#include "loopAppDataCalc.h"
#include "loopSysModeCheck.h"

void systemInit() {
    uart1.begin(19200, SERIAL_8N1, 17, 16);  //rx-tx
    uart2.begin(9600, SERIAL_8N1, 21, 32);   //rx-tx

    modbus115200 = new ModbusToMqtt(uart1, modbus1);
    modbus9600 = new ModbusToMqtt(uart2, modbus2);

    //send_user_mode();

    //modbus115200->updateWriteTableFromFile("maps/rinv.w.txt"); //del data in inverter
    //modbus9600->updateWriteTableFromFile("maps/rcb.w.txt");  //del data in cb

    //cb_set_address("0X00","0X0004");

    setDefaults();

    ts.add(
        READING_REGISTER_EXECUTION, 30000, [&](void*) {
            if (!firstStart) {
                sysModeCheck();
                coreDataCalc();
                appDataCalc();
                pubMqttData();
            }
            firstStart = false;

            modbus115200->updateReadTableFromFile("maps/inv001.r.txt");

            if (isBatteryConnected(1)) modbus115200->updateReadTableFromFile("maps/bat001.r.txt");
            if (isBatteryConnected(2)) modbus115200->updateReadTableFromFile("maps/bat002.r.txt");
            if (isBatteryConnected(3)) modbus115200->updateReadTableFromFile("maps/bat003.r.txt");
            if (isBatteryConnected(4)) modbus115200->updateReadTableFromFile("maps/bat004.r.txt");
            if (isBatteryConnected(5)) modbus115200->updateReadTableFromFile("maps/bat005.r.txt");

            modbus9600->updateReadTableFromFile("maps/cb001.r.txt");
            modbus9600->updateReadTableFromFile("maps/cb002.r.txt");
            modbus9600->updateReadTableFromFile("maps/cb003.r.txt");
            modbus9600->updateReadTableFromFile("maps/cb004.r.txt");
        },
        nullptr, false);
}

void setDefaults() {
    setDefaultRegisterValues();
    setDafaultRegisterValuesFromJson();
    setDefaultSysMode();
}

void setDefaultRegisterValues() {
    modbus115200->updateWriteTableFromFile("maps/start.w.txt");
}

void setDafaultRegisterValuesFromJson() {
    //=======================================grid settings===========================================================================
    writeSingleRegInt("modbus115200", "inv001", "0x04", 10102, jsonReadInt(configSetupJson, "vsi"), 10);   //Grid Voltage
    writeSingleRegInt("modbus115200", "inv001", "0x04", 20103, jsonReadInt(configSetupJson, "fsi"), 100);  //Grid Frequency
    //writeSingleRegInt("modbus115200", "inv001", "0x04", 20111, jsonReadInt(configSetupJson, "gpsi"), 0);   //Grid protection standart
    //writeSingleRegInt("modbus115200", "inv001", "0x04", 20108, jsonReadInt(configSetupJson, "gtei"), 0);   //Export to grid allowed

    //====================================service settings===========================================================================
    writeSingleRegInt("modbus115200", "inv001", "0x04", 10110, jsonReadInt(configSetupJson, "bti"), 0);
    writeSingleRegInt("modbus115200", "inv001", "0x04", 10111, jsonReadInt(configSetupJson, "cbti"), 0);

    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20113, jsonReadFloat(configSetupJson, "idsi"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20125, jsonReadFloat(configSetupJson, "igchmi"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 10108, jsonReadFloat(configSetupJson, "ipvmi"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20132, jsonReadFloat(configSetupJson, "ichmi"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20119, jsonReadFloat(configSetupJson, "vbsci"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20118, jsonReadFloat(configSetupJson, "vbsdi"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 10104, jsonReadFloat(configSetupJson, "vabi"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 10103, jsonReadFloat(configSetupJson, "vfli"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20127, jsonReadFloat(configSetupJson, "vbli"), 10);
    writeSingleRegFloat("modbus115200", "inv001", "0x04", 20128, jsonReadFloat(configSetupJson, "vbhi"), 10);

    writeSingleRegInt("modbus115200", "inv001", "0x04", 20144, jsonReadInt(configSetupJson, "spbi"), 0);
}

void setDefaultSysMode() {
    int setmode = jsonReadInt(configSetupJson, "setmode");
    if (setmode == 1) inverter_mode_1();
    if (setmode == 2) inverter_mode_2();
    if (setmode == 3) inverter_mode_3();
    if (setmode == 4) inverter_mode_4();
    if (setmode == 5) inverter_mode_5();
    if (setmode == 6) inverter_mode_6();
    if (setmode == 7) inverter_mode_7();
    if (setmode == 8) inverter_mode_8();
}

boolean isBatteryConnected(int batNum) {
    if (jsonReadInt(configSetupJson, "bnset") >= batNum && jsonReadInt(configSetupJson, "bnset") <= 5) {
        return true;
    } else {
        return false;
    }
}
