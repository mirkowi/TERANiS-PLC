
#ifndef PLCIOMRAA_H
#define PLCIOMRAA_H

#include "PlcIo.h"
#include <mraa.h>

// Libmraa ist ausserdem einzubinden
// mraa ist eine C/C++ library fuer die IO auf Galileo, Edison & anderen Platformen
// z.B. geeignet fuer Siemens IOT2000
class PlcIoMraa : public PlcIo {
public:

    // Pass-Through-Constructor
    PlcIoMraa(
            unsigned int _inputLength,
            unsigned int _outputLength,
            unsigned int _flagsLength,
            unsigned int _systemFlagsLength
    );

    /// Override
    /// IO-System Initialisieren
    void begin() override;

    /// Override
    /// Lesen der Eingaenge in das Prozessabbild
    void read() override;

    /// Override
    /// Schreiben der Ausgaenge aus dem Prozessabbild
    void write(const) override;

    /// Override
    /// IO-System schliessen
    void end() override;
};


#endif
