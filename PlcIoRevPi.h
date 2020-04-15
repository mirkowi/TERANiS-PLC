//
// Created by mirko on 25.09.2019.
//

#ifndef PLCIOREVPI_H
#define PLCIOREVPI_H

#include "PlcIo.h"
#include <cstdint>

class TPlcIoRevPi : public TPlcIo {
private:
    void read(uint32_t offset, uint32_t length);
    void write(uint32_t offset, uint32_t length);
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
