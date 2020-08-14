#include "loopAppDataCalc.h"

#include "Mqtt.h"
#include "SystemModes.h"

void appDataCalc() {
    //==================================I.CALCULATION OF DIRECT VALUES===========================================================================================================
    //=========================================FROM WEB INTERFACE================================================================================================================
    jsonWriteInt(core001, "vgmincr", jsonReadInt(configSetupJson, "vgmin"));
    jsonWriteInt(core001, "vgmaxcr", jsonReadInt(configSetupJson, "vgmax"));
    jsonWriteInt(core001, "expcr", jsonReadInt(configSetupJson, "export"));
    jsonWriteInt(core001, "dsocr", jsonReadInt(configSetupJson, "dso"));
    jsonWriteInt(core001, "socfcr", jsonReadInt(configSetupJson, "socf"));
    jsonWriteInt(core001, "socmcr", jsonReadInt(configSetupJson, "socm"));
    jsonWriteInt(core001, "soclcr", jsonReadInt(configSetupJson, "socl"));

    jsonWriteInt(core001, "upstb", jsonReadInt(configSetupJson, "upstb"));
    jsonWriteInt(core001, "upstfs", jsonReadInt(configSetupJson, "upstfs"));
    jsonWriteInt(core001, "usermodecr", jsonReadInt(configSetupJson, "usermodecr"));
    //===============================================CB============================================================================================================================
    jsonWriteUint16(cb001, "vg1br", jsonReadUint16(cb001, "vgbr"));
    jsonWriteUint16(cb001, "ig1br", jsonReadUint16(cb001, "igbr"));
    jsonWriteUint16(cb001, "pg1br", jsonReadUint16(cb001, "pgbr"));

    jsonWriteUint16(cb002, "vg1br", jsonReadUint16(cb002, "vgbr"));
    jsonWriteUint16(cb002, "ig1br", jsonReadUint16(cb002, "igbr"));
    jsonWriteUint16(cb002, "pg1br", jsonReadUint16(cb002, "pgbr"));

    jsonWriteUint16(cb003, "vg1br", jsonReadUint16(cb003, "vgbr"));
    jsonWriteUint16(cb003, "ig1br", jsonReadUint16(cb003, "igbr"));
    jsonWriteUint16(cb003, "pg1br", jsonReadUint16(cb003, "pgbr"));

    jsonWriteUint16(cb004, "vg1br", jsonReadUint16(cb004, "vgbr"));
    jsonWriteUint16(cb004, "ig1br", jsonReadUint16(cb004, "igbr"));
    jsonWriteUint16(cb004, "pg1br", jsonReadUint16(cb004, "pgbr"));

    highlowJson(cb001, "eslbrl", "eslbrh", "eslbr", 10, true);
    highlowJson(cb002, "eslbrl", "eslbrh", "eslbr", 10, true);
    highlowJson(cb003, "eslbrl", "eslbrh", "eslbr", 10, true);
    highlowJson(cb004, "eslbrl", "eslbrh", "eslbr", 10, true);

    highlowJson(cb001, "ebybrl", "ebybrh", "ebybr", 10, true);
    highlowJson(cb002, "ebybrl", "ebybrh", "ebybr", 10, true);
    highlowJson(cb003, "ebybrl", "ebybrh", "ebybr", 10, true);
    highlowJson(cb004, "ebybrl", "ebybrh", "ebybr", 10, true);

    jsonWriteInt(cb001, "edirbr", 0);
    jsonWriteInt(cb002, "edirbr", 0);
    jsonWriteInt(cb003, "edirbr", 0);
    jsonWriteInt(cb004, "edirbr", 0);
    //============================================INVERTER=======================================================================================================================================
    int eci = ((jsonReadInt(inv001, "ecih") * 10) + jsonReadInt(inv001, "ecil"));
    int esui = ((jsonReadInt(inv001, "esuih") * 10) + jsonReadInt(inv001, "esuil"));
    int ebyi = ((jsonReadInt(inv001, "ebyih") * 10) + jsonReadInt(inv001, "ebyil"));
    int esli = ((jsonReadInt(inv001, "eslih") * 10) + jsonReadInt(inv001, "eslil"));
    int echi = ((jsonReadInt(inv001, "echih") * 10) + jsonReadInt(inv001, "echil"));
    int egchi = ((jsonReadInt(inv001, "egchih") * 10) + jsonReadInt(inv001, "egchil"));
    int edsi = ((jsonReadInt(inv001, "edsih") * 10) + jsonReadInt(inv001, "edsil"));
    int epvsi = ((jsonReadInt(inv001, "epvsih") * 10) + jsonReadInt(inv001, "epvsil"));
    int epvi = ((jsonReadInt(inv001, "epvih") * 10) + jsonReadInt(inv001, "epvil"));

    jsonWriteInt(inv001, "eci", eci);
    jsonWriteInt(inv001, "esui", esui);
    jsonWriteInt(inv001, "ebyi", ebyi);
    jsonWriteInt(inv001, "esli", esli);
    jsonWriteInt(inv001, "echi", echi);
    jsonWriteInt(inv001, "egchi", egchi);
    jsonWriteInt(inv001, "edsi", edsi);
    jsonWriteInt(inv001, "epvsi", epvsi);
    jsonWriteInt(inv001, "epvi", epvi);

    negativeValuesConverterInt16(inv001, "pgi");
    negativeValuesConverterInt16(inv001, "pi");

    //===========================II.CALCULATION OF CORE VALUES FROM DIRECT VALUES===================================================================================================================================================
    //===========================================BATTERY============================================================================================================================================================================
    int ichb = jsonReadInt(bat001, "ichb") + jsonReadInt(bat002, "ichb") + jsonReadInt(bat003, "ichb") + jsonReadInt(bat004, "ichb") + jsonReadInt(bat005, "ichb");
    jsonWriteInt(core001, "ichbcr", ichb);

    int idsb = jsonReadInt(bat001, "idsb") + jsonReadInt(bat002, "idsb") + jsonReadInt(bat003, "idsb") + jsonReadInt(bat004, "idsb") + jsonReadInt(bat005, "idsb");
    jsonWriteInt(core001, "idsbcr", idsb);

    int pchb = (jsonReadInt(inv001, "vdci") * ichb) * 1.15;
    jsonWriteInt(core001, "pchbcr", pchb);

    int pdsb = (jsonReadInt(inv001, "vdci") * idsb) * 1.15;
    jsonWriteInt(core001, "pdsbcr", pdsb);

    int itpv = jsonReadInt(configSetupJson, "itpv");
    jsonWriteInt(core001, "itpvcr", itpv);

    int socb = (jsonReadInt(bat001, "socb") + jsonReadInt(bat002, "socb") + jsonReadInt(bat003, "socb") + jsonReadInt(bat004, "socb") + jsonReadInt(bat005, "socb")) / jsonReadInt(configSetupJson, "bnset");
    jsonWriteInt(configSetupJson, "test", socb);

    int vgi = jsonReadInt(inv001, "vgi");

    int stpv = jsonReadInt(configSetupJson, "stpv");

    int socf = jsonReadInt(configSetupJson, "socf");
    int socm = jsonReadInt(configSetupJson, "socm");
    int socl = jsonReadInt(configSetupJson, "socl");

    jsonWriteInt(core001, "itpvcr", itpv);

    jsonWriteInt(core001, "peakflagcr", inPeak());

    //===========================================CB==============================================================================================================================================================================
    if (sysmod == 1 || sysmod == 2) {
        jsonWriteUint16(core001, "pccr", jsonReadUint16(cb001, "pgbr"));
    } else {
        jsonWriteUint16(core001, "pccr", jsonReadUint16(inv001, "pci"));
    }
    //===========================================INVERTER========================================================================================================================================================================
    jsonWriteInt(core001, "ppvcr", jsonReadInt(inv001, "ipvi") * jsonReadInt(inv001, "vdci"));
    //if (sysmod == 8) {
    //    jsonWriteInt(core001, "pgencr", jsonReadInt(inv001, "pgi"));
    //    jsonWriteInt(core001, "egencr", jsonReadInt(inv001, "ebyih") + jsonReadInt(inv001, "ebyil"));
    //    jsonWriteInt(core001, "vgencr", jsonReadInt(inv001, "vgi"));
    //} else {
    //    jsonWriteInt(core001, "pgencr", 0);
    //    jsonWriteInt(core001, "egencr", 0);
    //    jsonWriteInt(core001, "vgencr", 0);
    //}
    int eccr = jsonReadInt(cb001, "ebybr") - ebyi + eci + jsonReadInt(core001, "egencr");
    jsonWriteInt(core001, "eccr", eccr);

    //===========================III.ALGORITHMS EXECUTION=========================================================================================================================================================================
    //===================================comfort and economy==================================================================================================================================================================================
    if (jsonReadInt(configSetupJson, "manual") == 0) {
        mqtt_send_debug("off", "manual");
        //================================================================================================================================================================================================================================================
        if (jsonReadInt(configSetupJson, "usermodecr") == 1) {
            mqtt_send_debug("on", "comfort");

            mqtt_send_debug(String(socb), "socb");

            mqtt_send_debug(String(socf), "socf");
            mqtt_send_debug(String(socm), "socm");
            mqtt_send_debug(String(socl), "socl");

            mqtt_send_debug(String(ichb), "ichb");

            if (vgi > 0) {  //наличие напряжения на вводах сети
                mqtt_send_debug("connected", "grid");
                if (stpv == 1) {  //солнечные батареи подключены
                    mqtt_send_debug("connected", "solarBatery");
                    if (itpv == 0) {  //если солнца нет
                        mqtt_send_debug("no", "solar");
                        if (socb < socf) {
                            mqtt_send_debug("soc < full", "soc");
                            inverter_mode_1();  //заряжаем
                        } else if (socb >= socf) {
                            mqtt_send_debug("socb > full", "soc");
                            inverter_mode_2();  //не заряжаем
                        }
                    }
                    if (itpv > 0 && itpv < ichb) {  //если есть но не достаточно
                        mqtt_send_debug("not enough", "solar");
                        inverter_mode_2();
                    }
                    if (itpv > ichb) {  //если есть достаточно
                        mqtt_send_debug("enough", "solar");
                        if (socb < socf) {
                            mqtt_send_debug("soc < full", "soc");
                            inverter_mode_5();
                        } else {
                            if (jsonReadInt(configSetupJson, "export") == 1) {
                                inverter_mode_6();
                            } else {
                                inverter_mode_7();
                            }
                        }
                    }
                } else {  //солнечные батареи не подключены
                    mqtt_send_debug("not connected", "solarBatery");
                    if (socb < socf) {
                        mqtt_send_debug("soc < full", "soc");
                        inverter_mode_1();  //заряжаем
                    } else if (socb >= socf) {
                        mqtt_send_debug("socb > full", "soc");
                        inverter_mode_2();  //не заряжаем
                    }
                }
            } else {  //сеть не подключена
                mqtt_send_debug("not connected", "grid");
                if (socb > socl) {
                    inverter_mode_3();
                } else {
                    inverter_mode_8();
                }
            }
        } else {  //данный режим выключен
            mqtt_send_debug("off", "comfort");
        }
        //=============================================================================================================================================================================================================================================================
        if (jsonReadInt(configSetupJson, "usermodecr") == 2) {
            mqtt_send_debug("on", "economy");

            mqtt_send_debug(String(socb), "socb");

            mqtt_send_debug(String(socf), "socf");
            mqtt_send_debug(String(socm), "socm");
            mqtt_send_debug(String(socl), "socl");

            mqtt_send_debug(String(ichb), "ichb");

            if (vgi > 0) {  //наличие напряжения на вводах сети
                mqtt_send_debug("connected", "grid");
                if (stpv == 1) {  //солнечные батареи подключены
                    mqtt_send_debug("connected", "solarBatery");

                    if (itpv == 0) {  //если солнца нет
                        mqtt_send_debug("no", "solar");
                        //if (socb < socf) {
                        //    mqtt_send_debug("soc < full", "soc");
                        //    inverter_mode_1();  //заряжаем
                        //} else if (socb >= socf) {
                        //    mqtt_send_debug("socb > full", "soc");
                        //    inverter_mode_2();  //не заряжаем
                        //}
                    }
                    if (itpv > 0 && itpv < ichb) {  //если есть но не достаточно
                        mqtt_send_debug("not enough", "solar");
                        //inverter_mode_2();
                    }
                    if (itpv > ichb) {  //если есть достаточно
                        mqtt_send_debug("enough", "solar");
                        //if (socb < socf) {
                        //    mqtt_send_debug("soc < full", "soc");
                        //    inverter_mode_5();
                        //} else {
                        //    if (jsonReadInt(configSetupJson, "export") == 1) {
                        //        inverter_mode_6();
                        //    } else {
                        //        inverter_mode_7();
                        //    }
                        //}
                    }
                } else {  //солнечные батареи не подключены
                    mqtt_send_debug("not connected", "solarBatery");
                    //if (socb < socf) {
                    //    mqtt_send_debug("soc < full", "soc");
                    //    inverter_mode_1();  //заряжаем
                    //} else if (socb >= socf) {
                    //    mqtt_send_debug("socb > full", "soc");
                    //    inverter_mode_2();  //не заряжаем
                    //}
                }
            } else {  //сеть не подключена
                mqtt_send_debug("not connected", "grid");
                //if (socb > socl) {
                //    inverter_mode_3();
                //} else {
                //    inverter_mode_8();
                //}
            }
        } else {  //данный режим выключен
            mqtt_send_debug("off", "economy");
        }
        //=============================================================================================================================================================================================================
    } else {  //включен режим ручного управления
        mqtt_send_debug("on", "manual");
    }
}

