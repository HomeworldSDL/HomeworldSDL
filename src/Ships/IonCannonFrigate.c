// =============================================================================
//  IonCannonFrigate.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 11/5/1997 by khent
// =============================================================================

#include "IonCannonFrigate.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    udword dummy;
} IonCannonFrigateSpec;

typedef struct
{
    real32 frigateGunRange[NUM_TACTICS_TYPES];
    real32 frigateTooCloseRange[NUM_TACTICS_TYPES];
} IonCannonFrigateStatics;

IonCannonFrigateStatics IonCannonFrigateStaticRace1;
IonCannonFrigateStatics IonCannonFrigateStaticRace2;

void IonCannonFrigateStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    IonCannonFrigateStatics *frigstat = (statinfo->shiprace == R1) ? &IonCannonFrigateStaticRace1 : &IonCannonFrigateStaticRace2;

    statinfo->custstatinfo = frigstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        frigstat->frigateGunRange[i] = statinfo->bulletRange[i]*0.725f;
        frigstat->frigateTooCloseRange[i] = statinfo->minBulletRange[i] * 0.5f;
    }
}

void IonCannonFrigateAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    IonCannonFrigateStatics *frigstat = (IonCannonFrigateStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship,target,frigstat->frigateGunRange[ship->tacticstype],frigstat->frigateTooCloseRange[ship->tacticstype]);
}

void IonCannonFrigateAttackPassive(Ship *ship,Ship *target,bool32 rotate)
{
    if ((rotate) & ((bool32)((ShipStaticInfo *)(ship->staticinfo))->rotateToRetaliate))
    {
        attackPassiveRotate(ship,target);
    }
    else
    {
        attackPassive(ship,target);
    }
}

CustShipHeader IonCannonFrigateHeader =
{
    IonCannonFrigate,
    sizeof(IonCannonFrigateSpec),
    IonCannonFrigateStaticInit,
    NULL,
    NULL,
    NULL,
    IonCannonFrigateAttack,
    DefaultShipFire,
    IonCannonFrigateAttackPassive,
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

