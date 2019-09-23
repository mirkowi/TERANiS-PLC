//
// Created by mirko on 15.09.2019.
//

#include "PlcTask.h"
#include "teranis.h"

void TPlcTask::begin() {

}

void TPlcTask::run() {
    // minimale Zykluszeit eingestellt
    clock_t minclockdelta = CLOCKS_PER_SEC / 1000 * minCycleSetMs;
    clock_t maxclockdelta = CLOCKS_PER_SEC / 1000 * maxCycleSetMs;
    clock_t now = clock();
    if (lastclock == 0 || (now - lastclock) >= minclockdelta) {
        if (lastclock!=0) {
            // Zykluszeit messen und min/max setzen
            cycleActMs = (now - lastclock) * 1000 / CLOCKS_PER_SEC;
            if (minCycleActMs==0 || minCycleActMs>cycleActMs) minCycleActMs=cycleActMs;
            if (maxCycleActMs<cycleActMs) maxCycleActMs=cycleActMs;
        }
        lastclock = now;
        loop_teranis();
    }
}
