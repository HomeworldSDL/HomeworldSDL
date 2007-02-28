/*=============================================================================
    Name    : Camera.c
    Purpose : Contains camera code

    Created 6/20/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "Camera.h"

#include <math.h>
#include <string.h>

#include "CRC32.h"
#include "CameraCommand.h"
#include "Debug.h"
#include "FastMath.h"
#include "Key.h"
#include "render.h"
#include "StatScript.h"
#include "Universe.h"

/*=============================================================================
    Public variables:
=============================================================================*/

sdword camMouseX = 0;
sdword camMouseY = 0;

bool8 wheel_up = FALSE;
bool8 wheel_down = FALSE;
bool8 useSlowWheelZoomIn = FALSE;
bool  zoomOutNow = FALSE;
bool  zoomInNow = FALSE;

/*=============================================================================
    Tweakables:
=============================================================================*/

real32 CAMERA_MOUSE_SENS_MIN   = -1.0f;                 //in log-base-2 space
real32 CAMERA_MOUSE_SENS_MAX   = 1.0f;
real32 CAMERA_MOUSE_SENS       = 1.0f;                  //centre of that range
real32 CAMERA_ANGLE_ROT        = DEG_TO_RAD(0.1f);
real32 CAMERA_DECLINATION_ROT  = DEG_TO_RAD(0.1f);
real32 CAMERA_MAX_DECLINATION  = DEG_TO_RAD(85.0f);
real32 CAMERA_MIN_DECLINATION  = DEG_TO_RAD(-85.0f);
real32 CAMERA_ZOOM_IN          = 0.995f;
real32 CAMERA_ZOOM_OUT         = 1.005f;
real32 CAMERA_KEY_ZOOM_IN      = 0.90f;
real32 CAMERA_KEY_ZOOM_OUT     = 1.10f;
real32 CAMERA_AUTO_ZOOM_IN     = 0.95f;
real32 CAMERA_AUTO_ZOOM_OUT    = 1.05f;
real32 CAMERA_CLIP_NEAR        = 1.0f;
real32 CAMERA_CLIP_FAR         = 35000.0f;
real32 CAMERA_CLIP_FAR_PLANET  = 455000.0f;
real32 CAMERA_FIELD_OF_VIEW    = 90.0f;
real32 CAMERA_WHEEL_ZOOM_IN    = 0.85f;
real32 CAMERA_WHEEL_ZOOM_OUT   = 1.15f;

real32 CAMERA_MAX_ZOOMOUT_DISTANCE = 10000.0f;
real32 CAMERA_MIN_ZOOMOUT_DISTANCE = 10.0f;

real32 CAMERA_FLOAT_SCALAR0 = 1.05f;
real32 CAMERA_FLOAT_SCALAR1 = 1.1f;
real32 CAMERA_FLOAT_DIST_SCALAR = 0.0075f;
real32 CAMERA_FLOAT_OFFSET_SCALAR0 = 0.3f;
real32 CAMERA_FLOAT_OFFSET_SCALAR1 = 0.3f;
real32 CAMERA_FLOAT_OFFSET_SCALAR2 = 0.4f;

