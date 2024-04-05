/*=============================================================================
    Name    : ShipView.c
    Purpose : Render a specific ship to a window.

    Created 7/27/1998 by pgrant
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "ShipView.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Camera.h"
#include "ConsMgr.h"
#include "FastMath.h"
#include "FEColour.h"
#include "FEReg.h"
#include "font.h"
#include "FontReg.h"
#include "glinc.h"
#include "Gun.h"
#include "Light.h"
#include "mouse.h"
#include "Options.h"
#include "Probe.h"
#include "render.h"
#include "rglu.h"
#include "SoundEvent.h"
#include "StatScript.h"
#include "StringSupport.h"
#include "Types.h"
#include "Tweak.h"
#include "Universe.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif


/*=============================================================================
    Private Types:
=============================================================================*/

#define SV_ShipViewName             "SV_ShipView"

#define SV_FirepowerName            "SV_Firepower"
#define SV_CoverageName             "SV_Coverage"
#define SV_ManeuverName             "SV_Maneuver"
#define SV_ArmorName                "SV_Armor"
#define SV_TopSpeedName             "SV_TopSpeed"
#define SV_MassName                 "SV_Mass"

#define SV_InitialCameraDistance    900.0f

#define SV_FontNameLength           64
#define SV_DefaultFont              "default.hff"
#define SV_StatFont                 "HW_EuroseCond_11.hff" //"Small_Fonts_8.hff"

#define SV_ViewMargin               8

#define SV_360_ROTATION_SECS        5.0f
#define SV_PITCH_FLATTEN_SECS       2.0f

regionhandle svShipViewRegion  = NULL;
regionhandle svFirepowerRegion = NULL;
regionhandle svCoverageRegion  = NULL;
regionhandle svManeuverRegion  = NULL;
regionhandle svArmorRegion     = NULL;
regionhandle svTopSpeedRegion  = NULL;
regionhandle svMassRegion      = NULL;

fonthandle svShipViewFont = 0;
fonthandle svShipStatFont = 0;

bool32 ReZoom=TRUE;

real32 savecamMouseX = 0.0f;

char svShipViewFontName[SV_FontNameLength] = SV_DefaultFont;
char svShipStatFontName[SV_FontNameLength] = SV_StatFont;


// storage for printing numbers and some strings relating to Ship statistics
char buf[40];

/*=============================================================================
    Data:
=============================================================================*/

Camera svCamera;

real32 svAngle         = -150.0;
real32 svDeclination   = -20.0;
real32 svZoomOutScalar = 1.1f;
real32 svZoomInScalar  = 1.05f;

scriptEntry ShipViewTweaks[] =
{
    makeEntry(svAngle, scriptSetReal32CB),
    makeEntry(svDeclination, scriptSetReal32CB),
    makeEntry(svZoomInScalar, scriptSetReal32CB),
    makeEntry(svZoomOutScalar,scriptSetReal32CB),
    
    END_SCRIPT_ENTRY
};

ShipType svShipType = DefaultShip;

sdword svMouseCentreX = 0;
sdword svMouseCentreY = 0;
sdword svMouseLastX   = 0;
sdword svMouseLastY   = 0;

bool8 svMouseInside     = FALSE;
bool8 svMousePressLeft  = FALSE;
bool8 svMousePressRight = FALSE;

static GLfloat lightPosition0[] = {10000.0f, 1.0f, 1.0f, 0.1f};

/*=============================================================================
    Function Prototypes
=============================================================================*/

uword svShipCoverage(ShipStaticInfo *statinfo);
void svShipManeuverability(ShipStaticInfo *statinfo,char *name);

/*=============================================================================
    Functions:
=============================================================================*/



