#pragma once
#include <Arduino.h>

void Time_Init();

void time_check();

void reconfigTime();

/*
* Получение текущего времени
*/
String GetTime();

String GetTimeGMT();

String GetTimeUnix();

String GetTimeWOsec();

String GetDate();

String GetDataDigital();

long timeToSec(String Time);
