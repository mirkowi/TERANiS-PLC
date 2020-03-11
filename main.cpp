// This ist the main function

#include <iostream>
#include <csignal>
#include <cstring>
#include "Plc.h"

bool terminate = false;

//---------------------------------------------------------------------------
void signalSigterm(int signum) {
    // While-Schleife unterbrechen
    terminate = true;
}

//---------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    try {
        // register signal and signal handler
        signal(SIGTERM, signalSigterm);
        TPlc plc;
        // simple Argumentpruefung
        if (argc==3 && strcmp(argv[1],"-mbport")==0) plc.setMbport(atoi(argv[2]));
        // Initialisieren der PLC-Umgebung
        plc.setMinCycleSetMs(50); // 50ms minimale Zykluszeit
        plc.begin();
        // PLC-Zyklus so lange ausfuehren bis SIGTERM eintrifft, oder Exception
        while (!terminate) plc.run();
        plc.end();
        return 1;
    }
    catch (...) {
        std::cerr << " Es ist ein unbekannter Fehler aufgetreten." << std::endl;
    }
    return 0;
}
//---------------------------------------------------------------------------
