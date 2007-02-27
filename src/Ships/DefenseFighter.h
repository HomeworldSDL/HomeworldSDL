// =============================================================================
//  DefenseFighter.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 1/27/1998 by Bryce Pasechnik
// =============================================================================

#ifndef ___DEFENSEFIGHTER_H
#define ___DEFENSEFIGHTER_H

#include "FlightMan.h"
#include "Types.h"

typedef struct
{
    udword NumTargetsCanAttack;
    real32 CoolDownTimePerLaser;
    real32 DamageReductionLow;
    real32 DamageReductionHigh;
    udword DamageRate;
    udword RangeCheckRate;
    sdword TargetOwnBullets;
    sdword MultipleTargettingofSingleBullet;
    real32 max_rot_speed;
    real32 rotate_time;
    real32 rotate_recover_time;
    FlightManProb flightmanProb[NUM_TACTICS_TYPES][NUM_CLASSES+1];
} DefenseFighterStatics;

extern CustShipHeader DefenseFighterHeader;

void defenseFighterAdjustLaser(Bullet *bullet);
void DefenseFighterBulletRemoval(Bullet *bullettogoByeBye);
void DefenseFighterReportBullet(Ship *ship, Bullet *bullet);

#endif
