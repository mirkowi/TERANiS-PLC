//
// Created by mirko on 15.09.2019.
//

#ifndef PLCTASK_H
#define PLCTASK_H

#include <sys/time.h>

class TPlcTask {
private:
    /// minimale Zykluszeit in Millisekunden
    unsigned minCycleSetMs;

    /// minimale Zykluszeit in Millisekunden
    unsigned maxCycleSetMs;

    /// gemerkte letzte Zeit
    timeval lastTime;
public:
    unsigned int getMinCycleSetMs() const { return minCycleSetMs; }

    void setMinCycleSetMs(unsigned int minCycleSetMs) { this->minCycleSetMs = minCycleSetMs; }

    unsigned int getMaxCycleSetMs() const { return maxCycleSetMs; }

    void setMaxCycleSetMs(unsigned int maxCycleSetMs) { this->maxCycleSetMs = maxCycleSetMs; }

    unsigned int getCycleActMs() const { return cycleActMs; }

    unsigned int getMinCycleActMs() const { return minCycleActMs; }

    unsigned int getMaxCycleActMs() const { return maxCycleActMs; }

private:
    /// aktuelle Zykluszeit in Millisekunden
    unsigned cycleActMs;
    /// minimale Zykluszeit in Millisekunden
    unsigned minCycleActMs;
    /// maximale Zykluszeit in Millisekunden
    unsigned maxCycleActMs;
public:
    TPlcTask() : minCycleSetMs(1), maxCycleSetMs(300), cycleActMs(0), minCycleActMs(0),
                 maxCycleActMs(0) { lastTime.tv_sec = 0; };

    TPlcTask(unsigned minCycleMs, unsigned maxCycleMs) : minCycleSetMs(minCycleMs), maxCycleSetMs(maxCycleMs),
                                                         cycleActMs(0), minCycleActMs(0),
                                                         maxCycleActMs(0) { lastTime.tv_sec = 0; };

    /// Initialsierung
    void begin();

    /// laufender Zyklus
    void run();

    /// Aufraeumen
    void end() {};
    
    ~TPlcTask() { end(); }
};


#endif //PLCTASK_H
