// =============================================================================
//  ResourceCollector.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "ResourceCollector.h"

#include "RepairCorvette.h"
#include "ResCollect.h"
#include "ShipSelect.h"
#include "StatScript.h"

typedef struct
{
    sdword resourcesCollected;
} ResourceCollectorSpec;


typedef struct
{
    real32 repairApproachDistance;
} ResourceCollectorStatics;

ResourceCollectorStatics ResourceCollectorStatic;

ResourceCollectorStatics ResourceCollectorStaticRace1;
ResourceCollectorStatics ResourceCollectorStaticRace2;

scriptStructEntry ResourceCollectorStaticScriptTable[] =
{
    { "repairApproachDistance",    scriptSetReal32CB,  &(ResourceCollectorStatic.repairApproachDistance),  &(ResourceCollectorStatic) },

    END_SCRIPT_STRUCT_ENTRY
};

void ResourceCollectorStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    ResourceCollectorStatics *resourcestat = (statinfo->shiprace == R1) ? &ResourceCollectorStaticRace1 : &ResourceCollectorStaticRace2;

    statinfo->custstatinfo = resourcestat;
    scriptSetStruct(directory,filename,ResourceCollectorStaticScriptTable,(ubyte *)resourcestat);

}

void ResourceCollectorInit(Ship *ship)
{
    ResourceCollectorSpec *spec = (ResourceCollectorSpec *)ship->ShipSpecifics;

    spec->resourcesCollected = 0;
}


bool32 ResourceCollectorSpecialTarget(Ship *ship, void *custom)
{
    ResourceCollectorStatics *resourcestat = (ResourceCollectorStatics *)ship->staticinfo->custstatinfo;
    SelectAnyCommand *targets;
    targets = (SelectAnyCommand *)custom;
    return(refuelRepairShips(ship, targets,resourcestat->repairApproachDistance));
}


void ResourceCollectorAttack(Ship *ship, SpaceObjRotImpTarg *target,real32 maxdist)
{
    R1ResourcerAttacksShip(ship,target,FALSE);
}

CustShipHeader ResourceCollectorHeader =
{
    ResourceCollector,
    sizeof(ResourceCollectorSpec),
    ResourceCollectorStaticInit,
    NULL,
    ResourceCollectorInit,
    NULL,
    ResourceCollectorAttack,
    NULL,
    NULL,
    NULL,
    ResourceCollectorSpecialTarget,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

