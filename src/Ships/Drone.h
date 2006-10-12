// =============================================================================
//  Drone.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#ifndef ___DRONE_H
#define ___DRONE_H

#include "SpaceObj.h"

#define DRONE_STATE_DORMANT      0
#define DRONE_STATE_LAUNCHING    1
#define DRONE_STATE_LAUNCHED     2
#define DRONE_STATE_DOCKING      3

typedef struct
{
    udword droneNumber;
    udword droneState;
} DroneSpec;

extern CustShipHeader DroneHeader;

#endif