/*-----------------------------------------------------------------------------
    Name        : svDirtyShipView
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void svDirtyShipView(void)
{
    if (svShipViewRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svShipViewRegion);
#endif
        svShipViewRegion->status |= RSF_DrawThisFrame;
    }
    if (svFirepowerRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svFirepowerRegion);
#endif
        svFirepowerRegion->status |= RSF_DrawThisFrame;
    }
    if (svCoverageRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svCoverageRegion);
#endif
        svCoverageRegion->status |= RSF_DrawThisFrame;
    }
    if (svManeuverRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svManeuverRegion);
#endif
        svManeuverRegion->status |= RSF_DrawThisFrame;
    }
    if (svArmorRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svArmorRegion);
#endif
        svArmorRegion->status |= RSF_DrawThisFrame;
    }
    if (svTopSpeedRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svTopSpeedRegion);
#endif
        svTopSpeedRegion->status |= RSF_DrawThisFrame;
    }
    if (svMassRegion != NULL)
    {
#ifdef DEBUG_STOMP
        regVerify(svMassRegion);
#endif
        svMassRegion->status |= RSF_DrawThisFrame;
    }
}

/*-----------------------------------------------------------------------------
    Name        : svSelectShip
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void svSelectShip(ShipType type)
{
    svShipType = type;
        ReZoom=TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : svClose
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void svClose(void)
{
    svShipViewRegion  = NULL;
    svFirepowerRegion = NULL;
    svCoverageRegion  = NULL;
    svManeuverRegion  = NULL;
    svArmorRegion     = NULL;
    svTopSpeedRegion  = NULL;
    svMassRegion      = NULL;

    svShipType = DefaultShip;
}

/*-----------------------------------------------------------------------------
    Name        : svReadMouseEvent
    Description : Callback for reading mouse events for use by svShipViewRender
    Inputs      : standard region callback
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
sdword svReadMouseEvent(regionhandle region, sdword ID, udword event, udword data)
{
    rectangle *rect = &region->rect;
    rectangle defaultRect = {0, 0, MAIN_WindowWidth, MAIN_WindowHeight};

    switch(event)
    {
    case RPE_WheelUp:
        wheel_up = TRUE;
        break;

    case RPE_WheelDown:
        wheel_down = TRUE;
        break;

    case RPE_Enter:
        svMouseInside = TRUE;
        break;

    case RPE_Exit:
        svMouseInside     = FALSE;
        svMousePressRight = FALSE;
        svMousePressLeft  = FALSE;
        break;

    case RPE_PressRight:
        // Save last known position of cursor
        svMouseLastX = mouseCursorX();
        svMouseLastY = mouseCursorY();

        // Centre mouse
        svMouseCentreX = (rect->x0 + rect->x1) >> 1;
        svMouseCentreY = (rect->y0 + rect->y1) >> 1;
        mousePositionSet(svMouseCentreX, svMouseCentreY);
        mouseClipToRect(rect);

        svMousePressRight = TRUE;
        break;

    case RPE_PressLeft:
        svMousePressLeft = TRUE;
        break;

    case RPE_ReleaseRight:
        mousePositionSet(svMouseLastX, svMouseLastY);
        mouseClipToRect(&defaultRect);
        svMousePressRight = FALSE;
        break;

    case RPE_ReleaseLeft:
        svMousePressLeft = FALSE;
        break;
    }

    return 0;
}


#define SPIN_FEEDBACK 0.5
/*-----------------------------------------------------------------------------
    Name        : svShipViewRender
    Description : Callback which draws the main ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svShipViewRender(featom* atom, regionhandle region)
{
    rectangle drawRect;
    rectangle* rect;
    rectangle viewRect;
    fonthandle currentFont;
    GLint viewPort[4];
    GLint box[4];
    ShipStaticInfo* info;
    real32 scale;
    sdword width, height;
    sdword x, y;
    keyindex key;
    char* keystring;
    bool32 resetRender = FALSE;
    char    temp[100];

    // facilitates smooth transition between auto/manual rotation of ship
    static real32 angle_user_rotated_to       = 0.0f;
    static real32 declination_user_rotated_to = 0.0f;
    static real32 time_user_rotated_view      = 0.0f;
           real32 real_time_angle             = 0.0f;
    static real32 user_real_angle_offset      = 0.0f;
    
    rect = &region->rect;
    viewRect.x0 = 0;
    viewRect.y0 = 0;
    viewRect.x1 = MAIN_WindowWidth  - 1;
    viewRect.y1 = MAIN_WindowHeight - 1;
    info = NULL;

    if (svShipType != DefaultShip)
    {
        if (universe.curPlayerPtr)
        {
            info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);
        }
        if (info == NULL)
        {
            info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
        }
        if (info == NULL)
        {
            return;
        }
    }

    svShipViewRegion = region;

    if (!resetRender)
    {
        if (svMouseInside)
        {
            ferDrawFocusWindow(region, lw_focus);
        }
        else
        {
            ferDrawFocusWindow(region, lw_normal);
        }
    }

    soundEventUpdate();

    currentFont = fontMakeCurrent(svShipViewFont);

    if (region->flags == 0 || region->flags == RPE_DrawFunctionAdded)
    {                                         //if region not processed yet
        region->flags =
            RPE_Enter | RPE_Exit |
            RPE_WheelDown | RPE_WheelUp |
            RPE_PressLeft | RPE_ReleaseLeft |
            RPE_PressRight | RPE_ReleaseRight;

        regFunctionSet(region, (regionfunction) svReadMouseEvent);               //set new region handler function
    }

    //scale = *svScale[svShipType];
        //svCamera.closestZoom  = *svMinZoom[svShipType];
    //svCamera.farthestZoom = *svMaxZoom[svShipType];

    scale = 1.0f; //*svScale[svShipType];

    if(svShipType != DefaultShip)
    {
        svCamera.closestZoom = info->minimumZoomDistance*svZoomInScalar;
        svCamera.farthestZoom = (svCamera.closestZoom+info->staticheader.staticCollInfo.approxcollspheresize)*svZoomOutScalar;
        if(ReZoom)
        {
            ReZoom=FALSE;
            cameraZoom(&svCamera,1.0f,FALSE);
        }

        if (svMouseInside && (wheel_down || wheel_up))
        {
            cameraControl(&svCamera, FALSE);
        }
        else if (svMouseInside && svMousePressRight)
        {
            camMouseX = (svMouseCentreX - mouseCursorX()) * 4;      //was 2
            camMouseY = (svMouseCentreY - mouseCursorY()) * 4;
            savecamMouseX = savecamMouseX * SPIN_FEEDBACK +
                (real32)camMouseX * (1.0f - SPIN_FEEDBACK);
            cameraControl(&svCamera, FALSE);                         //update the camera

            mouseCursorHide();
            mousePositionSet(svMouseCentreX, svMouseCentreY); // Reset position so it doesn't walk off region

            // keep track of where the user left the camera so we can sync auto-rotation with it
            angle_user_rotated_to       = svCamera.angle;
            declination_user_rotated_to = svCamera.declination;
            time_user_rotated_view      = universe.totaltimeelapsed;
        }
        else // auto rotate ship model
        {
            // continual 360 degree yaw rotation
            real_time_angle = DEG_TO_RAD(remainder(universe.totaltimeelapsed, SV_360_ROTATION_SECS) / SV_360_ROTATION_SECS * 360);

            if (angle_user_rotated_to >= 0.0) {
                user_real_angle_offset = angle_user_rotated_to - real_time_angle;
                angle_user_rotated_to = -1.0;
            }

            svCamera.angle = real_time_angle + user_real_angle_offset;
            
            // collapse pitch to default declination
            if (time_user_rotated_view > 0.0) {
                if (universe.totaltimeelapsed > time_user_rotated_view + SV_PITCH_FLATTEN_SECS) {
                    svCamera.declination = DEG_TO_RAD(svDeclination);
                    time_user_rotated_view = 0.0;
                }
                else {
                    svCamera.declination = declination_user_rotated_to + (DEG_TO_RAD(svDeclination) - declination_user_rotated_to) * ((universe.totaltimeelapsed - time_user_rotated_view) / SV_PITCH_FLATTEN_SECS);
                }
            }
            
            if (svMouseInside) mouseCursorShow();
        }
    }
    //rotation

    drawRect.x0 = rect->x0 + SV_ViewMargin;
    drawRect.y0 = rect->y0 + SV_ViewMargin;
    drawRect.x1 = rect->x1 - SV_ViewMargin;
    drawRect.y1 = rect->y1 - SV_ViewMargin;

    width  = drawRect.x1 - drawRect.x0;
    height = drawRect.y1 - drawRect.y0;

    glGetIntegerv(GL_VIEWPORT, viewPort);

    // Fix up the viewport for the ship view
    // Since we request a fullscreen window and not true fullscreen the MAIN_WindowX values
    // do not represent the actual render resolution. Only the ship view messes with the
    // viewport and trips over this disconnect. Fix this up here until we have a clean
    // solution for UI scaling and windows vs fullscreen rendering.
    // original code: glViewport(drawRect.x0, MAIN_WindowHeight - drawRect.y1, width, height);
    float factorX = (float)viewPort[2]/(float)MAIN_WindowWidth;
    float factorY = (float)viewPort[3]/(float)MAIN_WindowHeight;
    glViewport(drawRect.x0 * factorX, viewPort[3] - drawRect.y1 * factorY, width * factorX, height * factorY);
    
    primModeSet2();
    if (!resetRender)
    {
        primRectSolid2(&viewRect, FEC_Background);
    }
    primModeClear2();

    glEnable(GL_SCISSOR_TEST);
    glGetIntegerv(GL_SCISSOR_BOX, box);
    glScissor(drawRect.x0, MAIN_WindowHeight - drawRect.y1, width, height);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    rndPerspectiveCorrection(TRUE);

    //svCamera.lookatpoint.x = -info->staticheader.staticCollInfo.collsphereoffset.z * scale;
    //svCamera.lookatpoint.y = -info->staticheader.staticCollInfo.collsphereoffset.x * scale;
    //svCamera.lookatpoint.z = -info->staticheader.staticCollInfo.collsphereoffset.y * scale;
    if (svShipType == DefaultShip)
    {
        svCamera.lookatpoint.x=0.0f;
        svCamera.lookatpoint.y=0.0f;
        svCamera.lookatpoint.z=0.0f;
    }
    else
    {
        svCamera.lookatpoint.x = -info->staticheader.staticCollInfo.collsphereoffset.z;
        svCamera.lookatpoint.y = -info->staticheader.staticCollInfo.collsphereoffset.x;
        svCamera.lookatpoint.z = -info->staticheader.staticCollInfo.collsphereoffset.y;
    }

    cameraSetEyePosition(&svCamera);

    rndLightingEnable(TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    rgluPerspective(
        svCamera.fieldofview,
        (float)(width) / (float)(height) /*rndAspectRatio*/,    //set projection matrix
        svCamera.clipPlaneNear,
        svCamera.clipPlaneFar);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    rgluLookAt(
        svCamera.eyeposition.x,
        svCamera.eyeposition.y,
        svCamera.eyeposition.z,
        svCamera.lookatpoint.x,
        svCamera.lookatpoint.y,
        svCamera.lookatpoint.z,
        svCamera.upvector.x,
        svCamera.upvector.y,
        svCamera.upvector.z);

    glPushMatrix();

    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)(&rndCameraMatrix));
    glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat *)(&rndProjectionMatrix));

    glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);      //position light(s) within world

    glScalef(scale, scale, scale);

    if (svShipType != DefaultShip)
    {
        sdword index;
        //try player index colours first
        index = universe.curPlayerIndex;
        if (info->teamColor[index] == 0)
        {
            //colour scheme doesn't exist, search for something valid
            for (index = 0; index < MAX_MULTIPLAYER_PLAYERS; index++)
            {
                if (info->teamColor[index] != 0)
                {
                    break;
                }
            }
            if (index == MAX_MULTIPLAYER_PLAYERS)
            {
                //this ship doesn't have any colour info,
                //at least avoid a GPF
                index = universe.curPlayerIndex;
            }
        }
        meshRender((meshdata *)info->staticheader.LOD->level[0].pData, index);
    }

    glDisable(GL_NORMALIZE);

    glPopMatrix();

    primModeSet2();

    glScissor(box[0], box[1], box[2], box[3]);
    glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);

    rndLightingEnable(FALSE);
    rndPerspectiveCorrection(FALSE);

    x = rect->x0 + 2 + SV_ViewMargin;
    y = rect->y0 + 2 + SV_ViewMargin;

    if (svShipType != DefaultShip && !resetRender)
    {
        fontPrintf(
            x,
            y,
            FEC_ListItemStandard,
            "%s",
            ShipTypeToNiceStr(svShipType));

        y += fontHeight(" ");

        sprintf(temp, "%s %d %s",strGetString(strSVCost),info->buildCost, strGetString(strSVRUs));

        fontPrintf(
            x,
            y,
            FEC_ListItemStandard,
            temp);

        if (cmPrintHotKey)
        {
            x = rect->x1 - 2 - SV_ViewMargin;
            y = rect->y0 + 2 + SV_ViewMargin;

            key = cmShipTypeToKey(svShipType);
            keystring = opKeyToNiceString((keyindex)(key & 0x00ff));

            if (key & CM_SHIFT)
            {
                width = fontWidthf("[SHIFT-%s]",keystring);
                fontPrintf(
                    x-width,
                    y,
                    FEC_ListItemStandard,
                    "[SHIFT-%s]",
                    keystring);
            }
            else if (key)
            {
                width = fontWidthf("[%s]",keystring);
                fontPrintf(
                    x-width,
                    y,
                    FEC_ListItemStandard,
                    "[%s]",
                    keystring);
            }
        }
    }
    fontMakeCurrent(currentFont);

    svDirtyShipView();
}


