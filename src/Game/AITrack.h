// =============================================================================
//  AITrack.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/25/1997 by gshaw
// =============================================================================

#ifndef ___AITRACK_H
#define ___AITRACK_H

#include "SpaceObj.h"

/*=============================================================================
    Defines:
=============================================================================*/

// Defines for flags in aitrackHeading functions
#define AITRACKHEADING_IGNOREUPVEC      1
#define AITRACKHEADING_DONTROLL         2
#define AITRACK_DONT_ZERO_ME            4

/*=============================================================================
    Functions:
=============================================================================*/

bool32 aitrackIsStabilizeShip(Ship *ship);      // if ship is not rotating, and oriented up
bool32 aitrackIsZeroVelocity(Ship *ship);       // if ship is not drifting
bool32 aitrackIsZeroRotation(Ship *ship);       // if ship is not rotating

bool32 aitrackIsShipSteady(Ship *ship);         // if ship is stabilized, and not drifting
bool32 aitrackSteadyShip(Ship *ship);           // stabilize ship, and stop drifting

// save as above functions but will not necessarily stop the ship oriented up
bool32 aitrackIsShipSteadyAnywhere(Ship *ship);   // if ship is not rotating (any orientation), and not drifting
bool32 aitrackSteadyShipAnywhere(Ship *ship);     // stop ship from rotating (any orientation), and not drifting

bool32 aitrackStabilizeGuidance(SpaceObjRotImpTargGuidance *ship);        // stabilize ship
#define aitrackStabilizeShip(ship) aitrackStabilizeGuidance((SpaceObjRotImpTargGuidance *)ship)

bool32 aitrackZeroRotationAnywhere(Ship *ship);           // stop ship from rotating (any orientation)
bool32 aitrackZeroVelocity(Ship *ship);                   // stop ship from drifting
bool32 aitrackZeroForwardVelocity(Ship *ship);            // stop ship from drifting (forward only)

bool32 aitrackSteadyShipDriftOnly(Ship *ship);

void aitrackForceSteadyShip(Ship *ship);
void aitrackForceGuidanceZeroRotation(SpaceObjRotImpTargGuidance *ship);
#define aitrackForceShipZeroRotation(ship) aitrackForceGuidanceZeroRotation((SpaceObjRotImpTargGuidance *)ship)
void aitrackForceZeroVelocity(Ship *ship);
void aitrackForceHeading(Ship *ship, vector *heading, vector *upvector);

// Note: desiredHeading should be normalized
bool32 aitrackHeadingFunc(SpaceObjRotImpTargGuidance *ship,vector *desiredHeading,real32 accuracy,real32 upaccuracy, real32 sinbank,udword flags);

// Note: desiredHeading, desiredUp should be normalized
bool32 aitrackHeadingAndUp(Ship *ship,vector *desiredHeading,vector *desiredUp,real32 accuracy);

// Note: desiredHeading does not have to be normalized
bool32 aitrackHeadingWithBankPitchFunc(Ship *ship,vector *desiredHeading,real32 accuracy,real32 sinbank,real32 pitchdescend,real32 pitchturn,udword flags);

// Note: desiredRightVector should be normalized
bool32 aitrackRightVector(Ship *ship,vector *desiredRightVector,real32 accuracy);

void aitrackVelocityVectorGuidance(SpaceObjRotImpTargGuidance *ship,vector *desiredVelocity);
#define aitrackVelocityVector(ship,v) aitrackVelocityVectorGuidance((SpaceObjRotImpTargGuidance *)ship,v)

bool32 MoveReachedDestinationVariable(Ship *ship,vector *destination,real32 bounds);

real32 MoveLeftToGo(Ship *ship,vector *destination);

bool32 aitrackRotationSpeed(Ship *ship, real32 rotspeed, uword track);

/*=============================================================================
    Macros
=============================================================================*/

#define aitrackHeading(shp,head,accur) aitrackHeadingFunc((SpaceObjRotImpTargGuidance *)shp,head,accur,accur,0.0f,0)
#define aitrackHeadingWithFlags(shp,head,accur,flag) aitrackHeadingFunc((SpaceObjRotImpTargGuidance *)shp,head,accur,accur, 0.0f,flag)
#define aitrackHeadingWithBank(shp,head,accur,bank) aitrackHeadingFunc((SpaceObjRotImpTargGuidance *)shp,head,accur,accur,bank,0)
#define aitrackHeadingWithBankFlags(shp,head,accur,bank,flag) aitrackHeadingFunc((SpaceObjRotImpTargGuidance *)shp,head,accur,accur,bank,flag)
#define aitrackHeadingWithTwoAccuracies(shp,head,accur,accurup) aitrackHeadingFunc((SpaceObjRotImpTargGuidance *)shp,head,accur,accurup, 0.0f,0)

#define aitrackHeadingWithBankPitch(shp,head,accur,bank,pitchdes,pitchtur) aitrackHeadingWithBankPitchFunc(shp,head,accur,bank,pitchdes,pitchtur,0)
#define aitrackHeadingWithBankPitchFlags(shp,head,accur,bank,pitchdes,pitchtur,flag) aitrackHeadingWithBankPitchFunc(shp,head,accur,bank,pitchdes,pitchtur,flag)

/*=============================================================================
    Globals
=============================================================================*/

extern real32 STILL_ROT_LO;
extern real32 STILL_ROT_HI;

#endif
