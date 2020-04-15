
#ifndef TERANIS_PLC_PLCTASK_H
#define TERANIS_PLC_PLCTASK_H

#include <ctime>
#include <chrono>
#include "PlcMemory.h"

class PlcTask {
public:
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> timepoint;

    PlcTask();

    PlcTask(unsigned int minCycleMs, unsigned int maxCycleMs);

    /// Initialsierung
    void begin();

    /// laufender Zyklus
    void run();

    /// Aufraeumen
    void end();

    unsigned int getMinCycleSetMs() const;

    void setMinCycleSetMs(unsigned int minCycleSetMs);

    unsigned int getMaxCycleSetMs() const;

    void setMaxCycleSetMs(unsigned int maxCycleSetMs);

    unsigned int getCycleActMs() const;

    unsigned int getMinCycleActMs() const;

    unsigned int getMaxCycleActMs() const;

protected:
    /// Tatsächliche Implementierung des Task
    virtual void beginImpl();

    /// Tatsächliche Implementierung des Task
    virtual void runImpl();

    /// Tatsächliche Implementierung des Task
    virtual void endImpl();

private:
    /// minimale Zykluszeit in Millisekunden
    unsigned int minCycleTime;
    /// minimale Zykluszeit in Millisekunden
    unsigned int maxCycleTime;
    /// aktuelle Zykluszeit in Millisekunden
    unsigned int actualCycleTime;
    /// minimale Zykluszeit in Millisekunden
    unsigned int actualMinCycleTime;
    /// maximale Zykluszeit in Millisekunden
    unsigned int actualMaxCycleTime;

    /// gemerkte letzte Zeit
    timepoint lastCycleTimepoint;
};

#endif
