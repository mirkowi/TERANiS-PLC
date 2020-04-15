#include "Plc.h"

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

void Plc::begin() {
    // Bei begin() bleibt der Speicher im Zustand in dem er ist

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

    try {
        io->read();

        task->run();

        io->write();

        return true;
    } catch (...) {
        state = ERROR;
        return false;
    }
}

PlcTask *Plc::getTask() const {
    return task;
}

void Plc::setTask(PlcTask *task) {
    this->task = task;
}