scriptEntry CameraTweaks[] =
{
    makeEntry(CAMERA_ANGLE_ROT,scriptSetAngCB),
    makeEntry(CAMERA_DECLINATION_ROT,scriptSetAngCB),
    makeEntry(CAMERA_MAX_DECLINATION,scriptSetAngCB),
    makeEntry(CAMERA_MIN_DECLINATION,scriptSetAngCB),
    makeEntry(CAMERA_ZOOM_IN,scriptSetReal32CB),
    makeEntry(CAMERA_ZOOM_OUT,scriptSetReal32CB),
    makeEntry(CAMERA_KEY_ZOOM_IN,scriptSetReal32CB),
    makeEntry(CAMERA_KEY_ZOOM_OUT,scriptSetReal32CB),
    makeEntry(CAMERA_AUTO_ZOOM_IN,scriptSetReal32CB),
    makeEntry(CAMERA_AUTO_ZOOM_OUT,scriptSetReal32CB),
    makeEntry(CAMERA_FIELD_OF_VIEW,scriptSetReal32CB),
    makeEntry(CAMERA_CLIP_NEAR,scriptSetReal32CB),
    makeEntry(CAMERA_CLIP_FAR,scriptSetReal32CB),
    makeEntry(CAMERA_CLIP_FAR_PLANET,scriptSetReal32CB),
    makeEntry(CAMERA_WHEEL_ZOOM_IN,scriptSetReal32CB),
    makeEntry(CAMERA_WHEEL_ZOOM_OUT,scriptSetReal32CB),
    makeEntry(CAMERA_MAX_ZOOMOUT_DISTANCE,scriptSetReal32CB),
    makeEntry(CAMERA_MIN_ZOOMOUT_DISTANCE,scriptSetReal32CB),
    makeEntry(CAMERA_FLOAT_SCALAR0,scriptSetReal32CB),
    makeEntry(CAMERA_FLOAT_SCALAR1,scriptSetReal32CB),
    makeEntry(CAMERA_FLOAT_DIST_SCALAR,scriptSetReal32CB),
    makeEntry(CAMERA_FLOAT_OFFSET_SCALAR0,scriptSetReal32CB),
    makeEntry(CAMERA_FLOAT_OFFSET_SCALAR1,scriptSetReal32CB),
    makeEntry(CAMERA_FLOAT_OFFSET_SCALAR2,scriptSetReal32CB),
    makeEntry(CAMERA_MOUSE_SENS_MIN,scriptSetReal32CB),
    makeEntry(CAMERA_MOUSE_SENS_MAX,scriptSetReal32CB),
    
    END_SCRIPT_ENTRY
};

/*=============================================================================
    Private Functions:
=============================================================================*/
/*-----------------------------------------------------------------------------
    Name        : camAngleVerify
    Description : Ensures the angle (azimuth) of the camera is within valid ranges.
    Inputs      : camera - camera to examine and potentially modify.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void camAngleVerify(Camera *camera)
{
    if (camera->angle < 0)
    {
        camera->angle += TWOPI;
    }
    else if (camera->angle > TWOPI)
    {
        camera->angle -= TWOPI;
    }
}

/*-----------------------------------------------------------------------------
    Name        : camDeclinationVerify
    Description : Ensures the camera's declination is within valid ranges.
    Inputs      : camera - camera to examine and potentially modify.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void camDeclinationVerify(Camera *camera)
{
    if (camera->declination > CAMERA_MAX_DECLINATION)
    {
        camera->declination = CAMERA_MAX_DECLINATION;
    }
    if (camera->declination < CAMERA_MIN_DECLINATION)
    {
        camera->declination = CAMERA_MIN_DECLINATION;
    }
}

/*=============================================================================
    Public Functions:
=============================================================================*/

void cameraSetEyePosition(Camera *camera)
{
    vector dirvec;
    matrix rotymatrix;
    matrix rotzmatrix;
    vector rot1vec;
    vector rot2vec;

    vecSet(dirvec,camera->distance,0.0f,0.0f);

    camAngleVerify(camera);
    camDeclinationVerify(camera);

    matMakeRotAboutY(&rotymatrix,(real32)cos(camera->declination),(real32)sin(camera->declination));
    matMakeRotAboutZ(&rotzmatrix,(real32)cos(camera->angle),(real32)sin(camera->angle));

    matMultiplyMatByVec(&rot1vec,&rotymatrix,&dirvec);
    matMultiplyMatByVec(&rot2vec,&rotzmatrix,&rot1vec);

    vecAdd(camera->eyeposition,camera->lookatpoint,rot2vec);
}

void cameraRotateAbout(Camera *camera,vector about,real32 deg)
{
    real32 rot = DEG_TO_RAD(deg);
    vector position;
    vector rotatedposition;
    matrix rotzmatrix;

    matMakeRotAboutZ(&rotzmatrix,(real32)cos(rot),(real32)sin(rot));

    camera->oldlookatpoint = camera->lookatpoint;
    vecSub(position,camera->lookatpoint,about);
    matMultiplyMatByVec(&rotatedposition,&rotzmatrix,&position);
    vecAdd(camera->lookatpoint,about,rotatedposition);

    vecSub(position,camera->eyeposition,about);
    matMultiplyMatByVec(&rotatedposition,&rotzmatrix,&position);
    vecAdd(camera->eyeposition,about,rotatedposition);
}

