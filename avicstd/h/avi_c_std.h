#ifndef INC_GUARD_AVI_C_STD_H
#define INC_GUARD_AVI_C_STD_H

/* ---------------------------------------------------------------------------
* Copyright 2018 by AVI GmbH, Tim Trense
* This file defines syntactic features expected to be defined by TeCom-C.
* To generate code, compilable for a standard-c-compiler, these definitions must be included.
* If the target system does not support several of these features they must at least be 
* syntactically available.
* --------------------------------------------------------------------------- */

// Introducing the BOOL data type
#include "datatypes/st_bool.h"

// Introducing the ANY data types
#include "datatypes/st_numeric.h"

// Introducing the STRING data type
#include "datatypes/st_string.h"

// Introducing the time- & date- data types
#include "datatypes/st_time.h"

// Definition for standard functions like malloc() and free()
//#include <stdlib.h>

// Definition of mathematical functions
//#include "avi_math.h"

#endif