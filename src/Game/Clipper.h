// =============================================================================
//  Clipper.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 4/10/1998 by khent
// =============================================================================

#ifndef ___CLIPPER_H
#define ___CLIPPER_H

#include "Vector.h"

sdword clipViewclipLine(real32* vectors, udword* i, udword* j);
void clipTransformPoints(udword n, real32* vobj, real32* vEye, real32* m);
bool clipIsPerspective(real32* m);
void clipProjectPoints(udword n, real32* vEye, real32* vClip, ubyte* clipmask,
                       ubyte* clipormask, ubyte* clipandmask, real32* m);
void clipViewportMap(udword n, real32* vClip, ubyte* clipmask, real32* vWin,
                     vector* viewportS, vector* viewportT, bool force);

bool clipLineToScreen(vector* pa, vector* pb, real32* modelview, real32* projection, vector* screenA, vector* screenB);
bool clipPointToScreen(vector* p, vector* screen, bool force);
bool clipPointToScreenWithMatrices(vector* p, vector* screen, real32* modelview, real32* projection, bool force);
bool clipBBoxIsClipped(real32* collrectoffset, real32 uplength, real32 rightlength, real32 forwardlength);

#endif

