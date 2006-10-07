// =============================================================================
//  Randy.h
//  - Generic random-number generators with multiple streams
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 11/11/1997 by lmoloney
// =============================================================================

#ifndef ___RANDY_H
#define ___RANDY_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------

#ifndef HW_Release
    #define RANDOM_ERROR_CHECKING      1    // general error checking
    #define RANDOM_VERBOSE_LEVEL       2    // control specific output code
    #define RANDOM_DEBUG_CALL_SEQ      1    // debug random number calling sequence
#else
    #define RANDOM_ERROR_CHECKING      0
    #define RANDOM_VERBOSE_LEVEL       0
    #define RANDOM_DEBUG_CALL_SEQ      0
#endif


#define RANDOM_PARTICLE_STREAM         0
#define RANDOM_ETG                     1
#define RANDOM_SOUND                   2
#define RANDOM_TRAILS                  3
#define RANDOM_CLOUDS                  4
#define RANDOM_NEBULAE                 5
#define RANDOM_AI_PLAYER               6
#define RANDOM_DAMAGE                  7
#define RANDOM_BATTLE                  8
#define RANDOM_STATIC                  9
#define RANDOM_GAME                   10    // if you change this, change RANDOM_SOUND_GAME_THREAD too
#define RANDOM_SOUND_GAME_THREAD      11
#define RANDOM_SOUND_BOTH_THREADS     12
#define RANDOM_TRAILS_0               13
#define RANDOM_TRAILS_1               14
#define RANDOM_TRAILS_2               15
#define RANDOM_TRAILS_3               16
#define RANDOM_TRAILS_4               17
#define RANDOM_SOUND_GAME_THREAD_0    18
#define RANDOM_SOUND_GAME_THREAD_1    19
#define RANDOM_SOUND_GAME_THREAD_2    20
#define RANDOM_SOUND_GAME_THREAD_3    21
#define RANDOM_SOUND_GAME_THREAD_4    22
#define RANDOM_SOUND_GAME_THREAD_5    23
#define RANDOM_SOUND_GAME_THREAD_6    24
#define RANDOM_NUMBER_STREAMS         25


typedef struct
{
    udword x, y, z, c, n;
}
ranstream;


#if RANDOM_DEBUG_CALL_SEQ
    udword ranRandomFnSimple(sdword ranIndex);
    udword ranRandomFn(sdword ranIndex, char *file, sdword line);

    #define ranRandom(i)  ranRandomFn(i, NULL, 0)
    #define gamerand()    ranRandomFn(RANDOM_GAME, __FILE__, __LINE__)
    
    extern bool ranCallerDebug;
#else
    #define ranRandomFnSimple  ranRandomFn
    udword  ranRandomFn(sdword ranIndex);

    #define ranRandom(i)  ranRandomFn(i)
    #define gamerand()    ranRandomFn(RANDOM_GAME)
#endif


#define randomG(n)          (gamerand() % (n))
#define frandom(n)          (gamerand() * (((real32)(n)) * (1.0f / ((real32)UDWORD_Max))))
#define randombetween(a,b)  (randomG((b)-(a)+1) + (a))
#define frandombetween(a,b) (frandom((b)-(a)  ) + (a))


void ranRandomize(sdword ranIndex);

udword ranNumberGet(sdword ranIndex);
void   ranNumberSet(sdword ranIndex, udword nn);

void ranParametersGet(sdword ranIndex, udword *xx, udword *yy, udword *zz, udword *cc, udword *nn);
void ranParametersSet(sdword ranIndex, udword  xx, udword  yy, udword  zz, udword  cc, udword  nn);
void ranParametersReset(sdword ranIndex);

void ranStartup(void);
void ranShutdown(void);

void ranLoad(void);
void ranSave(void);


#endif
