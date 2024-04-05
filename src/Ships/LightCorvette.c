// =============================================================================
//  LightCorvette.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "LightCorvette.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    AttackSideStep attacksidestep;
} LightCorvetteSpec;

typedef struct
{
    AttackSideStepParameters sidestepParameters;
} LightCorvetteStatics;

LightCorvetteStatics LightCorvetteStaticRace1;
LightCorvetteStatics LightCorvetteStaticRace2;


void LightCorvetteStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    LightCorvetteStatics *corvstat = (statinfo->shiprace == R1) ? &LightCorvetteStaticRace1 : &LightCorvetteStaticRace2;

    memset(corvstat, 0, sizeof(LightCorvetteStatics));
    scriptSetStruct(directory,filename,AttackSideStepParametersScriptTable,(ubyte *)&corvstat->sidestepParameters);

    statinfo->custstatinfo = corvstat;
}

void LightCorvetteInit(Ship *ship)
{
    LightCorvetteSpec *spec = (LightCorvetteSpec *)ship->ShipSpecifics;

    attackSideStepInit(&spec->attacksidestep);

}

void LightCorvetteAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    LightCorvetteSpec *spec = (LightCorvetteSpec *)ship->ShipSpecifics;
    LightCorvetteStatics *corvstat = (LightCorvetteStatics *)((ShipStaticInfo *)ship->staticinfo)->custstatinfo;

    attackSideStep(ship,target,&spec->attacksidestep,&corvstat->sidestepParameters);
}

void LightCorvetteAttackPassive(Ship *ship,Ship *target,bool32 rotate)
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


CustShipHeader LightCorvetteHeader =
{
    LightCorvette,
    sizeof(LightCorvetteSpec),
    LightCorvetteStaticInit,
    NULL,
    LightCorvetteInit,
    NULL,
    LightCorvetteAttack,
    DefaultShipFire,
    LightCorvetteAttackPassive,
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

