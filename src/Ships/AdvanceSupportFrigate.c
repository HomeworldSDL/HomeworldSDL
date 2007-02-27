// =============================================================================
//  AdvanceSupportFrigate.c
// =============================================================================
//  Copyright Relic Entertainment, Inc.  All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "AdvanceSupportFrigate.h"

#include "Attack.h"
#include "DefaultShip.h"
#include "RepairCorvette.h"
#include "ShipSelect.h"
#include "StatScript.h"

typedef struct
{
    udword dummy;
} AdvanceSupportFrigateSpec;

typedef struct
{
    real32 asfGunRange[NUM_TACTICS_TYPES];
    real32 asfTooCloseRange[NUM_TACTICS_TYPES];
    real32 repairApproachDistance;
} AdvanceSupportFrigateStatics;

AdvanceSupportFrigateStatics AdvanceSupportFrigateStatic;
AdvanceSupportFrigateStatics AdvanceSupportFrigateStaticRace1;
AdvanceSupportFrigateStatics AdvanceSupportFrigateStaticRace2;

scriptStructEntry ASFStaticScriptTable[] =
{
    { "repairApproachDistance",    scriptSetReal32CB, (udword) &(AdvanceSupportFrigateStatic.repairApproachDistance), (udword) &(AdvanceSupportFrigateStatic) },

    END_SCRIPT_STRUCT_ENTRY
};


void AdvanceSupportFrigateStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    AdvanceSupportFrigateStatics *asfstat = (statinfo->shiprace == R1) ? &AdvanceSupportFrigateStaticRace1 : &AdvanceSupportFrigateStaticRace2;

    statinfo->custstatinfo = asfstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        asfstat->asfGunRange[i] = statinfo->bulletRange[i];
        asfstat->asfTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }
    
    scriptSetStruct(directory,filename,ASFStaticScriptTable,(ubyte *)asfstat);
}

void AdvanceSupportFrigateAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    AdvanceSupportFrigateStatics *asfstat = (AdvanceSupportFrigateStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship,target,asfstat->asfGunRange[ship->tacticstype],asfstat->asfTooCloseRange[ship->tacticstype]);
}

void AdvanceSupportFrigateAttackPassive(Ship *ship,Ship *target,bool rotate)
{
    if ((rotate) & ((bool)((ShipStaticInfo *)(ship->staticinfo))->rotateToRetaliate))
    {
        attackPassiveRotate(ship,target);
    }
    else
    {
        attackPassive(ship,target);
    }
}

bool AdvacedSupportFrigateSpecialTarget(Ship *ship,void *custom)
{
    AdvanceSupportFrigateStatics *asfstat = (AdvanceSupportFrigateStatics *)ship->staticinfo->custstatinfo;
    SelectAnyCommand *targets;
    targets = (SelectAnyCommand *)custom;
    return(refuelRepairShips(ship, targets,asfstat->repairApproachDistance));
}

CustShipHeader AdvanceSupportFrigateHeader =
{
    AdvanceSupportFrigate,
    sizeof(AdvanceSupportFrigateSpec),
    AdvanceSupportFrigateStaticInit,
    NULL,
    NULL,
    NULL,
    AdvanceSupportFrigateAttack,
    DefaultShipFire,
    AdvanceSupportFrigateAttackPassive,
    NULL,
    AdvacedSupportFrigateSpecialTarget,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};
