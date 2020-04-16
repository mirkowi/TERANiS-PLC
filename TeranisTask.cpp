
#include "PlcMemory.h"
#include <cstdint>
#include <iostream>
#include "teranis_code.h"
#include "TeranisTask.h"
#include "avicstd/h/avi_c_std.h"

TeranisTask::TeranisTask() : PlcTask() {
}

void TeranisTask::runImpl() {
    Cycle();
}
