#pragma once
#include <Arduino.h>

void Time_Init();

void time_check();

void reconfigTime();

String GetTimeUnix();

String GetFormattedTimeLocal();

String GetFormattedTimeGmt();

long timeToSec(String Time);
