#pragma once

#include <Arduino.h>
#include <Mqtt.h>

#include "Global.h"

class ModbusToMqtt {
   private:
    String _readTable;
    String _writeTable;

    String slaveDeviceNameReadingPrev;
    String slaveDeviceNameWritingPrev;

    bool nextReading;
    bool nextWriting;

    bool busyReading;
    bool busyWriting;

    bool skipReading;
    bool skipWriting;

    unsigned long currentMillisReading;
    unsigned long prevMillisReading;
    unsigned long differenceReading;
    volatile unsigned int delayReading;

    unsigned long currentMillisWriting;
    unsigned long prevMillisWriting;
    unsigned long differenceWriting;
    volatile unsigned int delayWriting;

    unsigned int readingAttempts;
    unsigned int readingCount;
    unsigned int writingAttempts;
    unsigned int writingCount;

    unsigned int readingFatalErrors;
    unsigned int writingFatalErrors;

    ModbusMaster _modbus;
    HardwareSerial _uart;

    String toPrintR;
    String toPrintW;

#define mapfiles LITTLEFS

   public:
    ModbusToMqtt(HardwareSerial uart, ModbusMaster modbus) :

                                                             _readTable{""},
                                                             _writeTable{""},

                                                             slaveDeviceNameReadingPrev{""},
                                                             slaveDeviceNameWritingPrev{""},

                                                             nextReading{false},
                                                             nextWriting{false},

                                                             busyReading{false},
                                                             busyWriting{false},

                                                             skipReading{false},
                                                             skipWriting{false},

                                                             currentMillisReading{0},
                                                             prevMillisReading{0},
                                                             differenceReading{0},
                                                             delayReading{0},

                                                             currentMillisWriting{0},
                                                             prevMillisWriting{0},
                                                             differenceWriting{0},
                                                             delayWriting{0},

                                                             readingAttempts{30},
                                                             readingCount{0},
                                                             writingAttempts{30},
                                                             writingCount{0},

                                                             readingFatalErrors{0},
                                                             writingFatalErrors{0},

                                                             _modbus{modbus},
                                                             _uart{uart},

                                                             toPrintR{""},
                                                             toPrintW{""}

                                                             {};
    //==========================================LOOP======================================

    void loop() {
        currentMillisReading = millis();
        differenceReading = currentMillisReading - prevMillisReading;
        if (differenceReading >= delayReading && nextReading) {
            if (!busyWriting) {
                readingRagister();
            }
        }

        currentMillisWriting = millis();
        differenceWriting = currentMillisWriting - prevMillisWriting;
        if (differenceWriting >= delayWriting && nextWriting) {
            if (!busyReading) {
                writingRagister();
            }
        }
    }

    //=================================READING SECTION=========================================================

    void updateReadTableFromFile(String fileForReading) {
        if (!busyReading) {
            if (fileForReading != "") {
                _readTable += readMap(fileForReading, 5120) + "\n";
                nextReading = true;
            }
        }
    }

    void updateReadTableFromStr(String strForReading) {
        if (!busyReading) {
            if (strForReading != "") {
                _readTable += strForReading + "\n";
                nextReading = true;
            }
        }
    }

    bool busyForReading() {
        return busyReading;
    }

