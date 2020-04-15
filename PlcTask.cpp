#include "PlcTask.h"
#include <chrono>
#include <iostream>

PlcTask::PlcTask() : minCycleTime(1), maxCycleTime(300), actualCycleTime(0), actualMinCycleTime(0),
                     actualMaxCycleTime(0) {
}

PlcTask::PlcTask(unsigned int minCycleMs, unsigned int maxCycleMs) : minCycleTime(minCycleMs),
                                                                     maxCycleTime(maxCycleMs),
                                                                     actualCycleTime(0), actualMinCycleTime(0),
                                                                     actualMaxCycleTime(0) {
};

unsigned int PlcTask::getMinCycleSetMs() const {
    return minCycleTime;
}

void PlcTask::setMinCycleSetMs(unsigned int minCycleSetMs) {
    this->minCycleTime = minCycleSetMs;
    if (this->maxCycleTime < this->minCycleTime) {
        this->maxCycleTime = this->minCycleTime + 1;
    }
}

unsigned int PlcTask::getMaxCycleSetMs() const {
    return maxCycleTime;
}

void PlcTask::setMaxCycleSetMs(unsigned int maxCycleSetMs) {
    this->maxCycleTime = maxCycleSetMs;
}

unsigned int PlcTask::getCycleActMs() const {
    return actualCycleTime;
}

unsigned int PlcTask::getMinCycleActMs() const {
    return actualMinCycleTime;
}

unsigned int PlcTask::getMaxCycleActMs() const {
    return actualMaxCycleTime;
}

void PlcTask::begin() {
    lastCycleTimepoint = std::chrono::system_clock::now();
    beginImpl();
}

void PlcTask::run() {
    plc_timepoint now = std::chrono::system_clock::now();

    actualCycleTime = (now - lastCycleTimepoint).count() * 1000 / std::nano::den;

    if (actualCycleTime >= minCycleTime) {
        // so schnell wie möglich setzen, damit Berechnungszeit des Cycle nicht die Aufrufzeit beeinflusst
        lastCycleTimepoint = now;

        if (actualMinCycleTime == 0 || actualMinCycleTime > actualCycleTime) actualMinCycleTime = actualCycleTime;
        if (actualMaxCycleTime < actualCycleTime) actualMaxCycleTime = actualCycleTime;
        if (actualCycleTime > maxCycleTime) {
            std::cerr << "Warnung: PlcTask Zykluszeitueberschreitung " << actualCycleTime << "ms" << std::endl;
        }

        runImpl();
    }
}

void PlcTask::end() {
    endImpl();
}

void PlcTask::runImpl() {
    // may be overridden by subclass
}

void PlcTask::beginImpl() {}

void PlcTask::endImpl() {}