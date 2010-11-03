// =============================================================================
//  RepairCorvette.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/30/1997 by gshaw
// =============================================================================

#include "RepairCorvette.h"

#include "AIShip.h"
#include "AITrack.h"
#include "Collision.h"
#include "CommandDefs.h"
#include "DefaultShip.h"
#include "SaveGame.h"
#include "SoundEvent.h"
#include "Universe.h"
#include "UnivUpdate.h"


#define flytoRepairDest(scalar,tol) \
        matGetVectFromMatrixCol1(targetup,spec->target->rotinfo.coordsys);                  \
        matGetVectFromMatrixCol3(targetheading,spec->target->rotinfo.coordsys);             \
                                                                                            \
        trackflag = TRUE;                                                                   \
        vecScalarMultiply(targetdown,targetup,-1.0f);                                       \
                                                                                            \
        if(ship->shiprace == R1)                                                            \
        {                                                                                   \
            vecScalarMultiply(trackup,targetup,-1.0f);                                      \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            vecScalarMultiply(trackup,targetup, 1.0f);                                      \
        }                                                                                   \
                                                                                            \
        if(!aitrackHeadingAndUp(ship,&targetheading,&trackup,0.999f))                       \
        {                                                                                   \
            trackflag = FALSE;                                                              \
        }                                                                                   \
                                                                                            \
        vecScalarMultiply(destination,targetdown,scalar);                                   \
        vecAddTo(destination,spec->target->posinfo.position);                               \
                                                                                            \
        if(!MoveReachedDestinationVariable(ship,&destination,tol))                          \
        {                                                                                   \
            aishipFlyToPointAvoidingObjs(ship,&destination,0,0.0f);                         \
            trackflag = FALSE;                                                              \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            aitrackZeroVelocity(ship);                                                      \
        }
/*
        aishipGetTrajectory(ship,(SpaceObjRotImpTarg *)spec->target,&trajectory);           \
        range = RangeToTarget(ship,(SpaceObjRotImpTarg *)spec->target,&trajectory);         \
*/                                                                                          




#define DEBUG_REPAIR_CORVETTE  0


void stopRepairEffect(Ship *ship);

typedef struct
{
    AttackSideStepParameters sidestepParameters;
    real32 repairApproachDistance;

    real32 approachAndWaitDistance;
    real32 rotationStopDistance;
    real32 stopRotMultiplier;
    real32 sloppyRotThreshold;
    real32 dockWithRotationSpeed;
    real32 targetStartDockDistance;
    real32 startdockTolerance;
    real32 finaldockDistance;
    real32 CapitalDistanceRepairStart;
    real32 capitalShipHealthPerSecond;
    real32 AngleDotProdThreshold;
} RepairCorvetteStatics;

RepairCorvetteStatics RepairCorvetteStaticRace1;
RepairCorvetteStatics RepairCorvetteStaticRace2;

RepairCorvetteStatics RepairCorvetteStatic;

scriptStructEntry RepairCorvetteStaticScriptTable[] =
{
    { "repairApproachDistance",    scriptSetReal32CB,  &(RepairCorvetteStatic.repairApproachDistance),  &(RepairCorvetteStatic) },

    { "approachAndWaitDistance",    scriptSetReal32CB,  &(RepairCorvetteStatic.approachAndWaitDistance),  &(RepairCorvetteStatic) },
    { "rotationStopDistance",    scriptSetReal32CB,  &(RepairCorvetteStatic.rotationStopDistance),  &(RepairCorvetteStatic) },
    { "stopRotMultiplier",    scriptSetReal32CB,  &(RepairCorvetteStatic.stopRotMultiplier),  &(RepairCorvetteStatic) },
    { "sloppyRotThreshold",    scriptSetReal32CB,  &(RepairCorvetteStatic.sloppyRotThreshold),  &(RepairCorvetteStatic) },
    { "dockWithRotationSpeed",    scriptSetReal32CB,  &(RepairCorvetteStatic.dockWithRotationSpeed),  &(RepairCorvetteStatic) },
    { "targetStartDockDistance",    scriptSetReal32CB,  &(RepairCorvetteStatic.targetStartDockDistance),  &(RepairCorvetteStatic) },
    { "startdockTolerance",    scriptSetReal32CB,  &(RepairCorvetteStatic.startdockTolerance),  &(RepairCorvetteStatic) },
    { "finaldockDistance",    scriptSetReal32CB,  &(RepairCorvetteStatic.finaldockDistance),  &(RepairCorvetteStatic) },

    { "CapitalDistanceRepairStart",    scriptSetReal32CB,  &(RepairCorvetteStatic.CapitalDistanceRepairStart),  &(RepairCorvetteStatic) },
    { "capitalShipHealthPerSecond",    scriptSetReal32CB,  &(RepairCorvetteStatic.capitalShipHealthPerSecond),  &(RepairCorvetteStatic) },
    { "AngleDotProdThreshold",    scriptSetReal32CB,  &(RepairCorvetteStatic.AngleDotProdThreshold),  &(RepairCorvetteStatic) },

    END_SCRIPT_STRUCT_ENTRY
};

void RepairCorvetteStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    RepairCorvetteStatics *corvstat = (statinfo->shiprace == R1) ? &RepairCorvetteStaticRace1 : &RepairCorvetteStaticRace2;

    memset(corvstat,sizeof(*corvstat),0);
    scriptSetStruct(directory,filename,AttackSideStepParametersScriptTable,(ubyte *)&corvstat->sidestepParameters);
    scriptSetStruct(directory,filename,RepairCorvetteStaticScriptTable,(ubyte *)corvstat);

    statinfo->custstatinfo = corvstat;
}

void RepairCorvetteInit(Ship *ship)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;
    spec->repairState = REPAIR_BEGIN;
    spec->target = NULL;
    spec->hyst = FALSE;
    attackSideStepInit(&spec->attacksidestep);
}

void RepairCorvetteAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;
    RepairCorvetteStatics *corvstat = (RepairCorvetteStatics *)((ShipStaticInfo *)ship->staticinfo)->custstatinfo;

    attackSideStep(ship,target,&spec->attacksidestep,&corvstat->sidestepParameters);
}

void RepairCorvetteAttackPassive(Ship *ship,Ship *target,bool rotate)
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

void RepairCorvetteRemoveShipReferences(Ship *ship, Ship *shiptoremove)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;
    if(shiptoremove == spec->target)
    {
        spec->repairState = REPAIR_BEGIN;
        spec->target = NULL;
    }
}

void RepairCorvetteDied(Ship *ship)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;
    if(spec->target != NULL)
    {
        bitClear(spec->target->dontrotateever,1);
        bitClear(spec->target->dontapplyforceever,1);

        spec->target = NULL;
    }

}