    void readingRagister() {
        //======================================================================
        busyReading = true;
        nextReading = false;
        //======================================================================
        //50;inv001;0x04;0x03;25206;*;0;+;0;vi;
        String tmp = selectToMarkerMap(_readTable, "\n");
        //50;
        delayReading = selectToMarkerMap(tmp, ";").toInt();
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //inv001;
        String slaveDeviceName = selectToMarkerMap(tmp, ";");
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //0x04;
        uint8_t slaveDevAddress = hexStringToUint8(selectToMarkerMap(tmp, ";"));
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //0x03;
        uint8_t modbusReqType = hexStringToUint8(selectToMarkerMap(tmp, ";"));
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //25206;
        uint16_t registerNumber = hexStringToUint16(selectToMarkerMap(tmp, ";"));
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //*;
        String sign1 = selectToMarkerMap(tmp, ";");
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //0;
        float coff1 = selectToMarkerMap(tmp, ";").toFloat();
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //+;
        String sign2 = selectToMarkerMap(tmp, ";");
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //0;
        float coff2 = selectToMarkerMap(tmp, ";").toFloat();
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //vi;
        String jsonKeyBuf = selectToMarkerMap(tmp, ";");
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //======================================================================
        if (slaveDeviceNameReadingPrev == slaveDeviceName && skipReading) {
            printLogR("===========>SKIP READING " + slaveDeviceName + "\n");
            _readTable = deleteBeforeDelimiterMap(_readTable, "\n");
            isReadingCompleted();
            slaveDeviceNameReadingPrev = slaveDeviceName;
            return;
        }
        //======================================================================
        if (slaveDeviceNameReadingPrev != slaveDeviceName) {
            printLogR("===========>READING OF: " + slaveDeviceName + "\n");
            skipReading = false;
        }
        slaveDeviceNameReadingPrev = slaveDeviceName;
        //======================================================================
        _modbus.begin(slaveDevAddress, _uart);
        uint16_t reqisterValue = _modbus.readHoldingRegisters(registerNumber, 1);
        //======================================================================
        printLogR("R; name: ");
        printLogR(String(slaveDeviceName));
        printLogR("; ");
        printLogR("addr: ");
        printLogR(String(slaveDevAddress));
        printLogR("; ");
        printLogR("reg: ");
        printLogR(String(registerNumber));
        printLogR("; ");
        printLogR("id: ");
        printLogR(jsonKeyBuf);
        printLogR("; ");
        //======================================================================
        if (getResultMsg(&_modbus, reqisterValue)) {
            reqisterValue = _modbus.getResponseBuffer(0);

            reqisterValue = correction(reqisterValue, sign1, coff1);

            printLogR("val: ");
            printLogR(String(reqisterValue));
            printLogR(";\n");

            writeToJson(slaveDeviceName, jsonKeyBuf, reqisterValue);

            _readTable = deleteBeforeDelimiterMap(_readTable, "\n");

            readingCount = 0;

        } else {
            printLogR("val: error");
            printLogR(";\n");

            readingCount++;
            if (readingCount >= readingAttempts) {
                printLogR("===========>FATAL READING ERROR\n");
                readingFatalErrors++;
                skipReading = true;
                readingCount = 0;
            }
        }
        //======================================================================
        if (isReadingCompleted()) printLogR("===========>READING end=========\n");
//==============================================================================
#ifdef printRegisterReadingLog
        Serial.print(toPrintR);
#endif
#ifdef publishRegisterReadingLogToMqtt
        modbusLog += toPrintR;
#endif
        toPrintR = "";
//==============================================================================
    }

    uint16_t correction(uint16_t reqisterValue, String sign1, float coff1) {
        if (coff1 != 0) {
            printLogR("[");
            printLogR(String(reqisterValue));

            if (sign1 == "*") {
                reqisterValue = reqisterValue * coff1;
            }
            if (sign1 == "/") {
                reqisterValue = reqisterValue / coff1;
            }
            printLogR(sign1);
            printLogR(String(coff1));
            printLogR("]; ");
        }
        return reqisterValue;
    }

    bool isReadingCompleted() {
        if (_readTable == "") {
            busyReading = false;
            return true;
        } else {
            nextReading = true;
            prevMillisReading = millis();
            return false;
        }
    }

    //===================================WRITING SECTION=======================================================

    void updateWriteTableFromFile(String fileForWriting) {
        if (fileForWriting != "") {
            _writeTable += readMap(fileForWriting, 5120) + "\n";
            nextWriting = true;
        }
    }

    void updateWriteTableFromStr(String strForWriting) {
        if (strForWriting != "") {
            _writeTable += strForWriting + "\n";
            nextWriting = true;
        }
    }

    bool busyForWriting() {
        return busyWriting;
    }

    void writingRagister() {
        busyWriting = true;
        nextWriting = false;
        //======================================================================
        //50;inv001;0x04;0x06;20109;3
        String tmp = selectToMarkerMap(_writeTable, "\n");
        //50;
        delayWriting = selectToMarkerMap(tmp, ";").toInt();
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //inv001;
        String slaveDeviceName = selectToMarkerMap(tmp, ";");
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //0x04;
        uint8_t slaveDevAddress = hexStringToUint8(selectToMarkerMap(tmp, ";"));
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //0x06;
        uint8_t modbusReqType = hexStringToUint8(selectToMarkerMap(tmp, ";"));
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //20109;
        uint16_t registerNumber = hexStringToUint16(selectToMarkerMap(tmp, ";"));
        tmp = deleteBeforeDelimiterMap(tmp, ";");
        //;3
        uint16_t value = hexStringToUint16(selectToMarkerMap(tmp, ";"));
        //======================================================================
        if (slaveDeviceNameWritingPrev == slaveDeviceName && skipWriting) {
            printLogW("===========>SKIP WRITING " + slaveDeviceName + "\n");
            _writeTable = deleteBeforeDelimiterMap(_writeTable, "\n");
            isWritingCompleted();
            slaveDeviceNameWritingPrev = slaveDeviceName;
            return;
        }
        //======================================================================
        if (slaveDeviceNameWritingPrev != slaveDeviceName) {
            printLogW("===========>WRITING OF: " + slaveDeviceName + "\n");
            skipWriting = false;
        }
        slaveDeviceNameWritingPrev = slaveDeviceName;
        //======================================================================
        printLogW("W; name: ");
        printLogW(String(slaveDeviceName));
        printLogW("; ");
        printLogW("addr: ");
        printLogW(String(slaveDevAddress));
        printLogW("; ");
        printLogW("reg: ");
        printLogW(String(registerNumber));
        printLogW("; ");
        printLogW("val: ");
        printLogW(String(value));
        printLogW("; ");
        //======================================================================
        _modbus.begin(slaveDevAddress, _uart);
        uint8_t result = _modbus.writeSingleRegister(registerNumber, value);
        //======================================================================
        if (result == _modbus.ku8MBSuccess) {
            printLogW("ok;\n");
            _writeTable = deleteBeforeDelimiterMap(_writeTable, "\n");
            writingCount = 0;
        } else {
            printLogW("error;\n");
            writingCount++;
            if (writingCount >= writingAttempts) {
                printLogW("FATAL WRITING ERROR\n");
                writingFatalErrors++;
                writingCount = 0;
                skipWriting = true;
            }
        }
        //======================================================================
        if (isWritingCompleted()) printLogW("===========>WRITING end=========\n");
//==============================================================================
#ifdef printRegisterWritingLog
        Serial.print(toPrintW);
#endif
#ifdef publishRegisterWritingLogToMqtt
        modbusLog += toPrintW;
#endif
        toPrintW = "";
        //===============================================================================
    }

