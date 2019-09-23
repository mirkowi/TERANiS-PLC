//
// Created by mirko on 14.09.2019.
//

#include "plc.h"
#include "plcio.h"
#include "teranis.h"
#include "MModbusTCPServer.h"
#include <time.h>
#include <sys/time.h>

// Singleton Instanz des MODBUS-Servers
TMModbusTCPServer modbusTcpServer;

// Singleton Instanz des IO-Systems
TPlcIo plcIo;

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
        if ( usDiff < 1000000) usleep(1000000-usDiff);
    }
    lastTime = now;

    // Read Inputs
    plcIo.read();

    // Cycle PLC-Program
    plcTask.run();

    // Zykluszeiten System-Flags schreiben
    S(uint32_t,10) = plcTask.getCycleActMs();
    S(uint32_t,14) = plcTask.getMinCycleActMs();
    S(uint32_t,18) = plcTask.getMaxCycleActMs();
    S(int64_t,22) = usTicks; // Mikrosekunden

    // Write Outputs
    plcIo.write();

    // Client Communication
    modbusTcpServer.run();
}