void cameraCopyPositionInfo(Camera *dst,Camera *src)
{
    dst->angle          = src->angle;
    dst->declination    = src->declination;
    dst->distance       = src->distance;
    dst->eyeposition    = src->eyeposition;
    dst->lookatpoint    = src->lookatpoint;
    dst->oldlookatpoint = src->oldlookatpoint;
}

void cameraInit(Camera *camera,real32 distance)
{
    camera->playerowner = NULL;

    camera->angle = 0.0f;
    camera->declination = 0.0f;
    camera->distance = distance;

    memset(&camera->lookatpoint,    0, sizeof(vector));
    memset(&camera->oldlookatpoint, 0, sizeof(vector));

    vecSet(camera->upvector,0.0f,0.0f,1.0f);

    cameraSetEyePosition(camera);

    camera->fieldofview   = CAMERA_FIELD_OF_VIEW;
    camera->clipPlaneNear = CAMERA_CLIP_NEAR;
    camera->clipPlaneFar  = CAMERA_CLIP_FAR;
    camera->closestZoom   = CAMERA_MIN_ZOOMOUT_DISTANCE;
    camera->farthestZoom  = CAMERA_MAX_ZOOMOUT_DISTANCE;
    camera->ignoreZoom    = FALSE;
}

void cameraChangeLookatpoint(Camera *camera,vector *newlookatpoint)
{
    camera->oldlookatpoint = camera->lookatpoint;
    camera->lookatpoint    = *newlookatpoint;

    cameraSetEyePosition(camera);
}

void cameraRotAngle(Camera *camera,real32 angle)
{
    camera->angle += angle;
    camAngleVerify(camera);
}

void cameraRotDeclination(Camera *camera,real32 declination)
{
    camera->declination += declination;
    camDeclinationVerify(camera);
}

/*-----------------------------------------------------------------------------
    Name        : cameraMinimumZoom
    Description : walks the focus command to determine minimum zoom distance
    Inputs      : camera - current camera
    Outputs     :
    Return      : minimum zoom distance
----------------------------------------------------------------------------*/
real32 cameraMinimumZoom(Camera* camera)
{
    sdword i;
    FocusCommand* focus;
    real32 dist;
    Ship* ship;
    ShipStaticInfo* shipstaticinfo;
    DerelictStaticInfo* derelictstaticinfo;

    focus = &currentCameraStackEntry(&universe.mainCameraCommand)->focus;
    dist = 0.0f;

    for (i = 0; i < focus->numShips; i++)
    {
        ship = focus->ShipPtr[i];
        if (ship->objtype == OBJ_ShipType)
        {
            shipstaticinfo = (ShipStaticInfo*)ship->staticinfo;
            if (shipstaticinfo->minimumZoomDistance > dist)
            {
                dist = shipstaticinfo->minimumZoomDistance;
            }
        }
        else if (ship->objtype == OBJ_DerelictType)
        {
            derelictstaticinfo = (DerelictStaticInfo*)ship->staticinfo;
            if (derelictstaticinfo->minimumZoomDistance > dist)
            {
                dist = derelictstaticinfo->minimumZoomDistance;
            }
        }
    }

    if (dist < camera->closestZoom)
        dist = camera->closestZoom;

    return dist;
}

