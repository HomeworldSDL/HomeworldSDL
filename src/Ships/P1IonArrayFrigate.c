// =============================================================================
//  P1IonArrayFrigate.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/06/1998 by ddunlop
// =============================================================================

#include "P1IonArrayFrigate.h"

#include "Attack.h"
#include "Battle.h"
#include "DefaultShip.h"
#include "Gun.h"
#include "SoundEvent.h"
#include "Universe.h"

typedef struct
{
    udword dummy;
    real32 CloakingStatus; // Gradient value for cloaking and decloaking state.
    bool CloakLowWarning;  // Indicates whether the ship's cloak mana is low.
    bool ReCloak;          // Indicates whether the ship needs to cloak again.
    real32 ReCloakTime;    // Timestamp (in seconds) against game time elapsed during which the ship must cloak again.
} P1IonArrayFrigateSpec;

typedef struct
{
    real32 frigateGunRange[NUM_TACTICS_TYPES];
    real32 frigateTooCloseRange[NUM_TACTICS_TYPES];
    real32 CloakingTime;      // Cloaking delay (in seconds).
    real32 DeCloakingTime;    // Decloaking delay (in seconds).
    real32 VisibleState;      // Threshold at which the ship becomes "invisible" to sensors, ships and everything else.
    real32 battleReCloakTime; // Recloaking delay (in seconds) after the ship has fired.
} P1IonArrayFrigateStatics;

P1IonArrayFrigateStatics P1IonArrayFrigateStatic;

scriptStructEntry P1IonArrayFrigateStaticScriptTable[] =
{
    { "CloakingTime",               scriptSetReal32CB,  &(P1IonArrayFrigateStatic.CloakingTime),                &(P1IonArrayFrigateStatic) },
    { "DeCloakingTime",             scriptSetReal32CB,  &(P1IonArrayFrigateStatic.DeCloakingTime),              &(P1IonArrayFrigateStatic) },
    { "VisibleState",               scriptSetReal32CB,  &(P1IonArrayFrigateStatic.VisibleState),                &(P1IonArrayFrigateStatic) },
    { "battleReCloakTime",          scriptSetReal32CB,  &(P1IonArrayFrigateStatic.battleReCloakTime),           &(P1IonArrayFrigateStatic) },

    END_SCRIPT_STRUCT_ENTRY
};

void P1IonArrayFrigateStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    udword i;
    P1IonArrayFrigateStatics *frigstat = &P1IonArrayFrigateStatic;

    statinfo->custstatinfo = frigstat;

    for(i=0;i<NUM_TACTICS_TYPES;i++)
    {
        frigstat->frigateGunRange[i] = statinfo->bulletRange[i];
        frigstat->frigateTooCloseRange[i] = statinfo->minBulletRange[i] * 0.9f;
    }

    scriptSetStruct(directory, filename, P1IonArrayFrigateStaticScriptTable, (ubyte *)frigstat);

    frigstat->CloakingTime = 1 / frigstat->CloakingTime;
}

void P1IonArrayFrigateInit(Ship *ship)
{
    P1IonArrayFrigateSpec *spec = (P1IonArrayFrigateSpec *)ship->ShipSpecifics;
    spec->CloakingStatus = 1.0f; //ship isn't cloaked
    spec->CloakLowWarning = FALSE;
    spec->ReCloak = FALSE;
}

void P1IonArrayFrigateAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    ShipStaticInfo *shipstaticinfo = (ShipStaticInfo *)ship->staticinfo;
    P1IonArrayFrigateStatics *frigstat = (P1IonArrayFrigateStatics *)shipstaticinfo->custstatinfo;

    attackStraightForward(ship, target, frigstat->frigateGunRange[ship->tacticstype], frigstat->frigateTooCloseRange[ship->tacticstype], NULL, NULL);
}

void P1IonArrayFrigateAttackPassive(Ship *ship,Ship *target,bool rotate)
{
    if ((rotate) & ((bool)((ShipStaticInfo *)(ship->staticinfo))->rotateToRetaliate))
    {
        attackPassiveRotate(ship, target, NULL, NULL);
    }
    else
    {
        attackPassive(ship, target, NULL, NULL);
    }
}

