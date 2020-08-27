#pragma once
#include <Arduino.h>


void saveConfig();

int count(String str, String found);

boolean isDigitStr(String str);

void safeDataToFile(String data, String Folder);

String readFile(String fileName, size_t len);

String sizeFile(String fileName);

String writeFile(String fileName, String strings);

String addFile(String fileName, String strings);

String readFileString(const String& filename, const String& str_to_found);