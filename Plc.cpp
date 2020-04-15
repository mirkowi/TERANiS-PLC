//
// Created by mirko on 14.09.2019.
//

#include "Plc.h"
#include "teranis.h"
#include "MModbusTCPServer.h"

#include "PlcIo.h"

#ifdef ENV_MRAA
#include "PlcIoMraa.h"
#include <sys/time.h>
#elif defined(ENV_REVPI)
#include "PlcIoRevPi.h"
#include <sys/time.h>
#elif defined(ENV_WINDOWS)
#warning "I/O is not implemented in ENV_WINDOWS"
#else
#error "Unknown environment"
#endif

// Singleton Instanz des IO-Systems
TPlcIo *plcIo = NULL;

// Singleton Instanz des MODBUS-Servers
TMModbusTCPServer modbusTcpServer;

void TPlc::begin() {
#ifdef ENV_MRAA
    plcIo = new TPlcIoMraa();
#endif
#if ENV_REVPI
    plcIo = new TPlcIoRevPi();
#endif
    if (plcIo != NULL) {
        plcIo->begin();
    }
    setup_teranis();
    plcTask.begin();
    if (mbport > 0) {
        modbusTcpServer.setMbport(mbport);
        modbusTcpServer.begin();
    }
}

void TPlc::run() {
    timeval now;
    gettimeofday(&now, 0);
    // Aus dem Zeitvergleich Ticks ausrechnen
    if (lastTime.tv_sec == 0) S_usTicks = 0;
    else {
        long usDiff = (now.tv_sec - lastTime.tv_sec) * 1000000 + (now.tv_usec - lastTime.tv_usec);
        S_usTicks += usDiff;
        // Wartezeit bis zum minimalen Zyklus berechnen
        lastSleep = (long) minCycleSetMs * 1000 - usDiff + lastSleep;
        // Den Rest der minimalen Zykluszeit verschlafen
        if (lastSleep > 0) {
#ifdef _WIN32
            Sleep(lastSleep / 1000);
#else
            usleep(lastSleep);
#endif
        } else {
            lastSleep = 0;
        }
    }
    lastTime = now;

    // Read Inputs
    if (plcIo != NULL) {
        plcIo->read();
    }

    // Cycle PLC-Program
    plcTask.run();

    // Zykluszeiten System-Flags schreiben
    S_cycleActMs = plcTask.getCycleActMs();
    S_minCycleActMs = plcTask.getMinCycleActMs();
    S_maxCycleActMs = plcTask.getMaxCycleActMs();

    // Write Outputs
    if (plcIo != NULL) {
        plcIo->write();
    }

    if (mbport > 0) {
        // Client Communication
        modbusTcpServer.run();
    }
}

void TPlc::end() {
    if (plcIo != NULL) {
        plcIo->end();
    }
    plcTask.end();
    if (mbport > 0) {
        modbusTcpServer.end();
    }
    plcIo = NULL;
}

