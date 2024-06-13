// =============================================================================
//  HeavyCorvette.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "HeavyCorvette.h"

#include "AIShip.h"
#include "AITrack.h"
#include "Battle.h"
#include "DefaultShip.h"
#include "FastMath.h"
#include "Gun.h"
#include "SoundEvent.h"
#include "Tactics.h"
#include "Universe.h"


#define DEBUG_HEAVY_CORVETTE  0

typedef struct
{
    AttackSideStepParameters sidestepParameters;
} HeavyCorvetteStatics;

HeavyCorvetteStatics HeavyCorvetteStaticRace1;
HeavyCorvetteStatics HeavyCorvetteStaticRace2;

void heavyCleanSpec(Ship *ship);
bool32 doBurstCharging(Ship *ship);
bool32 flytoBurstPosition(Ship *ship);
bool32 doBurstFire(Ship *ship);

void HeavyCorvetteStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    HeavyCorvetteStatics *corvstat = (statinfo->shiprace == R1) ? &HeavyCorvetteStaticRace1 : &HeavyCorvetteStaticRace2;

    memset(corvstat, 0, sizeof(HeavyCorvetteStatics));
    scriptSetStruct(directory,filename,AttackSideStepParametersScriptTable,(ubyte *)&corvstat->sidestepParameters);

    statinfo->custstatinfo = corvstat;
}

void HeavyCorvetteInit(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;

    attackSideStepInit(&spec->attacksidestep);
    heavyCleanSpec(ship);

}

void HeavyCorvetteAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;
    HeavyCorvetteStatics *corvstat = (HeavyCorvetteStatics *)((ShipStaticInfo *)ship->staticinfo)->custstatinfo;

    attackSideStep(ship,target,&spec->attacksidestep,&corvstat->sidestepParameters);
}

void HeavyCorvetteAttackPassive(Ship *ship,Ship *target,bool32 rotate)
{
    if ((rotate) & ((bool32)((ShipStaticInfo *)(ship->staticinfo))->rotateToRetaliate))
    {
        attackPassiveRotate(ship,target);
    }
    else
    {
        attackPassive(ship,target);
    }
}

//State defines for burst attack
#define BURST_Begin             0
#define BURST_FlyToFirePoint    1
#define BURST_Fire              2


bool32 HeavyCorvetteSpecialTarget(Ship *ship, void *custom)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;
    SelectAnyCommand *targets = (SelectAnyCommand *)custom;
    real32 tempreal;
    vector Av;
    sdword i,flag;

    if(spec->cooldown)
    {
        aitrackSteadyShip(ship);
        return FALSE;
    }

    if (targets->numTargets == 0)
    {
        return TRUE;
    }

    switch(spec->burstState)
    {
    case BURST_Begin:
        //get average position of targets
        vecSet(Av,0.0f,0.0f,0.0f);
        for(i=0;i<targets->numTargets;i++)
        {
            vecAddTo(Av,targets->TargetPtr[i]->collInfo.collPosition);
        }
        tempreal = 1.0f/((real32)i);
        vecScalarMultiply(spec->burstFireVector,Av,tempreal);

        ///////////////////////////////
        /////burst attack speech event!
        //event num: COMM_HVette_BurstAttack
        //use battle chatter
        if(ship->playerowner->playerIndex ==
           universe.curPlayerIndex)
        {
            if(ship->playerowner->playerIndex == universe.curPlayerIndex)
            {
                if (battleCanChatterAtThisTime(BCE_COMM_HVette_BurstAttack, ship))
                {
                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_COMM_HVette_BurstAttack, ship, SOUND_EVENT_DEFAULT);
                }
            }
        }

        ///////////////////////////////

        spec->burstState=BURST_FlyToFirePoint;
        break;
    case BURST_FlyToFirePoint:
        flag = 0;
        if(doBurstCharging(ship))
        {
            //ship is fully charged
            flag = 1;
        }
        if(flytoBurstPosition(ship))
        {
            //ship is in position fire
            if(flag == 1)
            {
                spec->burstState=BURST_Fire;
            }
        }
        break;
    case BURST_Fire:
        if(doBurstFire(ship))
        {
            spec->burstState = BURST_Begin;
            dbgMessagef("Firing Burst Fire.");
        }
        break;
    default:
        dbgFatalf(DBG_Loc,"\nUnknown BurstState %d for shiptype %d.",spec->burstState,ship->shiptype);
    }

    //perform burst attack AI here...

    return FALSE;
}

