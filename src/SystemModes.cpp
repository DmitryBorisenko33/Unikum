#include "SystemModes.h"

#include "DG.h"
#include "Global.h"
#include "ModbusToMqtt.h"

void inverter_mode_1() {
    if (sysmod != 1) {
        modbus115200->updateWriteTableFromFile("maps/mod1.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 1);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("1", "mode");
        mqtt_send_debug("from grid", "home");
        mqtt_send_debug("charging from grid", "battery");
        mqtt_send_debug("OFF", "export");
    }
}

void inverter_mode_2() {
    if (sysmod != 2) {
        modbus115200->updateWriteTableFromFile("maps/mod2.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 2);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("2", "mode");
        mqtt_send_debug("from grid", "home");
        mqtt_send_debug("charging OFF", "battery");
        mqtt_send_debug("OFF", "export");
    }
}

void inverter_mode_3() {
    if (sysmod != 3) {
        modbus115200->updateWriteTableFromFile("maps/mod3.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 3);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("3", "mode");
        mqtt_send_debug("from solar and battery", "home");
        mqtt_send_debug("charging OFF", "battery");
        mqtt_send_debug("OFF", "export");
    }
}

void inverter_mode_4() {
    if (sysmod != 4) {
        modbus115200->updateWriteTableFromFile("maps/mod4.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 4);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("4", "mode");
        mqtt_send_debug("from solar and battery", "home");
        mqtt_send_debug("charging OFF", "battery");
        mqtt_send_debug("ON", "export");
    }
}

void inverter_mode_5() {
    if (sysmod != 5) {
        modbus115200->updateWriteTableFromFile("maps/mod5.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 5);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("5", "mode");
        mqtt_send_debug("from solar and grid", "home");
        mqtt_send_debug("charging OFF", "battery");
        mqtt_send_debug("OFF", "export");
    }
}

void inverter_mode_6() {
    if (sysmod != 6) {
        modbus115200->updateWriteTableFromFile("maps/mod6.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 6);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("6", "mode");
        mqtt_send_debug("from solar and grid", "home");
        mqtt_send_debug("charging OFF", "battery");
        mqtt_send_debug("ON", "export");
    }
}

void inverter_mode_7() {
    if (sysmod != 7) {
        modbus115200->updateWriteTableFromFile("maps/mod7.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 7);
        saveConfig();
        stop_dg_procedure();

        mqtt_send_debug("7", "mode");
        mqtt_send_debug("from solar and grid", "home");
        mqtt_send_debug("charging OFF", "battery");
        mqtt_send_debug("ON", "export");
    }
}

void inverter_mode_8() {
    if (sysmod != 8) {
        modbus115200->updateWriteTableFromFile("maps/mod8.w.txt");
        jsonWriteInt(configSetupJson, "setmode", 8);
        saveConfig();
        start_dg_procedure();
        mqtt_send_debug("8", "mode");
    }
}

void cb_set_address(String currentAddr, String setAddr) {
    modbus9600->updateWriteTableFromFile("500;80;setAddr;1;" + currentAddr + ";0x06;0001;" + setAddr + "\n");
}

void writeSingleRegInt(String modbus, String devName, String devAddress, int reqisterNum, int value, int coef) {
    //50;inv001;0x04;0x06;20109;3
    if (coef != 0) value = value * coef;
    if (modbus == "modbus115200") {
        modbus115200->updateWriteTableFromStr("50;" + devName + ";" + devAddress + ";0x06;" + String(reqisterNum) + ";" + String(value));
    }
    if (modbus == "modbus9600") {
        modbus115200->updateWriteTableFromStr("50;" + devName + ";" + devAddress + ";0x06;" + String(reqisterNum) + ";" + String(value));
    }
}

void writeSingleRegFloat(String modbus, String devName, String devAddress, int reqisterNum, float value, int coef) {
    int valueInt;
    if (coef != 0) valueInt = value * coef;
    if (modbus == "modbus115200") {
        modbus115200->updateWriteTableFromStr("50;" + devName + ";" + devAddress + ";0x06;" + String(reqisterNum) + ";" + String(valueInt));
    }
    if (modbus == "modbus9600") {
        modbus115200->updateWriteTableFromStr("50;" + devName + ";" + devAddress + ";0x06;" + String(reqisterNum) + ";" + String(valueInt));
    }
}