// =============================================================================
//  AIFleetMan.h
//  - Controls construction of ships requested from combatman, defenseman, resourceman
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 1998/05/28 by gshaw
// =============================================================================

#ifndef ___AIFLEETMAN_H
#define ___AIFLEETMAN_H

#include "AIPlayer.h"

void aifFleetCommand(void);

void aifInit(AIPlayer *aiplayer);
void aifClose(AIPlayer *aiplayer);

void aifTeamDied(AIPlayer *aiplayer, AITeam *team, bool removeAllReferencesToTeam);

bool aifShipDied(AIPlayer *aiplayer, ShipPtr ship);

void aifResourceManRequestsShipsCB(ShipType shiptype, sdword number, sdword priority);
void aifAttackManRequestsShipsCB(ShipType shiptype, sdword number, sdword priority);
void aifDefenseManRequestsShipsCB(ShipType shiptype, sdword number, sdword priority);

void aifTeamRequestsShipsCB(ShipType shiptype, sdword number, AITeam *team, char *doneSetVar, sdword priority);

void aifHyperspaceInit(AIPlayer *aiplayer);

#endif
