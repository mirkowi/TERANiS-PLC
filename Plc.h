#ifndef PLC_H
#define PLC_H

#include "PlcIo.h"
#include "PlcTask.h"
#include "PlcMemory.h"

/**
 * Implementierung einer Softwarebasierten SPS.
 * Der Speicher der SPS ist programmglobal, weil der gesamte Prozess EINE SPS beschreibt.
 * Diese Klasse dient daher nur der Übersichtlichkeit
 *
 * @authors Mirko Wittek, Tim Trense
 */
class Plc {
public:
    /**
     * SPS-Zustand
     */
    enum State {
        RUN, STOP, ERROR
    };

    Plc(PlcIo *_io);

    ~Plc();

    State getState() const;

    PlcIo *getIo() const;

    PlcTask *getTask() const;

    void setTask(PlcTask *task);

    unsigned int getCooldownCycleTime() const;

    void setCooldownCycleTime(unsigned int _cooldownCycleTime);

    /**
     * Setup der PLC-Umgebung
     */
    void begin();

    /**
     * Laufender Zyklus der PLC-Umgebung
     */
    bool cycle();

    /**
     * Aufräumen vor dem Ende PLC-Umgebung
     */
    void end();

private:
    /**
     * Intervall in Millisekunden das nach der Ausführung von cycle() eingehalten wird, um die CPU-Last zu begrenzen
     */
    unsigned int cooldownCycleTime;
    /**
     * Zeitpunkt des letzten Zyklus
     */
    plc_timepoint lastCycle;
    /**
     * Zeitpunkt des Aufrufs von begin()
     */
    plc_timepoint startTime;
    /**
     * SPS-Zustand
     */
    State state;
    /**
     * I/O Logik der SPS
     */
    PlcIo *io;

    PlcTask *task; //TODO: make a list that runs async to one another
};


#endif //PLC_H
