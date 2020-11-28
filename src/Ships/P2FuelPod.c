// =============================================================================
//  P2FuelPod.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/07/1998 by ddunlop
// =============================================================================

#include "P2FuelPod.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    udword dummy;
} P2FuelPodSpec;

typedef struct
{
    real32 podGunRange[NUM_TACTICS_TYPES];
    real32 podTooCloseRange[NUM_TACTICS_TYPES];
} P2FuelPodStatics;

P2FuelPodStatics P2FuelPodStatic;

void P2FuelPodStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    P2FuelPodStatics *podstat = &P2FuelPodStatic;

    statinfo->custstatinfo = podstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        podstat->podGunRange[i] = statinfo->bulletRange[i];
        podstat->podTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }
}

void P2FuelPodAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    P2FuelPodStatics *podstat = (P2FuelPodStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship, target, podstat->podGunRange[ship->tacticstype], podstat->podTooCloseRange[ship->tacticstype], NULL, NULL);
}

void P2FuelPodAttackPassive(Ship *ship,Ship *target,bool rotate)
{
    if ((rotate) & ((bool)((ShipStaticInfo *)(ship->staticinfo))->rotateToRetaliate))
    {
        attackPassiveRotate(ship, target, NULL, NULL);
    }
    else
    {
        attackPassive(ship, target, NULL, NULL);
    }
}

CustShipHeader P2FuelPodHeader =
{
    P2FuelPod,
    sizeof(P2FuelPodSpec),
    P2FuelPodStaticInit,
    NULL,
    NULL,
    NULL,
    P2FuelPodAttack,
    DefaultShipFire,
    P2FuelPodAttackPassive,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

