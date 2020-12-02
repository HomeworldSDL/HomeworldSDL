// =============================================================================
//  StandardDestroyer.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 01/06/1998 by bpasechnik
// =============================================================================

#include "StandardDestroyer.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    udword dummy;
} StandardDestroyerSpec;

typedef struct
{
    real32 standarddestroyerGunRange[NUM_TACTICS_TYPES];
    real32 standarddestroyerTooCloseRange[NUM_TACTICS_TYPES];
} StandardDestroyerStatics;

StandardDestroyerStatics StandardDestroyerStaticRace1;
StandardDestroyerStatics StandardDestroyerStaticRace2;

void StandardDestroyerStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    StandardDestroyerStatics *sdestroyerstat = (statinfo->shiprace == R1) ? &StandardDestroyerStaticRace1 : &StandardDestroyerStaticRace2;

    statinfo->custstatinfo = sdestroyerstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        sdestroyerstat->standarddestroyerGunRange[i] = statinfo->bulletRange[i];
        sdestroyerstat->standarddestroyerTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }
}

void StandardDestroyerAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    StandardDestroyerStatics *sdestroyerstat = (StandardDestroyerStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship, target, sdestroyerstat->standarddestroyerGunRange[ship->tacticstype], sdestroyerstat->standarddestroyerTooCloseRange[ship->tacticstype], NULL, NULL);
}

void StandardDestroyerAttackPassive(Ship *ship,Ship *target,bool rotate)
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

CustShipHeader StandardDestroyerHeader =
{
    StandardDestroyer,
    sizeof(StandardDestroyerSpec),
    StandardDestroyerStaticInit,
    NULL,
    NULL,
    NULL,
    StandardDestroyerAttack,
    DefaultShipFire,
    StandardDestroyerAttackPassive,
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

