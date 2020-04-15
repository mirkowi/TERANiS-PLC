//---------------------------------------------------------------------------
#include "platform.h"

#include <string>

#ifdef _WIN32

#include <io.h>

#else

#include <sys/io.h>
#include <fcntl.h>
#include <cstring> // fuer memset etc

#endif

#include "TCPSocket.h"

//---------------------------------------------------------------------------
TTCPSocket::TTCPSocket() {
    ready = false;
    busy = false;
    descriptor = 0;
    port = 0;
}

//---------------------------------------------------------------------------
TTCPSocket::TTCPSocket(std::string hostport) {
    ready = false;
    busy = false;
    descriptor = 0;
    setHostPort(hostport);
}

//---------------------------------------------------------------------------
TTCPSocket::TTCPSocket(int port, std::string host) {
    ready = false;
    busy = false;
    descriptor = 0;
    this->port = port;
    ip = Host2Ip(host);
}

//---------------------------------------------------------------------------
TTCPSocket::~TTCPSocket() {
    disconnect();
}

//---------------------------------------------------------------------------
void TTCPSocket::setHostPort(std::string hostport) {
    port = 0;
    std::string host;
    Hostport2HostPort(hostport, host, port);
    ip = Host2Ip(host);
}

//---------------------------------------------------------------------------
void TTCPSocket::disconnect() {
    ready = false;
    busy = false;
#ifdef _WIN32
    closesocket(descriptor);
#else
    close(descriptor);
#endif
    descriptor = 0;
}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TTCPSocket::connect() {
    if (busy) {
        // warten auf Verbindung
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        fd_set writeset;
        FD_ZERO(&writeset);
        FD_SET(descriptor, &writeset);
        // Select testet, ob der Descriptor zum Schreiben bereit ist
        int res = select(descriptor + 1, NULL, &writeset, NULL, &tv);
        if (res < 0) {
#ifdef _WIN32
            if (WSAGetLastError() != WSAEINPROGRESS)
#else
                if (errno != EINTR && errno != EAGAIN)
#endif
            {
                // Fehler
                busy = false;
                return;
            }
            // sonst weiter warten
        } else if (res > 0) {
            // Socket zum Schreiben ist im Set
            int valopt = 0;
#ifdef _WIN32
            int
#else
            socklen_t
#endif
                    lon = sizeof(int);
            if (getsockopt(descriptor, SOL_SOCKET, SO_ERROR, (char *) (&valopt), &lon) < 0) {
                // Fehler bei getsockopt
                busy = false;
                return;
            }
            // Check the value returned...
            if (valopt) {
                // Fehler im Socket (beim Verbindungsaufbau)
                busy = false;
                return;
            }
            // erfolgreich verbunden und bereit zum Schreiben
            busy = false;
            ready = true;
        }
        // Ansonsten weiter warten
        return;
    }

    if (descriptor == 0) if (!create()) return;

    SOCKADDR_IN addr;
#ifdef _WIN32
    memset(&addr, 0, sizeof(SOCKADDR_IN)); // zuerst alles auf 0 setzten
#else
    std::memset(&addr, 0, sizeof(SOCKADDR_IN)); // zuerst alles auf 0 setzten
#endif
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

#ifdef _WIN32
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
#else
    struct hostent *server = gethostbyname(ip.c_str());
    if (server == NULL) return;
    bcopy((char *) server->h_addr, (char *) &addr.sin_addr.s_addr, server->h_length);
#endif

    setNonblocking();

    if (::connect(descriptor, (SOCKADDR *) &addr, sizeof(addr)) == 0) {
        // erfolgreich verbunden
        busy = false;
        ready = true;
    } else {
#ifdef _WIN32
        if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
            if (errno == EINPROGRESS)
#endif
        {
            // Busy anzeigen bis Verbindung aufgebaut
            busy = true;
            ready = false;
        } else {
            // Fehler
            busy = false;
            ready = false;
        }
    }
}

//---------------------------------------------------------------------------
void TTCPSocket::reconnect() {
    disconnect();
    connect();
}

//---------------------------------------------------------------------------
void TTCPSocket::send(const void *buffer, unsigned int bufferlen) {
    ::send(this->descriptor, (const char *) buffer, bufferlen, 0);
}

