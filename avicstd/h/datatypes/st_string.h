/* ---------------------------------------------------------------------------
* Copyright 2018 by AVI GmbH, Mirko Wittek
* ST-Type-Definition, expected to be available by TeCom-C.
* --------------------------------------------------------------------------- */

#ifndef INC_GUARD_AVI_DATATYPE_STRING_H
#define INC_GUARD_AVI_DATATYPE_STRING_H

#ifdef _cplusplus
template<unsigned L> 
class TeString 
{
  public:
    uint16_t len; 
    uint16_t maxlen; 
    char str[L+1];
    TeString() { len=0; maxlen=L; str[L]=0; }
};
#define STRINGDEF(L) TeString<L>;
#else
#define STRINGDEF(L) struct { uint16_t len; uint16_t maxlen; char str[L+1]; };
#endif


#endif