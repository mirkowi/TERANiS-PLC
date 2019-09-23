//
// Created by mirko on 19.09.2019.
//

#ifndef PLCIO_H
#define PLCIO_H

// Standard Schnittstelle zum Uebertragen der IO-Signale in das Prozessabbild
#include <stdint.h>

class TPlcIo {
private:
    void read(uint32_t offset, uint32_t length);
    void write(uint32_t offset, uint32_t length);
public:
    /// IO-System Initialisieren
    void begin();

    /// Lesen der Eingaenge in das Prozessabbild
    void read();

    /// Schreiben der Ausgaegne aus dem Prozessabbild
    void write();

    /// IO-System schliessen
    void end();
};

#endif // PLCIO_H
