#include <iostream>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include "Plc.h"

#include "TeranisTask.h"

#include "MModbusTCPServer.h"
#include "OpcuaServer.h"
#include "opcua_configuration.h"

#if defined(ENV_MRAA)

#include "PlcIoMraa.h"

#elif defined(ENV_REVPI)
#include "PlcIoRevPi.h"
#elif defined(ENV_WINDOWS)
#warning No I/O implementation in this environment yet
#else
#warning Unknown Environment
#endif

bool terminate = false;

void signalSigterm(int signum) {
    terminate = true;
}

int main(int argc, char *argv[]) {
    try {
        signal(SIGTERM, signalSigterm);

        TMModbusTCPServer modbusTcpServer;
        OpcuaServer opcuaServer;

        // TERANIS Code laden
        PlcTask *task = new TeranisTask();
        task->setMinCycleSetMs(1);
        task->setMaxCycleSetMs(250); // sollte größer als plc.getCooldownCycleTime() sein

        // PLC vorbereiten
        PlcIo *io;
#if defined(ENV_MRAA)
        io = new PlcIoMraa();
#elif defined(ENV_REVPI)
        io = new PlcIoRevPi();
#elif defined(ENV_WINDOWS)
        io = new PlcIo(); //TODO: where to map i/o here?
#else
        io = new PlcIo();
#endif

        Plc plc(io);
        plc.setCooldownCycleTime(50);
        plc.setTask(task);

        // simple Argumentpruefung
        if (argc == 3 && strcmp(argv[1], "-mbport") == 0) modbusTcpServer.setMbport(atoi(argv[2]));

        modbusTcpServer.begin();
        plc.begin();

        configureOpcuaServer(&opcuaServer);
        std::set<OpcuaVariable*> opcuaVariables;
        configureOpcuaVariables(opcuaVariables);
        opcuaServer.addVariables(opcuaVariables);
        opcuaServer.begin();

        // PLC-Zyklus so lange ausfuehren bis SIGTERM eintrifft, oder Exception
        while (!terminate) {
            plc.cycle();
            modbusTcpServer.run();
            opcuaServer.cycle();
        }

        plc.end();
        modbusTcpServer.end();
        opcuaServer.end();

        return 0;
    }
    catch (...) {
        std::cerr << " Es ist ein unbekannter Fehler aufgetreten." << std::endl;
        return 1;
    }
}