void RepairCorvetteOrderChanged(Ship *ship)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;

    //for super duper cool intelligence, we could check if the order is the
    //same thing as what we're doing currently!  Then we don't stop if it
    //is.

    spec->repairState = REPAIR_BEGIN;
    if(spec->target != NULL)
    {
        bitClear(spec->target->dontrotateever,1);
        bitClear(spec->target->dontapplyforceever,1);

        spec->target = NULL;
    }

    if(ship->rceffect != NULL)
    {
        stopRepairEffect(ship);
    }
    spec->hyst = FALSE;
}
void startRepairEffect(Ship *ship,SpaceObjRotImpTarg *target,vector *trajectory,real32 distance)
{
    ShipStaticInfo *shipstatic = (ShipStaticInfo *)ship->staticinfo;
    RepairNozzleStatic *repnozzlestatic = shipstatic->repairNozzleStatic;
    //TURN ON EFFECT HERE
    //
    //Use ship->rceffect, since ship isn't a resource collector,
    //the pointer isn't being used
    etglod *etgLOD = etgSpecialPurposeEffectTable[EGT_REPAIR_BEAM];

    etgeffectstatic *stat;
    sdword LOD;
    udword intLength;
    udword intWidth;
    matrix coordsys;
    real32 targetRadius = target->staticinfo->staticheader.staticCollInfo.collspheresize;
    vector repairBeamPosition;

    matMultiplyMatByVec(&repairBeamPosition,&ship->rotinfo.coordsys,&repnozzlestatic->position);

    matCreateCoordSysFromHeading(&coordsys,trajectory);

    intLength = TreatAsUdword(distance);
    intWidth = TreatAsUdword(targetRadius);

    vecAddTo(repairBeamPosition,ship->posinfo.position);

    //create an effect for bullet, if applicable
    if (etgLOD != NULL)
    {
        LOD = ship->currentLOD;
        if (LOD >= etgLOD->nLevels)
        {
            stat = NULL;
        }
        else
        {
            stat = etgLOD->level[LOD];
        }
    }
    else
    {
        stat = NULL;
    }
    #if ETG_DISABLEABLE
    if (stat != NULL && etgEffectsEnabled && !etgFrequencyExceeded(stat))
    #else
    if (stat != NULL && !etgFrequencyExceeded(stat))
    #endif
    {
        ship->rceffect = etgEffectCreate(stat, ship, &repairBeamPosition, &ship->posinfo.velocity, &coordsys, 1.0f, EAF_Velocity | EAF_NLips, 2, intLength, intWidth);
    }
    else
    {
        ship->rceffect = NULL;                              //play no effect
    }


}
void stopRepairEffect(Ship *ship)
{

#ifdef HW_BUILD_FOR_DEBUGGING
    //make sure we only entered here if effect was playing!
    dbgAssertOrIgnore(ship->rceffect != NULL);
#endif

    if (bitTest(((etgeffectstatic *)ship->rceffect->staticinfo)->specialOps, ESO_SelfDeleting))
    {                                                       //if the effect will delete itself
        ((real32 *)ship->rceffect->variable)[ETG_ResourceDurationParam] =
            ship->rceffect->timeElapsed;                            //time it out
    }
    else
    {                                                       //else it's a free-running effect... delete it
        etgEffectDelete(ship->rceffect);
        univRemoveObjFromRenderList((SpaceObj *)ship->rceffect);
        listDeleteNode(&ship->rceffect->objlink);
    }

    //soundEventStop(ship->soundevent.specialHandle);

    ship->rceffect = NULL;
}

void ModifyRepairEffect(Effect *effect,Ship *ship,vector *trajectory,real32 distance, SpaceObjRotImpTarg *target)
{
    ShipStaticInfo *shipstatic = (ShipStaticInfo *)ship->staticinfo;
    RepairNozzleStatic *repnozzlestatic = shipstatic->repairNozzleStatic;
    vector repairBeamPosition;

    dbgAssertOrIgnore(effect);

    matMultiplyMatByVec(&repairBeamPosition,&ship->rotinfo.coordsys,&repnozzlestatic->position);

    vecAdd(effect->posinfo.position,repairBeamPosition,ship->posinfo.position);

    matCreateCoordSysFromHeading(&effect->rotinfo.coordsys,trajectory);

	distance = max(200.0f,distance);
	if(target->objtype == OBJ_ShipType)
	{
		if(((Ship *)target)->shiptype == Mothership)
		{
			distance = max(800.0f,distance);
		}
	}


    ((real32 *)effect->variable)[ETG_ResourceLengthParam] = distance;
    ((real32 *)effect->variable)[ETG_ResourceRadiusParam] = target->staticinfo->staticheader.staticCollInfo.collspheresize;
}

