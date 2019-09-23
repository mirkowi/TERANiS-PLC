//
// Created by mirko on 15.09.2019.
//

#ifndef PLCTASK_H
#define PLCTASK_H

#include <time.h>

class TPlcTask {
private:
    /// minimale Zykluszeit in Millisekunden
    unsigned minCycleSetMs;
    /// minimale Zykluszeit in Millisekunden
    unsigned maxCycleSetMs;
    /// gemerkte letzte Zeit
    clock_t lastclock;
public:
    unsigned int getMinCycleSetMs() const {
        return minCycleSetMs;
    }

    unsigned int getMaxCycleSetMs() const {
        return maxCycleSetMs;
    }

    unsigned int getCycleActMs() const {
        return cycleActMs;
    }

    unsigned int getMinCycleActMs() const {
        return minCycleActMs;
    }

    unsigned int getMaxCycleActMs() const {
        return maxCycleActMs;
    }

private:
    /// aktuelle Zykluszeit in Millisekunden
    unsigned cycleActMs;
    /// minimale Zykluszeit in Millisekunden
    unsigned minCycleActMs;
    /// maximale Zykluszeit in Millisekunden
    unsigned maxCycleActMs;
public:
    TPlcTask() : minCycleSetMs(1), maxCycleSetMs(300), lastclock(0), cycleActMs(0), minCycleActMs(0),
                 maxCycleActMs(0) {};

    TPlcTask(unsigned minCycleMs, unsigned maxCycleMs) : minCycleSetMs(minCycleMs), maxCycleSetMs(maxCycleMs),
                                                         lastclock(0), cycleActMs(0), minCycleActMs(0),
                                                         maxCycleActMs(0) {};

    /// Initialsierung
    void begin();

    /// laufender Zyklus
    void run();
};


#endif //PLCTASK_H
