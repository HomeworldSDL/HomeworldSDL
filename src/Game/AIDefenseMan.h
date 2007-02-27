/*=============================================================================
    Name    : AIDefenseMan.h
    Purpose : Definitions for AIDefenseMan

    Created 1998/05/28 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___AIDEFENSEMAN_H
#define ___AIDEFENSEMAN_H

#include "AIPlayer.h"

void aidDefenseManager(void);
bool aidShipDied(AIPlayer *aiplayer, ShipPtr ship);
void aidTeamDied(AIPlayer *aiplayer, AITeam *team);

void aidInit(AIPlayer *aiplayer);
void aidClose(AIPlayer *aiplayer);

void aidSendDistressSignal(SelectCommand *ships);
void aidClearDistressSignal(AIPlayer *aiplayer);

#endif

