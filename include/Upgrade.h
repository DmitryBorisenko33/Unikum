#pragma once

#include <Arduino.h>
#include "Global.h"
#include "Mqtt.h"

extern void initUpgrade();
extern void upgrade_firmware();
extern void handle_upgrade();