void heavyCleanSpec(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;
    spec->burstState = BURST_Begin;
    spec->burstChargeState = 0.0f;
    spec->burstChargeState2 = 0.0f;
    spec->chargeEffect = NULL;
    spec->cooldown = FALSE;
}

void heavyCorvetteOrderChanged(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;
    spec->burstState = BURST_Begin;
}

//this function charges the ship for burst fire.  Specialized for
//heavy corvette.
//
//Function also creates effect for charging
bool32 doBurstCharging(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;

    if(spec->chargeEffect == NULL)
    {
        //start charging effect if not playing allready

    }

    if(spec->burstChargeState < burstChargeTime)
    {
        spec->burstChargeState += universe.phystimeelapsed;
        return(FALSE);
    }
#if DEBUG_HEAVY_CORVETTE
    dbgMessagef("Burst Fire Charged!");
#endif
    return(TRUE);
}

//this function flies the ship to within range of the desired target location and
//orients itself to fire.  Returns true when in position
bool32 flytoBurstPosition(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;
    vector heading;
    bool32 ready = FALSE;
    //maybe track to within gun gimbleness
    vecSub(heading,spec->burstFireVector,ship->posinfo.position);
    vecNormalize(&heading);
    if(aitrackHeadingWithFlags(ship,&heading,0.99f,AITRACKHEADING_IGNOREUPVEC))
    {
        ready = TRUE;
    }

    if(!MoveReachedDestinationVariable(ship,&spec->burstFireVector,burstRange))
    {
        aishipFlyToPointAvoidingObjs(ship,&spec->burstFireVector,AISHIP_FastAsPossible,0.0f);
        return(FALSE);
    }
    aitrackZeroVelocity(ship);
    return(ready);
}

//fires the bursts for the ship
bool32 doBurstFire(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;
    sdword done;
    vector trajectory,heading;
    real32 range,one_over_range;
    Ship dummyTarg;

    vecSub(trajectory,spec->burstFireVector,ship->posinfo.position);
    range = vecMagnitudeSquared(trajectory);
    range = fsqrt(range);
    one_over_range = 1.0f/range;
    vecScalarMultiply(trajectory,trajectory,one_over_range);

    dummyTarg.objtype = OBJ_ShipType;
    dummyTarg.posinfo.position = spec->burstFireVector;
    dummyTarg.collInfo.collPosition = spec->burstFireVector;
    dummyTarg.currentLOD = ship->currentLOD;
    dummyTarg.collMyBlob = ship->collMyBlob;
    vecSet(dummyTarg.posinfo.velocity,0.0f,0.0f,0.0f);

    //track target even more precisely
    vecSub(heading,spec->burstFireVector,ship->posinfo.position);
    vecNormalize(&heading);
    aitrackHeadingWithFlags(ship,&heading,0.9999f,AITRACKHEADING_IGNOREUPVEC);

    //set special information that needs to be 'transmitted'
    //to the code in gunshoot.

    //fix later
    spec->bulletLifeTime = range*oneOverburstSpeed;

    bitSet(ship->specialFlags,SPECIAL_BurstFiring);
    done = gunShootGunsAtTarget(ship,&dummyTarg,0.0f,&trajectory);
    bitClear(ship->specialFlags,SPECIAL_BurstFiring);
    if(done == TRUE)
    {
        spec->cooldown = TRUE;
        spec->burstChargeState2 = burstCoolDownTime;
    }
    return done;
}

void HeavyCorvetteHouseKeep(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;

    if(spec->cooldown == TRUE)
    {
        spec->burstChargeState2-=universe.phystimeelapsed;
#if DEBUG_HEAVY_CORVETTE
            dbgMessagef("Cooling Down...");
#endif
        if(spec->burstChargeState2 <= 0.0f)
        {
            spec->burstChargeState = 0.0f;
            spec->cooldown = FALSE;
        }
    }
}

void HeavyCorvette_PreFix(Ship *ship)
{
    HeavyCorvetteSpec *spec = (HeavyCorvetteSpec *)ship->ShipSpecifics;

    spec->chargeEffect = NULL;
}


CustShipHeader HeavyCorvetteHeader =
{
    HeavyCorvette,
    sizeof(HeavyCorvetteSpec),
    HeavyCorvetteStaticInit,
    NULL,
    HeavyCorvetteInit,
    NULL,
    HeavyCorvetteAttack,
    DefaultShipFire,
    HeavyCorvetteAttackPassive,
    NULL,
    HeavyCorvetteSpecialTarget,
    HeavyCorvetteHouseKeep,
    NULL,
    NULL,
    HeavyCorvette_PreFix,
    NULL,
    NULL,
    NULL
};

