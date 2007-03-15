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


/*=============================================================================
    Switches
=============================================================================*/

#ifdef HW_Debug

#define ANIM_ERROR_CHECKING      1               //General error checking
#define ANIM_VERBOSE_LEVEL       3               //print extra info
#define ANIM_SKIP_PLAY           0               //print extra info

#else

#define ANIM_ERROR_CHECKING      0               //General error checking
#define ANIM_VERBOSE_LEVEL       0               //print extra info
#define ANIM_SKIP_PLAY           0               //print extra info

#endif

#endif
