#include "platform.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include "MModbusTCPServer.h"
// Register kommen von TERANiS
#include "teranis.h"

//#define MODBUSDEBUG     // uncomment to enable Debug output

//
// MODBUS Function Codes
//
#define MODBUS_FC_NONE 0
#define MODBUS_FC_READ_COILS 1
#define MODBUS_FC_READ_DISCRETE_INPUT 2
#define MODBUS_FC_READ_REGISTERS 3              //implemented
#define MODBUS_FC_READ_INPUT_REGISTERS 4        //implemented
#define MODBUS_FC_WRITE_COIL 5
#define MODBUS_FC_WRITE_REGISTER 6              //implemented
#define MODBUS_FC_WRITE_MULTIPLE_COILS 15
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS 16   //implemented
// Extensions
#define MODBUS_FC_READ_OUTPUT_REGISTERS 31      //implemented
#define MODBUS_FC_READ_RFLAGS 32                //implemented
#define MODBUS_FC_READ_SFLAGS 33                //implemented
#define MODBUS_FC_WRITE_OUTPUT_REGISTERS 41
#define MODBUS_FC_WRITE_RFLAGS 42
#define MODBUS_FC_WRITE_SFLAGS 43
//
// MODBUS Error Codes
//
#define MODBUS_EC_NONE 0
#define MODBUS_EC_ILLEGAL_FUNCTION 1
#define MODBUS_EC_ILLEGAL_DATA_ADDRESS 2
#define MODBUS_EC_ILLEGAL_DATA_VALUE 3
#define MODBUS_EC_SLAVE_DEVICE_FAILURE 6
//
// MODBUS offsets
//
#define MODBUS_TCP_TID          0
#define MODBUS_TCP_PID          2
#define MODBUS_TCP_LEN          4
#define MODBUS_TCP_UID          6
#define MODBUS_TCP_FUNC         7
#define MODBUS_TCP_REGISTER_START   8
#define MODBUS_TCP_REGISTER_NUMBER  10

#ifdef ARDUINO
#define MODBUS_CLIENTS_MAX 2
WiFiServer ModbusServer(MODBUS_PORT);
WiFiClient ModbusClient[MODBUS_CLIENTS_MAX];
#else
#define MODBUS_CLIENTS_MAX 20
TTCPServer ModbusServer;
TTCPClient ModbusClient[MODBUS_CLIENTS_MAX];
#endif

