// =============================================================================
//  Animatic.h
//  - playback of animatics via OpenGL
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 2/11/1999 by khent
// =============================================================================

#ifndef ___ANIMATIC_H
#define ___ANIMATIC_H

#include "Types.h"

extern sdword animaticJustPlayed;

void animStartup(void);
void animShutdown(void);

void animAviSetup(bool on);
bool animAviPlay(sdword a, sdword b);
void animAviEnd(void);

bool animBinkPlay(sdword a, sdword b);

void animAviDecode(sdword frame);

#endif
