/*============================================================================= 
    Name    : ShipView.h
    Purpose : Renders a specific ship to a window

    Created 7/27/1998 by pgrant
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___SHIPVIEW_H
#define ___SHIPVIEW_H

#include "FEFlow.h"
#include "Region.h"
#include "ShipDefs.h"

void svStartup(void);
void svShutdown(void);

void svSelectShip(ShipType type);
void svClose(void);

void svDirtyShipView(void);

void svShipViewRender(featom* atom, regionhandle region);

#endif
