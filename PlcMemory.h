
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


#endif
