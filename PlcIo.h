#ifndef PLCIO_H
#define PLCIO_H

#include "PlcMemory.h"

/**
 * Standard Schnittstelle zum Uebertragen der IO-Signale in das Prozessabbild
 *
 * @authors Mirko Wittek, Tim Trense
 */
class PlcIo {
public:
    /// IO-System Initialisieren
    virtual void begin();

    /// Lesen der Eingaenge in das Prozessabbild
    virtual void read();

    /// Schreiben der Ausgaenge aus dem Prozessabbild
    virtual void write();

    /// IO-System schliessen
    virtual void end();

};

#endif