//---------------------------------------------------------------------------
int TTCPSocket::recv(void *buffer, unsigned int bufferlen) {
    int result = ::recv(this->descriptor, (char *) buffer, bufferlen, 0);
#ifdef _WIN32
    // Wenn nur keine Daten, dann einfach 0 zurueckliefern
    if (result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) return 0;
#else
    // Wenn nur keine Daten, dann einfach 0 zurueckliefern
    if (result < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return 0;
#endif
    return result;
}

//---------------------------------------------------------------------------
char TTCPSocket::recvChar() {
    char buffer;
    if (recv(&buffer, 1) == 1) return buffer;
    return 0;
}

//---------------------------------------------------------------------------
void TTCPSocket::send(std::string s) {
    // Stringlaenge abschicken
    send(s.c_str(), s.length());
}

//---------------------------------------------------------------------------
std::string TTCPSocket::recv() {
    // alles was sich im Empfangspuffer befindet in einen String auslesen
    std::string result;
    while (true) {
        char buffer[255];
        for (int i = 0; i < 254; i++) buffer[i] = 0;
        int len = recv(buffer, 253);
        result += buffer;
        if (len < 253) break;
    }
    return result;
}

//---------------------------------------------------------------------------
void TTCPSocket::flush() {
    // alles was sich im Empfangspuffer befindet auslesen
    while (true) {
        char buffer[255];
        for (int i = 0; i < 254; i++) buffer[i] = 0;
        int len = recv(buffer, 253);
        if (len < 253) break;
    }
}

//---------------------------------------------------------------------------
int TTCPSocket::recvFixed(char *buffer, unsigned int bufferlen, int wait_time, int wait_count) {
    int rc = 1;
    unsigned int read = 0;
    int i = 0;
    while (read < bufferlen && rc > 0) {
        rc = recv(buffer + read, bufferlen);
        if (rc == -1)throw Exception("socket error");
        else read += rc;
        if (read < bufferlen) {
            if (++i > wait_count)
                throw TMException("socket error : can't receive enough data : ? / ?", read, bufferlen);
#ifdef _WIN32
            Sleep(wait_time);
#else
            usleep(wait_time * 1000);
#endif
        }
    }
    return read;
}

bool TTCPSocket::create() {
    descriptor = socket(AF_INET, SOCK_STREAM, 0);
#ifdef _WIN32
    if (this->descriptor == INVALID_SOCKET) return false;
#else
    if (descriptor < 0) return false;
#endif
    return true;
}

/*
 * Man kann nicht kopieren, da der Socket sonst geschlossen wird
 TTCPSocket::TTCPSocket(const TTCPSocket &other) {
    port = other.port;
    ip = other.ip;
    descriptor = other.descriptor;
    ready = other.ready;
    busy = other.busy;
}
*/

bool TTCPSocket::isConnected() {
    if (!isReady()) return false;
    char buf;
    int flags =
#ifdef _WIN32
            MSG_PEEK
#else
    MSG_PEEK | MSG_DONTWAIT
#endif
    ;
    int result = ::recv(descriptor, &buf, 1, flags);
    if (result >= 0 || errno == EAGAIN) return true;
    return false;
}

bool TTCPSocket::available() {
    if (!isReady()) return false;
    char buf;
    int flags =
#ifdef _WIN32
            MSG_PEEK
#else
    MSG_PEEK | MSG_DONTWAIT
#endif
    ;
    int result = ::recv(descriptor, &buf, 1, flags);
    if (result > 0) return true;
    return false;
}

uint8_t TTCPSocket::read() {
    uint8_t buffer;
    int result = recv(&buffer, 1);
    if (result == 1) return buffer;
    if (result == 0) throw TMException("receive buffer empty");
    throw TMException("receive error", result);
}

void TTCPSocket::setNonblocking() {
    // Socket in nonblocking schalten
#ifdef _WIN32
    unsigned long ctl = 1;
    if (ioctlsocket(descriptor, FIONBIO, &ctl)) return;
#else
    int flags;
    if ((flags = fcntl(descriptor, F_GETFL, 0)) < 0) return;
    if (fcntl(descriptor, F_SETFL, flags | O_NONBLOCK) < 0) return;
#endif

}

void TTCPSocket::setDescriptor(SOCKET descriptor) {
    this->descriptor = descriptor;
    ready = descriptor > 0;
    busy = false;
    setNonblocking();
}
//---------------------------------------------------------------------------
/*bool TTCPSocket::readable()
{
    FD_SET fdSet;
    TIMEVAL timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    long status;

    FD_ZERO(&fdSet);
    FD_SET(this->descriptor,&fdSet);
    status = select(0,&fdSet,0,0,&timeout);
    if(status <= 0) FD_ZERO(&fdSet);
    if(!FD_ISSET(this->descriptor,&fdSet))  return false;
    return true;
}
*/
//---------------------------------------------------------------------------
