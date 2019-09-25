//
// Created by mirko on 19.09.2019.
//

#ifndef PLCIO_H
#define PLCIO_H

// Standard Schnittstelle zum Uebertragen der IO-Signale in das Prozessabbild
#include <stdint.h>

class TPlcIo {
public:
    /// IO-System Initialisieren
    virtual void begin() {};

    /// Lesen der Eingaenge in das Prozessabbild
    virtual void read() {};

    /// Schreiben der Ausgaenge aus dem Prozessabbild
    virtual void write() {};

    /// IO-System schliessen
    virtual void end() {};
};

#endif // PLCIO_H