//function will return the most heavily damaged target in the list, OR
//if all are fully healthy, it will return the FIRST target in the list
bool areAllHealthy(Ship *ship,SelectAnyCommand *targets)
{
    sdword i;
    for(i=0;i<targets->numTargets;i++)
    {
        if(targets->TargetPtr[i]->health != ((ShipStaticInfo *)targets->TargetPtr[i]->staticinfo)->maxhealth)
        {
            return FALSE;
        }
    }
    return (TRUE);
}
bool refuelRepairShips(Ship *ship, SelectAnyCommand *targets,real32 rangetoRefuel)
{
    //remove unwantedships from this selection
    //optimize by doing only once for this ship
    SelectCommand selectOne;
    sdword i;
    SpaceObjRotImpTarg *target,*targettemp;
    vector trajectory,curheading;
    real32 range,dotprod;
    CommandToDo *targetCommand;

    //filter target list for unrepairable ships!

    for(i=0;i<targets->numTargets;)
    {
        if(targets->TargetPtr[i]->objtype != OBJ_ShipType)
        {
            targets->numTargets--;
            targets->TargetPtr[i] = targets->TargetPtr[targets->numTargets];
            continue;
        }
        targetCommand = getShipAndItsCommand(&universe.mainCommandLayer,(Ship *)targets->TargetPtr[i]);
        if(targetCommand != NULL)
        {
            if(targetCommand->ordertype.order == COMMAND_DOCK)
            {
                //remove docking ships from the list
                if(((Ship *)targets->TargetPtr[i])->dockingship != NULL)
                {
                    //only if in final stages
                    targets->numTargets--;
                    targets->TargetPtr[i] = targets->TargetPtr[targets->numTargets];
                    continue;
                }
            }
        }

        //object is a ship
        if((((Ship *)targets->TargetPtr[i])->staticinfo->shipclass != CLASS_Fighter &&
            ((Ship *)targets->TargetPtr[i])->staticinfo->shipclass != CLASS_Corvette) ||
            ((Ship *)targets->TargetPtr[i])->playerowner != ship->playerowner)
        {
            if(ship->staticinfo->repairBeamCapable)
            {
                //repair corvette can repair capital ships!
                //class doesn't matter
                if(((Ship *)targets->TargetPtr[i])->playerowner == ship->playerowner)
                {
                    //ship is not a fighter or a corvette, but is ours, so we can fix it!
                    i++;
                    continue;
                }
            }
            targets->numTargets--;
            targets->TargetPtr[i] = targets->TargetPtr[targets->numTargets];
        }
        else
        {
            i++;
        }
    }

    if (targets->numTargets <= 0)
    {
        return TRUE;
    }



    target = targets->TargetPtr[0];

//need to add
//repairBeamCapable
//    real32 healthPerSecond,CapitalDistanceRepairStart;
//    real32 CapitalDistanceRepairStart2;

    if(ship->staticinfo->repairBeamCapable)
    {
        //assume target is a ship!!!!

        //targets might be a capital ship
        if(((Ship *)target)->staticinfo->shipclass != CLASS_Fighter)
        {
            //target is such that it should be BEAM repaired!
            //so lets fly upto it and ZAP repair it!
            real32 range1,range2;
            aishipGetTrajectory(ship,target,&trajectory);
            range = RangeToTarget(ship,target,&trajectory);

            vecNormalize(&trajectory);
            aitrackHeading(ship,&trajectory,0.999f);

            if(((Ship *)target)->shiptype == Mothership)
            {
                range1 = 2*ship->staticinfo->CapitalDistanceRepairStart;
                range2 = 2*ship->staticinfo->CapitalDistanceRepairStart2;
            }
            else
            {
                range1 = ship->staticinfo->CapitalDistanceRepairStart;
                range2 = ship->staticinfo->CapitalDistanceRepairStart2;
            }
            //we add 300 to the range checker for a sort of hysterysis!
            if(range > range1)
            {
                aishipFlyToShipAvoidingObjs(ship,target,AISHIP_PointInDirectionFlying + AISHIP_FastAsPossible,0.0f);
                if(range > range2)
                {
                    //far enough away to stop the effect and ONLY fly towards ship
                    if(ship->rceffect != NULL)
                    {
                        //turn off ships effect
                        stopRepairEffect(ship);
                    }
                    return FALSE;
                }
            }
            else
            {
                aitrackZeroVelocity(ship);
            }

            //in range to repair this li'l ship


            matGetVectFromMatrixCol3(curheading,ship->rotinfo.coordsys);
            dotprod = vecDotProduct(trajectory,curheading);

            if(!areAllHealthy(ship,targets))
            {
                if(dotprod > ship->staticinfo->AngleDotProdThreshold)
                {
                    //within angle of 'repair beam'
                    if(ship->rceffect == NULL)
                    {
                        //turn on effect if we should!
                        startRepairEffect(ship,target,&trajectory,range);
                    }
                    else
                    {
                        //fix up effect
                        ModifyRepairEffect(ship->rceffect,ship,&trajectory,range, target);
                    }
                    target->health+=ship->staticinfo->healthPerSecond*universe.phystimeelapsed;
                    if(((Ship *)target)->health > ((Ship *)target)->staticinfo->maxhealth)
                    {
                        if(ship->rceffect != NULL)
                        {
                            //turn off ships effect
                            stopRepairEffect(ship);
                        }
                        target->health = ((Ship *)target)->staticinfo->maxhealth;

                        //fix this in a bit
                        targettemp = targets->TargetPtr[0];
                        for(i=0;i<(targets->numTargets-1);i++)
                        {
                            targets->TargetPtr[i]=targets->TargetPtr[i+1];
                        }
                        targets->TargetPtr[targets->numTargets-1] = targettemp;

                        //if(targets->numTargets <= 0)
                        //    return TRUE;
                        //targets->TargetPtr[0] = targets->TargetPtr[targets->numTargets];
                        return FALSE;
                    }
                }
                else
                {
                    //not pointing at ship, turn off effect and other
                    //needed things!
                    if(ship->rceffect != NULL)
                    {
                        //turn off ships effect
                        stopRepairEffect(ship);
                    }
                }
            }
            else
            {
                if(ship->rceffect != NULL)
                {
                    //turn off ships effect
                    stopRepairEffect(ship);
                }
            }
            return FALSE;
        }
    }

    aishipGetTrajectory(ship,target,&trajectory);
    aishipFlyToShipAvoidingObjs(ship,target,AISHIP_PointInDirectionFlying + AISHIP_FastAsPossible +AISHIP_FirstPointInDirectionFlying,0.0f);
    range = RangeToTarget(ship,target,&trajectory);

    if(range <= rangetoRefuel)
    {
        //within repair range
        selectOne.numShips = 1;
        selectOne.ShipPtr[0] = (Ship *) target;
        if(target->objtype == OBJ_ShipType)
        {
            ((Ship *)target)->fuel += 300.0f;   //??? appropriate ammount?
            if(((Ship *)target)->fuel > ((Ship *)target)->staticinfo->maxfuel)
                ((Ship *)target)->fuel = ((Ship *)target)->staticinfo->maxfuel;

            //"speech event?  Switching to Emergency Docking Fuel Supply?
            //cldock
            clDock(&universe.mainCommandLayer,&selectOne,DOCK_AT_SPECIFIC_SHIP,ship);
        }
        targets->numTargets--;
        if(targets->numTargets <= 0)
            return TRUE;
        targets->TargetPtr[0] = targets->TargetPtr[targets->numTargets];
    }

    return FALSE;
}

