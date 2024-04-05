// =============================================================================
//  GenericInterceptor.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#ifndef ___GENERICINTERCEPTOR_H
#define ___GENERICINTERCEPTOR_H

#include "ClassDefs.h"
#include "FlightMan.h"
#include "ObjTypes.h"
#include "Types.h"

#define STATE_INIT              0
#define STATE_APPROACH          1
#define STATE_KILL              2
#define STATE_BREAK             3
#define STATE_TURNAROUND        4
#define STATE_REAPPROACH        5

#define STATE_WHIPSTRAFE1       10
#define STATE_WHIPSTRAFE2       11
#define STATE_BARRELROLL_OUT    12
#define STATE_START_BOMBING_RUN 13
#define STATE_BOMBING_DIVE      14

#define isShipInterceptor(ship) (((ShipStaticInfo *)((ship)->staticinfo))->custshipheader.CustShipAttack == GenericInterceptorAttack)
#define isShipStaticInterceptor(shipstatic) ((shipstatic)->custshipheader.CustShipAttack == GenericInterceptorAttack)

typedef struct
{
    real32 firetime;
    real32 maxFlyAwayDist[NUM_TACTICS_TYPES][NUM_CLASSES+1];
    real32 maxFlyAwayDistSquared[NUM_TACTICS_TYPES][NUM_CLASSES+1];    // use NUM_CLASSES entry as DefaultClass (so array len = NUM_CLASSES+1)
    real32 breakRange[NUM_TACTICS_TYPES][NUM_CLASSES+1];
    real32 flyPastDist[NUM_TACTICS_TYPES][NUM_CLASSES+1];
    real32 triggerHappy[NUM_TACTICS_TYPES][NUM_CLASSES+1];
    real32 faceTargetAccuracy[NUM_TACTICS_TYPES][NUM_CLASSES+1];
    real32 maxAttackTime[NUM_TACTICS_TYPES][NUM_CLASSES+1];
    FlightManProb flightmanProb[NUM_TACTICS_TYPES][NUM_CLASSES+1];
} GenericInterceptorStatics;

extern CustShipHeader AttackBomberHeader;
extern CustShipHeader CloakedFighterHeader;
extern CustShipHeader GenericInterceptorHeader;
extern CustShipHeader TargetDroneHeader;

void GenericInterceptorAttack(Ship *ship, SpaceObjRotImpTarg *target, real32 maxdist);
bool32 GenericInterceptorCanFire(Ship *ship, SpaceObjRotImpTarg *target, vector *trajectory, real32 triggerHappy);
FlightManProb *GenericInterceptorGetFlightManProb(Ship *ship, SpaceObjRotImpTarg *target);
bool32 InterceptorInRange(Ship *ship, SpaceObjRotImpTarg *target);
void SpawnCloakingEffect(Ship *ship, etglod *etgLOD);

#endif
