// =============================================================================
//  RepairCorvette.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#ifndef ___REPAIRCORVETTE_H
#define ___REPAIRCORVETTE_H

#include "Attack.h"
#include "ShipSelect.h"
#include "SpaceObj.h"

#define REPAIR_Begin        0
#define REPAIR_Approach     1
#define REPAIR_Nearing      2
#define REPAIR_StopRot      3
#define REPAIR_Dock1        4
#define REPAIR_Dock2        5
#define REPAIR_Repair       6
#define REPAIR_Disengage1   7
#define REPAIR_Disengage2   8
#define REPAIR_Done         9
#define REPAIR_Hackwork     10

typedef struct
{
    AttackSideStep attacksidestep;
    sdword repairState;
    struct Ship *target;
    sdword hyst;
    real32 disengagetime;
} RepairCorvetteSpec;

extern CustShipHeader RepairCorvetteHeader;

bool refuelRepairShips(Ship *ship, SelectAnyCommand *targets, real32 rangetoRefuel);
void RepairCorvetteOrderChanged(Ship *ship);
void stopRepairEffect(Ship *ship);

#endif