/*=============================================================================
    Name    : avi.h
    Purpose : routines for playing AVI files

    Created 1/9/1999 by jdorie, khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___AVI_H
#define ___AVI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"
#include "Globals.h"
#include "Subtitle.h"
#include "Debug.h"
#include "Animatic.h"
#include "SoundEvent.h"
#include "soundlow.h"
#include "glinc.h"
#include "mouse.h"
#include "render.h"
#include "Universe.h"
#include "glext.h"
#include "StringSupport.h"
#include "File.h"
#include "NIS.h"
#include "avi.h"
#include <unistd.h>
#include <ffmpeg/avformat.h>
#include <ffmpeg/avcodec.h>
#include <SDL.h>

/*=============================================================================
    Switches
=============================================================================*/

#ifdef HW_Debug

#define AVI_ERROR_CHECKING      1               //General error checking
#define AVI_VERBOSE_LEVEL       3               //print extra info
#define AVI_SKIP_PLAY           0               //print extra info

#else

#define AVI_ERROR_CHECKING      0               //General error checking
#define AVI_VERBOSE_LEVEL       0               //print extra info
#define AVI_SKIP_PLAY           0               //print extra info

#endif

/*=============================================================================
    Defitions:
=============================================================================*/

/*=============================================================================
    Type definitions:
=============================================================================*/

extern bool utilPlayingIntro;


/*=============================================================================
    Functions:
=============================================================================*/


int aviInit(void);
bool aviPlay(char* filename);
int aviStop(void);
int aviCleanup(void);

int aviGetSamples(void* pBuf, long* pNumSamples, long nBufSize);

extern int aviIsPlaying;



#ifdef __cplusplus
}
#endif

#endif

