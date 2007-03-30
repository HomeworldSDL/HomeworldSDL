// =============================================================================
//  P2MultiBeamFrigate.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/07/1998 by ddunlop
// =============================================================================

#include "P2MultiBeamFrigate.h"

#include "AIShip.h"
#include "AITrack.h"
#include "Collision.h"
#include "Gun.h"
#include "StatScript.h"
#include "Universe.h"

#define STATE_INIT          0
#define STATE_APPROACH      1
#define STATE_SPINUP        2
#define STATE_FIRE          3
#define STATE_SPINDOWN      4

//#define ROTSPEED_FACTOR     20
//#define ROTACCEL_FACTOR     100

typedef struct
{
    real32 aiattacklast;        // time stamp for spin up and down time count
    real32 aifirestarttime;     // time stamp for start of firing
    bool   spining;
    bool   steady;
} P2MultiBeamFrigateSpec;

typedef struct
{
    real32 MBRstat;
    real32 MultiBeamRange[NUM_TACTICS_TYPES];
    real32 AttackRotationSpeed;
    real32 BeamFireTime;
	real32 fireDownTime;
} P2MultiBeamFrigateStatics;

P2MultiBeamFrigateStatics P2MultiBeamFrigateStatic;

void P2MultiBeamFrigateAttackDoAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist,bool PassiveAttack);
void P2MultiBeamFrigateFire(Ship *ship,SpaceObjRotImpTarg *target);

scriptStructEntry P2MultiBeamFrigateScriptTable[] =
{
    { "MultiBeamRange"          , scriptSetReal32CB     ,  &(P2MultiBeamFrigateStatic.MBRstat)       ,  &(P2MultiBeamFrigateStatic) },
    { "AttackRotationSpeed"     , scriptSetReal32CB     , &(P2MultiBeamFrigateStatic.AttackRotationSpeed)   ,  &(P2MultiBeamFrigateStatic) },
    { "BeamFireTime"            , scriptSetReal32CB     , &(P2MultiBeamFrigateStatic.BeamFireTime)          ,  &(P2MultiBeamFrigateStatic) },
	{ "fireDownTime"            , scriptSetReal32CB     , &(P2MultiBeamFrigateStatic.fireDownTime)          ,  &(P2MultiBeamFrigateStatic) },

//    {"MultiBeamRange", scriptsetReal32CB, &(P2MultiBeamFrigateStatic.MultiBeamRange)},
//    {"MultiBeamRange", scriptsetReal32CB, &(P2MultiBeamFrigateStatic.MultiBeamRange)},

    END_SCRIPT_STRUCT_ENTRY
};

void P2MultiBeamFrigateStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    P2MultiBeamFrigateStatics *frigstat = &P2MultiBeamFrigateStatic;

    statinfo->custstatinfo = frigstat;

    scriptSetStruct(directory, filename, P2MultiBeamFrigateScriptTable, (ubyte *)frigstat);

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        frigstat->MultiBeamRange[i] = frigstat->MBRstat * statinfo->bulletRange[i]; // multiply by gun range to get real beam range
    }
}

void P2MultiBeamFrigateInit(Ship *ship)
{
    P2MultiBeamFrigateSpec    *spec     = (P2MultiBeamFrigateSpec *)ship->ShipSpecifics;

    ship->aistateattack = STATE_INIT;
    spec->spining = FALSE;
}

void P2MultiBeamFrigateAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
	P2MultiBeamFrigateAttackDoAttack(ship,target,maxdist,FALSE);
}

void P2MultiBeamFrigateAttackDoAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist,bool PassiveAttack)

{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;

    P2MultiBeamFrigateStatics *frigstat = (P2MultiBeamFrigateStatics *)shipstaticinfo->custstatinfo;
    P2MultiBeamFrigateSpec    *spec     = (P2MultiBeamFrigateSpec *)ship->ShipSpecifics;

    vector trajectory;
    vector shipRightVec;
    real32 range;

        spec->aiattacklast = universe.totaltimeelapsed;
    ship->autostabilizeship = FALSE;

    aishipGetTrajectory(ship, target, &trajectory);
    range = RangeToTarget(ship,target,&trajectory);

    if (range > frigstat->MultiBeamRange[ship->tacticstype])
    {
        //not in range
        if(!PassiveAttack)
        {
            //do aiship fly to stuff here

            //if not in range...fly to the ship
            //else track zero velocity
                aishipFlyToShipAvoidingObjs(ship,target,AISHIP_PointInDirectionFlying + AISHIP_CarTurn,0.0f);
        }
        return;
    }

    //we are in range

    vecNormalize(&trajectory);
    if(!PassiveAttack)
	{
		aitrackZeroVelocity(ship);
	}
    matGetVectFromMatrixCol2(shipRightVec,ship->rotinfo.coordsys);

    aitrackHeadingAndUp(ship,&trajectory,&shipRightVec,0.9999f);

    
	if(vecMagnitudeSquared(ship->rotinfo.rotspeed) > frigstat->AttackRotationSpeed)
    {
		if(aitrackHeadingWithFlags(ship,&trajectory,0.99f,AITRACKHEADING_IGNOREUPVEC|AITRACK_DONT_ZERO_ME))
		{
			//rotation speed reached
			//lets fire guns if we can
			P2MultiBeamFrigateFire(ship,target);
		}
    }
}

