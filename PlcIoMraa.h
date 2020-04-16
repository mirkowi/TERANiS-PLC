
#ifndef PLCIOMRAA_H
#define PLCIOMRAA_H

#include "PlcIo.h"
#include <mraa.h>

// Libmraa ist ausserdem einzubinden
// mraa ist eine C/C++ library fuer die IO auf Galileo, Edison & anderen Platformen
// z.B. geeignet fuer Siemens IOT2000
class PlcIoMraa : public PlcIo {
public:
    /// Override
    /// IO-System Initialisieren
    void begin();

    /// Override
    /// Lesen der Eingaenge in das Prozessabbild
    void read();

    /// Override
    /// Schreiben der Ausgaenge aus dem Prozessabbild
    void write();

    /// Override
    /// IO-System schliessen
    void end();
};


#endif
