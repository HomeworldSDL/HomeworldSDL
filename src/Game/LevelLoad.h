
#ifndef ___LEVELLOAD_H
#define ___LEVELLOAD_H

#include "ObjTypes.h"
#include "ShipDefs.h"
#include "Universe.h"

/*=============================================================================
    Functions:
=============================================================================*/

void levelPreInit(char *directory,char *pickedMission);
void levelInit(char *directory,char *pickedMission);

void levelStartNext(char *directory,char *pickedMission);        // for single player game only

// utility functions
ShipType GetAppropriateShipTypeForRace(ShipType request,ShipRace shiprace);
void TryToFindMothershipsForPlayers(void);
ShipRace GetSinglePlayerRaceEquivalent(ShipRace race);

bool32 AddResourceToSphere(ResourceVolume *sphere,bool32 regrowing);
bool32 AddResourceToCylinder(ResourceVolume *cylinder,bool32 regrowing);
bool32 AddResourceToRectangle(ResourceVolume *rectangle,bool32 regrowing);

void SetInfoNeededForShipAndRelatedStaticInfo(ShipType type,ShipRace race,bool8 dataToFillIn);

/*=============================================================================
    Global variables:
=============================================================================*/

extern udword nebAttributes;
extern sdword SongNumber;

#endif

