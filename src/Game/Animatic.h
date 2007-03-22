/*=============================================================================
    Name    : animatic.h
    Purpose : playback of animatics via OpenGL

    Created 2/11/1999 by khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

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
