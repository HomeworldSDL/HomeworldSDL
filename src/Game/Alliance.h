// =============================================================================
//  Alliance.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/31/1998 by ddunlop
// =============================================================================

#ifndef ___ALLIANCE_H
#define ___ALLIANCE_H

#include "CommandNetwork.h"
#include "Types.h"
#include "Universe.h"

// INTERFACE -------------------------------------------------------------------

#define ALLIANCE_FORM_NEW_ALLIANCE   1
#define ALLIANCE_BREAK_ALLIANCE      2

void allianceFormWith(udword playerindex);
void allianceBreakWith(udword playerindex);
void allianceFormRequestRecievedCB(ChatPacket *packet);
void allianceSetAlliance(udword AllianceType, uword playerone, uword playertwo);

bool allianceIsShipAlly(Ship *ship, Player *player);
bool allianceArePlayersAllied(Player *playerone, Player *playertwo);

void allianceBreakAll(void);

#endif
