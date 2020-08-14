
#ifndef TERANIS_TASK_H
#define TERANIS_TASK_H

#include "PlcTask.h"

/**
 * Der SPS-Task "TERANiS" führt den code aus teranis_code.h aus, der aus TERANiS exportiert wurde.
 */
class TeranisTask : public PlcTask {
public:
    TeranisTask();

protected:

    /// Tatsächliche Implementierung des Task
    void runImpl() override;

};

#endif
