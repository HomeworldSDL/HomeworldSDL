// =============================================================================
//  FastMath.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 9/9/1997 by gshaw
// =============================================================================

#ifndef ___FASTMATH_H
#define ___FASTMATH_H

#include "Types.h"

#define fsqrt(x)     (real32)fmathSqrtDouble((real32)(x))
#define fmathSqrt(x) (real32)fmathSqrtDouble((real32)(x))

// pi
#ifndef PI
    #define PI 3.14159265358979f
#endif

#define TWOPI   (2.0f * PI)
#define FOURPI  (4.0f * PI)

// angles
#define DEG_PER_RADIAN (360.0f / TWOPI)
#define RADIAN_PER_DEG (TWOPI / 360.0f)

#define DEG_TO_RAD(x) ((x) * RADIAN_PER_DEG)
#define RAD_TO_DEG(x) ((x) * DEG_PER_RADIAN)

// volume, area computations
#define circleCircumference(radius) ( TWOPI * (radius))
#define circleArea(radius)          (    PI * (radius) * (radius))
#define sphereArea(radius)          (FOURPI * (radius) * (radius))
#define sphereVolume(radius)        (FOURPI * (radius) * (radius) * (radius) / 3.0f)

void   fmathInit(void);
double fmathSqrtDouble(double f);

#endif

