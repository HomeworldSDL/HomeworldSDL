// =============================================================================
//  AutoLOD.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 9/8/1998 by khent
// =============================================================================

#ifndef ___AUTOLOD_H
#define ___AUTOLOD_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------

void alodStartup(void);
void alodReset(void);
void alodShutdown(void);
void alodSetMinMax(real32 minScale, real32 maxScale);
real32 alodGetMin(void);
real32 alodGetMax(void);
void alodSetTargetPolys(udword targetPolys, udword polyDelta);
void alodGetTargetPolys(udword* targetPolys, udword* polyDelta);
void alodSetPolys(udword polys);
void alodIncPolys(udword polys);
void alodEnable(bool32 enable);
void alodAdjustScaleFactor(void);
udword alodGetPolys(void);
bool32 alodGetPanic(void);
void alodSetPanic(bool32 panic);

#endif
