// =============================================================================
//  ProximitySensor.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 01/06/1998 by bpasechnik
// =============================================================================

#ifndef ___PROXIMITYSENSOR_H
#define ___PROXIMITYSENSOR_H

#include "ShipSelect.h"
#include "SpaceObj.h"
#include "Universe.h"

#define SENSOR_BEGIN    0
#define SENSOR_SENSE    1
#define SENSOR_SENSED   2
#define SENSOR_SENSED2  3

typedef struct
{
    udword SearchRate;
    udword SearchRateAfterFound;
    real32 SearchRadius;
    real32 SearchRadiusSqr;
    sdword SensorCircleRadius;
    real32 SensorBlinkRate;
    real32 TriggerSpeed;
    real32 TriggerSpeedSqr;
    udword blipColor;
    sdword blipThickness;
} ProximitySensorStatics;

typedef struct
{
    udword sensorState;
    sdword blipState;
    real32 blipRadius;
    sdword TAGGED;
} ProximitySensorSpec;

extern color ProximitySensorBlipColor;
extern CustShipHeader ProximitySensorHeader;

void proxInFocusSelection(SelectCommand *focus);
bool32 proximityCanPlayerSeeShip(Player *player,Ship *shipInQuestion);

#endif
