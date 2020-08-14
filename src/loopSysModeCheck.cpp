#include "loopSysModeCheck.h"

#include "Global.h"

void sysModeCheck() {
    
    int ospi = jsonReadInt(inv001, "ospi");
    int gpsi = jsonReadInt(inv001, "gpsi");
    int sspi = jsonReadInt(inv001, "sspi");
    int gtei = jsonReadInt(inv001, "gtei");
    int cspi = jsonReadInt(inv001, "cspi");

    if (ospi == 3 && gpsi == 0 && sspi == 1 && gtei == 0 && cspi == 2) {
        sysmod = 1;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 3 && gpsi == 0 && sspi == 1 && gtei == 0 && cspi == 3) {
        sysmod = 2;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 1 && gpsi == 0 && sspi == 0 && gtei == 0 && cspi == 3) {
        sysmod = 3;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 1 && gpsi == 0 && sspi == 0 && gtei == 1 && cspi == 3) {
        sysmod = 4;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 2 && gpsi == 0 && sspi == 1 && gtei == 0 && cspi == 0) {
        sysmod = 5;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 2 && gpsi == 0 && sspi == 1 && gtei == 1 && cspi == 3) {
        sysmod = 6;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 4 && gpsi == 0 && sspi == 0 && gtei == 0 && cspi == 3) {
        sysmod = 7;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else if (ospi == 3 && gpsi == 3 && sspi == 1 && gtei == 0 && cspi == 2) {
        sysmod = 8;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    } else {
        sysmod = 0;
        jsonWriteInt(core001, "sysmodcr", sysmod);
    }

    //mqtt_send_debug("System mode...", "sysmod");
    
}