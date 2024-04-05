// =============================================================================
//  CameraCommand.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/3/1997 by gshaw
// =============================================================================

#ifndef ___CAMERACOMMAND_H
#define ___CAMERACOMMAND_H

#include "Camera.h"
#include "ShipSelect.h"

/*=============================================================================
    Types:
=============================================================================*/

typedef SelectCommand FocusCommand;

// each camera stack entry contains information about the camera state,
// and what ships it should focus on.
typedef struct
{
    Node stacklink;
    Camera remembercam;
    FocusCommand focus;
} CameraStackEntry;

typedef enum
{
    CAMERA_SET,
    CAMERA_FOCUSORIENT,
    CAMERA_FOCUSMOVE,
    CAMERA_FOCUSPOP
} TransitionType;

typedef enum
{
    CAMACTION_FOCUS,
    CAMACTION_FOCUSFAR,
    CAMACTION_CANCELFOCUS,
    CAMACTION_FORWARDFOCUS,
    CAMACTION_VIEWMISSPHERE
} ActionType;

#define CCMODE_LOCK_OUT_USER_INPUT              1
#define CCMODE_FOCUS_CHANGES_LOOKATPOINT_ONLY   2
#define CCMODE_FOCUS_CHANGES_NOTHING            4

#define CCMODE_PILOT_SHIP                       8

// The CameraCommand object contains a camera stack, for focusing on ships
// and canceling focus, a toggle to ViewEntireMissphere, and also an
// actual camera which always tries to "chase" the currently active
// camera on the stack.
typedef struct
{
    LinkedList camerastack;
    CameraStackEntry *currentCameraStack;
    bool32 ViewEntireMissphere;
    Camera actualcamera;
    TransitionType transition;
    ActionType action;
    real32 transTimeStamp;
    sdword UserControlled;
    bool32 zoomInCloseAsPossible;
    real32 lockCameraTimeStamp;
    bool32 dontUseVelocityPredInChase;
    udword ccMode;
    CameraStackEntry *dontFocusOnMe;
} CameraCommand;

/*=============================================================================
    Macros:
=============================================================================*/

#define sizeofFocusCommand sizeofSelectCommand
#define sizeofCameraStackEntry(n) (sizeof(CameraStackEntry) + ((n-1)*sizeof(ShipPtr)))

// provides the currently active camera stack entry, where x is pointer to CameraCommand
#define currentCameraStackEntry(x) ((x)->currentCameraStack)

// provides the currently active focus, where x is pointer to CameraCommand
#define currentFocus(x) (&(currentCameraStackEntry(x))->focus)

#define ccFocusSize(n)  (sizeof(FocusCommand) + sizeof(ShipPtr) * ((n) - 1))

/*=============================================================================
    Data:
=============================================================================*/
extern real32 CAMERA_DEFAULT_DISTANCE;
extern real32 CAMERA_FAR_DISTANCE;
extern real32 CAMERA_FOCUSFAR_DISTANCE;

/*=============================================================================
    Functions:
=============================================================================*/

// Camera Command Layer

udword NewSetFocusPoint(CameraStackEntry *curentry, real32 *targetDistance);

void ccInit(CameraCommand *cameracommand);
void ccClose(CameraCommand *cameracommand);
void ccReset(CameraCommand *cameracommand);
void ccFocus(CameraCommand *cameracommand,FocusCommand *focuscom);
void ccFocusClose(CameraCommand *cameracommand, FocusCommand *focuscom);
void ccFocusFar(CameraCommand *cameracommand,FocusCommand *focuscom, Camera *currentCamera);
void ccFocusOnMyMothership(CameraCommand *cameracommand);
void ccFocusOnPlayersMothership(CameraCommand *cameracommand,uword playerindex);
bool32 ccFocusOnFleet(CameraCommand *cameracommand);
bool32 ccFocusExact(CameraCommand *cameracommand,CameraCommand *focustocopy);
void ccFocusOnFleetSmooth(CameraCommand *cameracommand);
void ccFocusGeneral(CameraCommand *cameracommand,FocusCommand *focuscom, bool32 bCloseUp);
void ccForwardFocus(CameraCommand *cameracommand);
void ccCancelFocus(CameraCommand *cameracommand);
void ccViewToggleMissionSphere(CameraCommand *cameracommand);
void ccControl(CameraCommand *cameracommand);
void ccLockOnTargetNow(CameraCommand *cameracommand);

void ccSetModeFlag(CameraCommand *cameracommand,udword ccModeFlag);     // use CCMODE_whatever flags
void ccClearModeFlag(CameraCommand *cameracommand,udword ccModeFlag);   // use CCMODE_whatever flags

void ccChangeAngleDeclination(CameraCommand *cameracommand,real32 angle,real32 declination);

void ccCopyCamera(CameraCommand *cameracommand,Camera *cameraToCopy);

// call this routine to update camera command layer to remove ship
void ccRemoveShip(CameraCommand *cameracommand,Ship *ship);

// call this routine to remove references to theseships from cameracommand
void ccRemoveTheseShips(CameraCommand *cameracommand,SelectCommand *theseships);
void GetDistanceAngleDeclination(Camera *camera,vector *distvec);

//cubic curve evaluation:
void EvalCubic( float *y0, float *m0, float y1, float t );
void ccGetShipCollCenter(Ship *ship, vector *pos, real32 *rad);

//cull a focus selection to a certain radius
sdword ccFocusCullRadiusMean(FocusCommand *selection, real32 radiusSqr, vector *centre);
sdword ccFocusCullRadiusGeneral(FocusCommand *out, FocusCommand *in, real32 radiusSqr, vector *centre);

#endif
