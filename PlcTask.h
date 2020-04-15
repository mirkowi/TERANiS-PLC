
#ifndef TERANIS_PLC_PLCTASK_H
#define TERANIS_PLC_PLCTASK_H

#include "PlcMemory.h"

/**
 * Basisklasse für SPS-Tasks. Durch diese Basisklasse wird das Timing der Ausführung des Tasks implementiert
 *
 * @authors Mirko Wittek, Tim Trense
 */
 //TODO: aktuell kann die SPS nur einen einzigen Task ausführen,
 // aber es sollten auch mehrere Tasks (vllt. sogar in jeweils eigenen Threads) ausgeführt werden können
class PlcTask {
public:
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
    plc_timepoint lastCycleTimepoint;
};

#endif
