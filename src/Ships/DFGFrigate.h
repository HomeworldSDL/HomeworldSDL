// =============================================================================
//  DFGFrigate.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#ifndef ___DFGFRIGATE_H
#define ___DFGFRIGATE_H

#include "SpaceObj.h"

typedef struct
{
    real32 DFGFrigateFieldRadius;     //DFG's Effectie Field Radius
    real32 DFGFrigateFieldRadiusSqr;  //DFG's Effectie Field Radius Squared
    real32 BulletDamageMultiplier;    //Bullets damage is multiplied by this factor
    real32 BulletLifeExtension;       //Bullets Life is Extended by this ammount (Set high to ensure collision)
    udword percentageThatGetThrough;  //percetage of bullets that make it through field
    udword angleVariance;
} DFGFrigateStatics;

extern CustShipHeader DFGFrigateHeader;

void univDFGFieldEffect(Ship *ship, Bullet *bullet, real32 totaltimeelapsed);

#endif
