//
// Created by mirko on 14.09.2019.
//

#include "Plc.h"
#include "teranis.h"
#include "MModbusTCPServer.h"
#include <sys/time.h>

// Hier den Typ des IO-Systems anpassen
#include "PlcIoMraa.h"

// Singleton Instanz des IO-Systems
// Hier den Typ des IO-Systems anpassen
TPlcIoMraa plcIo;

// Singleton Instanz des MODBUS-Servers
TMModbusTCPServer modbusTcpServer;

void TPlc::begin() {
    plcIo.begin();
    setup_teranis();
    plcTask.begin();
    if (mbport>0) {
        modbusTcpServer.setMbport(mbport);
        modbusTcpServer.begin();
    }
}

void TPlc::run() {
    timeval now;
    gettimeofday(&now,0);
    // Aus dem Zeitvergleich Ticks ausrechnen
    if (lastTime.tv_sec == 0) S_usTicks=0;
    else {
        long usDiff = (now.tv_sec - lastTime.tv_sec)*1000000 + (now.tv_usec - lastTime.tv_usec);
        S_usTicks += usDiff;
        // Wartezeit bis zum minimalen Zyklus berechnen
        lastSleep = (long)minCycleSetMs*1000-usDiff+lastSleep;
        // Den Rest der minimalen Zykluszeit verschlafen
        if ( lastSleep>0 ) usleep(lastSleep); else lastSleep=0;
    }
    lastTime = now;

    // Read Inputs
    plcIo.read();

    // Cycle PLC-Program
    plcTask.run();

    // Zykluszeiten System-Flags schreiben
    S_cycleActMs = plcTask.getCycleActMs();
    S_minCycleActMs = plcTask.getMinCycleActMs();
    S_maxCycleActMs = plcTask.getMaxCycleActMs();

    // Write Outputs
    plcIo.write();

    if (mbport>0) {
        // Client Communication
        modbusTcpServer.run();
    }
}

void TPlc::end() {
    plcIo.end();
    plcTask.end();
    if (mbport>0) {
        modbusTcpServer.end();
    }
}