/*-----------------------------------------------------------------------------
    Name        : cameraZoom
    Description : zooms the camera
    Inputs      : camera - the camera to zoom
                  zoomfactor - zoom scalar
                                  EnforceShipDistances  - use the camera zoom
                                  limits, or use focused ship minimums
    Outputs     : camera->distance is set
    Return      :
----------------------------------------------------------------------------*/
void cameraZoom(Camera *camera,real32 zoomfactor, bool EnforceShipDistances)
{
    real32 minZoom;

    camera->distance *= zoomfactor;

    if (!camera->ignoreZoom)
    {
        if (camera->distance > camera->farthestZoom)
        {
            camera->distance = camera->farthestZoom;
        }

        if(EnforceShipDistances)
        {
            minZoom = cameraMinimumZoom(camera);
            if (camera->distance < minZoom)
                camera->distance = minZoom;
        }
        else
        {
            if(camera->distance < camera->closestZoom)
                camera->distance = camera->closestZoom;
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : cameraControl
    Description : allows user to control the camera.  Call every frame you want
                  the user to be able to control the camera.
    Inputs      : camera, zoomInAllowed
    Outputs     :
    Return      : returns a mask of user actions performed
                  0x01 = moved  0x02 = zoomed
----------------------------------------------------------------------------*/
sdword cameraControl(Camera *camera, bool EnforceShipDistances)
{
    sdword UserAction = 0;

    if (wheel_down)
    {
        cameraZoom(camera,(CAMERA_WHEEL_ZOOM_OUT - 1.0f) * CAMERA_MOUSE_SENS + 1.0f, EnforceShipDistances);
        wheel_down = FALSE;
        UserAction |= CAM_USER_ZOOMED;
    }
    if (wheel_up)
    {
        cameraZoom(camera, 1.0f - (1.0f - (useSlowWheelZoomIn ? 0.9f : CAMERA_WHEEL_ZOOM_IN)) * CAMERA_MOUSE_SENS, EnforceShipDistances);
        wheel_up = FALSE;
        UserAction |= CAM_USER_ZOOMED;
    }
    else
    {
        if (wheel_up)
        {
            // keyScanCode[FLYWHEEL_UP].keynumpressed=0;
            keyClearRepeat(FLYWHEEL_UP);
            wheel_up = FALSE;
        }
    }

    if (zoomOutNow)//keyIsHit(NUMMINUSKEY) || keyIsHit(MINUSKEY))
    {
        cameraZoom(camera,(CAMERA_KEY_ZOOM_OUT - 1.0f) * CAMERA_MOUSE_SENS + 1.0f, EnforceShipDistances);
        UserAction |= CAM_USER_ZOOMED;
    }

    if (zoomInNow)//keyIsHit(NUMPLUSKEY) || keyIsHit(PLUSKEY))
    {
        cameraZoom(camera,1.0f - (1.0f - CAMERA_KEY_ZOOM_IN) * CAMERA_MOUSE_SENS, EnforceShipDistances);
        UserAction |= CAM_USER_ZOOMED;
    }

    if (camMouseX || camMouseY)
    {
        if (keyIsHit(RMOUSE_BUTTON))
        {
            if (keyIsHit(LMOUSE_BUTTON))
            {
                if (camMouseY < 0)
                {
                    cameraZoom(camera, 1.0f + (1.0f - CAMERA_ZOOM_OUT) * (real32)camMouseY * CAMERA_MOUSE_SENS, EnforceShipDistances);
                    UserAction |= CAM_USER_ZOOMED;
                }
                else if (camMouseY > 0)
                {
                    cameraZoom(camera, 1.0f - (1.0f - CAMERA_ZOOM_IN) * (real32)camMouseY * CAMERA_MOUSE_SENS, EnforceShipDistances);
                    UserAction |= CAM_USER_ZOOMED;
                }
            }
            else
            {
                cameraRotDeclination(camera,CAMERA_DECLINATION_ROT * (real32)camMouseY * CAMERA_MOUSE_SENS);
                cameraRotAngle(camera,CAMERA_ANGLE_ROT * (real32)camMouseX * CAMERA_MOUSE_SENS);
                UserAction |= CAM_USER_MOVED;
            }
        }
        camMouseX = camMouseY = 0;
    }

    return UserAction;
}

/*-----------------------------------------------------------------------------
    Name        : cameraSensitivitySet
    Description : Sets the mouse sensitivity.
    Inputs      : sens100 - a value from 0 to 100 with 0 being minimum sensitivity and 100 being max sensitivity.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void cameraSensitivitySet(sdword sens100)
{
    real32 value;
    value = ((real32)sens100 / 100.0f) * (CAMERA_MOUSE_SENS_MAX - CAMERA_MOUSE_SENS_MIN) + CAMERA_MOUSE_SENS_MIN;
    CAMERA_MOUSE_SENS = (real32)pow(2.0, (double)value);
}

/*-----------------------------------------------------------------------------
    Name        : cameraChecksum
    Description : Compute a checksum of a camera.
    Inputs      : cam - camera to compute checksum of.
    Outputs     :
    Return      : checksum for the camera
----------------------------------------------------------------------------*/
#if CAM_CHECKSUM
udword cameraChecksum(Camera *cam)
{
    return((udword)crc32Compute((ubyte *)&cam->angle, sizeof(Camera) - sizeof(struct Player *)));
}
#endif //CAM_CHECKSUM

/*-----------------------------------------------------------------------------
    Name        : cameraRayCast
    Description : Get a vector for the specified pixel
    Inputs      : cam - what camera to cast the ray from
                  screenX, screenY - what pixel to cast the ray from
                  screenWidth, screenHeight - screenSize
    Outputs     : dest - the casted vector
    Return      :
----------------------------------------------------------------------------*/
void cameraRayCast(vector *dest, Camera *cam, sdword screenX, sdword screenY, sdword screenWidth, sdword screenHeight)
{
    real32 mappedMouseX     = 0.0,
           mappedMouseY     = 0.0,
           normalisedHeight = 0.0;
    
    vector looking,
           right,
           down,
           temp;
    
    // form a right-handed, three axis reference frame with "looking" as the +z axis
    // and the +x axis to the right (which forces the +y axis down - this is the same
    // direction as the mouse coordinate system)
    vecSub(looking, cam->lookatpoint, cam->eyeposition);
    vecCrossProduct(right, looking, cam->upvector);
    vecCrossProduct(down,  looking, right        );

    vecNormalize(&looking);
    vecNormalize(&right);
    vecNormalize(&down);

    // map the cursor on to an imaginary plane a unit distance away from
    // eyeposition whose normal is parallel with "looking"

    // half-height of the screen has a normalised height of
    normalisedHeight = tan(DEG_TO_RAD(cam->fieldofview / 2));

    // NB: the height dimension is how the fieldofview is defined so this is used
    // as the unit measure. Also note that a centred origin is used in this mapping.
    mappedMouseY =  ((real32)screenY / (real32)screenHeight) - 0.5;
    mappedMouseX = (((real32)screenX / (real32)screenWidth ) - 0.5) * rndAspectRatio;

    // clean up the mouse vectors
    vecMultiplyByScalar(right, normalisedHeight * mappedMouseX);
    vecMultiplyByScalar(down,  normalisedHeight * mappedMouseY);

    // This is a hack - it shouldn't be here and I don't know what it represents.
    // The original Relic developer had a similar hack too and he didn't know what
    // it was either. I suspect that it's something to do with the inter-conversion
    // with the perspective projection since the maths in this function is dealing 
    // with orthogonal reference frames.
    vecMultiplyByScalar(looking, 0.49);

    // add the three vectors to get the cast ray vector
    // (eyeball to mouse cursor and beyond)
    vecAdd(temp, looking, right);
    vecAdd(*dest, down, temp);   

    // finally, normalise the cast ray
    vecNormalize(dest);
}

// R1 and other races
real32 PilotOffsets[TOTAL_NUM_SHIPS] =
{
    0.520271f,   //AdvanceSupportFrigate
    1.339939f,   //AttackBomber
    0.721288f,   //Carrier
    1.000000f,   //CloakedFighter
    0.590338f,   //CloakGenerator
    0.492416f,   //DDDFrigate
    0.500000f,   //DefenseFighter
    0.500000f,   //DFGFrigate
    0.527472f,   //GravWellGenerator
    1.151658f,   //HeavyCorvette
    0.701667f,   //HeavyCruiser
    0.877925f,   //HeavyDefender
    1.529971f,   //HeavyInterceptor
    0.554551f,   //IonCannonFrigate
    1.163247f,   //LightCorvette
    0.500000f,   //LightDefender
    1.638964f,   //LightInterceptor
    1.064361f,   //MinelayerCorvette
    0.704322f,   //MissileDestroyer
    0.284337f,   //Mothership
    1.083175f,   //MultiGunCorvette
    0.497371f,   //Probe
    0.500000f,   //ProximitySensor
    0.749807f,   //RepairCorvette
    0.583764f,   //ResearchShip
    0.658322f,   //ResourceCollector
    0.680934f,   //ResourceController
    0.816380f,   //SalCapCorvette
    0.500000f,   //SensorArray
    0.735038f,   //StandardDestroyer
    0.537480f,   //StandardFrigate
    0.500000f,   //Drone
    0.668166f,   //TargetDrone
    0.500000f,   //HeadShotAsteroid
    0.500000f,   //CryoTray
    1.048587f,   //P1Fighter
    0.567337f,   //P1IonArrayFrigate
    0.752711f,   //P1MissileCorvette
    0.577365f,   //P1Mothership
    0.722271f,   //P1StandardCorvette
    0.500000f,   //P2AdvanceSwarmer
    0.650220f,   //P2FuelPod
    0.536278f,   //P2Mothership
    0.681024f,   //P2MultiBeamFrigate
    0.500000f,   //P2Swarmer
    0.500000f,   //P3Destroyer
    0.500000f,   //P3Frigate
    0.500000f,   //P3Megaship
    0.500000f,   //FloatingCity
    0.500000f,   //CargoBarge
    0.500000f,   //MiningBase
    0.500000f,   //ResearchStation
    0.500000f,   //JunkYardDawg
    0.500000f    //JunkYardHQ
};

real32 PilotOffsetsR2[TOTAL_NUM_SHIPS] =
{
    // R2
    1.000000f,   //AdvanceSupportFrigate
    0.693640f,   //AttackBomber
    0.757119f,   //Carrier
    0.500000f,   //CloakedFighter
    1.000000f,   //CloakGenerator
    0.500000f,   //DDDFrigate
    0.500000f,   //DefenseFighter
    0.705870f,   //DFGFrigate
    1.000000f,   //GravWellGenerator
    0.747669f,   //HeavyCorvette
    0.639340f,   //HeavyCruiser
    0.500000f,   //HeavyDefender
    0.932979f,   //HeavyInterceptor
    0.730083f,   //IonCannonFrigate
    0.757052f,   //LightCorvette
    0.500000f,   //LightDefender
    0.747636f,   //LightInterceptor
    0.651380f,   //MinelayerCorvette
    0.662940f,   //MissileDestroyer
    0.940094f,   //Mothership
    0.836680f,   //MultiGunCorvette
    0.500000f,   //Probe
    0.500000f,   //ProximitySensor
    0.714634f,   //RepairCorvette
    0.500000f,   //ResearchShip
    0.504698f,   //ResourceCollector
    0.504698f,   //ResourceController
    0.553650f,   //SalCapCorvette
    0.500000f,   //SensorArray
    0.652220f,   //StandardDestroyer
    0.786846f,   //StandardFrigate
    0.500000f,   //Drone
    0.500000f,   //TargetDrone
    0.500000f,   //HeadShotAsteroid
    0.500000f,   //CryoTray
    0.500000f,   //P1Fighter
    0.500000f,   //P1IonArrayFrigate
    0.500000f,   //P1MissileCorvette
    0.500000f,   //P1Mothership
    0.500000f,   //P1StandardCorvette
    0.500000f,   //P2AdvanceSwarmer
    0.500000f,   //P2FuelPod
    0.500000f,   //P2Mothership
    0.500000f,   //P2MultiBeamFrigate
    0.500000f,   //P2Swarmer
    0.500000f,   //P3Destroyer
    0.500000f,   //P3Frigate
    0.500000f,   //P3Megaship
    0.500000f,   //FloatingCity
    0.500000f,   //CargoBarge
    0.500000f,   //MiningBase
    0.500000f,   //ResearchStation
    0.500000f,   //JunkYardDawg
    0.500000f    //JunkYardHQ
};

#ifdef HW_BUILD_FOR_DEBUGGING
real32 pilotupoffset = 0.5f;
#endif

void cameraSetEyePositionBasedOnShip(Camera *camera,struct Ship *ship)
{
    vector heading;
    vector up;
    vector offset;
    real32 d;
    real32 upoffset = 0.5f;

    matGetVectFromMatrixCol1(up,ship->rotinfo.coordsys);
    matGetVectFromMatrixCol3(heading,ship->rotinfo.coordsys);

    if ((ship->shiptype >= 0) && (ship->shiptype < TOTAL_NUM_SHIPS))
    {
        if (ship->shiprace == R2)
        {
            upoffset = PilotOffsetsR2[ship->shiptype];
        }
        else
        {
            upoffset = PilotOffsets[ship->shiptype];
        }
    }
#ifdef HW_BUILD_FOR_DEBUGGING
    if (pilotupoffset != 0.5f)
    {
        upoffset = pilotupoffset;
    }
#endif
    d = ship->staticinfo->staticheader.staticCollInfo.uplength * upoffset;

    vecScalarMultiply(offset,up,d);

    vecAdd(camera->eyeposition,ship->posinfo.position,offset);
    matGetVectFromMatrixCol3(heading,ship->rotinfo.coordsys);
    vecAdd(camera->lookatpoint,camera->eyeposition,heading);

    camera->upvector = up;
}

