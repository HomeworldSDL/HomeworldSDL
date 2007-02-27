/*=============================================================================
    Name    : bounties.h
    Purpose : bounty defines

    Created oct 6th, bpasechn
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___BOUNTIES_H
#define ___BOUNTIES_H

#include "Universe.h"

void BountyInit(sdword bountySettingSize);
sdword getPlayerBountyRender(Player *player);
void calculatePlayerBounties();                   //updates all players 'bounty' variable

sdword getBountyOnShip(Ship *ship);
void bountyShipWasKilled(Ship *ship);

#endif
