// =============================================================================
//  AIResourceMan.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/27/1998 by gshaw
// =============================================================================

#ifndef ___AIRESOURCEMAN_H
#define ___AIRESOURCEMAN_H

#include "AIPlayer.h"

void airResourceManager(void);

void airInit(AIPlayer *aiplayer);
void airClose(AIPlayer *aiplayer);

bool32 airShipDied(AIPlayer *aiplayer, ShipPtr ship);
void airTeamDied(AIPlayer *aiplayer, AITeam *team);

sdword NumberOfEasilyAccesibleRUs(Player *player);
sdword NumRUDockPointsOnThisShip(Ship *ship);

void airAddNewShip(Ship *ship);
void airProcessSpecialTeams(void);

#endif

