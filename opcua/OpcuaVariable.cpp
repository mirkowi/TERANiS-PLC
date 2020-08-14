#include "OpcuaVariable.h"

#include <utility>

OpcuaVariable::OpcuaVariable(
        std::string name,
        void *address,
        const UA_DataType *dataType)
        : OpcuaVariable(
        name,
        address,
        dataType,
        name,
        UA_ACCESSLEVELMASK_READ
) {
}

OpcuaVariable::OpcuaVariable(
        std::string _name,
        void *_address,
        const UA_DataType *_dataType,
        std::string _description,
        UA_Byte _accessLevelMask)
        : name(std::move(_name)),
          address(_address),
          dataType(_dataType),
          description(std::move(_description)),
          accessLevelMask(_accessLevelMask) {
}

OpcuaVariable::~OpcuaVariable() {

}

const std::string &OpcuaVariable::getName() const {
    return name;
}

void OpcuaVariable::setName(const std::string &_name) {
    name = _name;
}

void *OpcuaVariable::getAddress() const {
    return address;
}

void OpcuaVariable::setAddress(void *_address) {
    address = _address;
}

const UA_DataType *OpcuaVariable::getDataType() const {
    return dataType;
}

void OpcuaVariable::setDataType(const UA_DataType *_dataType) {
    dataType = _dataType;
}

const std::string &OpcuaVariable::getDescription() const {
    return description;
}

void OpcuaVariable::setDescription(const std::string &_description) {
    description = _description;
}

UA_Byte OpcuaVariable::getAccessLevelMask() const {
    return accessLevelMask;
}

void OpcuaVariable::setAccessLevelMask(UA_Byte _accessLevelMask) {
    accessLevelMask = _accessLevelMask;
}

void OpcuaVariable::setAccessLevelMaskReadWrite() {
    accessLevelMask |= UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
}

void OpcuaVariable::setAccessLevelMaskReadOnly() {
    accessLevelMask &= ~(UA_ACCESSLEVELMASK_WRITE);
}

bool OpcuaVariable::isAccessLevelMaskWrite() {
    return (accessLevelMask & UA_ACCESSLEVELMASK_WRITE) != 0;
}

bool OpcuaVariable::isAccessLevelMaskRead() {
    return (accessLevelMask & UA_ACCESSLEVELMASK_READ) != 0;
}
