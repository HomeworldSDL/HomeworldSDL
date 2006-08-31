/*=============================================================================
    Name    : ResourceCollector.h
    Purpose : Definitions for ResourceCollector

    Created 6/30/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___RESOURCECOLLECTOR_H
#define ___RESOURCECOLLECTOR_H

#include "Types.h"
#include "SpaceObj.h"

/*=============================================================================
    Types:
=============================================================================*/

typedef struct
{
    sdword resourcesCollected;
} ResourceCollectorSpec;

/*=============================================================================
    Public data:
=============================================================================*/

extern CustShipHeader ResourceCollectorHeader;

#endif