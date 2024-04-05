/*=============================================================================
        Name    : hs.h
        Purpose : hyperspace rendering & management routines

Created 7/10/1998 by khent
Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___HS_H
#define ___HS_H

#include "SpaceObj.h"

void hsStart(Ship* ship, real32 cliptDelta, bool32 into, bool32 displayEffect);
void hsContinue(Ship* ship, bool32 displayEffect);
void hsEnd(Ship* ship, bool32 displayEffect);
void hsUpdate(Ship* ship);
void hsFinish(Ship* ship);
bool32 hsShouldDisplayEffect(Ship* ship);

#define HS_STATIC_ACTIVE        1
#define HS_STATIC_COLLAPSING    2
#define HS_STATIC_EXPANDING     3

#define HSGATE_LABEL_LEN        16

typedef struct hsStaticGate
{
    bool32   active;
    vector position;
    real32 rotation;
    sdword state;
    sdword counter;
    Derelict *derelict;
    char label[HSGATE_LABEL_LEN];
} hsStaticGate;

// for hyperspace gates
void hsDerelictDied(Derelict *derelict);
void hsDerelictTakesDamage(Derelict *derelict);

void hsStaticInit(sdword nVectors); //level start (init structs)
void hsStaticReset(void);   //level reset (clear structs)
void hsStaticDestroy(hvector*); //destroy a gate
void hsSetStaticData(sdword size, ubyte* data); //set static gate data (for loading)
void hsStaticRender(void);  //render all static gates
void hsNoGate(bool32 state);

void SaveHyperspaceGates(void);
void LoadHyperspaceGates(void);

Derelict *GetHyperspaceGateFromVector(vector *compare);

#endif
