// =============================================================================
//  FloatingCity.c
//  - Bentusi Trader's Mothership
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/06/1998 by ddunlop
// =============================================================================

#include "FloatingCity.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    udword dummy;
} FloatingCitySpec;

typedef struct
{
    real32 floatingcityGunRange[NUM_TACTICS_TYPES];
    real32 floatingcityTooCloseRange[NUM_TACTICS_TYPES];
} FloatingCityStatics;

FloatingCityStatics FloatingCityStatic;

void FloatingCityStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    FloatingCityStatics *floatingcitystat = &FloatingCityStatic;

    statinfo->custstatinfo = floatingcitystat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        floatingcitystat->floatingcityGunRange[i] = statinfo->bulletRange[i];
        floatingcitystat->floatingcityTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }
}

void FloatingCityAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    FloatingCityStatics *floatingcitystat = (FloatingCityStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship, target, floatingcitystat->floatingcityGunRange[ship->tacticstype], floatingcitystat->floatingcityTooCloseRange[ship->tacticstype], NULL, NULL);
}

void FloatingCityAttackPassive(Ship *ship,Ship *target,bool rotate)
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

CustShipHeader FloatingCityHeader =
{
    FloatingCity,
    sizeof(FloatingCitySpec),
    FloatingCityStaticInit,
    NULL,
    NULL,
    NULL,
    FloatingCityAttack,
    DefaultShipFire,
    FloatingCityAttackPassive,
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