#ifdef _WIN32_FIX_ME
    #pragma warning( 4 : 4047)      // turns off "different levels of indirection warning"
#endif

void RepairCorvette_PreFix(Ship *ship)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;

    spec->target = (Ship *) SpaceObjRegistryGetID((SpaceObj *)spec->target);
}

void RepairCorvette_Fix(Ship *ship)
{
    RepairCorvetteSpec *spec = (RepairCorvetteSpec *)ship->ShipSpecifics;

    spec->target = (Ship *) SpaceObjRegistryGetShip((sdword)spec->target);
}

#ifdef _WIN32_FIX_ME
    #pragma warning( 2 : 4047)      // turn back on "different levels of indirection warning"
#endif

bool RepairCorvetteSpecialTarget(Ship *ship,void *custom)
{
    RepairCorvetteStatics *stat = (RepairCorvetteStatics *)ship->staticinfo->custstatinfo;
    SelectAnyCommand *targets;
    targets = (SelectAnyCommand *)custom;
    return(refuelRepairShips(ship, targets,stat->repairApproachDistance));
}
CustShipHeader RepairCorvetteHeader =
{
    RepairCorvette,
    sizeof(RepairCorvetteSpec),
    RepairCorvetteStaticInit,
    NULL,
    RepairCorvetteInit,
    NULL,
    RepairCorvetteAttack,
    DefaultShipFire,
    RepairCorvetteAttackPassive,
    NULL,
    RepairCorvetteSpecialTarget,
    NULL,
    RepairCorvetteRemoveShipReferences,
    RepairCorvetteDied,
    RepairCorvette_PreFix,
    NULL,
    NULL,
    RepairCorvette_Fix
};

