/*=============================================================================
    Name    : StandardDestroyer.h
    Purpose : Definitions for StandardDestroyer

    Created 01/06/1998 by bpasechnik
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___STANDARDDESTROYER_H
#define ___STANDARDDESTROYER_H

#include "Types.h"
#include "SpaceObj.h"

/*=============================================================================
    Defines:
=============================================================================*/

/*=============================================================================
    Types:
=============================================================================*/

typedef struct
{
    udword dummy;
} StandardDestroyerSpec;

/*=============================================================================
    Public data:
=============================================================================*/

extern CustShipHeader StandardDestroyerHeader;

#endif