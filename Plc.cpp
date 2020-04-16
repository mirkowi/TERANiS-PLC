#include "Plc.h"
#include <thread>

Plc::Plc(PlcIo *io) {
    state = STOP;
    this->io = io;

    // Beim erstmaligen Start der SPS wird der Speicher mit 0 initialisiert
    for (int i = 0; i < PLC_I_SIZE; i++) {
        Inputs[i] = 0;
    }

    for (int i = 0; i < PLC_Q_SIZE; i++) {
        Outputs[i] = 0;
    }

    for (int i = 0; i < PLC_F_SIZE; i++) {
        Flags[i] = 0;
    }

    for (int i = 0; i < PLC_R_SIZE; i++) {
        RFlags[i] = 0;// TODO: Remanente Flags aus Datei lesen
    }

    for (int i = 0; i < PLC_S_SIZE; i++) {
        SFlags[i] = 0;
    }
}

Plc::~Plc() {
    end();
}

Plc::State Plc::getState() const {
    return state;
}

PlcIo *Plc::getIo() const {
    return io;
}

unsigned int Plc::getCooldownCycleTime() const {
    return cooldownCycleTime;
}

void Plc::setCooldownCycleTime(unsigned int _cooldownCycleTime) {
    cooldownCycleTime = _cooldownCycleTime;
}

void Plc::begin() {
    // Bei begin() bleibt der Speicher im Zustand in dem er ist
    lastCycle = std::chrono::system_clock::now();
    startTime = std::chrono::system_clock::now();
    S_usTicks = 0;
    io->begin();
    task->begin();

    state = RUN;
}

void Plc::end() {
    state = STOP;
    task->end();
    io->end();
}

bool Plc::cycle() {
    if (!io) {
        state = ERROR;
        return false;
    }
    if (state != RUN) {
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(cooldownCycleTime));

    plc_timepoint now = std::chrono::system_clock::now();

    S_usTicks = ((now - startTime).count() * std::micro::den) / plc_timepoint::duration::period::den;
    S_cycleActMs = task->getCycleActMs();
    S_minCycleActMs = task->getMinCycleActMs();
    S_maxCycleActMs = task->getMaxCycleActMs();

    try {
        io->read();

        task->run();

        io->write();

        return true;
    } catch (...) {
        state = ERROR;
        return false;
    }

    lastCycle = now;
}

PlcTask *Plc::getTask() const {
    return task;
}

void Plc::setTask(PlcTask *task) {
    this->task = task;
}