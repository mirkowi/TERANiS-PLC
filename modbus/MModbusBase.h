#ifndef MModbusBase_H
#define MModbusBase_H

#include <time.h>
#include "platform.h"
#include <stdint.h>

class TMModbusBase
{
  /// Diese Klasse ist die Basis fuer alle Modbus Server und Clients
  /// Sie definiert Basismethoden zum Umgang mit den Modbus-Daten
	public:
    static uint16_t Endian_swap(uint16_t x);
    static void Init_float(float &x, uint8_t* buffer, int &offset);
    static void Init_int(int &x, uint8_t* buffer, int &offset);
    static void Init_unsigned_short(uint16_t &x, uint8_t* buffer, int &offset);
    static void Init_tm(tm &x, uint8_t* buffer, int &offset);
    static std::string tm_to_string(tm x, std::string format);
    static uint8_t two_nibble_to_byte(uint8_t two_nibble);
    static short two_byte_to_short(uint8_t &hi_byte, uint8_t &low_byte);
};

#endif