void TMModbusTCPServer::begin() {
#ifdef ARDUINO
    ModbusServer.begin();
    {
#else
    if (!ModbusServer.Bind(mbport)) {
        DEBUGPRINT("Fehler bei Bind ");
        DEBUGPRINT(ModbusServer.GetLastError())
        DEBUGPRINTLN(" ...");
    } else if (!ModbusServer.Listen()) {
        DEBUGPRINT("Fehler bei Listen ");
        DEBUGPRINT(ModbusServer.GetLastError())
        DEBUGPRINTLN(" ...");
    } else {
#endif
      DEBUGPRINT("Listening on ");
      DEBUGPRINT(mbport);
      DEBUGPRINTLN(" ...");
    }
}

void TMModbusTCPServer::run() {
#ifdef ARDUINO
    // Cleanup disconnected session
    for (uint8_t i = 0; i < MODBUS_CLIENTS_MAX; i++) {
        if (ModbusClient[i] && !ModbusClient[i].connected()) {
            DEBUGPRINT("Modbus Client disconnected ... terminate session ");
            DEBUGPRINTLN(i + 1);
            ModbusClient[i].stop();
        }
    }
    // Check new Modbus Client connections
    if (ModbusServer.hasClient()) {
        bool ConnectionEstablished = false; // Set to false

        for (uint8_t i = 0; i < MODBUS_CLIENTS_MAX; i++) {
            // find free socket
            if (!ModbusClient[i]) {
                ModbusClient[i] = ModbusServer.available();
                ConnectionEstablished = true;
                DEBUGPRINT("Modbus new Client connected.");
                DEBUGPRINTLN(i + 1);
                break;
            } else {
                // DEBUGPRINT("Session is in use");
            }
        }

        if (ConnectionEstablished == false) {
            DEBUGPRINTLN("No free sessions ... drop connection");
            ModbusServer.available().stop();
        }
    }

    for (uint8_t i = 0; i < MODBUS_CLIENTS_MAX; i++) {
        if (ModbusClient[i] && ModbusClient[i].connected()) {
            if (ModbusClient[i].available()) {
                receive(ModbusClient[i]);
            }
        }
    }
#else
    // Cleanup disconnected session
    for (int i = 0; i < MODBUS_CLIENTS_MAX; i++) {
        if (ModbusClient[i].isReady() && !ModbusClient[i].isConnected()) {
            DEBUGPRINT("Modbus Client disconnected ... terminate session ");
            DEBUGPRINTLN(i + 1);
            ModbusClient[i].disconnect();
        }
    }
    // find free Client
    int clientIndex = -1;
    for (int i = 0; i < MODBUS_CLIENTS_MAX; i++) {
        if (!ModbusClient[i].isReady()) {
            clientIndex = i;
            break;
        }
    }
    // Check new Client connection
    if (clientIndex >= 0) {
        if (ModbusServer.Accept(ModbusClient[clientIndex])) {
            DEBUGPRINT("New Modbus Client connected on session ");
            DEBUGPRINTLN(clientIndex+1);
        }
    }

    // receive from Clients
    for (uint8_t i = 0; i < MODBUS_CLIENTS_MAX; i++) {
        if (ModbusClient[i].isReady()) {
            receive(ModbusClient[i]);
        }
    }
#endif
}

void TMModbusTCPServer::receive(CLIENTTYPE &client) {
    if (!client.available()) return;
    uint8_t byteFN = MODBUS_FC_NONE;
    uint8_t byteEC = MODBUS_EC_NONE;
    int Start;
    int WordDataLength;
    int ByteDataLength;
    int MessageLength;
    uint8_t ByteArray[260];
    int i = 0;
    while (client.available() && i < (sizeof(ByteArray) / sizeof(*ByteArray))) {
        ByteArray[i] = client.read();
        i++;
    }
    client.flush();
    byteFN = ByteArray[MODBUS_TCP_FUNC];
    Start = word(ByteArray[MODBUS_TCP_REGISTER_START], ByteArray[MODBUS_TCP_REGISTER_START + 1]);
    WordDataLength = word(ByteArray[MODBUS_TCP_REGISTER_NUMBER], ByteArray[MODBUS_TCP_REGISTER_NUMBER + 1]);
#ifdef MODBUSDEBUG
    DEBUGPRINTLN("");
    DEBUGPRINT("RX: ");
    for (uint8_t thisByte = 0; thisByte < 20; thisByte++) {
        DEBUGPRINTHEX(ByteArray[thisByte]);
        DEBUGPRINT("-");
    }
    DEBUGPRINTLN("");
    DEBUGPRINT("Funktionscode: 0x");
    DEBUGPRINTHEX(byteFN);
    DEBUGPRINTLN("");
#endif
    // Handle request

    switch (byteFN) {
        case MODBUS_FC_READ_REGISTERS:  // 03 Read Holding Registers
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            if (((Start + WordDataLength) * 2) < PLC_F_SIZE) {
                for (int i = 0; i < WordDataLength; i++) {
                    // Register von Flags umspeichern, Bytes dabei drehen
                    ByteArray[9 + i * 2] = Flags[(Start + i) * 2 + 1];
                    ByteArray[10 + i * 2] = Flags[(Start + i) * 2];
                }
                MessageLength = ByteDataLength + 9;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINTHEX(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINTLN("");
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;

        case MODBUS_FC_READ_INPUT_REGISTERS:  // 04 Read Input Registers
            Start = word(ByteArray[MODBUS_TCP_REGISTER_START], ByteArray[MODBUS_TCP_REGISTER_START + 1]);
            WordDataLength = word(ByteArray[MODBUS_TCP_REGISTER_NUMBER], ByteArray[MODBUS_TCP_REGISTER_NUMBER + 1]);
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            if (((Start + WordDataLength)) * 2 < PLC_I_SIZE) {
                for (int i = 0; i < WordDataLength; i++) {
                    // Register vom Input umspeichern, Bytes dabei drehen
                    ByteArray[9 + i * 2] = Inputs[(Start + i) * 2 + 1];
                    ByteArray[10 + i * 2] = Inputs[(Start + i) * 2];
                }
                MessageLength = ByteDataLength + 9;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINTHEX(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINTLN("");
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;

        case MODBUS_FC_WRITE_REGISTER:  // 06 Write Holding Register
            if ((Start * 2 + 1) < PLC_F_SIZE) {
                // Register in Flags umspeichern, Bytes dabei drehen
                Flags[Start * 2] = ByteArray[MODBUS_TCP_REGISTER_NUMBER + 1];
                Flags[Start * 2 + 1] = ByteArray[MODBUS_TCP_REGISTER_NUMBER];
                ByteArray[5] = 6; //Number of bytes after this one.
                MessageLength = 12;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINT(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINTLN("");
                DEBUGPRINT("Write Holding Register: ");
                DEBUGPRINT(Start);
                DEBUGPRINT("=");
                DEBUGPRINTHEX((Start * 2 + 1 < PLC_F_SIZE) ? word(Flags[Start * 2],Flags[Start * 2 + 1]) : -1);
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;

        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:    //16 Write Holding Registers
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            if (((Start + WordDataLength) * 2) < PLC_F_SIZE) {
                for (int i = 0; i < WordDataLength; i++) {
                    // Register in Flags umspeichern, Bytes dabei drehen
                    Flags[Start * 2] = ByteArray[14 + i * 2];
                    Flags[Start * 2 + 1] = ByteArray[13 + i * 2];
                }
                MessageLength = 12;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINTHEX(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINT("");
                DEBUGPRINT("Write Holding Registers from: ");
                DEBUGPRINT(Start);
                DEBUGPRINT("=");
                DEBUGPRINTLN(WordDataLength);
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;
        case MODBUS_FC_READ_OUTPUT_REGISTERS:
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            if (((Start + WordDataLength) * 2) < PLC_Q_SIZE) {
                for (int i = 0; i < WordDataLength; i++) {
                    // Register von Flags umspeichern, Bytes dabei drehen
                    ByteArray[9 + i * 2] = Outputs[(Start + i) * 2 + 1];
                    ByteArray[10 + i * 2] = Outputs[(Start + i) * 2];
                }
                MessageLength = ByteDataLength + 9;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINTHEX(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINTLN("");
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;

        case MODBUS_FC_READ_RFLAGS:
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            if (((Start + WordDataLength) * 2) < PLC_R_SIZE) {
                for (int i = 0; i < WordDataLength; i++) {
                    // Register von Flags umspeichern, Bytes dabei drehen
                    ByteArray[9 + i * 2] = RFlags[(Start + i) * 2 + 1];
                    ByteArray[10 + i * 2] = RFlags[(Start + i) * 2];
                }
                MessageLength = ByteDataLength + 9;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINTHEX(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINTLN("");
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;

        case MODBUS_FC_READ_SFLAGS:
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            if (((Start + WordDataLength) * 2) < PLC_S_SIZE) {
                for (int i = 0; i < WordDataLength; i++) {
                    // Register von Flags umspeichern, Bytes dabei drehen
                    ByteArray[9 + i * 2] = SFlags[(Start + i) * 2 + 1];
                    ByteArray[10 + i * 2] = SFlags[(Start + i) * 2];
                }
                MessageLength = ByteDataLength + 9;
                client.write((const uint8_t *) ByteArray, MessageLength);
                #ifdef MODBUSDEBUG
                DEBUGPRINT("TX: ");
                for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    DEBUGPRINTHEX(ByteArray[thisByte]);
                    DEBUGPRINT("-");
                }
                DEBUGPRINTLN("");
                #endif
            } else {
                // Fehlercode zurückmelden
                byteEC = MODBUS_EC_ILLEGAL_DATA_ADDRESS;
            }
            break;
        default:
            // ungültiger Funktionscode
            byteEC = MODBUS_EC_ILLEGAL_FUNCTION;
    }
    if (byteEC != MODBUS_EC_NONE) {
#ifdef MODBUSDEBUG
        DEBUGPRINT("Modbus Fehler: ");
        DEBUGPRINTHEX(byteEC);
        DEBUGPRINT(" von ");
        DEBUGPRINT(Start);
        DEBUGPRINT(" bis ");
        DEBUGPRINTLN(WordDataLength);
#endif
        ByteArray[MODBUS_TCP_FUNC] |= 0x80; // Im Code Bit 7 setzen
        ByteArray[MODBUS_TCP_REGISTER_START] |= byteEC; // Fehlercode speichern
        ByteArray[5] = 4; // Datenbytes nach diesem Byte
        MessageLength = MODBUS_TCP_REGISTER_START + 1;
        client.write((const uint8_t *) ByteArray, MessageLength);
        #ifdef MODBUSDEBUG
        DEBUGPRINT("TX: ");
        for (uint8_t thisByte = 0; thisByte <= MessageLength; thisByte++) {
            DEBUGPRINTHEX(ByteArray[thisByte]);
            DEBUGPRINT("-");
        }
        DEBUGPRINTLN("");
        #endif
    }
}

void TMModbusTCPServer::end() {
#ifdef ARDUINO
    // TODO: Check Client Disconnect
    // ModbusServer.end();
#else
    for (int i = 0; i < MODBUS_CLIENTS_MAX; i++) {
      ModbusClient[i].disconnect();
    }
    ModbusServer.disconnect();
#endif
}
