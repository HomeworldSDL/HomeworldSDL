// =============================================================================
//  Attack.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 10/7/1997 by gshaw
// =============================================================================

#ifndef ___ATTACK_H
#define ___ATTACK_H

#include "SpaceObj.h"
#include "StatScript.h"

typedef struct
{
    real32 aitime;           // AI timestamp variable
    udword aidirection;      // AI miscellaneous variable
} AttackSideStep;

typedef struct
{
    real32 repositionTime;
    real32 circleRange;
    bool fullMovementFreedom;
} AttackSideStepParameters;

void attackPassiveRotate(Ship *ship, Ship *target, void (*onPreFire)(struct Ship *ship), void (*onPostFire)(struct Ship *ship));

void attackSideStepInit(AttackSideStep *attacksidestep);
void attackSideStep(Ship *ship, SpaceObjRotImpTarg *target, AttackSideStep *attacksidestep, AttackSideStepParameters *parameters, void (*onPreFire)(struct Ship *ship), void (*onPostFire)(struct Ship *ship));

void attackStraightForward(Ship *ship, SpaceObjRotImpTarg *target, real32 gunRange, real32 tooCloseRange, void (*onPreFire)(struct Ship *ship), void (*onPostFire)(struct Ship *ship));

void attackPassive(Ship *ship, Ship *target, void (*onPreFire)(struct Ship *ship), void (*onPostFire)(struct Ship *ship));

void attackSimple(Ship *ship, SpaceObjRotImpTarg *target, void (*onPreFire)(struct Ship *ship), void (*onPostFire)(struct Ship *ship));

/*=============================================================================
    Data:
=============================================================================*/

extern scriptStructEntry AttackSideStepParametersScriptTable[];

#endif
