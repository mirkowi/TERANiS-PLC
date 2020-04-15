//
// Created by mirko on 10.09.2019.
//

#include "TCPServer.h"

#ifdef _WIN32

#include <io.h>

#else

#include <sys/io.h>
#include <fcntl.h>
#include <cstring> // fuer memset etc

#endif

TTCPServer::TTCPServer(std::string ipport) : TTCPSocket() {
    Hostport2HostPort(ipport, ip, port);
}

bool TTCPServer::Bind() {
    if (!create()) return false;

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return (bind(descriptor, (struct sockaddr *) &my_addr,
                 sizeof(struct sockaddr_in)) != -1);
}

bool TTCPServer::Bind(std::string ipport) {
    Hostport2HostPort(ipport, ip, port);
    return Bind();
}

bool TTCPServer::Bind(int port) {
    this->port = port;
    return Bind();
}

bool TTCPServer::Bind(int port, std::string ip) {
    this->port = port;
    this->ip = ip;
    return Bind();
}

bool TTCPServer::Listen(int backlog) {
    setNonblocking();
    int res = listen(descriptor, backlog);
    return (res >= 0);
}

bool TTCPServer::Accept(TTCPClient &client) {
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
    struct sockaddr_in peer_addr;
    memset(&peer_addr, 0, sizeof(struct sockaddr_in));
    int cfd = accept(descriptor, (struct sockaddr *) &peer_addr,
                     &peer_addr_size);
    if (cfd < 0) {
        // Bei diesen Ergebnissen ist momentan keine neue Client-Verbindung vorhanden
#ifdef _WIN32
        if (WSAGetLastError() == WSAEWOULDBLOCK) return false;
#else
        if (errno == EAGAIN || errno == EWOULDBLOCK) return false;
#endif
        // Ansonsten koennte hier auch eine Exception stehen
        return false;
    }
    // cfd enthaelt eine neue Client Verbindung
    // TODO: peer_addr im Client ebenfalls uebergeben.
    //  Das ist aber nicht wichtig fuer die Verbindung
    //client.setPort()
    //client.setIp()
    client.setDescriptor(cfd);
    return true;
}
