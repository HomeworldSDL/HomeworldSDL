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


/*=============================================================================
    Definitions:
=============================================================================*/

#ifdef HW_BUILD_FOR_DEBUGGING
    #define AVI_VERBOSE_LEVEL  3    // print extra info
#else
    #define AVI_VERBOSE_LEVEL  0
#endif


extern bool utilPlayingIntro;


/*=============================================================================
    Functions:
=============================================================================*/


int aviInit(void);
bool aviPlay(char* filename);
int aviStop(void);
int aviCleanup(void);
void aviIntroPlay(void);

int aviGetSamples(void* pBuf, long* pNumSamples, long nBufSize);

extern int aviIsPlaying;



#ifdef __cplusplus
}
#endif

#endif

