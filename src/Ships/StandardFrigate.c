// =============================================================================
//  StandardFrigate.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "StandardFrigate.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    udword dummy;
} StandardFrigateSpec;

typedef struct
{
    real32 frigateGunRange[NUM_TACTICS_TYPES];
    real32 frigateTooCloseRange[NUM_TACTICS_TYPES];
} StandardFrigateStatics;

StandardFrigateStatics StandardFrigateStaticRace1;
StandardFrigateStatics StandardFrigateStaticRace2;

void StandardFrigateStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    StandardFrigateStatics *frigstat = (statinfo->shiprace == R1) ? &StandardFrigateStaticRace1 : &StandardFrigateStaticRace2;

    statinfo->custstatinfo = frigstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        frigstat->frigateGunRange[i] = statinfo->bulletRange[i];
        frigstat->frigateTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }
}

void StandardFrigateAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    StandardFrigateStatics *frigstat = (StandardFrigateStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship, target, frigstat->frigateGunRange[ship->tacticstype], frigstat->frigateTooCloseRange[ship->tacticstype], NULL, NULL);
}

void StandardFrigateAttackPassive(Ship *ship,Ship *target,bool rotate)
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

CustShipHeader StandardFrigateHeader =
{
    StandardFrigate,
    sizeof(StandardFrigateSpec),
    StandardFrigateStaticInit,
    NULL,
    NULL,
    NULL,
    StandardFrigateAttack,
    DefaultShipFire,
    StandardFrigateAttackPassive,
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

