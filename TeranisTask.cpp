
#include "PlcMemory.h"

#include "teranis.h"
;

#include <cstdint>
#include "teranis_code.h"


#include "TeranisTask.h"

TeranisTask::TeranisTask() : PlcTask() {
}

void TeranisTask::runImpl() {
    Cycle();
}
