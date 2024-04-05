/*=============================================================================
    Name    : InfoOverlay.h
    Purpose : this file has the definitions for the information overlay.

    Created 6/8/1998 by ddunlop
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___INFOOVERLAY_H
#define ___INFOOVERLAY_H

#include "Region.h"
#include "SpaceObj.h"

/*=============================================================================
    Defines:
=============================================================================*/

#define IO_ListTextColor        colRGB(0  , 100, 160)
#define IO_SelectedTextColor    colRGB(255, 255, 255)

/*=============================================================================
    Data Structures:
=============================================================================*/

typedef struct
{
    sword           nShips;
    bool16          bSelected;
    sdword          shipnum;
    ShipStaticInfo *statinfo;
} ShipListInfo;


typedef struct
{
    regionhandle  region;
    bool32          inlist;
    ShipListInfo *listinfo;
} ShipOverlayInfo;

extern bool32 ioRunning;


/*=============================================================================
    Function Prototypes:
=============================================================================*/

void ioSetSelection(bool32 shiftrelease);
void ioUpdateShipTotals(void);

void ioEnable(void);
bool32 ioDisable(void);

void ioStartup(void);
void ioShutdown(void);
void ioResolutionChange(void);

#endif
