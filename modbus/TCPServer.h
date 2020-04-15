//
// Created by mirko on 10.09.2019.
//

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "TCPSocket.h"
#include "TCPClient.h"

/// Erweiterung der TCPSocket-Klasse um Serverfunktionen
class TTCPServer : public TTCPSocket {
public:
    TTCPServer() : TTCPSocket() {};

    TTCPServer(int port) { this->port = port; };

    TTCPServer(int port, std::string ip) {
        this->port = port;
        this->ip = ip;
    };

    TTCPServer(std::string ipport);

    /// Server Socket an Adresse binden
    bool Bind();

    bool Bind(std::string ipport);

    bool Bind(int port);

    bool Bind(int port, std::string ip);

    /// Neue Verbindung
    /// @param backlog maximale Anzahl der Client-Verbindungen in der Warteschlange
    bool Listen(int backlog = 1);

    /// Neue Client Verbindung akzeptieren
    /// @param client Referenz auf einen neuen Client.
    /// @return true wenn neuer Client vorhanden und im Parameter client abgelegt.
    bool Accept(TTCPClient &client);
};


#endif //TCPSERVER_H
