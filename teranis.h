#ifndef teranis_h
#define teranis_h

#include "avicstd/h/avi_c_std.h"
#include "PlcMemory.h"


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
