// =============================================================================
//  Drone.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#ifndef ___DRONE_H
#define ___DRONE_H

#include "SpaceObj.h"

#define DRONESTATE_DORMANT      0
#define DRONESTATE_LAUNCHING    1
#define DRONESTATE_LAUNCHED     2
#define DRONESTATE_DOCKING      3

typedef struct
{
    udword droneNumber;
    udword droneState;
} DroneSpec;

extern CustShipHeader DroneHeader;

#endif