// =============================================================================
//  Alliance.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/31/1998 by ddunlop
// =============================================================================

#ifndef ___ALLIANCE_H
#define ___ALLIANCE_H

#include "Types.h"

// forward declarations
struct ChatPacket;
struct Ship;
struct Player;

// INTERFACE -------------------------------------------------------------------

#define ALLIANCE_FORM_NEW_ALLIANCE   1
#define ALLIANCE_BREAK_ALLIANCE      2

void allianceFormWith(udword playerindex);
void allianceBreakWith(udword playerindex);
void allianceFormRequestRecievedCB(struct ChatPacket *packet);
void allianceSetAlliance(udword AllianceType, uword playerone, uword playertwo);

bool allianceIsShipAlly(struct Ship *ship, struct Player *player);
bool allianceArePlayersAllied(struct Player *playerone, struct Player *playertwo);

void allianceBreakAll(void);

#endif