void P2MultiBeamFrigateFire(Ship *ship,SpaceObjRotImpTarg *target)
{
    GunInfo *guninfo = ship->gunInfo;
    sdword numGuns;
    sdword i;
    Gun *gun;

    if ((guninfo == NULL)||(target == NULL))
    {
        return;
    }

    numGuns = guninfo->numGuns;

    for (i=0;i<numGuns;i++)
    {
        gun = &guninfo->guns[i];
        if (gunCanShoot(ship, gun))
            if (gun->gunstatic->guntype==GUN_Gimble)
            {
                gunOrientGimbleGun(ship,gun,target);
                gunShoot(ship,gun,target);
            }
    }
}

void P2MultiBeamFrigateAttackPassive(Ship *ship,Ship *target,bool rotate)
{
    ShipStaticInfo            *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    P2MultiBeamFrigateSpec    *spec     = (P2MultiBeamFrigateSpec *)ship->ShipSpecifics;
    P2MultiBeamFrigateStatics *frigstat = (P2MultiBeamFrigateStatics *)shipstaticinfo->custstatinfo;
    vector trajectory;
    vector heading, temp;
    real32 range;

	P2MultiBeamFrigateAttackDoAttack(ship,(SpaceObjRotImpTarg *)target,1000.0f,TRUE);

    return;
	aishipGetTrajectory(ship, (SpaceObjRotImpTarg *)target, &trajectory);

    range = RangeToTarget(ship,(SpaceObjRotImpTarg *)target,&trajectory);

    if ( (range < frigstat->MultiBeamRange[ship->tacticstype]) && (rotate) )
    {
        temp = trajectory;
        temp.z+=5;
        temp.y+=5;
        temp.x+=5;

        vecCrossProduct(heading,trajectory,temp);

        vecNormalize(&heading);
        vecNormalize(&trajectory);

        if (aitrackHeadingAndUp(ship,&heading,&trajectory,0.99f))
        {
            if (!spec->steady) aitrackForceHeading(ship,&heading,&trajectory);
            spec->steady = TRUE;
        }
        else
            spec->steady = FALSE;
    }

    shipstaticinfo->custshipheader.CustShipFire(ship, (SpaceObjRotImpTarg *)target);
}

void P2MultiBeamFrigateHouseKeep(Ship *ship)
{
/*
    P2MultiBeamFrigateSpec *spec = (P2MultiBeamFrigateSpec *)ship->ShipSpecifics;
    ShipStaticInfo         *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    real32 desiredrotspeed;
    real32 rotspeed;

    if (spec->spining)
    {
        if (universe.totaltimeelapsed - spec->aiattacklast > 0.15)
        {
            desiredrotspeed = 0;
            rotspeed = ship->rotinfo.rotspeed.z;

            if (rotspeed > 0.2)
                aitrackRotationSpeed(ship,desiredrotspeed,ROT_ABOUTZCCW);
            else
                if (aitrackStabilizeShip(ship))
                {
                    spec->spining = FALSE;
                    aitrackForceShipZeroRotation(ship);
                    ship->aistateattack = STATE_INIT;
                }
        }
    }
*/
}

CustShipHeader P2MultiBeamFrigateHeader =
{
    P2MultiBeamFrigate,
    sizeof(P2MultiBeamFrigateSpec),
    P2MultiBeamFrigateStaticInit,
    NULL,
    P2MultiBeamFrigateInit,
    NULL,
    P2MultiBeamFrigateAttack,
//    DefaultShipFire,
    P2MultiBeamFrigateFire,
    P2MultiBeamFrigateAttackPassive,
    NULL,
    NULL,
    P2MultiBeamFrigateHouseKeep,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

