// =============================================================================
//  Bounties.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created oct 6th, bpasechn
// =============================================================================

#include "Bounties.h"

#include "MultiplayerGame.h"
#include "Tweak.h"


real32 getPlayerBountyWorthDeterm(real32 shipworth,real32 ruworth,real32 totalshipworth,real32 totalruworth);

//variables of import: tpGameCreated.flag, MG_BountiesEnabled bitmask
//                     tpGameCreated.bountySize

/*-----------------------------------------------------------------------------
    Name        : getPlayerBounty
    Description : returns the 'worth' of a players ships...
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

void BountyInit(sdword bountySettingSize)
{
    //small,med,large
    universe.bountySize = TW_BountySizes[bountySettingSize];
}

sdword getPlayerBountyRender(Player *player)
{
    return(player->bounty);
}
//called ONLY deterministically...
void calculatePlayerBounties(void)
{
    real32 worth[MAX_MULTIPLAYER_PLAYERS];
    real32 shipworth[MAX_MULTIPLAYER_PLAYERS];
    real32 ruworth[MAX_MULTIPLAYER_PLAYERS];
    real32 maxValue;
    real32 totalWorth = 0;
    real32 totalshipworth = 0,totalruworth = 0;
    sdword i;
    udword cap;
    real32 totalhumanruworth = 0.0f;

    if (singlePlayerGame)
    {
        return;
    }

    maxValue = (real32) (universe.numPlayers*universe.bountySize);

    for (i=0;i<universe.numPlayers;i++)
    {
        worth[i] = 0.0f;
        shipworth[i] = 0.0f;
        ruworth[i] = 0.0f;
    }

    // walk shiplist and calculate ship worth for everyone:
    {
        Node *objnode = universe.ShipList.head;
        Ship *ship;
        sdword playerindex;

        while (objnode != NULL)
        {
            ship = (Ship *)listGetStructOfNode(objnode);
            dbgAssertOrIgnore(ship->objtype == OBJ_ShipType);

            if ( ((ship->flags & (SOF_Dead|SOF_Disabled)) == 0) && (ship->shiptype != Drone) )
            {
                playerindex = ship->playerowner->playerIndex;
                if ((playerindex >= 0) && (playerindex < universe.numPlayers))
                {
                    shipworth[playerindex] += ship->staticinfo->buildCost;

                    // also check ships inside it:

                    if ((ship->shipsInsideMe != NULL) && (ship->shiptype != DDDFrigate))
                    {
                        // check ships inside too
                        Node *insidenode = ship->shipsInsideMe->insideList.head;
                        InsideShip *insideship2;

                        while (insidenode != NULL)
                        {
                            insideship2 = (InsideShip *)listGetStructOfNode(insidenode);
                            ship = insideship2->ship;
                            dbgAssertOrIgnore(ship->objtype == OBJ_ShipType);

                            if ( ((ship->flags & (SOF_Dead|SOF_Disabled)) == 0) && (ship->shiptype != Drone) )
                            {
                                playerindex = ship->playerowner->playerIndex;
                                if ((playerindex >= 0) && (playerindex < universe.numPlayers))
                                {
                                    shipworth[playerindex] += ship->staticinfo->buildCost;
                                }
                            }

                            insidenode = insidenode->next;
                        }
                    }
                }
            }

            objnode = objnode->next;
        }
    }

    totalhumanruworth = 0.0f;
    for (i=0;i<tpGameCreated.numPlayers;i++)
    {
        ruworth[i] = (real32)universe.players[i].resourceUnits;
        totalhumanruworth += ruworth[i];
    }

    if (tpGameCreated.numPlayers) totalhumanruworth /= tpGameCreated.numPlayers;

    if (totalhumanruworth)
    for (i=tpGameCreated.numPlayers;i<universe.numPlayers;i++)
    {
        ruworth[i] = totalhumanruworth;     // assume cmpt players ru worth is equal to avg of human players
    }


    for (i=0;i<universe.numPlayers;i++)
    {
        totalshipworth += shipworth[i];
        totalruworth += ruworth[i];
    }

    // we now have shipworth and ruworth for each player.  Let's calculate worth

    for(i=0;i<universe.numPlayers;i++)
    {
        worth[i] = getPlayerBountyWorthDeterm(shipworth[i],ruworth[i],totalshipworth,totalruworth);
        totalWorth += worth[i];
    }

    for(i=0;i<universe.numPlayers;i++)
    {
        cap = (udword)((worth[i]/totalWorth)*maxValue);

        if (cap > 250) cap = 250;

        universe.players[i].bounty = (ubyte)cap;
    }
}


/*-----------------------------------------------------------------------------
    Name        : getPlayerBountyWorthDeterm
    Description : Calculates an arbitrary number based on many different factors
                  of how a player is doing
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

//skewing values...player is assumed to have this many before actual statistics start taking over
#define BOUNTY_MIN_RESOURCE_SKEW            2000.0f
#define BOUNTY_MIN_RESOURCES_POSSESION_SKEW 2000.0f

//weight multipliers...each category is multiplied by these multipliers
//to give more importance to one particular topic
#define I_resourcesCollected    TW_Bounty_ResourcesCollected;
#define I_resourcesInPossesion  TW_Bounty_ResourcesInPossesion;
#define I_currentShipWorth      TW_Bounty_CurrentShipWorth;
#define I_AllyStrength          TW_Bounty_AllyStrength;
#define I_ResourcesKilled       TW_Bounty_ResourcesKilled;

real32 getPlayerBountyWorthDeterm(real32 shipworth,real32 ruworth,real32 totalshipworth,real32 totalruworth)
{
    real32 playerCurrentShipWorth;
    real32 playerRUWorth;
    real32 worth;

    if(totalshipworth == 0.0f)
    {
        playerCurrentShipWorth = 0.0f;
    }
    else
    {
        playerCurrentShipWorth = shipworth / totalshipworth;
        playerCurrentShipWorth *= I_currentShipWorth;
    }

    if (totalruworth == 0.0f)
    {
        playerRUWorth = 0.0f;
    }
    else
    {
        playerRUWorth = ruworth;
        playerRUWorth *=  TW_Bounty_ResourcesInPossesion/totalruworth;
    }

    worth = playerCurrentShipWorth + playerRUWorth;

    return worth;
}

/*-----------------------------------------------------------------------------
    Name        : getBountyOnShip
    Description : returns the RU worth of this ship.
                  value is based on the getPlayerBounty function
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
sdword getBountyOnShip(Ship *ship)
{
    //return buildcost times bounty multiplier.
    return((sdword)(ship->staticinfo->buildCost*((((real32)ship->playerowner->bounty)/100.0f))));
}

/*-----------------------------------------------------------------------------
    Name        : bountyShipWasKilled
    Description : deals with a ship that died in the case of bounties
                  This function gets called when a ship is KILLED by something
                  (via apply damage to target)
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void bountyShipWasKilled(Ship *ship)
{
    if((!singlePlayerGame) && (tpGameCreated.bountySize != MG_BountiesOff))
    {
        sdword bounty;
        //bounties enabled!!!
        switch(ship->howDidIDie)
        {
        case DEATH_Killed_By_Player:
        case DEATH_Killed_By_Player_Explosion:
        case DEATH_Killed_By_Kamikaze:
#ifdef HW_BUILD_FOR_DEBUGGING
            dbgAssertOrIgnore(ship->whoKilledMe != 99);
#endif
            //reimburse player for their troubles
            if(ship->playerowner->playerIndex != ship->whoKilledMe)
            {
                //make sure it was NOT the ship owner who killed the ship
                bounty = getBountyOnShip(ship);
                universe.players[ship->whoKilledMe].resourceUnits += bounty;
                //update statistic
                universe.gameStats.playerStats[ship->whoKilledMe].totalResourceUnitsViaBounties += bounty;
                universe.gameStats.playerStats[ship->whoKilledMe].totalResourceUnits += bounty;
            }
            break;
        }
    }
}

