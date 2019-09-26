//
// Created by mirko on 15.09.2019.
//

#include "PlcTask.h"
#include "teranis.h"
#include <sys/time.h>
#include <iostream>

void TPlcTask::begin() {

}

void TPlcTask::run() {
    timeval now;
    gettimeofday(&now,0);
    // Aus dem Zeitvergleich Differenz in Mikrosekunden ausrechnen
    long usDiff = (now.tv_sec - lastTime.tv_sec)*1000000 + (now.tv_usec - lastTime.tv_usec);
    if (lastTime.tv_sec == 0 || usDiff/1000 >= minCycleSetMs) {
        if (lastTime.tv_sec!=0) {
            // Zykluszeit messen und min/max setzen
            cycleActMs = usDiff/1000;
            if (minCycleActMs==0 || minCycleActMs>cycleActMs) minCycleActMs=cycleActMs;
            if (maxCycleActMs<cycleActMs) maxCycleActMs=cycleActMs;
            if (cycleActMs>maxCycleSetMs) {
                std::cerr << "Warnung: PlcTask Zykluszeitueberschreitung " << cycleActMs << "ms" << std::endl;
            }
        }
        lastTime = now;
        loop_teranis();
    }
}
