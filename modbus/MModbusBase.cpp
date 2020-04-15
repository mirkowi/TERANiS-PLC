#include "MModbusBase.h"

/***

Der Modbus-Standard hat 4 Adressbereiche.
Dafuer werden die folgenden Funktionscodes verwendet.

0x Binaere Ausgaenge (Coils) (1-bit Wert)
Adressbereich 00001 bis 09999 => z.B. 0x0001 bis 0x9999
    Funktionscodes
    1 Read Coil Status
    5 Write Coil Status

1x Binaere Eingaenge (1-bit Wert)
Adressbereich 10001 bis 19999 => z.B. 1x0001 bis 1x9999
    Funktionscode
    2 Read Input Status

3x Analoge Eingaenge - Input-Register (16-bit Wert)
Adressbereich 30001 bis 39999  => z.B. 3x0001 .. 3x9999
    Funktionscode
    4 Read Input Register

4x Analoge Ausgaenge - Holding-Register (16-bit Wert)
Adressbereich 40001 bis 49999 => z.B. 4x0001 bis 4x9999
    Funktionscodes
    3 Read Holding Register
    6 Write Holding Register

***/

//---------------------------------------------------------------------------
uint16_t TMModbusBase::Endian_swap(uint16_t x) {
    return (x >> 8) | (x << 8);
}

//---------------------------------------------------------------------------
void TMModbusBase::Init_float(float &x, uint8_t *buffer, int &offset) {
    uint8_t temp[4];
    temp[0] = buffer[offset + 1];
    temp[1] = buffer[offset + 0];
    temp[2] = buffer[offset + 3];
    temp[3] = buffer[offset + 2];
    x = *(float *) (&temp);
    offset += 4;
}

//---------------------------------------------------------------------------
void TMModbusBase::Init_int(int &x, uint8_t *buffer, int &offset) {
    uint8_t temp[4];
    temp[0] = buffer[offset + 1];
    temp[1] = buffer[offset + 0];
    temp[2] = buffer[offset + 3];
    temp[3] = buffer[offset + 2];
    x = *(int *) (&temp);
    offset += 4;
}

//---------------------------------------------------------------------------
void TMModbusBase::Init_tm(tm &x, uint8_t *buffer, int &offset) {
    uint8_t hi_byte = two_nibble_to_byte(buffer[offset + 6]);
    uint8_t low_byte = two_nibble_to_byte(buffer[offset + 7]);
    x.tm_year = two_byte_to_short(hi_byte, low_byte) - 1900;
    x.tm_mon = two_nibble_to_byte(buffer[offset + 4]) - 1;
    x.tm_mday = two_nibble_to_byte(buffer[offset + 5]);
    x.tm_hour = two_nibble_to_byte(buffer[offset + 2]);
    x.tm_min = two_nibble_to_byte(buffer[offset + 3]);
    x.tm_sec = two_nibble_to_byte(buffer[offset + 0]);
    offset += 8;
}
//---------------------------------------------------------------------------
MSTRING TMModbusBase::tm_to_string(tm x, MSTRING format) {
    char timestamp[100];
    strftime(timestamp, 100, format.c_str(), &x);
    return MSTRING(timestamp);
}

//---------------------------------------------------------------------------
uint8_t TMModbusBase::two_nibble_to_byte(uint8_t two_nibble) {
    return ((two_nibble >> 4) * 10) + (two_nibble & 15);
}

//---------------------------------------------------------------------------
short TMModbusBase::two_byte_to_short(uint8_t &hi_byte, uint8_t &low_byte) {
    short temp = hi_byte * 100 + low_byte;
    hi_byte = temp;
    low_byte = temp >> 8;
    return temp;
}

//---------------------------------------------------------------------------
void TMModbusBase::Init_unsigned_short(uint16_t &x, uint8_t *buffer, int &offset) {
    uint8_t temp[2];
    temp[0] = buffer[offset + 1];
    temp[1] = buffer[offset + 0];
    x = *(uint16_t *) (&temp);
    offset += 2;
}

