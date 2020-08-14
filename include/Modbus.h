#pragma once

#include <Arduino.h>
#include "ModbusToMqtt.h"
#include "Global.h"

extern void systemInit();
extern void startReadingRegister();
extern void delayNextSend(int delay);
extern void readingRagister();
extern void write_data_to_json(String slaveDeviceName, String jsonKeyBuf, uint16_t reqisterValue);
extern bool getResultMsg(ModbusMaster* modbus1, uint16_t result);
extern void setDefaults(); 
extern void setDefaultRegisterValues();
extern void setDafaultRegisterValuesFromJson();
extern void setDefaultSysMode();
extern void systemLoop();
extern boolean isBatteryConnected(int batNum);
