/*=============================================================================
    Name    : LightCorvette.h
    Purpose : Definitions for Light Corvette

    Created 6/30/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___LIGHTCORVETTE_H
#define ___LIGHTCORVETTE_H

#include "Types.h"
#include "SpaceObj.h"
#include "Attack.h"

/*=============================================================================
    Types:
=============================================================================*/

typedef struct
{
    AttackSideStep attacksidestep;
} LightCorvetteSpec;

/*=============================================================================
    Public data:
=============================================================================*/

extern CustShipHeader LightCorvetteHeader;

#endif