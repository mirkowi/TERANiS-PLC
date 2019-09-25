//
// Created by mirko on 14.09.2019.
//

#include "plc.h"
#include "teranis.h"
#include "MModbusTCPServer.h"
#include <sys/time.h>

// Hier den Typ des IO-Systems anpassen
#include "PlcIoRevPi.h"

// Singleton Instanz des IO-Systems
// Hier den Typ des IO-Systems anpassen
TPlcIoRevPi plcIo;

// Singleton Instanz des MODBUS-Servers
TMModbusTCPServer modbusTcpServer;

void TPlc::begin() {
    plcIo.begin();
    setup_teranis();
    plcTask.begin();
    plcTask.begin();
    modbusTcpServer.setMbport(mbport);
    modbusTcpServer.begin();
}

void TPlc::run() {
    timeval now;
    gettimeofday(&now,0);
    // Aus dem Zeitvergleich Ticks ausrechnen
    if (lastTime.tv_sec == 0) usTicks=0;
    else {
        long usDiff = (now.tv_sec - lastTime.tv_sec)*1000000 + (now.tv_usec - lastTime.tv_usec);
        usTicks += usDiff;
        // minimale Zykluszeit auf 1ms eingestellt, den Rest verschlafen
        if ( usDiff < 1000) usleep(1000-usDiff);
    }
    lastTime = now;

    // Read Inputs
    plcIo.read();

    // Cycle PLC-Program
    plcTask.run();

    // Zykluszeiten System-Flags schreiben
    cycleActMs = plcTask.getCycleActMs();
    minCycleActMs = plcTask.getMinCycleActMs();
    maxCycleActMs = plcTask.getMaxCycleActMs();

    // Write Outputs
    plcIo.write();

    // Client Communication
    modbusTcpServer.run();
}


