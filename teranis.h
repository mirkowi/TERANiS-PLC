#ifndef teranis_h
#define teranis_h
#include <stdint.h>
#include "avicstd/h/avi_c_std.h"

// Define the size in Bytes for different PLC memory types
#define PLC_I_SIZE 256 // Input Bytes
#define PLC_Q_SIZE 256 // Output Bytes
#define PLC_F_SIZE 256 // Flags Bytes
#define PLC_R_SIZE 256 // Remanent Flags Bytes
#define PLC_S_SIZE 256 // System Flags Bytes

typedef struct
{
  uint8_t x0 : 1;
  uint8_t x1 : 1;
  uint8_t x2 : 1;
  uint8_t x3 : 1;
  uint8_t x4 : 1;
  uint8_t x5 : 1;
  uint8_t x6 : 1;
  uint8_t x7 : 1;
} T8Bits;

// Macro zum casten eines Typs in ein Byte-Array
#ifdef __cplusplus
  // Templatefunktion zum Ermitteln eines Zeigers mit definiertem Typ in ein Variablenfeld
  // Dabei wird auf Überschreitung des Index getestet und zur Übersetzungszeit Fehler gemeldet
  // Leider ist es nicht möglich auszugeben, welches Array gemeint ist.
  template <typename T,const unsigned S,const unsigned I>
  T* TeArrayElement(uint8_t *A)
  {
    static_assert(S>=I, "index exceeds size of array");
    static_assert(S>=(I+sizeof(T)), "index and size of type exceeds size of array");
    return ((T*)(&(A[I])));
  }; 

  // Macrodefinition
  #define TREF(typ,arr,idx) (*TeArrayElement<typ,sizeof(arr),idx>(arr))   
#else
  // Die Standard-C Variante ohne Indexüberprüfung
  #define TREF(typ,arr,idx) (*((typ*)(&(arr[idx]))))
#endif

// Macro zum Zugriff auf ein Bit in einem Byte-Array
#define XREF(arr,idx,bitnr) TREF(T8Bits,arr,idx).x##bitnr

#if PLC_I_SIZE>0
  extern uint8_t Inputs[PLC_I_SIZE];
  #define I(typ,idx) TREF(typ,Inputs,idx)
  #define IX(idx,bitnr) XREF(Inputs,idx,bitnr)
#endif

#if PLC_Q_SIZE>0
  extern uint8_t Outputs[PLC_Q_SIZE];
  #define Q(typ,idx) TREF(typ,Outputs,idx)
  #define QX(idx,bitnr) XREF(Outputs,idx,bitnr)
#endif

#if PLC_F_SIZE>0
  extern uint8_t Flags[PLC_F_SIZE];
  #undef F
  #define F(typ,idx) TREF(typ,Flags,idx)
  #define FX(idx,bitnr) XREF(Flags,idx,bitnr)
#endif

#if PLC_R_SIZE>0
  extern uint8_t RFlags[PLC_F_SIZE];
  #define R(typ,idx) TREF(typ,RFlags,idx)
  #define RX(idx,bitnr) XREF(RFlags,idx,bitnr)
#endif

#if PLC_S_SIZE>0
  extern uint8_t SFlags[PLC_S_SIZE];
  #define S(typ,idx) TREF(typ,SFlags,idx)
  #define SX(idx,bitnr) XREF(SFlags,idx,bitnr)

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


void setup_teranis();
void loop_teranis();

#endif
