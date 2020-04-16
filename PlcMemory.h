
#ifndef TERANIS_PLC_PLCMEMORY_H
#define TERANIS_PLC_PLCMEMORY_H

/**
 * Eine SPS wird durch den gesamten Prozess beschrieben, daher ist ihr Speicher auch global
 */

// Define the size in Bytes for different PLC memory types
#define PLC_I_SIZE 256 // Input Bytes
#define PLC_Q_SIZE 256 // Output Bytes
#define PLC_F_SIZE 256 // Flags Bytes
#define PLC_R_SIZE 256 // Remanent Flags Bytes
#define PLC_S_SIZE 256 // System Flags Bytes

extern unsigned char Inputs[PLC_I_SIZE];
extern unsigned char Outputs[PLC_Q_SIZE];
extern unsigned char Flags[PLC_F_SIZE];
extern unsigned char RFlags[PLC_R_SIZE];
extern unsigned char SFlags[PLC_S_SIZE];

typedef struct {
    unsigned char x0 : 1;
    unsigned char x1 : 1;
    unsigned char x2 : 1;
    unsigned char x3 : 1;
    unsigned char x4 : 1;
    unsigned char x5 : 1;
    unsigned char x6 : 1;
    unsigned char x7 : 1;
} T8Bits;

#include <chrono>
typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> plc_timepoint;

// Macro zum casten eines Typs in ein Byte-Array
#ifdef __cplusplus

// Templatefunktion zum Ermitteln eines Zeigers mit definiertem Typ in ein Variablenfeld
// Dabei wird auf Überschreitung des Index getestet und zur Übersetzungszeit Fehler gemeldet
// Leider ist es nicht möglich auszugeben, welches Array gemeint ist.
template<typename T, const unsigned S, const unsigned I>
T *TeArrayElement(unsigned char *A) {
    static_assert(S >= I, "index exceeds size of array");
    static_assert(S >= (I + sizeof(T)), "index and size of type exceeds size of array");
    return ((T *) (&(A[I])));
};

// Macrodefinition
#define TREF(typ, arr, idx) (*TeArrayElement<typ,sizeof(arr),idx>(arr))
#else
// Die Standard-C Variante ohne Indexüberprüfung
#define TREF(typ,arr,idx) (*((typ*)(&(arr[idx]))))
#endif

// Macro zum Zugriff auf ein Bit in einem Byte-Array
#define XREF(arr, idx, bitnr) TREF(T8Bits,arr,idx).x##bitnr

#define I(typ, idx) TREF(typ,Inputs,idx)
#define IX(idx, bitnr) XREF(Inputs,idx,bitnr)


#define Q(typ, idx) TREF(typ,Outputs,idx)
#define QX(idx, bitnr) XREF(Outputs,idx,bitnr)

#define F(typ, idx) TREF(typ,Flags,idx)
#define FX(idx, bitnr) XREF(Flags,idx,bitnr)

#define R(typ, idx) TREF(typ,RFlags,idx)
#define RX(idx, bitnr) XREF(RFlags,idx,bitnr)

#define S(typ, idx) TREF(typ,SFlags,idx)
#define SX(idx, bitnr) XREF(SFlags,idx,bitnr)

// Systemvariablen
// Mikrosekunden Ticks zum Zyklusbeginn
#define S_usTicks S(int64_t, 10)
// aktuelle Zykluszeit
#define S_cycleActMs S(uint32_t, 18)
// minimale Zykluszeit
#define S_minCycleActMs S(uint32_t, 22)
// maximale Zykluszeit
#define S_maxCycleActMs S(uint32_t, 26)

#endif
