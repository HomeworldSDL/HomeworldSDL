// =============================================================================
//  P1MissileCorvette.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/06/1998 by ddunlop
// =============================================================================

#include "P1MissileCorvette.h"

#include "Attack.h"
#include "Battle.h"
#include "DefaultShip.h"
#include "Gun.h"
#include "SoundEvent.h"
#include "StatScript.h"
#include "Universe.h"

#define VOLLEY_BEGIN 10
#define VOLLEY_NOT_BEGIN 20

typedef struct
{
    AttackSideStep attacksidestep;
    real32 lasttimeRegeneratedMissiles;
    real32 lasttimeFiredVolley;
    real32 lasttimeDidSpecialTargeting;
    sdword curTargetIndex;
    sdword volleyState;
} P1MissileCorvetteSpec;

typedef struct
{
    real32 missilecorvetteGunRange[NUM_TACTICS_TYPES];
    real32 missilecorvetteTooCloseRange[NUM_TACTICS_TYPES];
    real32 missileRegenerateTime;
    real32 missileVolleyTime;
    real32 missileLagVolleyTime;
    AttackSideStepParameters sidestepParameters;
} P1MissileCorvetteStatics;

P1MissileCorvetteStatics P1MissileCorvetteStatic;

scriptStructEntry P1MissileCorvetteScriptTable[] =
{
    { "MissileRegenerateTime",scriptSetReal32CB, &(P1MissileCorvetteStatic.missileRegenerateTime), &(P1MissileCorvetteStatic) },
    { "MissileVolleyTime",scriptSetReal32CB, &(P1MissileCorvetteStatic.missileVolleyTime), &(P1MissileCorvetteStatic) },
    { "MissileLagVolleyTime",scriptSetReal32CB, &(P1MissileCorvetteStatic.missileLagVolleyTime), &(P1MissileCorvetteStatic) },

    END_SCRIPT_STRUCT_ENTRY
};

void P1MissileCorvetteStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    P1MissileCorvetteStatics *corvstat = &P1MissileCorvetteStatic;

    memset(corvstat, 0, sizeof(P1MissileCorvetteStatics));

    scriptSetStruct(directory,filename,AttackSideStepParametersScriptTable,(ubyte *)&corvstat->sidestepParameters);
    scriptSetStruct(directory,filename,P1MissileCorvetteScriptTable,(ubyte *)corvstat);

    statinfo->custstatinfo = corvstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        corvstat->missilecorvetteGunRange[i] = statinfo->bulletRange[i];
        corvstat->missilecorvetteTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }
}

void P1MissileCorvetteInit(Ship *ship)
{
    P1MissileCorvetteSpec *spec = (P1MissileCorvetteSpec *)ship->ShipSpecifics;

    attackSideStepInit(&spec->attacksidestep);

    spec->lasttimeRegeneratedMissiles = 0.0f;
    spec->lasttimeFiredVolley = 0.0f;
    spec->lasttimeDidSpecialTargeting = -100000.0f;
    spec->curTargetIndex = 0;
    spec->volleyState = VOLLEY_BEGIN;
}

void P1MissileCorvetteAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    P1MissileCorvetteSpec *spec = (P1MissileCorvetteSpec *)ship->ShipSpecifics;
    P1MissileCorvetteStatics *corvstat = (P1MissileCorvetteStatics *)((ShipStaticInfo *)ship->staticinfo)->custstatinfo;

    attackSideStep(ship,target,&spec->attacksidestep,&corvstat->sidestepParameters);
}

void P1MissileCorvetteAttackPassive(Ship *ship,Ship *target,bool rotate)
{
    if ((rotate) & ((bool)((ShipStaticInfo *)(ship->staticinfo))->rotateToRetaliate))
    {
        attackPassiveRotate(ship,target);
    }
    else
    {
        attackPassive(ship,target);
    }
}

