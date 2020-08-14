#pragma once
#include <Arduino.h>

String jsonReadStr(String& json, String name);

int jsonReadInt(String& json, String name);

float jsonReadFloat(String& json, String name);

uint16_t jsonReadUint16(String& json, String name);

String jsonWriteStr(String& json, String name, String volume);

String jsonWriteInt(String& json, String name, int volume);

String jsonWriteInt16(String& json, String name, int16_t volume);

String jsonWriteInt32(String& json, String name, int32_t volume);

String jsonWriteUint16(String& json, String name, uint16_t volume);

String jsonWriteUint32(String& json, String name, uint32_t volume);

String jsonWriteFloat(String& json, String name, float volume);
