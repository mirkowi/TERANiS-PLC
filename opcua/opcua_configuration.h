#include <set>
#include "OpcuaVariable.h"
#include "OpcuaServer.h"

/**
 * Konfigurationsheader fuer OPC/UA-Variablenmappings.
 * Die zugehoerige CPP-Datei muss implementiert werden,
 * um Speicherbereiche der TERANiS-PLC via OPC/UA verfuegbar zu machen
 * @author Tim Trense
 */

/**
 * Callback fuer das Konfigurieren des Servers.
 * Wird vor dessen begin() aufgerufen und erlaubt das Anpassen der Einstellungen.
 * Z.B.: Verbindungsverschluesselung einrichten und erzwingen, Clientauthentifizierung einrichten, ...
 *
 * Wenn diese Funktion als No-Op implementiert wird, so ist der Server dennoch mit Standardwerten lauffaehig
 *
 * @param server einzurichtender Server, nicht nullptr
 */
void configureOpcuaServer(OpcuaServer *server);

/**
 * Callback fuer das Konfigurieren des Variablenmappings.
 * Hier koennen Speicherbereiche oder sonstige Variablen der TERANiS-PLC via OPC/UA verfuegbar gemacht werden
 *
 * Wenn diese Funktion als No-Op implementiert wird, so sind auf dem Server dennoch die
 * vom OPC/UA-Standard geforderten Standardvariablen verfuegbar.
 *
 * @param vars out-parameter der von der Implementierung mit den gewuenschten Variablen gefuellt werden muss
 */
void configureOpcuaVariables(std::set<OpcuaVariable *> &vars);