void P1MissileCorvetteHousekeep(Ship *ship)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    P1MissileCorvetteSpec *spec = (P1MissileCorvetteSpec *)ship->ShipSpecifics;
    P1MissileCorvetteStatics *mcorvettestat = (P1MissileCorvetteStatics *)shipstaticinfo->custstatinfo;
    sdword numGuns;
    sdword i;
    Gun *gun;
    GunStatic *gunstatic;
    sdword minMissiles;
    sdword minIndex;

    if ((universe.totaltimeelapsed - spec->lasttimeDidSpecialTargeting) > mcorvettestat->missileLagVolleyTime)
    {
        if ((universe.totaltimeelapsed - spec->lasttimeRegeneratedMissiles) > mcorvettestat->missileRegenerateTime)
        {
            spec->lasttimeRegeneratedMissiles = universe.totaltimeelapsed;
            numGuns = ship->gunInfo->numGuns;

            // find missile launcher with least missiles, and give it a missile
            for (i=0,minIndex=-1,minMissiles=100000;i<numGuns;i++)
            {
                gunstatic = &shipstaticinfo->gunStaticInfo->gunstatics[i];
                if (gunstatic->guntype == GUN_MissileLauncher)
                {
                    gun = &ship->gunInfo->guns[i];
                    if (gun->numMissiles < gunstatic->maxMissiles)
                    {
                        if (gun->numMissiles < minMissiles)
                        {
                            minMissiles = gun->numMissiles;
                            minIndex = i;
                        }
                    }
                }
            }

            if (minIndex != -1)
            {
                ship->gunInfo->guns[minIndex].numMissiles++;
            }
        }
    }
}

bool P1MissileCorvetteSpecialTarget(Ship *ship,void *custom)
{
    SelectAnyCommand *targets;
    sdword i;
    sdword numShipsToTarget;
    sdword numGuns;
    Gun *gun;
    GunStatic *gunstatic;
    SpaceObjRotImpTarg *target;
    ShipStaticInfo *shipstaticinfo;
    P1MissileCorvetteSpec *spec;
    P1MissileCorvetteStatics *mcorvettestat;
    bool firedSomeMissiles;

    spec = (P1MissileCorvetteSpec *)ship->ShipSpecifics;
    shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    mcorvettestat = (P1MissileCorvetteStatics *)shipstaticinfo->custstatinfo;

    spec->lasttimeDidSpecialTargeting = universe.totaltimeelapsed;

    // check that the "volley reload time" has passed
    if ((universe.totaltimeelapsed - spec->lasttimeFiredVolley) > mcorvettestat->missileVolleyTime)
    {
        spec->lasttimeFiredVolley = universe.totaltimeelapsed;
    }
    else
    {
        return FALSE;
    }

    targets = (SelectAnyCommand *)custom;
    numShipsToTarget = targets->numTargets;

    if (numShipsToTarget == 0)
    {
        // have destroyed targets, so we are done
        spec->curTargetIndex = 0;
        spec->volleyState = VOLLEY_BEGIN;
        return TRUE;
    }

    if(spec->volleyState == VOLLEY_BEGIN)
    {
        spec->volleyState = VOLLEY_NOT_BEGIN;
        //////////////////////
        //Volley attack speech event!
        //event num: COMM_MissleDest_VolleyAttack
        //use battle chatter
        if(ship->playerowner->playerIndex == universe.curPlayerIndex)
        {
            if (battleCanChatterAtThisTime(BCE_COMM_MissleDest_VolleyAttack, ship))
            {
                battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_COMM_MissleDest_VolleyAttack, ship, SOUND_EVENT_DEFAULT);
            }
        }
        //////////////////////
    }

    if (spec->curTargetIndex >= numShipsToTarget)
    {
        spec->curTargetIndex = 0;
    }

    numGuns = ship->gunInfo->numGuns;
    firedSomeMissiles = FALSE;
    for (i=0;i<numGuns;i++)
    {
        gunstatic = &shipstaticinfo->gunStaticInfo->gunstatics[i];
        if (gunstatic->guntype == GUN_MissileLauncher)
        {
            gun = &ship->gunInfo->guns[i];
            if (gunHasMissiles(gun))
            {
                firedSomeMissiles = TRUE;

                target = targets->TargetPtr[spec->curTargetIndex];

                missileShoot(ship,gun,target);

                spec->curTargetIndex++;
                if (spec->curTargetIndex >= numShipsToTarget)
                {
                    spec->curTargetIndex = 0;
                }
            }
        }
    }

    if (firedSomeMissiles)
    {
        return FALSE;
    }
    else
    {
        // all empty of missiles, so we are done
        spec->curTargetIndex = 0;
        spec->volleyState = VOLLEY_BEGIN;
        return TRUE;
    }
}

CustShipHeader P1MissileCorvetteHeader =
{
    P1MissileCorvette,
    sizeof(P1MissileCorvetteSpec),
    P1MissileCorvetteStaticInit,
    NULL,
    P1MissileCorvetteInit,
    NULL,
    P1MissileCorvetteAttack,
    DefaultShipFire,
    P1MissileCorvetteAttackPassive,
    NULL,
    P1MissileCorvetteSpecialTarget,
    P1MissileCorvetteHousekeep,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

