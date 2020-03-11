#ifndef MModbusTCPServer_h
#define MModbusTCPServer_h

#ifdef ARDUINO
#include "Arduino.h"
#include <ESP8266WiFi.h>
#define CLIENTTYPE WiFiClient
#else

#include "TCPServer.h"
#include "TCPClient.h"

#define CLIENTTYPE TTCPClient
#endif

class TMModbusTCPServer {
private:
    int mbport;

    uint16_t word(uint8_t h, uint8_t l) { return ((uint16_t) h) << 8 | l; }

    void receive(CLIENTTYPE &client);

public:
    int getMbport() const { return mbport; };

    void setMbport(int mbport) { TMModbusTCPServer::mbport = mbport; };

    TMModbusTCPServer() : mbport(502) {};

    void begin();

    void run();

    void end();
    
    ~TMModbusTCPServer() { end(); };
};

#endif
