// =============================================================================
//  MinelayerCorvette.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 11/5/1997 by khent
// =============================================================================

#ifndef ___MINELAYERCORVETTE_H
#define ___MINELAYERCORVETTE_H

#include "SpaceObj.h"
#include "Attack.h"
#include "Formation.h"

#define MINE_DROP_ATTACK            0
#define MINE_DROP_FORMATION         1
#define MINE_SEARCH_AND_STOP        2
#define MINE_ATTACK_RUN             3
#define DO_WALL_FORMATION           4

typedef struct
{
    AttackSideStepParameters sidestepParameters;
    udword NumMinesInSide;
    real32 MINE_STOP_FRICTION;
    real32 MineSpacing;
    real32 MineDropDistance;
    real32 DropStopRadiusSqr;
    real32 breakInAwayDist;
    real32 DropRange;
    real32 DropStopRange;
    real32 FlyAwayDist[NUM_CLASSES];
    real32 FlyAwayTolerance;
    real32 Break2SphereizeFreq;
    udword NumMinesInSideSqr;
    real32 MineClearDistanceSQR;
    real32 MineClearDistance;
    real32 forced_drop_damage_hi;
    real32 forced_drop_damage_lo;
    real32 gunReFireTime;
    real32 forced_drop_lifetime;
    real32 mineRegenerateTime;
} MinelayerCorvetteStatics;

typedef struct
{
    AttackSideStep attacksidestep;
    sdword MiningStatus;
    udword dropcount;
    vector aivec;
    real32 aispheretime;
    udword mineaistate;
    vector formation_position;
    vector formation_up;
    vector formation_right;
    vector formation_left;
    vector formation_down;
    vector formation_heading;
    sdword MineDropNumber;
    sdword MiningSideMax;
    sdword MiningSideCount;
    MineFormationInfo *mineforminfo;
    sdword formation_number_X;
    sdword formation_number_Y;
    real32 lasttimeRegeneratedMines;
} MinelayerCorvetteSpec;

extern CustShipHeader MinelayerCorvetteHeader;

void MinelayerCorvetteOrderChangedCleanUp(Ship *ship);
void univUpdateMineWallFormations();

#endif