void highlowJson(String& file, String keyL, String keyH, String keyOut, float coef, bool direct) {
    uint16_t keyLu = jsonReadUint16(file, keyL);
    uint16_t keyHu = jsonReadUint16(file, keyH);
    uint32_t out;
    if (direct) {
        out = keyLu << 16;
        out += keyHu;
    } else {
        out = keyLu;
        out += keyHu << 16;
    }
    if (coef != 0) out = out / coef;
    jsonWriteUint32(file, keyOut, out);
}

void negativeValuesConverterInt16(String& file, String key) {
    int16_t tmp = jsonReadUint16(file, key);
    jsonWriteInt16(file, key, tmp);
}

bool inPeak() {
    bool ret = false;

    long zone = 3600 * jsonReadStr(configSetupJson, "timezone").toInt();

    long timeNowGMT = GetTimeUnix().toInt();
    long timeNowLocal = timeNowGMT + zone;

    long timeToDayStartGMT = timeNowGMT - timeToSec(GetTimeGMT());
    long timeToDayStartLocal = timeNowLocal - timeToSec(GetTime());

    long startPeakTimeLocal = timeToDayStartLocal + timeToSec(jsonReadStr(configSetupJson, "pt"));
    long endPeakTimeLocal = timeToDayStartLocal + timeToSec(jsonReadStr(configSetupJson, "opt"));

    long startPeakTimeGMT = startPeakTimeLocal - zone;
    long endPeakTimeGMT = endPeakTimeLocal - zone;

    if (timeNowGMT >= startPeakTimeGMT && timeNowGMT <= endPeakTimeGMT) {
        ret = true;
    }
    mqtt_send_debug(String(timeToDayStartGMT), "timeToDayStartGMT");
    mqtt_send_debug(String(timeNowGMT), "timeNowGMT");
    mqtt_send_debug(String(startPeakTimeGMT), "startPeakTimeGMT");
    mqtt_send_debug(String(endPeakTimeGMT), "endPeakTimeGMT");
    return ret;
}
