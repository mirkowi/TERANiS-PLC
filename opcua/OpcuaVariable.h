
#ifndef TERANIS_PLC_OPCUAVARIABLE_H
#define TERANIS_PLC_OPCUAVARIABLE_H

#include "open62541.h"
#include <string>

/**
 * Hilfsklasse fuer OpcuaServer die alle Informationen buendelt,
 * die fuer das Mappen eines PlcMemory-Speicherbereichs auf eine OPC/UA-Variable relevant sind:
 * <ul>
 * <li> Name der Variable
 * <li> Adresse im PlcMemory
 * <li> OPC/UA-Datentyp (gibt auch die Breite der Adresse an)
 * <li> Beschreibung der Variablen (optional, default = Variablenname)
 * <li> Zugriffslevel der Variablen (optional, default = READ-ONLY)
 * </ul>
 * @authors Tim Trense
 */
class OpcuaVariable {
public:
    /**
     * Konstruktor mit folgenden Standardwerten:
     * <ul>
     * <li>description = name
     * <li>accessLevelMask = UA_ACCESSLEVELMASK_READ
     * </ul>
     * @param name Name der OPC/UA-Variablen (wird Teil der ID)
     * @param address Adresse im PlcMemory, nicht nullptr
     * @param dataType OPC/UA-Datentyp der Variable, gibt auch die Breite der Adresse an, nicht nullptr
     */
    OpcuaVariable(std::string name, void *address, const UA_DataType *dataType);


    /**
     * Konstruktor
     * @param name Name der OPC/UA-Variablen (wird Teil der ID)
     * @param address Adresse im PlcMemory, nicht nullptr
     * @param dataType OPC/UA-Datentyp der Variable, gibt auch die Breite der Adresse an, nicht nullptr
     * @param description Beschreibung der OPC/UA-Variablen
     * @param accessLevelMask OPC/UA-Zugriffslevel auf die Variable, nicht 0
     */
    OpcuaVariable(std::string name, void *address, const UA_DataType *dataType, std::string description,
                  UA_Byte accessLevelMask);

    /**
     * Standarddestruktor
     */
    ~OpcuaVariable();

    const std::string &getName() const;

    void setName(const std::string &name);

    void *getAddress() const;

    void setAddress(void *address);

    const UA_DataType *getDataType() const;

    void setDataType(const UA_DataType *dataType);

    const std::string &getDescription() const;

    void setDescription(const std::string &description);

    UA_Byte getAccessLevelMask() const;

    void setAccessLevelMask(UA_Byte accessLevelMask);

    /**
     * setzt die Maskenbits fuer: Lesen und Schreiben
     * entfernt die Maskenbits fuer: nichts
     */
    void setAccessLevelMaskReadWrite();

    /**
     * setzt die Maskenbits fuer: Lesen
     * entfernt die Maskenbits fuer: nichts
     */
    void setAccessLevelMaskReadOnly();

    /**
     * Prueft das Maskenbit fuer Schreiben
     * @return true das Bit gesetzt ist
     */
    bool isAccessLevelMaskWrite();

    /**
     * Prueft das Maskenbit fuer Lesen
     * @return true das Bit gesetzt ist
     */
    bool isAccessLevelMaskRead();

private:
    std::string name;
    void *address;
    const UA_DataType *dataType;

    // optional
    std::string description;
    UA_Byte accessLevelMask;
};

#endif //TERANIS_PLC_OPCUAVARIABLE_H
