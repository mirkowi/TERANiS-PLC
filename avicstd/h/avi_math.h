/* ---------------------------------------------------------------------------
* Copyright 2018 by AVI GmbH, Tim Trense, Mirko Wittek
* ST-Definitions, expected to be available by TeCom-C.
* --------------------------------------------------------------------------- */

#ifndef INC_GUARD_AVI_MATH_H
#define INC_GUARD_AVI_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

inline double toRadians(double deg)
{
  return deg * M_PI / 180;
};

inline double toDegrees(double rad)
{
  return rad * 180 / M_PI;
};

inline float toRadians(float deg)
{
  return deg * M_PI / 180;
};

inline float toDegrees(float rad)
{
  return rad * 180 / M_PI;
};

#endif