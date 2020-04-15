
#ifndef TERANIS_TASK_H
#define TERANIS_TASK_H

#include "PlcTask.h"

class TeranisTask : public PlcTask {
public:
    TeranisTask();

protected:

    /// Tatsächliche Implementierung des Task
    virtual void runImpl();
};

#endif
