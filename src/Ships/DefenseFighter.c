// =============================================================================
//  DefenseFighter.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 1/27/1998 by Bryce Pasechnik
// =============================================================================

#include "DefenseFighter.h"

#include "AIShip.h"
#include "AITrack.h"
#include "Collision.h"
#include "FastMath.h"
#include "glinc.h"
#include "Memory.h"
#include "Randy.h"
#include "SaveGame.h"
#include "SoundEvent.h"
#include "StatScript.h"
#include "Universe.h"
#include "UnivUpdate.h"
#include "UnivUpdate.h"

typedef struct
{
    Node bulletnode;
    Bullet *bullet;
    Bullet *laser;
    real32 CoolDownTime;
    bool32 CoolDown;
    bool32 LaserDead;
} DefenseStruct;

typedef struct
{
    LinkedList DefenseList;
    sdword DefenseFighterCanNowRotate;
    real32 rotate_time_counter;
} DefenseFighterSpec;

DefenseFighterStatics DefenseFighterStatic;

DefenseFighterStatics DefenseFighterStaticRace1;
DefenseFighterStatics DefenseFighterStaticRace2;


scriptStructEntry DefenseFighterStaticScriptTable[] =
{
    { "NumTargetsCanAttack",    scriptSetUdwordCB,  &(DefenseFighterStatic.NumTargetsCanAttack),  &(DefenseFighterStatic) },
    { "CoolDownTimePerLaser",    scriptSetReal32CB,  &(DefenseFighterStatic.CoolDownTimePerLaser),  &(DefenseFighterStatic) },
    { "DamageReductionLow",    scriptSetReal32CB,  &(DefenseFighterStatic.DamageReductionLow),  &(DefenseFighterStatic) },
    { "DamageReductionHigh",    scriptSetReal32CB,  &(DefenseFighterStatic.DamageReductionHigh),  &(DefenseFighterStatic) },
    { "DamageRate",    scriptSetUdwordCB,  &(DefenseFighterStatic.DamageRate),  &(DefenseFighterStatic) },
    { "RangeCheckRate",    scriptSetUdwordCB,  &(DefenseFighterStatic.RangeCheckRate),  &(DefenseFighterStatic) },
    { "TargetOwnBullets",    scriptSetSdwordCB,  &(DefenseFighterStatic.TargetOwnBullets),  &(DefenseFighterStatic) },
    { "MultipleTargettingofSingleBullet",    scriptSetSdwordCB,  &(DefenseFighterStatic.MultipleTargettingofSingleBullet),  &(DefenseFighterStatic) },
    { "max_rot_speed",    scriptSetReal32CB,  &(DefenseFighterStatic.max_rot_speed),  &(DefenseFighterStatic) },
    { "rotate_recover_time",    scriptSetReal32CB,  &(DefenseFighterStatic.rotate_recover_time),  &(DefenseFighterStatic) },
    { "rotate_time",    scriptSetReal32CB,  &(DefenseFighterStatic.rotate_time),  &(DefenseFighterStatic) },

    { "flightmanTurnaround",    scriptSetFlightManTurnaroundCB,      &(DefenseFighterStatic.flightmanProb),      &(DefenseFighterStatic) },
    { "flightmanAIP",           scriptSetFlightManAIPCB,             &(DefenseFighterStatic.flightmanProb),      &(DefenseFighterStatic) },
    { "flightmanEvasiveBehind", scriptSetFlightManEvasiveBehindCB,   &(DefenseFighterStatic.flightmanProb),      &(DefenseFighterStatic) },
    { "flightmanEvasiveFront",  scriptSetFlightManEvasiveFrontCB,    &(DefenseFighterStatic.flightmanProb),      &(DefenseFighterStatic) },
    { "flightmanEvasivePure",   scriptSetFlightManEvasivePureCB,     &(DefenseFighterStatic.flightmanProb),      &(DefenseFighterStatic) },

    END_SCRIPT_STRUCT_ENTRY
};

void DefenseFighterStaticInit(char *directory,char *filename,struct ShipStaticInfo *statinfo)
{
    DefenseFighterStatics *DefenseFighterstat = (statinfo->shiprace == R1) ? &DefenseFighterStaticRace1 : &DefenseFighterStaticRace2;

    statinfo->custstatinfo = DefenseFighterstat;
    scriptSetStruct(directory,filename,DefenseFighterStaticScriptTable,(ubyte *)DefenseFighterstat);
    //Calculate the Square of the radius for speeds sake
}

