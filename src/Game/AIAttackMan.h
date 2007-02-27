/*=============================================================================
    Name    : AIAttackMan.h
    Purpose : Definitions for AIAttackMan

    Created 1998/05/28 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___AIATTACKMAN_H
#define ___AIATTACKMAN_H

#include "AIPlayer.h"
#include "AITeam.h"

AITeam *aiaSendRecon(SelectCommand *ships);
void aiaAttackManager(void);
void aiaProcessSpecialTeams(void);
bool aiaShipDied(AIPlayer *aiplayer, ShipPtr ship);
void aiaTeamDied(AIPlayer *aiplayer, AITeam *team);

void aiaP2AttackManager(void);

void aiaInit(AIPlayer *aiplayer);
void aiaClose(AIPlayer *aiplayer);

#endif

