//---------------------------------------------------------------------------
#ifndef TCPSocketH
#define TCPSocketH

#include <string>

#include "MSocket.h"
//---------------------------------------------------------------------------

class TTCPSocket : public TMSocket {
protected:
    SOCKET descriptor;
    bool ready;
    bool busy;
    int port;
    std::string ip;

    // Copy Constructor private = disable
    TTCPSocket(const TTCPSocket &other) {};
public:
    SOCKET getDescriptor() const { return descriptor; }
    void setDescriptor(SOCKET descriptor);;

    /// liefert true wenn Verbindung aufgebaut
    bool isReady() const { return ready; };

    bool isBusy() const { return busy; };

    int getPort() const { return port; }

    const std::string &getIp() const { return ip; }

    void setIp(std::string ip) { this->ip = ip; };

    void setPort(int port) { this->port = port; };

    void setHostPort(std::string hostport);

    TTCPSocket();

    TTCPSocket(std::string hostport);

    TTCPSocket(int port, std::string host);

    virtual ~TTCPSocket();

    /// erzeugt den Socket
    bool create();

    void reconnect();

    /// erzeugt den Socket ggf. und verbindet als Client mit einem Server
    /// nonblocking, so lange aufrufen bis isBusy() false wird.
    void connect();

    void disconnect();

    /// Sendet auf einem Puffer die angegebene Anzahl Zeichen/Bytes
    void send(const void *buffer, unsigned int bufferlen);

    void write(const void *buffer, unsigned int bufferlen) { send(buffer, bufferlen); };

    /// liest maximal die Pufferlaenge und liefert die gelesene Anzahl zurueck, nicht blockierend
    int recv(void *buffer, unsigned int bufferlen);

    /// liest ein Zeichen liefert 0 wenn nichts empfangen, nicht blockierend
    char recvChar();

    /// liest ein Byte, Exception wenn nichts empfangen, nicht blockierend
    /// vorher mit available pruefen
    uint8_t read();

    /// Sendet einen String
    void send(std::string s);

    /// liest alles was im Puffer ist, in einen String
    std::string recv();

    /// wirft alles was im Puffer ist weg
    void flush();

    /// liest die angegebene Anzahl Zeichen
    /// dafuer wird Recv wiederholt aufgerufen und wait_time dazwischen gewartet
    /// Der Aufruf kehrt erst zurueck wenn die Anzahl Zeichen gelesen wurde
    int recvFixed(char *buffer, unsigned int bufferlen, int wait_time, int wait_count);

    /// Fuert einen Verbindungstest aus und liefert true wenn ok
    bool isConnected();

    /// liefert true, wenn etwas im Empfangspuffer steht
    bool available();

    /// Socket auf nonblocking setzen, wird intern mehrfach benutzt
    void setNonblocking();
};

#endif
