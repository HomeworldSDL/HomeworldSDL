// =============================================================================
//  P2AdvanceSwarmer.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "P2AdvanceSwarmer.h"

#include "Attack.h"
#include "DefaultShip.h"

typedef struct
{
    AttackSideStep attacksidestep;
} P2AdvanceSwarmerSpec;

typedef struct
{
    AttackSideStepParameters sidestepParameters;
} P2AdvanceSwarmerStatics;

P2AdvanceSwarmerStatics P2AdvanceSwarmerStaticRace1;
P2AdvanceSwarmerStatics P2AdvanceSwarmerStaticRace2;


void P2AdvanceSwarmerStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    P2AdvanceSwarmerStatics *corvstat = (statinfo->shiprace == R1) ? &P2AdvanceSwarmerStaticRace1 : &P2AdvanceSwarmerStaticRace2;

    memset(corvstat, 0, sizeof(P2AdvanceSwarmerStatics));
    scriptSetStruct(directory,filename,AttackSideStepParametersScriptTable,(ubyte *)&corvstat->sidestepParameters);

    statinfo->custstatinfo = corvstat;
}

void P2AdvanceSwarmerInit(Ship *ship)
{
    P2AdvanceSwarmerSpec *spec = (P2AdvanceSwarmerSpec *)ship->ShipSpecifics;

    attackSideStepInit(&spec->attacksidestep);

}

void P2AdvanceSwarmerAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    P2AdvanceSwarmerSpec *spec = (P2AdvanceSwarmerSpec *)ship->ShipSpecifics;
    P2AdvanceSwarmerStatics *corvstat = (P2AdvanceSwarmerStatics *)((ShipStaticInfo *)ship->staticinfo)->custstatinfo;

    attackSideStep(ship,target,&spec->attacksidestep,&corvstat->sidestepParameters);
}

void P2AdvanceSwarmerAttackPassive(Ship *ship,Ship *target,bool32 rotate)
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


CustShipHeader P2AdvanceSwarmerHeader =
{
    P2AdvanceSwarmer,
    sizeof(P2AdvanceSwarmerSpec),
    P2AdvanceSwarmerStaticInit,
    NULL,
    P2AdvanceSwarmerInit,
    NULL,
    P2AdvanceSwarmerAttack,
    DefaultShipFire,
    P2AdvanceSwarmerAttackPassive,
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

