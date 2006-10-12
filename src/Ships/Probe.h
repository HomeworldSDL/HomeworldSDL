// =============================================================================
//  Probe.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 01/06/1998 by bpasechnik
// =============================================================================

#ifndef ___PROBE_H
#define ___PROBE_H

#include "SpaceObj.h"

typedef struct
{
    sdword HaveMoved;
    real32 moveTime;
} ProbeSpec;

typedef struct
{
    real32 ProbeDispatchMaxVelocity;
} ProbeStatics;

extern CustShipHeader ProbeHeader;

real32 ProbeGetMaxVelocity(Ship *ship);

#endif
