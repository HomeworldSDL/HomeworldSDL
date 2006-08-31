/*=============================================================================
    Name    : P1StandardCorvette.h
    Purpose : Definitions for P1StandardCorvette

    Created 5/06/1998 by ddunlop
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___P1STANDARDCORVETTE_H
#define ___P1STANDARDCORVETTE_H

#include "Types.h"
#include "SpaceObj.h"
#include "Attack.h"

/*=============================================================================
    Defines:
=============================================================================*/

/*=============================================================================
    Types:
=============================================================================*/

typedef struct
{
    AttackSideStep attacksidestep;
} P1StandardCorvetteSpec;

/*=============================================================================
    Public data:
=============================================================================*/

extern CustShipHeader P1StandardCorvetteHeader;

#endif