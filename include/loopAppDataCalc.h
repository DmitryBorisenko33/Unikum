#pragma once

#include <Arduino.h>
#include "Global.h"

extern void coreDataCalc();
extern void appDataCalc();
extern void highlowJson(String& file, String keyL, String keyH, String keyOut, float coef, bool direct);
extern void negativeValuesConverterInt16(String& file, String key);
extern void pubMqttData();
extern bool inPeak();