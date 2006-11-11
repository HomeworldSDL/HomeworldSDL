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

void   fmathInit(void);
double fmathSqrtDouble(double f);

#endif

