
#ifndef ___RESCOLLECT_H
#define ___RESCOLLECT_H

#include "Attributes.h"
#include "SpaceObj.h"

typedef struct
{
    ResourcePtr resource;
    real32 resourceVolumeSize;      // 0 indicates no volume
    vector resourceVolumePosition;  // position of volume
} CollectResourceCommand;

#define UNINITIALIZED_RESOURCE_COMMAND  {NULL, 0.0, VECTOR_ORIGIN}

struct CommandLayer;

void ChangeSingleShipToCollectResource(struct CommandToDo *command);

// returns TRUE if resource is already being targeted for harvesting
bool32 ResourceAlreadyBeingHarvested(struct CommandLayer *comlayer,struct CommandToDo *IAmThisCommand,Resource *resource);

// returns TRUE if resource is moving too fast to be harvested
bool32 ResourceMovingTooFast(Resource *resource);

bool32 processCollectResource(struct CommandToDo *collecttodo);

void TurnHarvestEffectOff(Ship *ship);

void TurnOffAnyResourcingEffects(struct CommandToDo *todo);

bool32 AsteroidTakesDamage(Asteroid *asteroid,sdword damagetaken,bool32 targetWasAlive);

bool32 DustCloudTakesDamage(DustCloud* dustcloud, sdword damagetaken, bool32 targetWasAlive);

void DustCloudChargesUp(DustCloud* dustcloud, sdword damagetaken, bool32 targetWasAlive);

void BreakAsteroidUp(Asteroid *asteroid);

void HandleDustCloudScaling(DustCloud* dustcloud);

void PreFix_ShipXHarvestsResourceY(Ship *ship);
void Fix_ShipXHarvestsResourceY(Ship *ship);

void R1ResourcerAttacksShip(struct Ship *ship,struct SpaceObjRotImpTarg *target,bool32 passiveAttacking);
void removeResourcerFromAttacking(Ship *ship);

extern real32 ASTEROIDBREAK_LO_ROTATE;
extern real32 ASTEROIDBREAK_HI_ROTATE;

extern real32 ASTEROID_LO_ROTATE[NUM_ASTEROIDTYPES];
extern real32 ASTEROID_HI_ROTATE[NUM_ASTEROIDTYPES];

extern udword ASTEROID_ROTATE_PROB;

extern real32 DUSTCLOUD_RCOLLECTOR_BLOWUP_SCALE;

// regrow resources stuff
extern udword REGROW_RESOURCES_CHECK_RATE;
extern udword REGROW_RESOURCES_CHECK_FRAME;
extern real32 REGROW_RATE_MODIFIER[REGROWRATE_Max+1];

extern sdword regenerateRUrate;

extern sdword regenerateRUsIfUnder;
extern sdword regenerateRUs;

extern udword CHECK_HARV_OUTOFWORLD_RESOURCE_RATE;
extern udword CHECK_SHIP_OUTOFWORLD_RATE;

#endif

