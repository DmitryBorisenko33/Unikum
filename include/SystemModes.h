#pragma once

#include <Arduino.h>

extern void inverter_mode_1();
extern void inverter_mode_2();
extern void inverter_mode_3();
extern void inverter_mode_4();
extern void inverter_mode_5();
extern void inverter_mode_6();
extern void inverter_mode_7();
extern void inverter_mode_8();
extern void cb_set_address(String currentAddr, String setAddr);
extern void writeSingleRegInt(String modbus, String devName, String devAddress, int reqisterNum, int value, int coef);
extern void writeSingleRegFloat(String modbus, String devName, String devAddress, int reqisterNum, float value, int coef);
