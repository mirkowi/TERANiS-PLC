
#ifndef TERANIS_PLC_OPCUASERVER_H
#define TERANIS_PLC_OPCUASERVER_H

#include "open62541.h"
#include "OpcuaVariable.h"
#include <string>
#include <set>

/**
 * Wrapper des Open62541-OPC/UA-Servers in der TERANiS-PLC
 *
 * @authors Tim Trense
 */
class OpcuaServer {
public:
    /**
     * Konstruktor mit folgenden Standardeinstellungen:
     * <ul>
     * <li>Sprache: de-DE
     * <li>UA_ServerConfig->verifyRequestTimestamp: UA_RULEHANDLING_ACCEPT
     * <li>variables: empty set
     * </ul>
     */
    OpcuaServer();

    /**
     * Standard-Destruktor, löscht den gewrappten Server
     */
    ~OpcuaServer();

    /**
     * Setup des Servers
     */
    void begin();

    /**
     * Laufender Zyklus, muss zyklisch innerhalb von #getMaxAllowedCycleTimeout() Millisekunden aufgerufen werden
     */
    void cycle();

    /**
     * Muss nach dem letzten Zyklus zum Bereinigen des gewrappten Servers aufgerufen werden
     */
    void end();

    /**
     * @return gibt an, ob der gewrappte Server != NULL ist
     */
    bool good() const;

    /**
     * Wird das Timeout ueberschritten, so ist die Stabilitaet des Servers nicht mehr gewaehrleistet.
     * Insbesondere erhalten dann Clients eventuell Antworten auf ihre Requests zu spaet
     * @return Maximales Timeout in Millisekunden bis zum naechsten Aufruf von cycle()
     */
    UA_UInt16 getMaxAllowedCycleTimeout() const;

    const std::string &getLanguage() const;

    void setLanguage(const std::string &language);

    /**
     * Registriert die angegebene Variable im gewrappten Server.
     * Wenn der Parameter nullptr ist, so ist dies ein no-op.
     * @param variable die neue Variable, nicht nullptr
     */
    void addVariable(OpcuaVariable *variable);

    /**
     * Registriert alle angegebene Variablen im gewrappten Server.
     * Wenn eine Variable nullptr ist, so ist dies fuer diese ein no-op.
     * @param variables alle neuen Variablen, ohne nullptr
     */
    void addVariables(const std::set<OpcuaVariable *> &variable);

    /**
     * Sucht die registrierte Variable mit dem angegebenen Namen
     * @param name Name der Variable
     * @return ein Pointer auf die gefundene Variable oder nullptr wenn der Name unbekannt ist
     */
    OpcuaVariable *getVariable(const std::string &name) const;

    /**
     * INTERN
     * Liest den Wert der Adresse der angegebenen Variablen aus
     * @param server interner Open62541-Parameter
     * @param sessionId interner Open62541-Parameter
     * @param sessionContext interner Open62541-Parameter
     * @param nodeId interner Open62541-Parameter
     * @param nodeContext interner Open62541-Parameter
     * @param sourceTimeStamp interner Open62541-Parameter
     * @param range interner Open62541-Parameter
     * @param dataValue interner Open62541-Parameter
     * @return Erfolg der Leseoperation
     */
    static UA_StatusCode readPlcValue(UA_Server *server,
                                      const UA_NodeId *sessionId, void *sessionContext,
                                      const UA_NodeId *nodeId, void *nodeContext,
                                      UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                      UA_DataValue *dataValue);

    /**
     * INTERN
     * Schreibt den Wert der Adresse der angegebenen Variablen
     * @param server interner Open62541-Parameter
     * @param sessionId interner Open62541-Parameter
     * @param sessionContext interner Open62541-Parameter
     * @param nodeId interner Open62541-Parameter
     * @param nodeContext interner Open62541-Parameter
     * @param range interner Open62541-Parameter
     * @param value interner Open62541-Parameter
     * @return Erfolg der Schreiboperation
     */
    static UA_StatusCode writePlcValue(UA_Server *server, const UA_NodeId *sessionId,
                                       void *sessionContext, const UA_NodeId *nodeId,
                                       void *nodeContext, const UA_NumericRange *range,
                                       const UA_DataValue *value);

private:
    /**
     * Sprachcode des Servers
     * default: "de-DE"
     */
    std::string language;
    /**
     * Implementierung des Open62541-OPC/UA-Servers
     */
    UA_Server *server;
    /**
     * Pufferspeicher des Ergebnisses des MainLoop-Aufrufs des Open62541-Servers
     */
    UA_UInt16 maxAllowedCycleTimeout;
    /**
     * Die Menge aller registrierten OPC/UA-Variablen
     */
    std::set<OpcuaVariable *> variables;
};

#endif //TERANIS_PLC_OPCUASERVER_H
