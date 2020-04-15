//---------------------------------------------------------------------------

#ifndef MSocketH
#define MSocketH

#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>

typedef int socklen_t;
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SOCKET int
#define SOCKADDR_IN sockaddr_in
#define SOCKADDR sockaddr
#endif

#include <string>
//---------------------------------------------------------------------------

class TMSocket {
    /// Diese Klasse verwaltet die statische Initialisierung der WinSock-Bibliothek
private:
    static int RefCount;

    static void Start();

    static void Stop();

public:
    TMSocket();

    static bool IsStarted() { return RefCount > 0; };

    virtual ~TMSocket();

    /// Hilfsfunktion zur Trennung Host:Port
    static void Hostport2HostPort(std::string hostport, std::string &host, int &port);

    /// Hilfsfunktion zur Namensaufloesung eines Hostnamens
    static std::string Host2Ip(std::string host);

    /// Fehlermeldung holen
    static std::string GetLastError();
};


#endif