    bool isWritingCompleted() {
        if (_writeTable == "") {
            busyWriting = false;
            return true;
        } else {
            nextWriting = true;
            prevMillisWriting = millis();
            return false;
        }
    }

    //==================================================OTHER SECTION======================================
    bool getResultMsg(ModbusMaster* _modbus, uint16_t result) {
        String tmpstr;
        switch (result) {
            case _modbus->ku8MBSuccess:
                return true;
                tmpstr += "Ok";
                break;
            case _modbus->ku8MBIllegalFunction:
                tmpstr += "Illegal Function";
                break;
            case _modbus->ku8MBIllegalDataAddress:
                tmpstr += "Illegal Data Address";
                break;
            case _modbus->ku8MBIllegalDataValue:
                tmpstr += "Illegal Data Value";
                break;
            case _modbus->ku8MBSlaveDeviceFailure:
                tmpstr += "Slave Device Failure";
                break;
            case _modbus->ku8MBInvalidSlaveID:
                tmpstr += "Invalid Slave ID";
                break;
            case _modbus->ku8MBInvalidFunction:
                tmpstr += "Invalid Function";
                break;
            case _modbus->ku8MBResponseTimedOut:
                tmpstr += "Response Timed Out";
                break;
            case _modbus->ku8MBInvalidCRC:
                tmpstr += "Invalid CRC";
                break;
            default:
                tmpstr += "Unknown error: " + String(result);
                break;
        }
        printLogR(tmpstr + "; ");
        return false;
    }

    int getWritingFatalErrors() {
        return writingFatalErrors;
    }

    int getReadingFatalErrors() {
        return readingFatalErrors;
    }

    //=====================================UTILITES SECTION===================================================
    String readMap(String fileName, size_t len) {
        File configFile = mapfiles.open("/" + fileName, "r");
        if (!configFile) {
            return "Failed";
        }
        size_t size = configFile.size();
        if (size > len) {
            configFile.close();
            return "Large";
        }
        String temp = configFile.readString();
        configFile.close();
        return temp;
    }

    String selectToMarkerMap(String str, String found) {
        int p = str.indexOf(found);
        return str.substring(0, p);
    }

    String deleteBeforeDelimiterMap(String str, String found) {
        int p = str.indexOf(found) + found.length();
        return str.substring(p);
    }

    void writeToJson(String slaveDeviceName, String jsonKeyBuf, uint16_t reqisterValue) {
        if (slaveDeviceName == "inv001") {
            jsonWriteUint16C(inv001, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "bat001") {
            jsonWriteUint16C(bat001, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "bat002") {
            jsonWriteUint16C(bat002, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "bat003") {
            jsonWriteUint16C(bat003, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "bat004") {
            jsonWriteUint16C(bat004, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "bat005") {
            jsonWriteUint16C(bat005, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "cb001") {
            jsonWriteUint16C(cb001, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "cb002") {
            jsonWriteUint16C(cb002, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "cb003") {
            jsonWriteUint16C(cb003, jsonKeyBuf, reqisterValue);
        }
        if (slaveDeviceName == "cb004") {
            jsonWriteUint16C(cb004, jsonKeyBuf, reqisterValue);
        }
    }

    String jsonWriteUint16C(String& json, String name, uint16_t volume) {
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(json);
        root[name] = volume;
        json = "";
        root.printTo(json);
        return json;
    }

    String jsonWriteStrC(String& json, String name, String volume) {
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(json);
        root[name] = volume;
        json = "";
        root.printTo(json);
        return json;
    }

    void printLogR(String msg) {
#ifdef publishRegisterReadingLogToMqtt
        toPrintR += msg;
#endif
    }

    void printLogW(String msg) {
#ifdef publishRegisterWritingLogToMqtt
        toPrintW += msg;
#endif
    }
};

extern ModbusToMqtt* modbus115200;
extern ModbusToMqtt* modbus9600;
