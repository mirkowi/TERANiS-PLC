//
// Created by mirko on 14.09.2019.
//

#ifndef PLC_H
#define PLC_H

#include "PlcTask.h"
#include <time.h>

class TPlc {
private:
    TPlcTask plcTask;
    timeval lastTime;
    long int usTicks;
    /// Modbus-Port
    int mbport;
public:
    int getMbport() const {
        return mbport;
    }

    void setMbport(int mbport) {
        TPlc::mbport = mbport;
    }

public:
    TPlc() : mbport(502) { lastTime.tv_sec=0; };

    /// Setup der PLC-Umgebung
    void begin();

    /// laufender Zyklus der PLC-Umgebung
    void run();
};


#endif //PLC_H
