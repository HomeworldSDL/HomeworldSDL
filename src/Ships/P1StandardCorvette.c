// =============================================================================
//  P1StandardCorvette.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/06/1998 by ddunlop
// =============================================================================

#include "P1StandardCorvette.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    AttackSideStep attacksidestep;
} P1StandardCorvetteSpec;

typedef struct
{
    AttackSideStepParameters sidestepParameters;
} P1StandardCorvetteStatics;

P1StandardCorvetteStatics P1StandardCorvetteStatic;

void P1StandardCorvetteStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    P1StandardCorvetteStatics *corvstat = &P1StandardCorvetteStatic;

    memset(corvstat, 0, sizeof(P1StandardCorvetteStatics));
    scriptSetStruct(directory,filename,AttackSideStepParametersScriptTable,(ubyte *)&corvstat->sidestepParameters);

    statinfo->custstatinfo = corvstat;
}

void P1StandardCorvetteInit(Ship *ship)
{
    P1StandardCorvetteSpec *spec = (P1StandardCorvetteSpec *)ship->ShipSpecifics;

    attackSideStepInit(&spec->attacksidestep);
}

void P1StandardCorvetteAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    P1StandardCorvetteSpec *spec = (P1StandardCorvetteSpec *)ship->ShipSpecifics;
    P1StandardCorvetteStatics *corvstat = (P1StandardCorvetteStatics *)((ShipStaticInfo *)ship->staticinfo)->custstatinfo;

    attackSideStep(ship, target, &spec->attacksidestep, &corvstat->sidestepParameters, NULL, NULL);
}

void P1StandardCorvetteAttackPassive(Ship *ship,Ship *target,bool rotate)
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

CustShipHeader P1StandardCorvetteHeader =
{
    P1StandardCorvette,
    sizeof(P1StandardCorvetteSpec),
    P1StandardCorvetteStaticInit,
    NULL,
    P1StandardCorvetteInit,
    NULL,
    P1StandardCorvetteAttack,
    DefaultShipFire,
    P1StandardCorvetteAttackPassive,
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