void DefenseFighterInit(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    DefenseFighterStatics *defensefighterstatics;
    defensefighterstatics = (DefenseFighterStatics *) ((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;
    spec->DefenseFighterCanNowRotate = TRUE;
    listInit(&spec->DefenseList);
}

/*-----------------------------------------------------------------------------
    Name        : defensefightertargetbullet
    Description : sets one of the 'ship's available lasers to target 'bullettotarget'
                  Allocates memory for defense fighters linked list, and for Laser object
                  later...add effects to this
    Inputs      : ship, bullettotarget
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

void defensefightertargetbullet(Ship *ship, Bullet *bullettotarget)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    DefenseFighterStatics *defensefighterstatics;
    DefenseStruct *newdefensestruct;
    Bullet *laser;
    GunStatic *gunstatic;
    Gun *gun;
    ShipStaticInfo *shipstatic;
    vector positionInWorldCoordSys,tempvec;
    real32 floatDamage;
    udword intDamage;
    udword intVelocity;
    udword intLength;
    etgeffectstatic *stat;
    etglod *etgLOD;
    sdword LOD;

    dbgAssertOrIgnore(ship->shiptype == DefenseFighter);

    gun = &ship->gunInfo->guns[0];
    gunstatic = gun->gunstatic;
    shipstatic = (ShipStaticInfo *)ship->staticinfo;
    defensefighterstatics = (DefenseFighterStatics *) ((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;

    bitSet(bullettotarget->SpecialEffectFlag, 0x0002);   //set the flag

    laser = memAlloc(sizeof(Bullet),"Bullet",0);
    memset(laser,0,sizeof(Bullet));      // for safety

    laser->objtype = OBJ_BulletType;
    laser->flags = 0;
    laser->staticinfo = NULL;
    ClearNode(laser->renderlink);
    laser->currentLOD = ship->currentLOD;
    laser->cameraDistanceSquared = ship->cameraDistanceSquared;

    laser->soundType = gunstatic->gunsoundtype;
    laser->bulletType = BULLET_Laser;
    laser->owner = ship;
    laser->gunowner = gun;
    laser->target = NULL;

    laser->bulletColor = etgBulletColor[shipstatic->shiprace][laser->soundType];

    laser->bulletmass = 0.0f; //gunstatic->bulletmass;
    laser->lengthmag = 600.0f;
    laser->damage = frandombetween(defensefighterstatics->DamageReductionLow,
                                     defensefighterstatics->DamageReductionHigh);
    laser->timelived = 0.0f;
    laser->totallifetime = 100.0f;  //laser will only live for a sec anyways...
    laser->SpecialEffectFlag = 0;
    laser->traveldist = gunstatic->bulletlength;
    laser->beamtraveldist = gunstatic->bulletlength;

    //SET_MOVING_LINEARLY(laser->posinfo.isMoving);
    //laser->posinfo.haventCalculatedDist = TRUE;
    laser->DFGFieldEntryTime = 0.0f;

    matMultiplyMatByVec(&positionInWorldCoordSys,&ship->rotinfo.coordsys,&gunstatic->position);
    vecAdd(laser->posinfo.position,positionInWorldCoordSys,ship->posinfo.position);
    vecSub(laser->lengthvec, bullettotarget->posinfo.position, laser->posinfo.position);
    // heading
    tempvec = laser->lengthvec;
    vecNormalize(&tempvec);

    //matMultiplyMatByVec(&gunheadingInWorldCoordSys, &ship->rotinfo.coordsys, &tempvec);
    //laser->bulletheading = gunheadingInWorldCoordSys;
    laser->bulletheading = tempvec;
    matCreateCoordSysFromHeading(&laser->rotinfo.coordsys,&tempvec);

    vecZeroVector(laser->posinfo.velocity);

    //Laser effect...
    floatDamage = (real32)laser->damage;
    intDamage = Real32ToUdword(floatDamage);
    intVelocity = Real32ToUdword(gunstatic->bulletspeed);
    intLength = Real32ToUdword(gunstatic->bulletlength);
    //create an effect for bullet, if applicable

    etgLOD = etgGunEventTable[shipstatic->shiprace][gunstatic->gunsoundtype][EGT_GunBullet];//get pointer to bullet effect
    //etgLOD = etgGunEventTable[0][gunstatic->gunsoundtype][EGT_GunBullet];//get pointer to bullet effect
  //in future change back to proper one above...
    if (etgLOD != NULL)
    {
        if (bullettotarget != NULL)
        {
          LOD = min(ship->currentLOD, bullettotarget->currentLOD);
        }
        else
        {
          LOD = ship->currentLOD;
        }
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
    if (stat != NULL && etgBulletEffectsEnabled && etgEffectsEnabled && !etgFrequencyExceeded(stat))
#else
    if (stat != NULL && etgBulletEffectsEnabled && !etgFrequencyExceeded(stat))
#endif
    {
        laser->effect = etgEffectCreate(stat, laser, NULL, NULL, NULL, 1.0f, EAF_AllButNLips, 3, intDamage, intVelocity, intLength);
//        univAddObjToRenderListIf((SpaceObj *)laser->effect,(SpaceObj *)ship);     // add to render list if parent ship is in render list
        //do length calculations :)
        ((real32 *)laser->effect->variable)[ETG_LengthVariable] =
                    fsqrt(vecMagnitudeSquared(laser->lengthvec));
    }
    else
    {
        laser->effect = NULL;                              //play no effect for this bullet
    }

//    laser->effect = NULL;               //need for render...add later?

    laser->hitEffect = etgGunEventTable[shipstatic->shiprace][bullettotarget->gunowner->gunstatic->gunsoundtype][EGT_BulletDestroyed];//get pointer to bullet effect

    if (ship->soundevent.burstHandle < 0)
    {
        soundEventBurstFire(ship, gun);
    }

    etgLOD = etgGunEventTable[shipstatic->shiprace][gunstatic->gunsoundtype][EGT_GunFire];//get pointer to bullet effect
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
    if (stat != NULL && etgEffectsEnabled && etgFireEffectsEnabled && !etgFrequencyExceeded(stat))
#else
    if (stat != NULL && etgFireEffectsEnabled && !etgFrequencyExceeded(stat))
#endif
    {
        /** Spawn a gun fire effect (nozzle flash)
         *
         * This new effect is spawned with `.owner = laser` but `Bullet` only has room for a *single* `.effect`
         * back-pointer which is already occupied by the beam effect. So when `laser` is deleted, we're out of luck to
         * find this effect to update/delete it.
         *
         * Gitlab #30 "Segfault on defense fighter laser" - Thibault Lemaire - 2024-04-10 - 1 of 3
         */
        etgEffectCreate(stat, laser, NULL, NULL, NULL, 1.0f, EAF_AllButNLips, 1, intDamage);
    }
/*
//spawn bullet hitting effect
    etgLOD = etgTractorBeamEffectTable[ship->shiprace];

    //etgLOD = etgGunEventTable[shipstatic->shiprace][gunstatic->gunsoundtype][EGT_GunFire];//get pointer to bullet effect
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
    if (stat != NULL && etgEffectsEnabled)
#else
    if (stat != NULL)
#endif
    {                                                       //if there is a gun fire effect
//        size = etgEffectSize(stat->nParticleBlocks);//compute size of effect
        size = stat->effectSize;
        newEffect = memAlloc(size, "DefenseFHittingEffect", 0);          //allocate the new effect

        newEffect->objtype = OBJ_EffectType;
        newEffect->flags = SOF_Rotatable | SOF_AttachVelocity | SOF_AttachPosition | SOF_AttachCoordsys;
        newEffect->staticinfo = (StaticInfo *)stat;
        ClearNode(newEffect->renderlink);
        newEffect->currentLOD = LOD;
        newEffect->cameraDistanceSquared = ship->cameraDistanceSquared;

        newEffect->timeElapsed = 0.0f;                          //brand new heavies

        floatDamage = 30.0f;
        intDamage = TreatAsUdword(floatDamage);

        newEffect->rotinfo.coordsys = bullettotarget->rotinfo.coordsys;
        newEffect->posinfo.position = bullettotarget->posinfo.position; //start at same spot as bullet
        newEffect->posinfo.velocity = bullettotarget->posinfo.velocity; //start at same spot as bullet
        etgEffectCodeStart(stat, newEffect, 1, intDamage);//get the code a-runnin'
        SET_MOVING_IMMOBILE(newEffect->posinfo.isMoving);
        newEffect->posinfo.haventCalculatedDist = TRUE;
        univUpdateObjRotInfo((SpaceObjRot *)newEffect);

//        newEffect->owner = NULL;                               // nothing owns this effect
        newEffect->owner = (Ship *) bullettotarget;
        listAddNode(&universe.SpaceObjList,&(newEffect->objlink),newEffect);
        univAddObjToRenderListIf((SpaceObj *)newEffect,(SpaceObj *)ship);     // add to render list if parent ship is in render list
    }
*/
    //Not sure If I need to add...

    listAddNode(&universe.SpaceObjList,&(laser->objlink),laser);
    listAddNode(&universe.BulletList,&(laser->bulletlink),laser);
    univAddObjToRenderListIf((SpaceObj *)laser,(SpaceObj *)ship);     // add to render list if parent ship is in render list

    newdefensestruct = memAlloc(sizeof(DefenseStruct),"DS(DefenseStruct)",Pyrophoric);
    newdefensestruct->bullet = bullettotarget;
    newdefensestruct->CoolDown = FALSE;
    newdefensestruct->CoolDownTime = 0.0f;
    newdefensestruct->LaserDead = FALSE;
    listAddNode(&spec->DefenseList,&newdefensestruct->bulletnode,newdefensestruct);
    newdefensestruct->laser = laser;



    if(bitTest(ship->flags,SOF_CloakGenField))
    {
        bitSet(ship->flags,SOF_DeCloaking);
        bitClear(ship->flags,SOF_Cloaked);
        bitClear(ship->flags,SOF_Cloaking);
    }
}

