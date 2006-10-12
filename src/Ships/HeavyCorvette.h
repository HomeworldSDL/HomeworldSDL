// =============================================================================
//  HeavyCorvette.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#ifndef ___HEAVYCORVETTE_H
#define ___HEAVYCORVETTE_H

#include "Attack.h"
#include "SpaceObj.h"

typedef struct
{
    AttackSideStep attacksidestep;
    sdword burstState;
    sdword cooldown;
    vector burstFireVector;
    real32 burstChargeState;
    real32 burstChargeState2;
    real32 bulletLifeTime;
    Effect *chargeEffect;
} HeavyCorvetteSpec;

extern CustShipHeader HeavyCorvetteHeader;

void heavyCorvetteOrderChanged(Ship *ship);

#endif
