/*=============================================================================
    Name    : TradeMgr.h
    Purpose : Definitions for the trade manager.

    Created 10/07/1997 by yo
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___TRADEMGR_H
#define ___TRADEMGR_H

#include "FEFlow.h"
#include "Region.h"
#include "SpaceObj.h"
#include "Types.h"

/*=============================================================================
    Switches:
=============================================================================*/
#define TM_NUM_TECHS 50


//for tmTechForSale[]
#define TM_TECH_IS_NOT_FOR_SALE         0
#define TM_TECH_IS_FOR_SALE             1
#define TM_TECH_IS_ALREADY_OWNED        2

//for tmTechResearchable[]
#define TM_TECH_IS_NOT_RESEARCHABLE     0
#define TM_TECH_IS_RESEARCHABLE         1



/*=============================================================================
    Type definitions:
=============================================================================*/

typedef sword TechType;


//ships available

typedef enum {
    DialogWelcome=0,
    DialogFirstClick,
    DialogCantAffordThat,
    DialogCantAffordAnything,
    DialogPurchaseMade,
    DialogNothingForSale,   //maybe implemented
} TradeDialogType;


/*=============================================================================
    Data:
=============================================================================*/


extern udword tmTechForSale[TM_NUM_TECHS];
extern bool32 tmCheaptechs;
extern sword tmTechResearchable[TM_NUM_TECHS];



/*=============================================================================
    Functions:
=============================================================================*/
void tmStartup(void);
void tmShutdown(void);

void tmTechInit(void);
void tmReset(void);
void tmClearTechs(void);


sdword tmTradeBegin(regionhandle region, sdword ID, udword event, udword data);
void tmLeave(char *string, featom *atom);

void tmCostListDraw(featom *atom, regionhandle region);
void tmDialogDraw(featom *atom, regionhandle region);
void tmTechInfoDraw(featom *atom, regionhandle region);




void AllowPlayerToResearch(char *techname);
void AllowPlayerToPurchase(char *techname);
void PlayerAcquiredTechnology(char *techname);
sdword CanPlayerResearch(char *techname);
sdword CanPlayerPurchase(char *techname);
sdword DoesPlayerHave(char *techname);
sdword GetBaseTechnologyCost(char *techname);
void SetBaseTechnologyCost(char *techname, sdword cost);



void tmEnableTraderGUI(void);
bool32 tmTraderGUIActive(void);
void tmSetDialog(sdword phrasenum, char *sentence);
void tmSetPriceScale(udword percent);
uword tmGetPriceScale(void);
void tmSetTradeDisabled(bool32 trade);

// Save Game stuff
void tmSave(void);
void tmLoad(void);





#define WK_MAX_SHIPS 100




typedef struct
{

    Ship           *ship;
    real32          x;
    real32          y;
    real32          vx;
    real32          vy;
    real32          ang;
    real32          vang;
    real32          vangacc;
    real32          vangmax;
    real32          acc;
    real32          maxvel;
    real32          revacc;
    real32          strafeacc;
    sword           controlthrust;
    sword           controlrot;
    sword           controlstrafe;
    sword           controlfire;
}
wkTradeType;

extern wkTradeType wkTradeShips[WK_MAX_SHIPS];
extern bool32 wkTradeStuffActive;
void mrTradeStuffTest(sdword *a, sdword *b);
void wkTradeUpdate(void);


#endif
