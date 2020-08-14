#include "loopCoreDataCalc.h"

#include "Global.h"

void coreDataCalc() {
    jsonWriteInt(core001, "uptcr", millis());
    jsonWriteInt(core001, "frrcr", ESP.getFreeHeap());
    jsonWriteInt(core001, "setmodcr", jsonReadInt(configSetupJson, "setmode"));

    jsonWriteInt(core001, "mb1rfcr", modbus115200->getReadingFatalErrors());
    jsonWriteInt(core001, "mb1wfcr", modbus115200->getWritingFatalErrors());

    jsonWriteInt(core001, "mb2rfcr", modbus9600->getReadingFatalErrors());
    jsonWriteInt(core001, "mb2wfcr", modbus9600->getWritingFatalErrors());
}