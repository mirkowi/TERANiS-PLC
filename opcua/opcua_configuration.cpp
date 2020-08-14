#include "opcua_configuration.h"
#include "PlcMemory.h"

void configureOpcuaServer(OpcuaServer *server) {
    // Standardimplementierung: No-Op
}

void configureOpcuaVariables(std::set<OpcuaVariable *> &vars) {
    // Standardimplementierung von teranis_code.h aufgreifen
    // und dortigen Debug-Zykluszaehler via OPC/UA verfuegbar machen

    vars.emplace(new OpcuaVariable(
            "DebugPlcCycleCount",
            &F(uint32_t, 30),
            &UA_TYPES[UA_TYPES_UINT32],
            "Dies ist eine DEBUG-Variable und gegen die Standardimplementierung von teranis_code.h angelegt",
            UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE
    ));
}