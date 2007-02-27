// =============================================================================
//  CloakGenerator.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 01/06/1998 by bpasechnik
// =============================================================================

#ifndef ___CLOAKGENERATOR_H
#define ___CLOAKGENERATOR_H

#include "LinkedList.h"
#include "SpaceObj.h"
#include "Types.h"

typedef struct
{
    sdword CloakOn;
    real32 CloakStatus;
    LinkedList CloakList;
    bool CloakLowWarning;
} CloakGeneratorSpec;

typedef struct
{
    real32 CloakingRadius;
    real32 CloakingTime;
    real32 DeCloakingTime;
    real32 MaxCloakingTime;
    real32 MinCharge;
    real32 ReChargeRate;
    real32 CloakingRadiusSqr;
} CloakGeneratorStatics;

extern CustShipHeader CloakGeneratorHeader;

void cloakGeneratorJustDisabled(Ship *ship);

#endif
