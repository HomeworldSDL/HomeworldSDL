// =============================================================================
//  AIShip.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/26/1997 by gshaw
// =============================================================================

#ifndef ___AISHIP_H
#define ___AISHIP_H

#include "SpaceObj.h"

/*=============================================================================
    Defines:
=============================================================================*/

#define AISHIP_FirstPointInDirectionFlying  1
#define AISHIP_PointInDirectionFlying       2
#define AISHIP_FastAsPossible               4
#define AISHIP_CarTurn                      8
#define AISHIP_DontFlyToObscuredPoints      16
#define AISHIP_ReturnImmedIfPointObscured   32
#define AISHIP_IgnoreFormationObscuredPoints    64
#define AISHIP_IgnoreDockWithObscuredPoints     128
#define AISHIP_StopIfVelocityTooLow             256

#define AISHIP_FLY_OK                       0
#define AISHIP_FLY_OBJECT_IN_WAY            1

#define AISHIP_BIGSHIP      1000.0f

/*=============================================================================
    Functions:
=============================================================================*/

void aishipPrecalcInfo(ShipStaticInfo *shipstatinfo);

void aishipGetTrajectory(Ship *ship,SpaceObjRotImpTarg *target,vector *trajectory);
void aishipGetTrajectoryWithVelPrediction(Ship *ship,SpaceObjRotImpTarg *target,real32 bulletspeed,vector *trajectory);
void aishipGetTrajectoryWithAngleCorrection(Ship *ship,SpaceObjRotImpTarg *target,vector *trajectory);

udword aishipFlyToPointAvoidingObjsFunc(Ship *ship,vector *destination,udword aishipflags,real32 limitvel,vector *withVel);

bool32 aishipGuideMissile(Missile *missile);
bool32 aishipGuideMine(Missile *mine);

real32 GetCollSizeInDirection(SpaceObjRotImp *obj,vector dir);

void rowRemoveShipFromGettingOutOfWay(Ship *ship);
void rowFlyShipOutOfWay(Ship *ship);
void rowGetOutOfWayShipDiedCB(Ship *ship);

Ship *getShipNearObjTok(SpaceObjRotImpTarg *obj, real32 range);

/*=============================================================================
    Macros:
=============================================================================*/

#define aishipFlyToPointAvoidingObjs(shp,dst,flags,limitv) aishipFlyToPointAvoidingObjsFunc(shp,dst,flags,limitv,NULL)
#define aishipFlyToShipAvoidingObjs(shp,targ,flags,limitv) aishipFlyToPointAvoidingObjsFunc(shp,&((targ)->posinfo.position),flags,limitv,NULL)

#define aishipFlyToPointAvoidingObjsWithVel(shp,dst,flags,limitv,withvel) aishipFlyToPointAvoidingObjsFunc(shp,dst,flags,limitv,withvel)
#define aishipFlyToShipAvoidingObjsWithVel(shp,targ,flags,limitv,withvel) aishipFlyToPointAvoidingObjsFunc(shp,&((targ)->posinfo.position),flags,limitv,withvel)

/*=============================================================================
    Data:
=============================================================================*/

extern bool32 aishipTempDisableAvoiding;

/*=============================================================================
    Tweakable Constants:
=============================================================================*/

extern real32 FLYSHIP_HEADINGACCURACY;
extern real32 FLYSHIP_ATTACKACCURACY;
extern real32 INTERCEPTORBREAK_TOLERANCE;
extern real32 INTERCEPTORBREAK_MINVELOCITY;
extern real32 INTERCEPTORKILL_MINVELOCITY;

extern real32 AVOID_OBJ_PADDING_SCALE_BIG;
extern real32 AVOID_OBJ_PADDING_SCALE;

#define AISHIP_STATS 0

#if AISHIP_STATS && defined(HW_BUILD_FOR_DEBUGGING)

typedef struct AIshipStats
{
    bool32 isValid;
    bool32 isDisplayable;
    sdword state;
    sdword avoidedWalks;
    sdword avoidedConsidered;
    real32 distanceToCollision;
    bool32 passingOnRight;
    vector passingOnRightVec;
    sdword repulsenum;
    vector repulse;
    vector desiredVel;
    vector desiredHeading;
    vector actualHeading;
} AIshipStats;

extern AIshipStats aishipStats;

void aishipStatsPrint(sdword *y);

void aishipStatsInitFunc(Ship *ship);
void aishipStatsCloseFunc();

#define aishipStatsInit(s)   aishipStatsInitFunc(s)
#define aishipStatsClose()  aishipStatsCloseFunc()
#define aishipStatsState(x) if (aishipStats.isValid) aishipStats.state = (x)
#define aishipStatsAvoidedWalks() if (aishipStats.isValid) aishipStats.avoidedWalks++
#define aishipStatsAvoidedConsidered() if (aishipStats.isValid) aishipStats.avoidedConsidered++
#define aishipStatsDistanceToCollision(x) if (aishipStats.isValid) aishipStats.distanceToCollision = (x)
#define aishipStatsPassingOnRight(v) if (aishipStats.isValid) { aishipStats.passingOnRight = TRUE; aishipStats.passingOnRightVec = (v); }
#define aishipStatsRepulse(v) if (aishipStats.isValid) { aishipStats.repulsenum++; aishipStats.repulse = (v); }
#define aishipStatsDesiredVel(v) if (aishipStats.isValid) aishipStats.desiredVel = (v)
#define aishipStatsDesiredHeading(v) if (aishipStats.isValid) aishipStats.desiredHeading = (v)
#define aishipStatsActualHeading(v) if (aishipStats.isValid) aishipStats.actualHeading = (v)

#else

#define aishipStatsInit(s)
#define aishipStatsClose()
#define aishipStatsState(x)
#define aishipStatsAvoidedWalks()
#define aishipStatsAvoidedConsidered()
#define aishipStatsDistanceToCollision(x)
#define aishipStatsPassingOnRight(v)
#define aishipStatsRepulse(v)
#define aishipStatsDesiredVel(v)
#define aishipStatsDesiredHeading(v)
#define aishipStatsActualHeading(v)

#endif  // AISHIP_STATS


#endif


