// =============================================================================
//  SalCapCorvette.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 11/5/1997 by khent
// =============================================================================

#ifndef ___SALCAPCORVETTE_H
#define ___SALCAPCORVETTE_H

#include "Attack.h"
#include "ShipSelect.h"
#include "SpaceObj.h"

#define SALVAGE_AT_GET_TECH       0x01

#define SAL_WL_FLYTOCONEORIGIN    7

typedef struct
{
    AttackSideStep attacksidestep;
    sdword salvageState;               //sal/cap corv aistate for salvaging
    SpaceObjRotImpTargGuidanceShipDerelict *target;
    Ship *dockwith;
    sdword dockindex;
    sdword salvageIndex;
    sdword salvageAttributes;
    real32 timeCounter;
    real32 getTechTime;
    sdword tractorBeam;
    sdword noAvoid;
    real32 noDamageTargetTime;
    sdword groupme;
	SpaceObjRotImpTargGuidanceShipDerelict *noDamageTarget;
} SalCapCorvetteSpec;

typedef struct
{
    AttackSideStepParameters sidestepParameters;
    real32 HealthThreshold;
    real32 healthRemovedPerSecond;
    real32 getTechTime;
    real32 flyToDistance;
    real32 maxPushingVelocitySingle;
    real32 noLightClampingDistance;
} SalCapCorvetteStatics;

extern CustShipHeader JunkYardDawgHeader;
extern CustShipHeader SalCapCorvetteHeader;

bool DropTargetInShip(Ship *dockwith,sdword *targetDepotState, SpaceObjRotImpTargGuidanceShipDerelict *target,sdword *dockindex);
bool isThereAnotherTargetForMe(Ship *ship,SelectAnyCommand *targets);
void salCapCleanUpCloakingTarget(Ship *ship, Ship *shiptoremove);
void salCapClearTechBool();
void SalCapCorvette_RegisterExtraSpaceObjs(Ship *ship);
void SalCapDropTarget(Ship *ship);
void salCapExtraSpecialOrderCleanUp(SelectCommand *selection,udword ordertype,Ship *dockwith,SelectCommand *targets);
void salCapHarvestTarget(SpaceObjRotImpTargGuidanceShipDerelict *target,Ship *dockwith);
sdword salCapNeedBig(Ship *ship,SpaceObjRotImpTargGuidanceShipDerelict *target);
void SalCapOrderChangedCleanUp(Ship *ship);
void salCapRemoveDerelictReferences(Ship *ship,Derelict *d);

#if RND_VISUALIZATION
void dockDrawSalvageInfo(SpaceObjRotImpTargGuidanceShipDerelict *obj);
#endif

#endif