/*-----------------------------------------------------------------------------
    Name        : isbulletbeingtargeted
    Description : determines if a bullet is being targetted currently by 'ship'
    Inputs      :
    Outputs     :
    Return      :  TRUE if bullet is being targetted, FALSE otherwise
----------------------------------------------------------------------------*/

bool32 isbulletbeingtargeted(Ship *ship, Bullet *bullet)
{
   DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
   Node const* bulletnode;
   DefenseStruct *defensestruct;

   bulletnode = spec->DefenseList.head;

   while(bulletnode != NULL)
   {
     defensestruct = (DefenseStruct *)listGetStructOfNode(bulletnode);
     if(defensestruct->bullet == bullet)
     {      //bullet is in list
        //do laser update stuff here

        return(TRUE);
     }
     bulletnode = bulletnode->next;
   }
   //bullet wasn't in list
return(FALSE);
}

/*-----------------------------------------------------------------------------
    Name        : defensefighterCheckInFront
    Description : uses the dotproduct to determine if a bullet is infront of a ship
                so it can be shot at
    Inputs      :
    Outputs     :
    Return      : TRUE if bullet is infront of ship
----------------------------------------------------------------------------*/

bool32 defensefighterCheckInFront(Ship *ship, Bullet *bullet)
{
    vector shipheading; //ship velocity
    vector shiptobullet;
    vecSub(shiptobullet, bullet->posinfo.position, ship->posinfo.position);
    //vecSub(shipheading, ship->posinfo.position, ship->enginePosition);
    matGetVectFromMatrixCol3(shipheading,ship->rotinfo.coordsys);

    if(vecDotProduct(shiptobullet, shipheading) > 0)
    {
        return TRUE;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
    Name        :  DefenseFighterReportBullet
    Description :  Called by univupdate.c in univcheckbulletcollision()
                   decides whethe or not to target a bullet...later need line of sight
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

void DefenseFighterReportBullet(Ship *ship, Bullet *bullet)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    DefenseFighterStatics *defensefighterstatics;

    defensefighterstatics = (DefenseFighterStatics *) ((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;

    /*dbgMessagef("B: %d",universe.univUpdateCounter);

    if (bullet->playerowner!= NULL)
        dbgMessagef(" %d ", bullet->playerowner->playerIndex);
    else
        dbgMessagef(" N ");

    dbgMessagef("(%f %f %f) [%f %f] %f <%f %f> %f %f",
            bullet->posinfo.position.x,
            bullet->posinfo.position.y,
            bullet->posinfo.position.z,
            bullet->timelived,
            bullet->totallifetime,
            bullet->traveldist,
            bullet->damage,
            bullet->damageFull,
            bullet->DFGFieldEntryTime,
            bullet->BulletSpeed);

    if (bullet->collMyBlob)
        dbgMessagef( "%f", bullet->collMyBlob->sortDistance);
    else
        dbgMessagef( "N");*/

    if(spec->DefenseList.num < defensefighterstatics->NumTargetsCanAttack)
    {       //don't exceed maximum # of targets
        //dbgMessagef("A: %d",universe.univUpdateCounter);
        if(bullet->bulletType == BULLET_Projectile)  //Maybe make
        {
            //dbgMessagef("C: %d",universe.univUpdateCounter);
            if(bullet->owner != NULL)
            {   //Only attack owned bullets...a small weakness, but easily fixable by making this more of a mess :)
                //dbgMessagef("D: %d",universe.univUpdateCounter);
                if(bullet->owner->playerowner != ship->playerowner || defensefighterstatics->TargetOwnBullets == TRUE)
                {
                    //dbgMessagef("E: %d",universe.univUpdateCounter);
                    if(bullet->timelived > universe.phystimeelapsed)
                    {    //don't target bullet if it is in the gun still...it sucks
                        //dbgMessagef("F: %d",universe.univUpdateCounter);
                        if(bitTest(bullet->SpecialEffectFlag,0x0002))
                        {   //Bullet IS being targetted by 'a' defense fighter already...
                            //dbgMessagef("G: %d",universe.univUpdateCounter);
                            if(defensefighterstatics->MultipleTargettingofSingleBullet)
                            {   //we want multiple defense fighters to target the same bullet
                                //dbgMessagef("H: %d",universe.univUpdateCounter);
                                if(!isbulletbeingtargeted(ship, bullet))   //does laser update if bullet IS in list
                                {    //bullet ISN'T being targetted by this ship
                                    //dbgMessagef("I: %d",universe.univUpdateCounter);
                                    if(defensefighterCheckInFront(ship, bullet))
                                    {
                                        //dbgMessagef("J: %d",universe.univUpdateCounter);
                                        defensefightertargetbullet(ship, bullet);
                                        //dbgMessagef("Targetted a bullet already being targetted.");
                                    }
                                }
                            }
                        }
                        else
                        {    //bullet is new to defense fighters
                            //dbgMessagef("K: %d",universe.univUpdateCounter);
                            if(defensefighterCheckInFront(ship, bullet))
                            {
                                //dbgMessagef("L: %d",universe.univUpdateCounter);
                                defensefightertargetbullet(ship, bullet);
                                //dbgMessagef("Targetted a fresh bullet");
                            }
                        }
                    }
                }
            }
            else
            {
                #ifdef HW_BUILD_FOR_DEBUGGING
  //                  dbgMessagef("Dfighter: owner null @ %d - f %x",universe.univUpdateCounter,bullet->flags);
                #endif
            }
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        :  DefenseFighterDestroyedABullet
    Description :  bullet was destroyed...so increase time to kill it..later
                    add effect
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

void DefenseFighterDestroyedABullet(Ship *ship, Bullet *bullet, Bullet *laser)
{
    //sdword damagetaken = (sdword)bullet->damage;
    //Ship *bulletowner = bullet->owner;
    //real32 massovertime;
    //vector recoil;
    //bool32 shipWasAlive;
    //etgeffectstatic *stat;
    //sdword LOD;
    //vector LOF;
    //bool32 fatalHit;
    //vector hitlocation;
    //real32 floatDamage;
    //udword intDamage;
//    real32 velMagnitude;
//    udword velMagnitudeDword;
//    vector newVelocity;


    bullet->timelived = 10000.0f;  //this will destroy the bullet
    /*
    //commented out because the bullet death effect was buggy and ugly
    //play the imact effect
    if (laser->hitEffect != NULL)
    {
        LOD = ship->currentLOD;
        if (LOD < laser->hitEffect->nLevels)
        {
            stat = laser->hitEffect->level[LOD];
           //stat = laser->hitEffect->level[0];
        }
        else
        {
            stat = NULL;
        }
    }
    else
    {
        stat = NULL;
    }
#if ETG_DISABLEABLE
    if (stat != NULL && etgHitEffectsEnabled && etgEffectsEnabled && !etgFrequencyExceeded(stat))
#else
    if (stat != NULL && etgHitEffectsEnabled && !etgFrequencyExceeded(stat))
#endif
    {
        //floatDamage = 30.0f;     //?????
        floatDamage = bullet->damageFull;
        intDamage = TreatAsUdword(floatDamage);

//        vecScalarMultiply(newVelocity,bullet->posinfo.velocity,0.3f);
//        velMagnitude = fsqrt(vecMagnitudeSquared(newVelocity));
//        velMagnitudeDword = TreatAsUdword(velMagnitude);

        etgEffectCreate(stat, NULL, &bullet->posinfo.position, &bullet->posinfo.velocity, &bullet->rotinfo.coordsys, 1.0f, 0, 1, intDamage);
    }
    */
}

void DefenseFighterHouseKeep(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    DefenseFighterStatics *defensefighterstatics;
    Node const* bulletnode;
    Node *tempnode;
    DefenseStruct *defensestruct;
    vector seperationvector;
    vector tempvec,rot_vector;
    GunStatic *gunstatic;
    Gun *gun;

    vector positionInWorldCoordSys;
    gun = &ship->gunInfo->guns[0];
    gunstatic = gun->gunstatic;

    defensefighterstatics = (DefenseFighterStatics *) ((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;

    bulletnode = spec->DefenseList.head;

    while (bulletnode != NULL)
    {   //as long as theres a bullet to deal with
        defensestruct = (DefenseStruct *)listGetStructOfNode(bulletnode);

/*        if (defensestruct->bullet)
            dbgMessagef("DS: %d %f %d %f %d %d",
                        universe.univUpdateCounter,
                        defensestruct->bullet->collOptimizeDist,
                        defensestruct->laser ? 1:0,
                        defensestruct->CoolDownTime,
                        defensestruct->CoolDown ? 1:0,
                        defensestruct->LaserDead ? 1:0);
        else
            dbgMessagef("DS: %d N  %d %f %d %d",
                        universe.univUpdateCounter,
                        defensestruct->laser ? 1:0,
                        defensestruct->CoolDownTime,
                        defensestruct->CoolDown ? 1:0,
                        defensestruct->LaserDead ? 1:0);*/

        if(!defensestruct->CoolDown)
        {   //laser cannon isn't cooling down, so continue tracking
            if (defensestruct->bullet == NULL || defensestruct->bullet->damage <= 0)
            {    //bullet is already dead...don't kill it again
                defensestruct->bullet = NULL;   //destroy pointer...
                if (defensestruct->laser != NULL)
                {
                    defensestruct->laser->timelived =10000.0f; //kill laser
                }
                defensestruct->LaserDead = TRUE;    //set killed flag
                defensestruct->laser=NULL;
                defensestruct->CoolDown = TRUE;     //begin laser cooldown
                defensestruct->CoolDownTime = 0.0f;    //reset to 0 for cooldown count
                bulletnode=bulletnode->next;
                continue;
            }
            if(UNIVERSE_WOODPECKER(defensefighterstatics->DamageRate, 0))
            {
                // Time to do damage...
                //Do damage to bullet
                defensestruct->bullet->damage = (defensestruct->bullet->damage - frandombetween(defensefighterstatics->DamageReductionLow,defensefighterstatics->DamageReductionHigh));
                if(defensestruct->bullet->damage <= 0)
                {
                    //bullet is destroyed
                    DefenseFighterDestroyedABullet(ship, defensestruct->bullet, defensestruct->laser);
                    defensestruct->bullet->damage = 0;      //cap at 0;
                    defensestruct->bullet = NULL;   //destroy pointer...
                    //dbgMessagef("Defense Fighter Destroyed A Bullet.");
                    if (defensestruct->laser != NULL)
                    {
                        defensestruct->laser->timelived =10000.0f; //kill laser
                    }
                    defensestruct->LaserDead = TRUE;    //set killed flag
                    if(defensestruct->laser->effect != NULL)
                    {
                        univRemoveObjFromRenderList((SpaceObj *) defensestruct->laser->effect);
                    }
                    defensestruct->laser=NULL;
                    defensestruct->CoolDown = TRUE;     //begin laser cooldown
                    defensestruct->CoolDownTime = 0.0f;    //reset to 0 for cooldown count
                }
            }
            if(defensestruct->laser != NULL)
            {
                //check if bullet is still in range and infront...
                if(UNIVERSE_WOODPECKER(defensefighterstatics->RangeCheckRate, 0))
                {
                    //time to check if in front
                    vecSub(seperationvector, ship->posinfo.position, defensestruct->bullet->posinfo.position);
                    if(vecMagnitudeSquared(seperationvector) > ship->staticinfo->bulletRangeSquared[ship->tacticstype])
                    {
                        //bullet is out of range
                        defensestruct->laser->timelived =10000.0f; //kill laser
                        defensestruct->LaserDead = TRUE;    //set killed flag
                        if(defensestruct->laser->effect != NULL)
                        {
                            univRemoveObjFromRenderList((SpaceObj *) defensestruct->laser->effect);
                        }
                        defensestruct->laser=NULL;
                        //dbgMessagef("Bullet out of range.");
                        bitClear(defensestruct->bullet->SpecialEffectFlag,0x0002);
                        defensestruct->CoolDown = TRUE;     //begin laser cooldown
                        defensestruct->bullet = NULL;       //set target to NULL so it isn't referenced again!
                        defensestruct->CoolDownTime = 0.0f;    //reset to 0 for cooldown count
                    }
                    else if(!defensefighterCheckInFront(ship, defensestruct->bullet))
                    {
                        //if bullet ISN'T in front
                        defensestruct->laser->timelived =10000.0f; //kill laser
                        defensestruct->LaserDead = TRUE;    //set killed flag
                        if(defensestruct->laser->effect != NULL)
                        {
                            univRemoveObjFromRenderList((SpaceObj *) defensestruct->laser->effect);
                        }
                        defensestruct->laser=NULL;
                        //dbgMessagef("Bullet Not infront anymore...stop tracking.");
                        if(defensefighterstatics->MultipleTargettingofSingleBullet)
                        {
                            bitClear(defensestruct->bullet->SpecialEffectFlag,0x0002);
                        }
                        defensestruct->CoolDown = TRUE;     //begin laser cooldown
                        defensestruct->bullet = NULL;       //set target to NULL so it isn't referenced again!
                        defensestruct->CoolDownTime = 0.0f;    //reset to 0 for cooldown count
                    }

                }
            }
            //This code is for the sake of the visual effect which STILL won't work
            //properly!!!  So it is temperary only!
            if(defensestruct->laser != NULL)
            {   //update bullent info...for visual effect
                dbgAssertOrIgnore(defensestruct->bullet != NULL);
                matMultiplyMatByVec(&positionInWorldCoordSys,&ship->rotinfo.coordsys,&gunstatic->position);
                vecAdd(defensestruct->laser->posinfo.position,positionInWorldCoordSys,ship->posinfo.position);
                vecSub(defensestruct->laser->lengthvec, defensestruct->bullet->posinfo.position, defensestruct->laser->posinfo.position);

                // heading
                tempvec = defensestruct->laser->lengthvec;
                vecNormalize(&tempvec);

                //matMultiplyMatByVec(&gunheadingInWorldCoordSys, &ship->rotinfo.coordsys, &tempvec);
                //laser->bulletheading = gunheadingInWorldCoordSys;
                defensestruct->laser->bulletheading = tempvec;
                matCreateCoordSysFromHeading(&defensestruct->laser->rotinfo.coordsys,&tempvec);
                if(defensestruct->laser->effect != NULL)
                {    //adjust length
                    ((real32 *)defensestruct->laser->effect->variable)[ETG_LengthVariable] =
                        fsqrt(vecMagnitudeSquared(defensestruct->laser->lengthvec));
                }
            }

        }
        else
        {    //this laser cannon is cooling, so cool it
            defensestruct->CoolDownTime += universe.phystimeelapsed;
            if(defensestruct->CoolDownTime > defensefighterstatics->CoolDownTimePerLaser)
            {    //Laser Terminal has cooled down...so free it up
                tempnode = bulletnode->next;
                listDeleteNode(bulletnode);
                //dbgMessagef("Deleting defense node in CoolDown.");
                bulletnode = tempnode;
                continue;
            }
        }
        bulletnode = bulletnode->next;
    }

    if (spec->DefenseList.num == 0)
    {
        soundEventBurstStop(ship, gun);
    }

    //do special rotation if neccessary
    if(bitTest(ship->dontrotateever,1))
    {
        //matGetVectFromMatrixCol3(rot_vector,ship->rotinfo.coordsys);
        vecSet(rot_vector,100.0f,0.0f,0.0f);
        vecAddTo(ship->rotinfo.torque, rot_vector);
        vecCapVectorSloppy(&ship->rotinfo.rotspeed, defensefighterstatics->max_rot_speed );
        spec->rotate_time_counter -= universe.phystimeelapsed;
        if(spec->rotate_time_counter <= 0.0f)
        {
            bitClear(ship->dontrotateever,1);
            spec->DefenseFighterCanNowRotate = FALSE;
        }
    }
    else if (spec->DefenseFighterCanNowRotate == FALSE)
    {
        spec->rotate_time_counter += universe.phystimeelapsed;
        if(spec->rotate_time_counter >= defensefighterstatics->rotate_recover_time)
        {
            spec->DefenseFighterCanNowRotate = TRUE;
            spec->rotate_time_counter = defensefighterstatics->rotate_time;
        }

    }

}

/*-----------------------------------------------------------------------------
    Name        : defenseFighterAdjustLaser
    Description : Called by render.c to calculate the length vec of the laser
                at render time
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

void defenseFighterAdjustLaser(Bullet *laser)
{       //calculates laser direction n' such for rendering...
    DefenseFighterSpec *spec;
 //   DefenseFighterStatics *defensefighterstatics;
    Node const* bulletnode;
    DefenseStruct const* defensestruct;
    Ship *ship;

    vector tempvec;
    GunStatic *gunstatic;
    Gun *gun;

    vector positionInWorldCoordSys;

    if(laser->owner == NULL)
        return;

    ship = laser->owner;

    dbgAssertOrIgnore(ship->gunInfo != NULL);
    gun = &ship->gunInfo->guns[0];
    gunstatic = gun->gunstatic;

    dbgAssertOrIgnore(ship->shiptype == DefenseFighter);
    dbgAssertOrIgnore(ship->sizeofShipSpecifics == sizeof(DefenseFighterSpec));
    spec= (DefenseFighterSpec *)ship->ShipSpecifics;

//    defensefighterstatics = (DefenseFighterStatics *) ((ShipStaticInfo *)(ship->staticinfo))->custstatinfo;

    bulletnode = spec->DefenseList.head;

    while (bulletnode != NULL)
    {   //as long as theres a bullet to deal with
        defensestruct = bulletnode->structptr;
        if(defensestruct->LaserDead == TRUE)
        {
            bulletnode = bulletnode->next;
            continue;
        }
        if(laser == defensestruct->laser)
        {
            dbgAssertOrIgnore(defensestruct->bullet != NULL);
            matMultiplyMatByVec(&positionInWorldCoordSys,&ship->rotinfo.coordsys,&gunstatic->position);
            vecAdd(laser->posinfo.position,positionInWorldCoordSys,ship->posinfo.position);
            vecSub(laser->lengthvec, defensestruct->bullet->posinfo.position, laser->posinfo.position);

            // heading
            tempvec = defensestruct->laser->lengthvec;
            vecNormalize(&tempvec);

            //matMultiplyMatByVec(&gunheadingInWorldCoordSys, &ship->rotinfo.coordsys, &tempvec);
            //laser->bulletheading = gunheadingInWorldCoordSys;
            defensestruct->laser->bulletheading = tempvec;
            matCreateCoordSysFromHeading(&defensestruct->laser->rotinfo.coordsys,&tempvec);
            if(defensestruct->laser->effect != NULL)
            {    //adjust length
                ((real32 *)defensestruct->laser->effect->variable)[ETG_LengthVariable] =
                    fsqrt(vecMagnitudeSquared(defensestruct->laser->lengthvec));
            }
            return;
        }
        bulletnode = bulletnode->next;
    }
    //we shouldn't get to this point :)
    //dbgMessagef("Laser Drawn doesn't exist in defense fighter records.");

    //on further reflection..if we kill the laster...but it doesn't go bye bye, we might get here...
}

/*-----------------------------------------------------------------------------
    Name        : DefenseFighterBulletRemoval
    Description : searches the universe.shiplist for all defense fighters and
                  searches their local lists to remove any references to the
                  passed bullet
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/

void DefenseFighterBulletRemoval(Bullet *bullettogoByeBye)
{
    Node *shipnode = universe.ShipList.head;
    Node const* defnode;
    Ship *ship;
    DefenseFighterSpec *spec;
    DefenseStruct *defstruct;

    while(shipnode != NULL)
    {
        ship = (Ship *) listGetStructOfNode(shipnode);
        if(ship->shiptype == DefenseFighter)
        {   //ship is a defensefighter
            spec= (DefenseFighterSpec *)ship->ShipSpecifics;
            defnode = spec->DefenseList.head;

            while(defnode != NULL)
            {
                defstruct = (DefenseStruct *)listGetStructOfNode(defnode);
                if(defstruct->bullet == bullettogoByeBye)
                {   //bullet is being killed, so stop targetting
                    if(defstruct->LaserDead != TRUE)
                    {
                        defstruct->laser->timelived = 10000.0f;    //if laser isn't dead
                        if(defstruct->laser->effect != NULL)
                        {
                            univRemoveObjFromRenderList((SpaceObj *) defstruct->laser->effect);
                        }
                        defstruct->laser = NULL;        //remove pointer to laser...
                        defstruct->LaserDead = TRUE;    //signify dead laser!!!!!!!!!!
                        //dbgMessagef("Defense Targetted Bullet Died...timing out laser.");
                    }
                    defstruct->CoolDown = TRUE;     //Start or continue cool down process
                    defstruct->bullet = NULL;       //set target to NULL so it isn't referenced again!
                    defstruct->CoolDownTime = 0.0f;
                    break;      //done for this defense fighter...maybe more so can't return
                }
                defnode=defnode->next;
            }

        }
        shipnode = shipnode->next;
    }
}
void DefenseFighterDied(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    Node const* bulletnode;
    Node const* tempnode;
    DefenseStruct *defensestruct;

    bulletnode = spec->DefenseList.head;
    dbgMessagef("DefenseFighter Died: Cleaning up.");
    while(bulletnode != NULL)
    {
        defensestruct = (DefenseStruct *)listGetStructOfNode(bulletnode);
        if(defensestruct->LaserDead != TRUE)
        {
            //listRemoveNode(&defensestruct->laser->bulletlink);           //removefrom bullet list too?
            if (defensestruct->laser != NULL)
            {
                univRemoveObjFromRenderList((SpaceObj *) defensestruct->laser);
                defensestruct->laser->timelived = 10000.0f;
                bitSet(defensestruct->laser->flags,SOF_Hide);
            }
            //listDeleteNode(&defensestruct->laser->objlink);
            //dbgMessagef("Defense Dead: Deleting Laser from existance");
            soundEventBurstStop(ship, &ship->gunInfo->guns[0]);
        }
        tempnode = bulletnode->next;
        listDeleteNode(bulletnode);
        //dbgMessagef("Defense Dead: Deleting defense node.");

        bulletnode = tempnode;
    }
}

bool32 DefenseFighterSpecialActivate(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;

    if(bitTest(ship->dontrotateever,1))
    {
        //Turn off Special Ops
        spec->DefenseFighterCanNowRotate = FALSE;       //signifies we need to re-charge for a bit
        bitClear(ship->dontrotateever,1);
    }
    else
    {
        //turn on special ops
        if(spec->DefenseFighterCanNowRotate)
        {
            //only turn on if we can rotate now...
            bitSet(ship->dontrotateever,1);
        }
    }
    return TRUE;
}

void DefenseFighterAttack(Ship *ship,SpaceObjRotImpTarg *target,real32 maxdist)
{
    vector trajectory;
    real32 range;

    aishipGetTrajectory(ship,target,&trajectory);
    range = RangeToTarget(ship,target,&trajectory);

    if (range < 1000.0f)
    {
        aitrackZeroVelocity(ship);
        vecNormalize(&trajectory);
        aitrackHeading(ship,&trajectory,0.9999f);
    }
    else
    {
        aishipFlyToPointAvoidingObjs(ship,&target->posinfo.position,AISHIP_PointInDirectionFlying,0.0f);
    }
}

void DefenseFighterPassiveAttack(Ship *ship,Ship *target,bool32 rotate)
{
    vector heading;
    if(!rotate)
        return;
    vecSub(heading,target->posinfo.position,ship->posinfo.position);
    vecNormalize(&heading);
    aitrackHeading(ship,&heading,0.9999f);
}

#ifdef _WIN32_FIX_ME
    #pragma warning( 4 : 4047)      // turns off "different levels of indirection warning"
#endif

void SaveDefenseStruct(DefenseStruct *defenseStruct)
{
    SaveChunk *chunk;
    DefenseStruct *savecontents;

    chunk = CreateChunk(BASIC_STRUCTURE,sizeof(DefenseStruct),defenseStruct);
    savecontents = (DefenseStruct *)chunkContents(chunk);

    savecontents->bullet = (Bullet *) SpaceObjRegistryGetID((SpaceObj *)savecontents->bullet);
    savecontents->laser = (Bullet *) SpaceObjRegistryGetID((SpaceObj *)savecontents->laser);

    SaveThisChunk(chunk);
    memFree(chunk);
}

void DefenseFighter_Save(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    Node const* node = spec->DefenseList.head;
    sdword cur = 0;

    SaveInfoNumber(spec->DefenseList.num);

    while (node != NULL)
    {
        cur++;
        SaveDefenseStruct((DefenseStruct *)listGetStructOfNode(node));
        node = node->next;
    }

    dbgAssertOrIgnore(cur == spec->DefenseList.num);
}

DefenseStruct *LoadDefenseStruct(void)
{
    SaveChunk *chunk;
    DefenseStruct *defenseStruct;

    chunk = LoadNextChunk();
    VerifyChunk(chunk,BASIC_STRUCTURE,sizeof(DefenseStruct));

    defenseStruct = memAlloc(sizeof(DefenseStruct),"DefenseStruct",0);
    memcpy(defenseStruct,chunkContents(chunk),sizeof(DefenseStruct));

    memFree(chunk);

    return defenseStruct;
}

void DefenseFighter_Load(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    sdword num;
    sdword i;
    DefenseStruct *defenseStruct;

    num = LoadInfoNumber();

    listInit(&spec->DefenseList);

    for (i=0;i<num;i++)
    {
        defenseStruct = LoadDefenseStruct();
        listAddNode(&spec->DefenseList,&defenseStruct->bulletnode,defenseStruct);
    }
}

void DefenseFighter_Fix(Ship *ship)
{
    DefenseFighterSpec *spec = (DefenseFighterSpec *)ship->ShipSpecifics;
    Node const* node = spec->DefenseList.head;
    DefenseStruct *defenseStruct;

    while (node != NULL)
    {
        defenseStruct = (DefenseStruct *)listGetStructOfNode(node);

        defenseStruct->bullet = SpaceObjRegistryGetBullet((sdword)defenseStruct->bullet);
        defenseStruct->laser = SpaceObjRegistryGetBullet((sdword)defenseStruct->laser);

        node = node->next;
    }
}

#ifdef _WIN32_FIX_ME
    #pragma warning( 2 : 4047)      // turn back on "different levels of indirection warning"
#endif

CustShipHeader DefenseFighterHeader =
{
    DefenseFighter,
    sizeof(DefenseFighterSpec),
    DefenseFighterStaticInit,
    NULL,
    DefenseFighterInit,
    DefenseFighterDied,
    DefenseFighterAttack,
    NULL,
    DefenseFighterPassiveAttack,
    DefenseFighterSpecialActivate,
    NULL,
    DefenseFighterHouseKeep,
    NULL,
    DefenseFighterDied,
    NULL,
    DefenseFighter_Save,
    DefenseFighter_Load,
    DefenseFighter_Fix
};
