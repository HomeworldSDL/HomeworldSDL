/*=============================================================================
    Name    : SensorArray.h
    Purpose : Definitions for the Sensor Array

    Created 01/06/1998 by bpasechnik
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___SENSORARRAY_H
#define ___SENSORARRAY_H

#include "Types.h"
#include "SpaceObj.h"

/*=============================================================================
    Types:
=============================================================================*/

typedef struct
{
    udword dummy;
} SensorArraySpec;

/*=============================================================================
    Public data:
=============================================================================*/

extern CustShipHeader SensorArrayHeader;

//void SensorArrayDied(Ship *ship);

#endif