//
// Created by mirko on 07.03.2020.
//

#ifndef PLCIOMRAA_H
#define PLCIOMRAA_H

#include "PlcIo.h"

// Libmraa ist ausserdem einzubinden
// mraa ist eine C/C++ library fuer die IO auf Galileo, Edison & anderen Platformen
// z.B. geeignet fuer Siemens IOT2000

class TPlcIoMraa : public TPlcIo {
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


#endif // PLCIOREVPI_H
