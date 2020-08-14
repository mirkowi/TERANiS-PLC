#include "Plc.h"
#include "OpcuaServer.h"

OpcuaServer::OpcuaServer() {
    language = "de-DE"; // opcua-default: en-US
    maxAllowedCycleTimeout = 0;
    server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);
    config->verifyRequestTimestamp = UA_RULEHANDLING_ACCEPT;
}

OpcuaServer::~OpcuaServer() {
    if (server == NULL) {
        return;
    }
    UA_Server_delete(server);
    server = NULL;
}

bool OpcuaServer::good() const {
    return server != NULL;
}

UA_UInt16 OpcuaServer::getMaxAllowedCycleTimeout() const {
    return maxAllowedCycleTimeout;
}

void OpcuaServer::begin() {
    if (server == NULL) {
        return;
    }
    UA_StatusCode retval = UA_Server_run_startup(server);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Server_delete(server);
        server = NULL;
    }
}

void OpcuaServer::end() {
    if (server == NULL) {
        return;
    }
    UA_StatusCode retval = UA_Server_run_shutdown(server);
}

void OpcuaServer::cycle() {
    if (server == NULL) {
        return;
    }
    maxAllowedCycleTimeout = UA_Server_run_iterate(server, false);
}

void OpcuaServer::addVariables(const std::set<OpcuaVariable *> &_variables) {
    for (std::set<OpcuaVariable *>::iterator it = _variables.begin(); it != _variables.end(); it++) {
        addVariable(*it);
    }
}

void OpcuaServer::addVariable(OpcuaVariable *variable) {
    if (variable == NULL) {
        return;
    }
    variables.emplace(variable);

    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.description = UA_LOCALIZEDTEXT_ALLOC(language.c_str(), variable->getDescription().c_str());
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC(language.c_str(), variable->getName().c_str());
    attr.accessLevel = variable->getAccessLevelMask();
    attr.dataType = variable->getDataType()->typeId;
    UA_NodeId nodeId = UA_NODEID_STRING_ALLOC(1, variable->getName().c_str());
    UA_QualifiedName qualifiedName = UA_QUALIFIEDNAME_ALLOC(1, variable->getName().c_str());
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource thisDataSource;
    thisDataSource.read = OpcuaServer::readPlcValue;
    thisDataSource.write = OpcuaServer::writePlcValue;

    UA_Server_addDataSourceVariableNode(server, nodeId, parentNodeId,
                                        parentReferenceNodeId, qualifiedName, variableTypeNodeId,
                                        attr, thisDataSource, this, NULL);

}

OpcuaVariable *OpcuaServer::getVariable(const std::string &name) const {
    for (std::set<OpcuaVariable *>::iterator it = variables.begin(); it != variables.end(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}

UA_StatusCode OpcuaServer::readPlcValue(UA_Server *server, const UA_NodeId *sessionId,
                                        void *sessionContext, const UA_NodeId *nodeId,
                                        void *nodeContext, UA_Boolean sourceTimeStamp,
                                        const UA_NumericRange *range, UA_DataValue *dataValue) {
    std::string name((char *) nodeId->identifier.string.data, 0, nodeId->identifier.string.length);
    OpcuaServer *thiz = (OpcuaServer *) nodeContext;
    OpcuaVariable *var = thiz->getVariable(name);

    if (var != NULL) {
        if (var->getAddress() != NULL) {
            UA_Variant_setScalarCopy(&dataValue->value, var->getAddress(), var->getDataType());
            dataValue->hasValue = true;
        } else {
            dataValue->hasValue = false;
        }
        return UA_STATUSCODE_GOOD;
    } else {
        dataValue->hasValue = false;
        return UA_STATUSCODE_BADNOTFOUND;
    }
}

UA_StatusCode OpcuaServer::writePlcValue(UA_Server *server, const UA_NodeId *sessionId,
                                         void *sessionContext, const UA_NodeId *nodeId,
                                         void *nodeContext, const UA_NumericRange *range,
                                         const UA_DataValue *value) {
    std::string name((char *) nodeId->identifier.string.data, 0, nodeId->identifier.string.length);
    OpcuaServer *thiz = (OpcuaServer *) nodeContext;
    OpcuaVariable *var = thiz->getVariable(name);

    if (var != NULL) {
        if (var->getAddress() != NULL) {
            void *data = value->value.data;
            int length = var->getDataType()->memSize;
            memcpy(var->getAddress(), data, length);
        }
        return UA_STATUSCODE_GOOD;
    } else {
        return UA_STATUSCODE_BADNOTFOUND;
    }
}

const std::string &OpcuaServer::getLanguage() const {
    return language;
}

void OpcuaServer::setLanguage(const std::string &_language) {
    language = _language;
}