/*-----------------------------------------------------------------------------
    Name        : svFirepowerRender
    Description : Callback which draws the firepower stat in ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svFirepowerRender(featom *atom, regionhandle region)
{
    rectangle *rect = &region->rect;
    fonthandle currentFont;
    uword firepower;
    char buftemp[50];
    ShipStaticInfo *info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);

    svFirepowerRegion = region;

    if (FELASTCALL(atom))
    {
        svFirepowerRegion = NULL;
        return;
    }

    if(svShipType == DefaultShip)
    {
        return;
    }
    if (info == NULL)
    {
        info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
    }
    if (info == NULL)
    {
        return;
    }

    currentFont = fontMakeCurrent(svShipStatFont);

    if(info->svFirePower !=0)
    {
        firepower = (uword) info->svFirePower;
    }
    else
    {
        firepower = (uword) gunShipFirePower(info, Neutral);
    }
    if(firepower != 0)
    {
        sprintf(buftemp,"%d",firepower);
    }
    else
    {
        sprintf(buftemp,"%s","-");
    }
    sprintf(buf,ShipStatToNiceStr(Firepower),buftemp);

    fontPrintf(
        rect->x0,
        rect->y0,
        FEC_ListItemStandard,
        "%s",
        buf);

    fontMakeCurrent(currentFont);
}

/*-----------------------------------------------------------------------------
    Name        : svCoverageRender
    Description : Callback which draws the coverage stat in ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svCoverageRender(featom *atom, regionhandle region)
{
    rectangle *rect = &region->rect;
    sdword width;
    fonthandle currentFont;
    char buftemp[50];
    uword coverage;
    ShipStaticInfo *info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);

    svCoverageRegion = region;

    if (FELASTCALL(atom))
    {
        svCoverageRegion = NULL;
        return;
    }

    if(svShipType == DefaultShip)
    {
        return;
    }

    if (info == NULL)
    {
        info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
    }

    if (info == NULL)
    {
        return; //maybe print UNKNOWN!
    }

    currentFont = fontMakeCurrent(svShipStatFont);

    coverage = svShipCoverage(info);
    if(coverage != 0)
    {
        sprintf(buftemp,"%d %s",coverage,ShipStatToNiceStr(CoverageUnits));
    }
    else
    {
        sprintf(buftemp,"%s","-");
    }
    sprintf(buf,ShipStatToNiceStr(Coverage),buftemp);

    width = fontWidthf("%s",buf);

    fontPrintf(
        rect->x0,//rect->x1 - width,
        rect->y0,
        FEC_ListItemStandard,
        "%s",buf);

    fontMakeCurrent(currentFont);
}

/*-----------------------------------------------------------------------------
    Name        : svManeuverRender
    Description : Callback which draws the maneuver stat in ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svManeuverRender(featom *atom, regionhandle region)
{
    rectangle *rect = &region->rect;
    fonthandle currentFont;
    char maneuverability[100] = "";
    ShipStaticInfo *info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);

    svManeuverRegion = region;

    if (FELASTCALL(atom))
    {
        svManeuverRegion = NULL;
        return;
    }

    if (svShipType == DefaultShip)
    {
        return;
    }
    if (info == NULL)
    {
        info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
    }
    if (info == NULL)
    {
        return;
    }

    currentFont = fontMakeCurrent(svShipStatFont);

    svShipManeuverability(info,maneuverability);
    sprintf(buf,ShipStatToNiceStr(Maneuver),maneuverability);

    fontPrintf(
        rect->x0,
        rect->y0,
        FEC_ListItemStandard,
        "%s",buf);

    fontMakeCurrent(currentFont);
}

/*-----------------------------------------------------------------------------
    Name        : svArmorRender
    Description : Callback which draws the armor stat in ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svArmorRender(featom *atom, regionhandle region)
{
    rectangle *rect = &region->rect;
    sdword width;
    char buftemp[50];

    fonthandle currentFont;
    ShipStaticInfo *info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);

    svArmorRegion = region;

    if (FELASTCALL(atom))
    {
        svArmorRegion = NULL;
        return;
    }

    if(svShipType == DefaultShip)
    {
        return;
    }
    if (info == NULL)
    {
        info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
    }
    if (info == NULL)
    {
        return;
    }

    svArmorRegion = region;

    currentFont = fontMakeCurrent(svShipStatFont);

    sprintf(buftemp,"%d",(udword) info->maxhealth);
    sprintf(buf,ShipStatToNiceStr(Armor),buftemp);

    width = fontWidthf("%s",buf);

    fontPrintf(
        rect->x0,//rect->x1 - width,
        rect->y0,
        FEC_ListItemStandard,
        "%s",buf);

    fontMakeCurrent(currentFont);
}

/*-----------------------------------------------------------------------------
    Name        : svTopSpeedRender
    Description : Callback which draws the top speed stat in ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svTopSpeedRender(featom *atom, regionhandle region)
{
    rectangle *rect = &region->rect;
    fonthandle currentFont;
    char buftemp[50];

    ShipStaticInfo *info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);

    svTopSpeedRegion = region;

    if (FELASTCALL(atom))
    {
        svTopSpeedRegion = NULL;
        return;
    }

    if(svShipType == DefaultShip)
    {
            return;
    }
    if (info == NULL)
    {
        info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
    }
    if (info == NULL)
    {
        return;
    }

    currentFont = fontMakeCurrent(svShipStatFont);

    //sprintf(buf,"%d", (uword)info->staticheader.maxvelocity);
    //topspeed contains a %d for the numerical location of the maxvelocity
    if(info->shiptype == Probe)
    {
        sprintf(buftemp,"%d",(udword) ((ProbeStatics *) info->custstatinfo)->ProbeDispatchMaxVelocity);
        sprintf(buf,ShipStatToNiceStr(TopSpeed),buftemp);
    }
    else
    {
        sprintf(buftemp,"%d",(udword) info->staticheader.maxvelocity);
        sprintf(buf,ShipStatToNiceStr(TopSpeed),buftemp);
    }
    fontPrintf(
        rect->x0,
        rect->y0,
        FEC_ListItemStandard,
        "%s",
        buf);



    fontMakeCurrent(currentFont);
}

/*-----------------------------------------------------------------------------
    Name        : svMassRender
    Description : Callback which draws the mass stat in ship view.
    Inputs      : standard FE callback
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void svMassRender(featom *atom, regionhandle region)
{
    rectangle *rect = &region->rect;
    fonthandle currentFont;
    sdword width;
    char buftemp[50];

    ShipStaticInfo *info = GetShipStaticInfoSafe(svShipType, universe.curPlayerPtr->race);

    svMassRegion = region;

    if (FELASTCALL(atom))
    {
        svMassRegion = NULL;
        return;
    }

    if(svShipType == DefaultShip)
    {
            return;
    }
    if (info == NULL)
    {
        info = GetShipStaticInfoSafe(svShipType, GetValidRaceForShipType(svShipType));
    }
    if (info == NULL)
    {
        return;
    }

    currentFont = fontMakeCurrent(svShipStatFont);

    //sprintf(buf, "%d", (uword)info->staticheader.mass);
    //Mass str contains a %d for the mass to expand into
    sprintf(buftemp,"%d",(udword)info->staticheader.mass);
    sprintf(buf,ShipStatToNiceStr(Mass),buftemp);

    width = fontWidthf("%s",
        buf);                       //width of number
        //ShipTypeStatToNiceStr(svShipType, Mass));//width of number


    fontPrintf(
        rect->x0,//rect->x1 - width,
        rect->y0,
        FEC_ListItemStandard,
        "%s",
        buf);
        //ShipTypeStatToNiceStr(svShipType, Mass));

    fontMakeCurrent(currentFont);
}


/*-----------------------------------------------------------------------------
    Name        :  svStartup()
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void svStartup(void)
{
    cameraInit(&svCamera, SV_InitialCameraDistance);

    svCamera.angle = DEG_TO_RAD(svAngle);
    svCamera.declination = DEG_TO_RAD(svDeclination);

    feDrawCallbackAdd(SV_ShipViewName, svShipViewRender);   //add render callbacks

    feDrawCallbackAdd(SV_FirepowerName, svFirepowerRender);
    feDrawCallbackAdd(SV_CoverageName, svCoverageRender);
    feDrawCallbackAdd(SV_ManeuverName, svManeuverRender);
    feDrawCallbackAdd(SV_ArmorName, svArmorRender);
    feDrawCallbackAdd(SV_TopSpeedName, svTopSpeedRender);
    feDrawCallbackAdd(SV_MassName, svMassRender);

    svMouseInside     = FALSE;
    svMousePressLeft  = FALSE;
    svMousePressRight = FALSE;

    svShipViewFont = frFontRegister(svShipViewFontName);
    svShipStatFont = frFontRegister(svShipStatFontName);
}

/*-----------------------------------------------------------------------------
    Name        :  svShutdown()
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void svShutdown(void)
{
}

uword svShipCoverage(ShipStaticInfo *statinfo)
{
    GunStaticInfo *guninfo = statinfo->gunStaticInfo;
    GunStatic *gun,*gun2;
    sdword i,j,numGuns;
    real32 coverage=0;
        real32 gunvalue,gunangledif;

    if(guninfo == NULL)
    {
        return(0);
    }

    numGuns = guninfo->numGuns;
    for(i=0;i<numGuns;i++)
    {
        gun = &guninfo->gunstatics[i];
        switch(gun->guntype)
        {
        case GUN_Fixed:
            //fixed guns don't really increase by anything..
            //but we'll make it feel better by giving it +1;
            coverage+=0.1f;
            break;
        case GUN_Gimble:
            gunvalue = (acos(gun->cosmaxAngleFromNorm))*2.0f;
                        for(j=i+1;j<numGuns;j++)
                        {
                                gun2 = &guninfo->gunstatics[i];
                                //angle difference between normals of two guns.
                                gunangledif = acos(vecDotProduct(gun->gunnormal,gun2->gunnormal));
                                gunangledif = gunangledif - acos(gun->cosmaxAngleFromNorm) - acos(gun2->cosmaxAngleFromNorm);
                                if(gunangledif < 0.0f)
                                        gunvalue+=gunangledif;
                                if(gunvalue <= 0.0f)
                                        break;
                        }
                        coverage+=max(gunvalue,0.0f);
            break;
        case GUN_NewGimble:
            coverage = coverage + (min(2*gun->maxturnangle,2*gun->maxdeclination));
                        break;
        case GUN_MissileLauncher:
        case GUN_MineLauncher:
            coverage=2*PI;
            goto donecoveragecalc;
            break;

        }
    }

donecoveragecalc:
    coverage = 100.0*coverage/(2*PI);
    return(min(((uword) coverage),100));
}

void svShipManeuverability(ShipStaticInfo *statinfo,char *name)
{
    sdword man=0;
    real32 turnspeed;
    if(statinfo->svManeuverability != 0)
    {
        man = statinfo->svManeuverability;
calcmanjump:
        switch (man)
        {
        case 1:         //very low
            sprintf(name,"%s",ShipStatToNiceStr(VeryLow));
            break;
        case 2:         //low
            sprintf(name,"%s",ShipStatToNiceStr(Low));
            break;
        case 3:         //medium
            sprintf(name,"%s",ShipStatToNiceStr(Medium));
            break;
        case 4:         //high
            sprintf(name,"%s",ShipStatToNiceStr(High));
            break;
        case 5:         //Very High
            sprintf(name,"%s",ShipStatToNiceStr(VeryHigh));
            break;
        default:
            dbgFatalf(DBG_Loc,"Error Calculating Maneuverability");
        }
        return;
    }
//calculate maneuverability
    turnspeed = statinfo->turnspeedstat[TURN_YAW]+
                statinfo->turnspeedstat[TURN_PITCH]+
                statinfo->turnspeedstat[TURN_ROLL];
    if(turnspeed < TW_SV_MAN_VERY_LOW)
        man=1;
    else if(turnspeed < TW_SV_MAN_LOW)
        man=2;
    else if(turnspeed < TW_SV_MAN_MEDIUM)
        man=3;
    else if(turnspeed < TW_SV_MAN_HIGH)
        man=4;
    else
        man=5;
    goto calcmanjump;
}