void P1IonArrayFrigateSpecialActivate(Ship *ship)
{
    P1IonArrayFrigateSpec *spec = (P1IonArrayFrigateSpec *)ship->ShipSpecifics;
    P1IonArrayFrigateStatics *stat = (P1IonArrayFrigateStatics *)((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;

    spec->ReCloak = FALSE;

    if (bitTest(ship->flags, SOF_Cloaked))
    {
        bitSet(ship->flags, SOF_DeCloaking); //begin DeCloaking Process;
        SpawnCloakingEffect(ship, etgSpecialPurposeEffectTable[EGT_CLOAK_OFF]);
        soundEvent(ship, Ship_CloakOff);
    }
    else
    {
        bitSet(ship->flags, SOF_Cloaking); //begin Cloaking Process
        SpawnCloakingEffect(ship, etgSpecialPurposeEffectTable[EGT_CLOAK_ON]);
        soundEvent(ship, Ship_CloakOn);
        if (battleCanChatterAtThisTime(BCE_CloakingOn, ship))
        {
            battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_CloakingOn, ship, SOUND_EVENT_DEFAULT);
        }
    }

    return TRUE;
}

void P1IonArrayFrigateHouseKeep(Ship *ship)
{
    P1IonArrayFrigateSpec *spec = (P1IonArrayFrigateSpec *)ship->ShipSpecifics;
    P1IonArrayFrigateStatics *stat = (P1IonArrayFrigateStatics *)((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;

    if (bitTest(ship->flags, SOF_Cloaking)) //ship is cloaking
    {
        //decrement Cloaking Status counter based on time elapsed
        /***** calculated the inverse of CloakingTime in P1IonArrayFrigateStaticInit and multiply instead of divide *****/
        spec->CloakingStatus -= universe.phystimeelapsed * stat->CloakingTime;
        if (spec->CloakingStatus <= stat->VisibleState)
        {                                     //if ship is now past the vissible threshold
            bitSet(ship->flags, SOF_Cloaked); //make it invisible to everything
            if (spec->CloakingStatus <= 0.0f) //if it is completly invisible stop decloaking
            {
                spec->CloakingStatus = 0.0f;         //reset to 0.0
                bitClear(ship->flags, SOF_Cloaking); //stop ship from 'cloaking'
                //Since ship is cloaked, must remove it from being targeted
                //Note:  This probably will not let a person attack their own cloaked ships...

                shipHasJustCloaked(ship);
            }
        }
    }
    else if (bitTest(ship->flags, SOF_DeCloaking)) //Ship is decloaking
    {
        //Increment Cloaking Status counter based on time elapsed
        /***** calculated the inverse of CloakingTime in P1IonArrayFrigateStaticInit and multiply instead of divide *****/
        spec->CloakingStatus += universe.phystimeelapsed * stat->CloakingTime;
        if (spec->CloakingStatus >= stat->VisibleState)
        { //ship is 'visible' since it is past visible threshold
            if (ship->flags & SOF_Cloaked)
            {
                bitClear(ship->flags, SOF_Cloaked);
                ship->shipDeCloakTime = universe.totaltimeelapsed;
            }
            if (spec->CloakingStatus >= 1.0f) //done decloaking
            {
                spec->CloakingStatus = 1.0f;
                bitClear(ship->flags, SOF_DeCloaking);
                spec->CloakLowWarning = FALSE; //reset flag
                if (spec->ReCloak == FALSE)
                { //only play speech when really decloaking for good
                    if (battleCanChatterAtThisTime(BCE_Decloaking, ship))
                    {
                        battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_Decloaking, ship, SOUND_EVENT_DEFAULT);
                    }
                }
            }
        }
    }

    // TODO: @kajitetsushi Consume energy for running cloak. See the cloak generator for ideas.

    if (spec->ReCloak == TRUE)
    {
        if (universe.totaltimeelapsed > spec->ReCloakTime)
        {
            spec->ReCloak = FALSE;
            bitSet(ship->flags, SOF_Cloaking);
            soundEvent(ship, Ship_CloakOn);
        }
    }
}

CustShipHeader P1IonArrayFrigateHeader =
{
    P1IonArrayFrigate,
    sizeof(P1IonArrayFrigateSpec),
    P1IonArrayFrigateStaticInit,
    NULL,
    P1IonArrayFrigateInit,
    NULL,
    P1IonArrayFrigateAttack,
    DefaultShipFire,
    P1IonArrayFrigateAttackPassive,
    P1IonArrayFrigateSpecialActivate,
    NULL,
    P1IonArrayFrigateHouseKeep,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

