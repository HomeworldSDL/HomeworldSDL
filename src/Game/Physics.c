/*=============================================================================
    Name    : Physics.c
    Purpose : Implements space physics of Homeworld

    Created 6/18/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "Physics.h"

#include <math.h>

#include "Attributes.h"
#include "Battle.h"
#include "Collision.h"
#include "Debug.h"
#include "FastMath.h"
#include "Gun.h"
#include "Matrix.h"
#include "MultiplayerGame.h"
#include "Select.h"
#include "SoundEvent.h"
#include "Tactics.h"
#include "Tweak.h"
#include "Universe.h"
#include "UnivUpdate.h"

#define UPDATE_COLLRECT_RATE    3
#define UPDATE_COLLRECT_FRAME   3

#define TEST_FUEL 0

/*-----------------------------------------------------------------------------
    Name        : physApplyForceToObj
    Description : applies a force in direction forcedir of magnitude force to
                  the object.  The force is translated into the mission sphere
                  coordinate system.
    Inputs      : obj,force,forcedir
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physApplyForceToObj(SpaceObj *obj,real32 force,uword transdir)
{
    vector vectorforce = VECTOR_ORIGIN;

    dbgAssertOrIgnore((obj->flags & SOF_Rotatable) != 0);

    if (obj->objtype == OBJ_ShipType)
    {
        if(((Ship *)obj)->dontapplyforceever)
        {
            //we don't want to try and fly anywheres with this ship obviously!
            return;
        }

        if (obj->flags & SOF_Crazy)
        {
            return;     // ship is crazy, so return
        }
    }

    switch (transdir)
    {
        case TRANS_UP:
        case TRANS_DOWN:
            matGetVectFromMatrixCol1(vectorforce,((SpaceObjRot *)obj)->rotinfo.coordsys);
            break;

        case TRANS_RIGHT:
        case TRANS_LEFT:
            matGetVectFromMatrixCol2(vectorforce,((SpaceObjRot *)obj)->rotinfo.coordsys);
            break;

        case TRANS_FORWARD:
        case TRANS_BACKWARD:
            matGetVectFromMatrixCol3(vectorforce,((SpaceObjRot *)obj)->rotinfo.coordsys);
            break;
    }

    if (transdir & 1)
    {
        force = -force;
    }

    vecMultiplyByScalar(vectorforce,force);
    vecAddTo(obj->posinfo.force,vectorforce);
}

/*-----------------------------------------------------------------------------
    Name        : physApplyRotToObj
    Description : applies a torque to an object about a rotation axis
                  and direction indicated by rotdir.
    Inputs      : torque, rotdir
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physApplyRotToObj(SpaceObjRot *obj,real32 torque,uword rotdir)
{
    dbgAssertOrIgnore((obj->flags & SOF_Rotatable) != 0);

    if (obj->objtype == OBJ_ShipType)
    {
        if (obj->flags & SOF_Crazy)
        {
            return;     // ship is crazy, so return
        }
    }

    if (rotdir & 1)
    {
        torque = -torque;
    }

    switch (rotdir)
    {
        case ROT_ABOUTXCCW:
        case ROT_ABOUTXCW:
            obj->rotinfo.torque.x += torque;
            break;

        case ROT_ABOUTYCCW:
        case ROT_ABOUTYCW:
            obj->rotinfo.torque.y += torque;
            break;

        case ROT_ABOUTZCCW:
        case ROT_ABOUTZCW:
            obj->rotinfo.torque.z += torque;
            break;
    }
}

/*-----------------------------------------------------------------------------
    Name        : physUpdateBulletPosVel
    Description : updates a bullets position
    Inputs      : phystimeelapsed - time elapsed since last physics frame
    Outputs     :
    Return      : true if the bullet should now be deleted
----------------------------------------------------------------------------*/
bool32 physUpdateBulletPosVel(Bullet *bullet,real32 phystimeelapsed)
{
    vector d;
    Ship *ship;
    vector gunheadingInWorldCoordSys;
    vector positionInWorldCoordSys, offset1, offset0;
    Gun *gun;
    GunStatic *gunstatic;

    switch (bullet->bulletType)
    {
        case BULLET_Beam:
            ship = bullet->owner;
            if (ship != NULL)
            {
                gun = bullet->gunowner;
                if (gun != NULL)
                {
                    gunstatic = gun->gunstatic;

                    if (bullet->target)
                    {
                        switch (gunstatic->guntype)
                        {
                            case GUN_Gimble:
                            case GUN_NewGimble:
                                gunOrientGimbleGun(ship,gun,bullet->target);
                                break;
                            default:
                                break;
                        }
                    }

                    // update beam's position based on owner

                    // heading
                    matMultiplyMatByVec(&gunheadingInWorldCoordSys,&ship->rotinfo.coordsys,&gun->gunheading);
                    bullet->bulletheading = gunheadingInWorldCoordSys;
                    matCreateCoordSysFromHeading(&bullet->rotinfo.coordsys,&gunheadingInWorldCoordSys);

                    // length of bullet
                    vecScalarMultiply(bullet->lengthvec,gunheadingInWorldCoordSys,bullet->beamtraveldist);

                    // position
                    matMultiplyMatByVec(&positionInWorldCoordSys,&ship->rotinfo.coordsys,&gunstatic->position);
                    if (gunstatic->offset.x != 0.0f)
                    {                                                       //if there is an offset vector
                        matMultiplyMatByVec(&offset0, &gun->curgunorientation, &gunstatic->offset);//offset in gun space
                        matMultiplyMatByVec(&offset1, &ship->rotinfo.coordsys, &offset0);//offset in world space
                        vecAdd(positionInWorldCoordSys, positionInWorldCoordSys, offset1);//add to position
                    }

                    vecAdd(bullet->posinfo.position,positionInWorldCoordSys,ship->posinfo.position);
                }
            }
            break;
        case BULLET_Laser:  //don't do move lasers
            break;
        default:
            // d = vt
            vecScalarMultiply(d,bullet->posinfo.velocity,phystimeelapsed);

            // x = x + d
            vecAddTo(bullet->posinfo.position,d);

            break;
    }

    SET_MOVING_LINEARLY(bullet->posinfo.isMoving);

    bullet->timelived += phystimeelapsed;
    if (bullet->timelived > bullet->totallifetime)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#define robj ((SpaceObjRot *)obj)
#define irobj ((SpaceObjRotImp *)obj)
#define shipobj ((Ship *)obj)

/*-----------------------------------------------------------------------------
    Name        : physUpdateObjPosVel
    Description : !!!!! THIS IS NO LONGER USED!!!! USE THE CUSTOM FUNCTIONS
                  BELOW!!!
                  updates a space objects position and orientation based
                  on any external forces/torques, as well as its current
                  velocity/spin.
    Inputs      : phystimeelapsed - time elapsed since last physics frame
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physUpdateObjPosVel(SpaceObj *obj,real32 phystimeelapsed)
{
    vector a;
    vector d;
    StaticHeader *staticheader = &obj->staticinfo->staticheader;
    ShipStaticInfo *shipstaticinfo = FALSE;
    bool32 coordsysChanged = FALSE;
    bool32 wasAboveLowFuel;
    //real32 burnmult;
    real32 speedmult;
    matrix rotxmatrix;
    matrix rotymatrix;
    matrix rotzmatrix;
    matrix tempmat;
    real32 angxrotate;
    real32 angyrotate;
    real32 angzrotate;

/*-----------------------------------------------------------------------------
    Update object position
-----------------------------------------------------------------------------*/

    // a = F/m
    vecScalarMultiply(a,obj->posinfo.force,staticheader->oneOverMass);

    // a = at
    vecMultiplyByScalar(a,phystimeelapsed);

    // v = v + at
    vecAddTo(obj->posinfo.velocity,a);

    if (Real32ToUdword(obj->posinfo.velocity.x) | Real32ToUdword(obj->posinfo.velocity.y) | Real32ToUdword(obj->posinfo.velocity.z))
    {
        SET_MOVING_LINEARLY(obj->posinfo.isMoving);

        if(obj->objtype == OBJ_MissileType)         //replace with a better method later
        {
            if(((Missile *)obj)->FORCE_DROPPED == TRUE)
            {
                // cap velocity vector
                vecCapVectorSloppy(&obj->posinfo.velocity,((MissileStaticInfo *)((Missile *)obj)->staticinfo)->maxvelocity_FORCED);
            }
            else
            {
                // cap velocity vector
                vecCapVectorSloppy(&obj->posinfo.velocity,staticheader->maxvelocity);
            }
        }
        else
        {
            // cap velocity vector
            if(obj->objtype == OBJ_ShipType)
            {
                speedmult = tacticsGetShipsMaxVelocity(((Ship *)obj));
                vecCapVectorSloppy(&obj->posinfo.velocity,speedmult);
            }
            else
            {
                vecCapVectorSloppy(&obj->posinfo.velocity,staticheader->maxvelocity);
            }

        }

        // d = vt
        vecScalarMultiply(d,obj->posinfo.velocity,phystimeelapsed);

        // x = x + d
        vecAddTo(obj->posinfo.position,d);

    }
    else
    {
        SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
        vecZeroVector(d);
    }

    if (obj->objtype == OBJ_ShipType)
    {
        shipstaticinfo = (ShipStaticInfo *)((Ship *)obj)->staticinfo;
        if ((obj->flags & SOF_Dead) == 0)
        {
            if (shipobj->flags & (SOF_Crazy|SOF_Disabled))
            {
                if (obj->posinfo.isMoving)
                {
                    if (!(Real32ToUdword(a.x) | Real32ToUdword(a.y) | Real32ToUdword(a.z)))   // if object is moving, but has no acceleration
                    {
                        if ((isBetweenExclusive(obj->posinfo.velocity.x,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                            (isBetweenExclusive(obj->posinfo.velocity.y,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                            (isBetweenExclusive(obj->posinfo.velocity.z,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) )
                        {
                            SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
                            vecZeroVector(d);
                            vecZeroVector(obj->posinfo.velocity);
                        }
                        else
                        {
                            obj->posinfo.velocity.x *= SHIP_CRAZY_FRICTION;
                            obj->posinfo.velocity.y *= SHIP_CRAZY_FRICTION;
                            obj->posinfo.velocity.z *= SHIP_CRAZY_FRICTION;
                        }
                    }
                }
            }
            else if ((shipstaticinfo->maxfuel != 0.0f) && obj->posinfo.isMoving && ((obj->flags & SOF_Clamped) == 0) )  // clamped objs don't burn fuel
            {
                // ship burns fuel
                if ( ((Ship *)obj)->fuel > 0.0f)
                {
                    if (((Ship *)obj)->fuel > shipstaticinfo->lowfuelpoint)
                    {
                        wasAboveLowFuel = TRUE;
                    }
                    else
                    {
                        wasAboveLowFuel = FALSE;
                    }

                    if ((singlePlayerGame) || (bitTest(tpGameCreated.flag,MG_FuelBurnEnabled)))
                    {
                        if (((Ship *)obj)->shipisattacking)
                        {
                            ((Ship *)obj)->fuel -= shipstaticinfo->attackfuelburnrate * getVectDistSloppy(d); // fsqrt(vecMagnitudeSquared(d));//*burnmult;
                        }
                        else
                        {
                            ((Ship *)obj)->fuel -= shipstaticinfo->fuelburnrate * getVectDistSloppy(d); //fsqrt(vecMagnitudeSquared(d));//*burnmult;
                        }
                    }
#if TEST_FUEL
                    dbgMessagef("Fuel %f for Ship %x",((Ship *)obj)->fuel,(udword)obj);
#endif

                    if (((Ship *)obj)->playerowner == universe.curPlayerPtr)
                    {
                        if (((Ship *)obj)->fuel <= 0.0f)
                        {
                            // made transition from some fuel to out of fuel
//                            speechEvent(obj, STAT_Strike_OutOfFuel, 0);
                            if (battleCanChatterAtThisTime(BCE_OutOfFuel, (Ship *)obj))
                            {
                                if(!selAnyHotKeyTest((Ship *)obj))
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_OutOfFuel, (Ship *)obj, SOUND_EVENT_DEFAULT);
                                }
                                else
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_OutOfFuel, (Ship *)obj, selHotKeyGroupNumberTest((Ship *)obj));
                                }
                            }
                        }
                        else if ((wasAboveLowFuel) && (((Ship *)obj)->fuel <= shipstaticinfo->lowfuelpoint))
                        {
                            // made transition from regular fuel to low on fuel
//                            speechEvent(obj, STAT_Strike_LowOnFuel, 0);
                            if (battleCanChatterAtThisTime(BCE_FuelLow, (Ship *)obj))
                            {
                                if(!selAnyHotKeyTest((Ship *)obj))
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_FuelLow, (Ship *)obj, SOUND_EVENT_DEFAULT);
                                }
                                else
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_FuelLow, (Ship *)obj, selHotKeyGroupNumberTest((Ship *)obj));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (obj->objtype == OBJ_DerelictType)
    {
        // space friction for derelicts
        if (obj->posinfo.isMoving)
        {
            if (!(Real32ToUdword(a.x) | Real32ToUdword(a.y) | Real32ToUdword(a.z)))   // if object is moving, but has no acceleration
            {
                if ((isBetweenExclusive(obj->posinfo.velocity.x,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                    (isBetweenExclusive(obj->posinfo.velocity.y,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                    (isBetweenExclusive(obj->posinfo.velocity.z,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) )
                {
                    SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
                    vecZeroVector(d);
                    vecZeroVector(obj->posinfo.velocity);
                }
                else
                {
                    obj->posinfo.velocity.x *= DONT_DRIFT_FRICTION;
                    obj->posinfo.velocity.y *= DONT_DRIFT_FRICTION;
                    obj->posinfo.velocity.z *= DONT_DRIFT_FRICTION;
                }
            }
        }
    }

    if (bitTest(obj->flags,SOF_Rotatable))
    {

    /*-----------------------------------------------------------------------------
        Update object orientation
    -----------------------------------------------------------------------------*/

        if ((robj->rotinfo.torque.x != 0) || (robj->rotinfo.torque.y != 0) || (robj->rotinfo.torque.z != 0))
        {

            // alpha = Torque/momentOfInertia (about each axis)
            a.x = robj->rotinfo.torque.x * staticheader->oneOverMomentOfInertiaX;
            a.y = robj->rotinfo.torque.y * staticheader->oneOverMomentOfInertiaY;
            a.z = robj->rotinfo.torque.z * staticheader->oneOverMomentOfInertiaZ;

            // alpha = alpha*t
            vecMultiplyByScalar(a,phystimeelapsed);

            // w = w + alpha*t
            vecAddTo(robj->rotinfo.rotspeed,a);

            // cap rotation speed
            vecCapVectorSloppy(&robj->rotinfo.rotspeed,staticheader->maxrot);

        }

        // We now have w.x, w.y, and w.z which indicate the
        // rotational velocities (rad/s) around the x,y, and z axes of the ship.

    /*-----------------------------------------------------------------------------
        Update coordsys based on changes in rotation
    -----------------------------------------------------------------------------*/

        if (robj->rotinfo.rotspeed.x)
        {
            angxrotate = robj->rotinfo.rotspeed.x * phystimeelapsed;

            matMakeRotAboutX(&rotxmatrix,(real32)cos(angxrotate),(real32)sin(angxrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotxmatrix);
            coordsysChanged = TRUE;
        }

        if (robj->rotinfo.rotspeed.y)
        {
            angyrotate = robj->rotinfo.rotspeed.y * phystimeelapsed;

            matMakeRotAboutY(&rotymatrix,(real32)cos(angyrotate),(real32)sin(angyrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotymatrix);
            coordsysChanged = TRUE;
        }

        if (robj->rotinfo.rotspeed.z)
        {
            angzrotate = robj->rotinfo.rotspeed.z * phystimeelapsed;

            matMakeRotAboutZ(&rotzmatrix,(real32)cos(angzrotate),(real32)sin(angzrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotzmatrix);
            coordsysChanged = TRUE;
        }

        if (bitTest(obj->flags,SOF_Impactable))
        {
            if (coordsysChanged)
            {
                matMultiplyMatByVec(&irobj->collInfo.collOffset,&(irobj->rotinfo.coordsys),&staticheader->staticCollInfo.collsphereoffset);
                if (UNIVERSE_WOODPECKER(UPDATE_COLLRECT_RATE, UPDATE_COLLRECT_FRAME))
                    collUpdateCollRectangle(irobj);
            }

            vecAdd(irobj->collInfo.collPosition,obj->posinfo.position,irobj->collInfo.collOffset);

            if (obj->objtype == OBJ_ShipType)
            {
                if (coordsysChanged)
                {
                    if ((shipstaticinfo->engineNozzleOffset[0].x != 0) || (shipstaticinfo->engineNozzleOffset[0].y != 0) || (shipstaticinfo->engineNozzleOffset[0].z != 0))
                    {
                        matMultiplyMatByVec(&shipobj->engineOffset,&(robj->rotinfo.coordsys),&shipstaticinfo->engineNozzleOffset[0]);
                    }
                }

                vecAdd(shipobj->enginePosition,obj->posinfo.position,shipobj->engineOffset);
            }
            else if (obj->objtype == OBJ_MissileType)
            {
                Missile* missile = (Missile*)obj;
                missile->enginePosition = obj->posinfo.position;
                if (missile->trail != NULL)
                {
                    if((universe.univUpdateCounter % UNIVERSE_UPDATE_RATE_FACTOR) == 0)
                    {
                        mistrailUpdate(missile->trail, &missile->enginePosition);
                    }
                }
            }
        }

        vecZeroVector(robj->rotinfo.torque);
    }

    vecZeroVector(obj->posinfo.force);
}

/*-----------------------------------------------------------------------------
    Name        : physUpdateObjPosVelShip
    Description : custom update function for SHIPS only
    Inputs      : phystimeelapsed - time elapsed since last physics frame
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physUpdateObjPosVelShip(Ship *obj,real32 phystimeelapsed)
{
    vector a;
    vector d;
    StaticHeader *staticheader = &obj->staticinfo->staticheader;
    ShipStaticInfo *shipstaticinfo;
    bool32 coordsysChanged = FALSE;
    bool32 wasAboveLowFuel;
    real32 speedmult;
    matrix rotxmatrix;
    matrix rotymatrix;
    matrix rotzmatrix;
    matrix tempmat;
    real32 angxrotate;
    real32 angyrotate;
    real32 angzrotate;

/*-----------------------------------------------------------------------------
    Update object position
-----------------------------------------------------------------------------*/

    // a = F/m
    vecScalarMultiply(a,obj->posinfo.force,staticheader->oneOverMass);

    // a = at
    vecMultiplyByScalar(a,phystimeelapsed);

    // v = v + at
    vecAddTo(obj->posinfo.velocity,a);

    if (Real32ToUdword(obj->posinfo.velocity.x) | Real32ToUdword(obj->posinfo.velocity.y) | Real32ToUdword(obj->posinfo.velocity.z))
    {
        SET_MOVING_LINEARLY(obj->posinfo.isMoving);

        // cap velocity vector
        speedmult = tacticsGetShipsMaxVelocity(((Ship *)obj));
        vecCapVectorSloppy(&obj->posinfo.velocity,speedmult);

        // d = vt
        vecScalarMultiply(d,obj->posinfo.velocity,phystimeelapsed);

        // x = x + d
        vecAddTo(obj->posinfo.position,d);

    }
    else
    {
        SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
        vecZeroVector(d);
    }

        shipstaticinfo = (ShipStaticInfo *)((Ship *)obj)->staticinfo;
        if ((obj->flags & SOF_Dead) == 0)
        {
            if (shipobj->flags & (SOF_Crazy|SOF_Disabled))
            {
                if (obj->posinfo.isMoving)
                {
                    if (!(Real32ToUdword(a.x) | Real32ToUdword(a.y) | Real32ToUdword(a.z)))   // if object is moving, but has no acceleration
                    {
                        if ((isBetweenExclusive(obj->posinfo.velocity.x,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                            (isBetweenExclusive(obj->posinfo.velocity.y,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                            (isBetweenExclusive(obj->posinfo.velocity.z,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) )
                        {
                            SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
                            vecZeroVector(d);
                            vecZeroVector(obj->posinfo.velocity);
                        }
                        else
                        {
                            obj->posinfo.velocity.x *= SHIP_CRAZY_FRICTION;
                            obj->posinfo.velocity.y *= SHIP_CRAZY_FRICTION;
                            obj->posinfo.velocity.z *= SHIP_CRAZY_FRICTION;
                        }
                    }
                }
            }
            else if ((shipstaticinfo->maxfuel != 0.0f) && obj->posinfo.isMoving && ((obj->flags & SOF_Clamped) == 0) )  // clamped objs don't burn fuel
            {
                // ship burns fuel
                if ( ((Ship *)obj)->fuel > 0.0f)
                {
                    if (((Ship *)obj)->fuel > shipstaticinfo->lowfuelpoint)
                    {
                        wasAboveLowFuel = TRUE;
                    }
                    else
                    {
                        wasAboveLowFuel = FALSE;
                    }

                    if ((singlePlayerGame) || (bitTest(tpGameCreated.flag,MG_FuelBurnEnabled)))
                    {
                        if (((Ship *)obj)->shipisattacking)
                        {
                            ((Ship *)obj)->fuel -= shipstaticinfo->attackfuelburnrate * getVectDistSloppy(d); // fsqrt(vecMagnitudeSquared(d));//*burnmult;
                        }
                        else
                        {
                            ((Ship *)obj)->fuel -= shipstaticinfo->fuelburnrate * getVectDistSloppy(d); //fsqrt(vecMagnitudeSquared(d));//*burnmult;
                        }
                    }
#if TEST_FUEL
                    dbgMessagef("Fuel %f for Ship %x",((Ship *)obj)->fuel,(udword)obj);
#endif

                    if (((Ship *)obj)->playerowner == universe.curPlayerPtr)
                    {
                        if (((Ship *)obj)->fuel <= 0.0f)
                        {
                            // made transition from some fuel to out of fuel
//                            speechEvent(obj, STAT_Strike_OutOfFuel, 0);
                            if (battleCanChatterAtThisTime(BCE_OutOfFuel, (Ship *)obj))
                            {
                                if(!selAnyHotKeyTest((Ship *)obj))
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_OutOfFuel, (Ship *)obj, SOUND_EVENT_DEFAULT);
                                }
                                else
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_OutOfFuel, (Ship *)obj, selHotKeyGroupNumberTest((Ship *)obj));
                                }
                            }
                        }
                        else if ((wasAboveLowFuel) && (((Ship *)obj)->fuel <= shipstaticinfo->lowfuelpoint))
                        {
                            // made transition from regular fuel to low on fuel
//                            speechEvent(obj, STAT_Strike_LowOnFuel, 0);
                            if (battleCanChatterAtThisTime(BCE_FuelLow, (Ship *)obj))
                            {
                                if(!selAnyHotKeyTest((Ship *)obj))
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_FuelLow, (Ship *)obj, SOUND_EVENT_DEFAULT);
                                }
                                else
                                {
                                    battleChatterAttempt(SOUND_EVENT_DEFAULT, BCE_FuelLow, (Ship *)obj, selHotKeyGroupNumberTest((Ship *)obj));
                                }
                            }
                        }
                    }
                }
            }
        }

    dbgAssertOrIgnore(bitTest(obj->flags,SOF_Rotatable));
    {

    /*-----------------------------------------------------------------------------
        Update object orientation
    -----------------------------------------------------------------------------*/

        if ((robj->rotinfo.torque.x != 0) || (robj->rotinfo.torque.y != 0) || (robj->rotinfo.torque.z != 0))
        {

            // alpha = Torque/momentOfInertia (about each axis)
            a.x = robj->rotinfo.torque.x * staticheader->oneOverMomentOfInertiaX;
            a.y = robj->rotinfo.torque.y * staticheader->oneOverMomentOfInertiaY;
            a.z = robj->rotinfo.torque.z * staticheader->oneOverMomentOfInertiaZ;

            // alpha = alpha*t
            vecMultiplyByScalar(a,phystimeelapsed);

            // w = w + alpha*t
            vecAddTo(robj->rotinfo.rotspeed,a);

            // cap rotation speed
            vecCapVectorSloppy(&robj->rotinfo.rotspeed,staticheader->maxrot);

        }

        // We now have w.x, w.y, and w.z which indicate the
        // rotational velocities (rad/s) around the x,y, and z axes of the ship.

    /*-----------------------------------------------------------------------------
        Update coordsys based on changes in rotation
    -----------------------------------------------------------------------------*/

        if (robj->rotinfo.rotspeed.x)
        {
            angxrotate = robj->rotinfo.rotspeed.x * phystimeelapsed;

            matMakeRotAboutX(&rotxmatrix,(real32)cos(angxrotate),(real32)sin(angxrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotxmatrix);
            coordsysChanged = TRUE;
            SET_MOVING_ROTATIONALLY(obj->posinfo.isMoving);
        }

        if (robj->rotinfo.rotspeed.y)
        {
            angyrotate = robj->rotinfo.rotspeed.y * phystimeelapsed;

            matMakeRotAboutY(&rotymatrix,(real32)cos(angyrotate),(real32)sin(angyrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotymatrix);
            coordsysChanged = TRUE;
            SET_MOVING_ROTATIONALLY(obj->posinfo.isMoving);
        }

        if (robj->rotinfo.rotspeed.z)
        {
            angzrotate = robj->rotinfo.rotspeed.z * phystimeelapsed;

            matMakeRotAboutZ(&rotzmatrix,(real32)cos(angzrotate),(real32)sin(angzrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotzmatrix);
            coordsysChanged = TRUE;
            SET_MOVING_ROTATIONALLY(obj->posinfo.isMoving);
        }

        if (bitTest(obj->flags,SOF_Impactable))
        {
            if (coordsysChanged)
            {
                matMultiplyMatByVec(&irobj->collInfo.collOffset,&(irobj->rotinfo.coordsys),&staticheader->staticCollInfo.collsphereoffset);
                if (UNIVERSE_WOODPECKER(UPDATE_COLLRECT_RATE, UPDATE_COLLRECT_FRAME))
                    collUpdateCollRectangle(irobj);
            }

            vecAdd(irobj->collInfo.collPosition,obj->posinfo.position,irobj->collInfo.collOffset);

            if (coordsysChanged)
            {
                if ((shipstaticinfo->engineNozzleOffset[0].x != 0) || (shipstaticinfo->engineNozzleOffset[0].y != 0) || (shipstaticinfo->engineNozzleOffset[0].z != 0))
                {
                    matMultiplyMatByVec(&obj->engineOffset,&(robj->rotinfo.coordsys),&shipstaticinfo->engineNozzleOffset[0]);
                }
            }
            vecAdd(shipobj->enginePosition,obj->posinfo.position,shipobj->engineOffset);
        }
        vecZeroVector(robj->rotinfo.torque);
    }
    vecZeroVector(obj->posinfo.force);
}

/*-----------------------------------------------------------------------------
    Name        : physUpdateObjPosVelDerelicts
    Description : updates a derelicts position and orientation based
                  on any external forces/torques, as well as its current
                  velocity/spin.
    Inputs      : phystimeelapsed - time elapsed since last physics frame
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physUpdateObjPosVelDerelicts(Derelict *obj,real32 phystimeelapsed)
{
    vector a;
    vector d;
    StaticHeader *staticheader = &obj->staticinfo->staticheader;
    bool32 coordsysChanged = FALSE;
    //real32 burnmult;
    matrix rotxmatrix;
    matrix rotymatrix;
    matrix rotzmatrix;
    matrix tempmat;
    real32 angxrotate;
    real32 angyrotate;
    real32 angzrotate;

/*-----------------------------------------------------------------------------
    Update object position
-----------------------------------------------------------------------------*/

    // a = F/m
    vecScalarMultiply(a,obj->posinfo.force,staticheader->oneOverMass);

    // a = at
    vecMultiplyByScalar(a,phystimeelapsed);

    // v = v + at
    vecAddTo(obj->posinfo.velocity,a);

    if (Real32ToUdword(obj->posinfo.velocity.x) | Real32ToUdword(obj->posinfo.velocity.y) | Real32ToUdword(obj->posinfo.velocity.z))
    {
        SET_MOVING_LINEARLY(obj->posinfo.isMoving);
        vecCapVectorSloppy(&obj->posinfo.velocity,staticheader->maxvelocity);
        // d = vt
        vecScalarMultiply(d,obj->posinfo.velocity,phystimeelapsed);
        // x = x + d
        vecAddTo(obj->posinfo.position,d);
    }
    else
    {
        SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
        vecZeroVector(d);
    }

    // space friction for derelicts
    if (obj->posinfo.isMoving)
    {
        if (!(Real32ToUdword(a.x) | Real32ToUdword(a.y) | Real32ToUdword(a.z)))   // if object is moving, but has no acceleration
        {
            if ((isBetweenExclusive(obj->posinfo.velocity.x,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                (isBetweenExclusive(obj->posinfo.velocity.y,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) &&
                (isBetweenExclusive(obj->posinfo.velocity.z,CONSIDERED_STILL_LO,CONSIDERED_STILL_HI)) )
            {
                SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
                vecZeroVector(d);
                vecZeroVector(obj->posinfo.velocity);
            }
            else
            {
                obj->posinfo.velocity.x *= DONT_DRIFT_FRICTION;
                obj->posinfo.velocity.y *= DONT_DRIFT_FRICTION;
                obj->posinfo.velocity.z *= DONT_DRIFT_FRICTION;
            }
        }
    }

    dbgAssertOrIgnore(bitTest(obj->flags,SOF_Rotatable));
    {

    /*-----------------------------------------------------------------------------
        Update object orientation
    -----------------------------------------------------------------------------*/

        if ((robj->rotinfo.torque.x != 0) || (robj->rotinfo.torque.y != 0) || (robj->rotinfo.torque.z != 0))
        {

            // alpha = Torque/momentOfInertia (about each axis)
            a.x = robj->rotinfo.torque.x * staticheader->oneOverMomentOfInertiaX;
            a.y = robj->rotinfo.torque.y * staticheader->oneOverMomentOfInertiaY;
            a.z = robj->rotinfo.torque.z * staticheader->oneOverMomentOfInertiaZ;

            // alpha = alpha*t
            vecMultiplyByScalar(a,phystimeelapsed);

            // w = w + alpha*t
            vecAddTo(robj->rotinfo.rotspeed,a);

            // cap rotation speed
            vecCapVectorSloppy(&robj->rotinfo.rotspeed,staticheader->maxrot);

        }

        // We now have w.x, w.y, and w.z which indicate the
        // rotational velocities (rad/s) around the x,y, and z axes of the ship.

    /*-----------------------------------------------------------------------------
        Update coordsys based on changes in rotation
    -----------------------------------------------------------------------------*/

        if (robj->rotinfo.rotspeed.x)
        {
            angxrotate = robj->rotinfo.rotspeed.x * phystimeelapsed;

            matMakeRotAboutX(&rotxmatrix,(real32)cos(angxrotate),(real32)sin(angxrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotxmatrix);
            coordsysChanged = TRUE;
            SET_MOVING_ROTATIONALLY(obj->posinfo.isMoving);
        }

        if (robj->rotinfo.rotspeed.y)
        {
            angyrotate = robj->rotinfo.rotspeed.y * phystimeelapsed;

            matMakeRotAboutY(&rotymatrix,(real32)cos(angyrotate),(real32)sin(angyrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotymatrix);
            coordsysChanged = TRUE;
            SET_MOVING_ROTATIONALLY(obj->posinfo.isMoving);
        }

        if (robj->rotinfo.rotspeed.z)
        {
            angzrotate = robj->rotinfo.rotspeed.z * phystimeelapsed;

            matMakeRotAboutZ(&rotzmatrix,(real32)cos(angzrotate),(real32)sin(angzrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotzmatrix);
            coordsysChanged = TRUE;
            SET_MOVING_ROTATIONALLY(obj->posinfo.isMoving);
        }

        if (bitTest(obj->flags,SOF_Impactable))
        {
            if (coordsysChanged)
            {
                matMultiplyMatByVec(&irobj->collInfo.collOffset,&(irobj->rotinfo.coordsys),&staticheader->staticCollInfo.collsphereoffset);
                if (UNIVERSE_WOODPECKER(UPDATE_COLLRECT_RATE, UPDATE_COLLRECT_FRAME))
                    collUpdateCollRectangle(irobj);
            }

            vecAdd(irobj->collInfo.collPosition,obj->posinfo.position,irobj->collInfo.collOffset);
        }
        vecZeroVector(robj->rotinfo.torque);
    }
    vecZeroVector(obj->posinfo.force);
}

/*-----------------------------------------------------------------------------
    Name        : physUpdateObjPosVelMissile
    Description : updates a missiles position and orientation based
                  on any external forces/torques, as well as its current
                  velocity/spin.
    Inputs      : phystimeelapsed - time elapsed since last physics frame
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physUpdateObjPosVelMissile(Missile *obj,real32 phystimeelapsed)
{
    vector a;
    vector d;
    StaticHeader *staticheader = &obj->staticinfo->staticheader;
    bool32 coordsysChanged = FALSE;
    matrix rotxmatrix;
    matrix rotymatrix;
    matrix rotzmatrix;
    matrix tempmat;
    real32 angxrotate;
    real32 angyrotate;
    real32 angzrotate;

/*-----------------------------------------------------------------------------
    Update object position
-----------------------------------------------------------------------------*/

    // a = F/m
    vecScalarMultiply(a,obj->posinfo.force,staticheader->oneOverMass);

    // a = at
    vecMultiplyByScalar(a,phystimeelapsed);

    // v = v + at
    vecAddTo(obj->posinfo.velocity,a);

    if (Real32ToUdword(obj->posinfo.velocity.x) | Real32ToUdword(obj->posinfo.velocity.y) | Real32ToUdword(obj->posinfo.velocity.z))
    {
        SET_MOVING_LINEARLY(obj->posinfo.isMoving);

        if(((Missile *)obj)->FORCE_DROPPED == TRUE)
        {
            // cap velocity vector
            vecCapVectorSloppy(&obj->posinfo.velocity,((MissileStaticInfo *)((Missile *)obj)->staticinfo)->maxvelocity_FORCED);
        }
        else
        {
            // cap velocity vector
            vecCapVectorSloppy(&obj->posinfo.velocity,staticheader->maxvelocity);
        }
        // d = vt
        vecScalarMultiply(d,obj->posinfo.velocity,phystimeelapsed);

        // x = x + d
        vecAddTo(obj->posinfo.position,d);

    }
    else
    {
        SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
        vecZeroVector(d);
    }

    dbgAssertOrIgnore(bitTest(obj->flags,SOF_Rotatable));
    {

    /*-----------------------------------------------------------------------------
        Update object orientation
    -----------------------------------------------------------------------------*/

        if ((robj->rotinfo.torque.x != 0) || (robj->rotinfo.torque.y != 0) || (robj->rotinfo.torque.z != 0))
        {

            // alpha = Torque/momentOfInertia (about each axis)
            a.x = robj->rotinfo.torque.x * staticheader->oneOverMomentOfInertiaX;
            a.y = robj->rotinfo.torque.y * staticheader->oneOverMomentOfInertiaY;
            a.z = robj->rotinfo.torque.z * staticheader->oneOverMomentOfInertiaZ;

            // alpha = alpha*t
            vecMultiplyByScalar(a,phystimeelapsed);

            // w = w + alpha*t
            vecAddTo(robj->rotinfo.rotspeed,a);

            // cap rotation speed
            vecCapVectorSloppy(&robj->rotinfo.rotspeed,staticheader->maxrot);

        }

        // We now have w.x, w.y, and w.z which indicate the
        // rotational velocities (rad/s) around the x,y, and z axes of the ship.

    /*-----------------------------------------------------------------------------
        Update coordsys based on changes in rotation
    -----------------------------------------------------------------------------*/

        if (robj->rotinfo.rotspeed.x)
        {
            angxrotate = robj->rotinfo.rotspeed.x * phystimeelapsed;

            matMakeRotAboutX(&rotxmatrix,(real32)cos(angxrotate),(real32)sin(angxrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotxmatrix);
            coordsysChanged = TRUE;
        }

        if (robj->rotinfo.rotspeed.y)
        {
            angyrotate = robj->rotinfo.rotspeed.y * phystimeelapsed;

            matMakeRotAboutY(&rotymatrix,(real32)cos(angyrotate),(real32)sin(angyrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotymatrix);
            coordsysChanged = TRUE;
        }

        if (robj->rotinfo.rotspeed.z)
        {
            angzrotate = robj->rotinfo.rotspeed.z * phystimeelapsed;

            matMakeRotAboutZ(&rotzmatrix,(real32)cos(angzrotate),(real32)sin(angzrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotzmatrix);
            coordsysChanged = TRUE;
        }

        if (bitTest(obj->flags,SOF_Impactable))
        {
            if (coordsysChanged)
            {
                matMultiplyMatByVec(&irobj->collInfo.collOffset,&(irobj->rotinfo.coordsys),&staticheader->staticCollInfo.collsphereoffset);
                if (UNIVERSE_WOODPECKER(UPDATE_COLLRECT_RATE, UPDATE_COLLRECT_FRAME))
                    collUpdateCollRectangle(irobj);
            }

            vecAdd(irobj->collInfo.collPosition,obj->posinfo.position,irobj->collInfo.collOffset);

            obj->enginePosition = obj->posinfo.position;
            if (obj->trail != NULL)
            {
                if((universe.univUpdateCounter % UNIVERSE_UPDATE_RATE_FACTOR) == 0)
                {
                    mistrailUpdate(obj->trail, &obj->enginePosition);
                }
            }
        }
        vecZeroVector(robj->rotinfo.torque);
    }
    vecZeroVector(obj->posinfo.force);
}

/*-----------------------------------------------------------------------------
    Name        : physUpdateObjPosVelBasic
    Description : Used for basic objects
                  updates a space objects position and orientation based
                  on any external forces/torques, as well as its current
                  velocity/spin.
    Inputs      : phystimeelapsed - time elapsed since last physics frame
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void physUpdateObjPosVelBasic(SpaceObj *obj,real32 phystimeelapsed)
{
    vector a;
    vector d;
    StaticHeader *staticheader = &obj->staticinfo->staticheader;
    bool32 coordsysChanged = FALSE;
    matrix rotxmatrix;
    matrix rotymatrix;
    matrix rotzmatrix;
    matrix tempmat;
    real32 angxrotate;
    real32 angyrotate;
    real32 angzrotate;

/*-----------------------------------------------------------------------------
    Update object position
-----------------------------------------------------------------------------*/

    // a = F/m
    vecScalarMultiply(a,obj->posinfo.force,staticheader->oneOverMass);

    // a = at
    vecMultiplyByScalar(a,phystimeelapsed);

    // v = v + at
    vecAddTo(obj->posinfo.velocity,a);

    if (Real32ToUdword(obj->posinfo.velocity.x) | Real32ToUdword(obj->posinfo.velocity.y) | Real32ToUdword(obj->posinfo.velocity.z))
    {
        SET_MOVING_LINEARLY(obj->posinfo.isMoving);

        vecCapVectorSloppy(&obj->posinfo.velocity,staticheader->maxvelocity);

        // d = vt
        vecScalarMultiply(d,obj->posinfo.velocity,phystimeelapsed);

        // x = x + d
        vecAddTo(obj->posinfo.position,d);

    }
    else
    {
        SET_MOVING_IMMOBILE(obj->posinfo.isMoving);
        vecZeroVector(d);
    }

    if (bitTest(obj->flags,SOF_Rotatable))
    {

    /*-----------------------------------------------------------------------------
        Update object orientation
    -----------------------------------------------------------------------------*/

        if ((robj->rotinfo.torque.x != 0) || (robj->rotinfo.torque.y != 0) || (robj->rotinfo.torque.z != 0))
        {

            // alpha = Torque/momentOfInertia (about each axis)
            a.x = robj->rotinfo.torque.x * staticheader->oneOverMomentOfInertiaX;
            a.y = robj->rotinfo.torque.y * staticheader->oneOverMomentOfInertiaY;
            a.z = robj->rotinfo.torque.z * staticheader->oneOverMomentOfInertiaZ;

            // alpha = alpha*t
            vecMultiplyByScalar(a,phystimeelapsed);

            // w = w + alpha*t
            vecAddTo(robj->rotinfo.rotspeed,a);

            // cap rotation speed
            vecCapVectorSloppy(&robj->rotinfo.rotspeed,staticheader->maxrot);

        }

        // We now have w.x, w.y, and w.z which indicate the
        // rotational velocities (rad/s) around the x,y, and z axes of the ship.

    /*-----------------------------------------------------------------------------
        Update coordsys based on changes in rotation
    -----------------------------------------------------------------------------*/

        if (robj->rotinfo.rotspeed.x)
        {
            angxrotate = robj->rotinfo.rotspeed.x * phystimeelapsed;

            matMakeRotAboutX(&rotxmatrix,(real32)cos(angxrotate),(real32)sin(angxrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotxmatrix);
            coordsysChanged = TRUE;
        }

        if (robj->rotinfo.rotspeed.y)
        {
            angyrotate = robj->rotinfo.rotspeed.y * phystimeelapsed;

            matMakeRotAboutY(&rotymatrix,(real32)cos(angyrotate),(real32)sin(angyrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotymatrix);
            coordsysChanged = TRUE;
        }

        if (robj->rotinfo.rotspeed.z)
        {
            angzrotate = robj->rotinfo.rotspeed.z * phystimeelapsed;

            matMakeRotAboutZ(&rotzmatrix,(real32)cos(angzrotate),(real32)sin(angzrotate));

            tempmat = robj->rotinfo.coordsys;
            matMultiplyMatByMat(&(robj->rotinfo.coordsys),&tempmat,&rotzmatrix);
            coordsysChanged = TRUE;
        }

        if (bitTest(obj->flags,SOF_Impactable))
        {
            if (coordsysChanged)
            {
                matMultiplyMatByVec(&irobj->collInfo.collOffset,&(irobj->rotinfo.coordsys),&staticheader->staticCollInfo.collsphereoffset);
                if (UNIVERSE_WOODPECKER(UPDATE_COLLRECT_RATE, UPDATE_COLLRECT_FRAME))
                    collUpdateCollRectangle(irobj);
            }

            vecAdd(irobj->collInfo.collPosition,obj->posinfo.position,irobj->collInfo.collOffset);

        }

        vecZeroVector(robj->rotinfo.torque);
    }

    vecZeroVector(obj->posinfo.force);
}
