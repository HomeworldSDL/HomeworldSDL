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

#define REPAIR_BEGIN          0
#define REPAIR_APPROACH       1
#define REPAIR_NEARING        2
#define REPAIR_STOP_ROTATION  3
#define REPAIR_DOCK_1         4
#define REPAIR_DOCK_2         5
#define REPAIR_REPAIR         6
#define REPAIR_DISENGAGE_1    7
#define REPAIR_DISENGAGE_2    8
#define REPAIR_DONE           9

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
