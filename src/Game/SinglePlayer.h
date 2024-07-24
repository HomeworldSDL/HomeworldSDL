/*=============================================================================
    Name    : SinglePlayer.h
    Purpose : Definitions for SinglePlayer.c

    Created 3/3/1998 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___SINGLEPLAYER_H
#define ___SINGLEPLAYER_H

#include "KAS.h"
#include "LinkedList.h"
#include "Types.h"

/*=============================================================================
    Switches:
=============================================================================*/

#ifdef HW_BUILD_FOR_DEBUGGING

#define SP_ERROR_CHECKING           1           //general error checking
#define SP_VERBOSE_LEVEL            3           //control specific output code
#define SP_NISLET_TEST              1           //test NISlets
#define SP_DEBUGKEYS                1           //single player debugging keys
#define SP_DEBUGLEVEL2              1           //more debug keys

#else

#define SP_ERROR_CHECKING           0           //general error checking
#define SP_VERBOSE_LEVEL            0           //control specific output code
#define SP_NISLET_TEST              0
#define SP_DEBUGKEYS                0           //single player debugging keys
#define SP_DEBUGLEVEL2              0           //more debug keys

#endif

/*=============================================================================
    Definitions:
=============================================================================*/
typedef enum
{
    NO_HYPERSPACE,
    HYPERSPACE_PREWAIT,
    HYPERSPACE_WAITINGROLLCALL,
    HYPERSPACE_WAITINGFORFLEETCOMMAND,
    HYPERSPACE_SHIPSLEAVING,
    HYPERSPACE_WHITEOUT,
    HYPERSPACE_LOADDONE,
    HYPERSPACE_SHIPSARRIVING,
    HYPERSPACE_FAILED
} HyperspaceState;

#define HS_INACTIVE         0
#define HS_POPUP_INTO       1
#define HS_POPUP_OUTOF      2
#define HS_SLICING_INTO     3
#define HS_SLICING_OUTOF    4
#define HS_COLLAPSE_INTO    5
#define HS_COLLAPSE_OUTOF   6
#define HS_FINISHED         7

#define HSF_WAIT            1   //wait for bit to clear before changing state

#define SP_CAMERA_POINT_NAME        "StartCameraPosition"

#define NUMBER_SINGLEPLAYER_MISSIONS 19

/*=============================================================================
    Type definitions:
=============================================================================*/
// for HYPERSPACE_SHIPSLEAVING, here are some substates:
typedef enum
{
    FOCUSING_ON_MOTHERSHIP,
    LOCKED_FOCUS_FOR_HOP
} HyperspaceSubState;

typedef enum
{
    MISSION_ENUM_NOT_INITIALISED = 0,

    // these preserve the original mission numbers 
    MISSION_1_KHARAK_SYSTEM,
    MISSION_2_OUTSKIRTS_OF_KHARAK_SYSTEM,
    MISSION_3_RETURN_TO_KHARAK,
    MISSION_4_GREAT_WASTELANDS_TRADERS,
    MISSION_5_GREAT_WASTELANDS_REVENGE,
    MISSION_6_DIAMOND_SHOALS,
    MISSION_7_THE_GARDENS_OF_KADESH,
    MISSION_8_THE_CATHEDRAL_OF_KADESH,
    MISSION_9_SEA_OF_LOST_SOULS,
    MISSION_10_SUPER_NOVA_STATION,
    MISSION_11_TENHAUSER_GATE,
    MISSION_12_GALACTIC_CORE,
    MISSION_13_THE_KAROS_GRAVEYARD,
    MISSION_14_BRIDGE_OF_SIGHS,
    MISSION_15_CHAPEL_PERILOUS,
    MISSION_16_HIIGARA,
    
    // whilst these are out of sequence
    MISSION_5B_TURANIC_RAIDER_PLANETOID,
    MISSION_TUTORIAL,

} MissionEnum;

typedef struct
{
    MissionEnum currentMission;
    sdword onCompletePlayNIS;
    bool32 onCompleteHyperspace;

    sdword onLoadPlayNIS;

    HyperspaceState hyperspaceState;
    udword oldAutoLaunch;    //bits 0,1,2 = mothership, carrier1, carrier2
    real32 oldClipPlane;
    sdword rollcallShipsDocked;
    sdword rollcallShipsRemaining;
    HyperspaceSubState hyperspaceSubState;
    real32 hyperspaceTimeStamp;
    LinkedList ShipsInHyperspace;   // linked list of ships in hyperspace ether (uses InsideShip structure)
    sdword resourceUnitsCollected;

    GrowSelection ShipsToHyperspace;
    bool32 giveComputerFleetControl;
    bool32 asteroid0sCanMove;

    bool32 playerCanHyperspace;
    bool32 hyperspaceFails;

} SinglePlayerGameInfo;             // later save this data structure

typedef enum
{
    SHIPHYPERSPACE_NONE,
    SHIPHYPERSPACE_WAITING,
    SHIPHYPERSPACE_ACCELERATINGAWAY,
    SHIPHYPERSPACE_WHIPAWAY,
    SHIPHYPERSPACE_GONE,
    SHIPHYPERSPACE_WHIPTOWARDS,
    SHIPHYPERSPACE_DECELERATE,
    SHIPHYPERSPACE_DONE
} ShipHyperspaceState;

typedef struct ShipSinglePlayerGameInfo
{
    bool32 midlevelHyperspaceIn;
    bool32 midlevelHyperspaceOut;
    bool32 staticHyperspaceGate;
    hvector midlevelHyperspaceTo;
    struct Effect* hyperspaceEffect;

    ShipHyperspaceState shipHyperspaceState;
    real32 timeToHyperspace;
    real32 timeToDelayHyperspace;
    real32 forbehind;       // how far ship is forward or behind mothership
    real32 rightleft;       // how far ship is right or left of mothership
    real32 updown;          // how far ship is up or down of mothership

    real32 clipt;                   //[0..1] or -1, hyperspace clip parameter
    real32 cliptDelta;              //clipt incrementer
    udword hsState;                 //hyperspace effect state
    udword hsFlags;
} ShipSinglePlayerGameInfo;

extern SinglePlayerGameInfo singlePlayerGameInfo;
extern bool32 singlePlayerGameLoadNewLevelFlag;
extern bool32 hyperspaceFails;
extern bool32 singlePlayerHyperspacingInto;
extern bool32 spCollectResourcesAtEndOfMission;

void spHyperspaceButtonPushed(void);

void spHyperspaceSelectionIn(SelectCommand *selection,hvector *destination);
void spHyperspaceSelectionOut(SelectCommand *selection);
void spHyperspaceSelectionInStatic(SelectCommand *selection,hvector *destination);
void spHyperspaceSelectionOutStatic(SelectCommand *selection);

void spResetMissionSequenceToBeginning(void);
void singlePlayerInit(void);
void singlePlayerPostInit(bool32 loadingSaveGame);
void singlePlayerClose(void);
void singlePlayerSetMissionAttributes(char *directory,char *filename);

void singlePlayerGameCleanup(void);

void singlePlayerGameUpdate(void);
void singlePlayerLoadNewLevel(void);
void singlePlayerStartGame(void);
void singlePlayerNextLevel(void);

void singlePlayerShipDied(Ship *ship);

void singlePlayerMissionCompleteCB(void);
void singlePlayerMissionFailedCB(void);

#if SP_DEBUGKEYS
void singlePlayerCheckDebugKeys(sdword ID);
#endif

void spSetCurrentMission(MissionEnum mission);

MissionEnum spGetPreviousMission(void);
MissionEnum spGetCurrentMission (void);
MissionEnum spGetNextMission    (void);

void GetMissionsDirAndFile(MissionEnum mission);

void spMainScreen(void);

bool32 GetStartPointPlayer(hvector *startpoint);
bool32 GetPointOfName(hvector *point,char *name);

ShipSinglePlayerGameInfo *spNewShipSinglePlayerGameInfo(void);

// flags for spLockout
#define SPLOCKOUT_MOUSE     1
#define SPLOCKOUT_MR        2
#define SPLOCKOUT_DESELECT  4
#define SPLOCKOUT_EVERYTHING    (SPLOCKOUT_MOUSE+SPLOCKOUT_MR+SPLOCKOUT_DESELECT)

void spMainScreen(void);
void spLockout(udword flags);
void spMainScreenAndLockout(udword flags);
void spUnlockout(void);

void LoadSinglePlayerGame(void);
void SaveSinglePlayerGame(void);

MissionEnum      WatchFunctionToMissionEnum(KASWatchFunction watchFunction);
KASWatchFunction MissionEnumToWatchFunction(MissionEnum mission);

udword FunctionListSize(MissionEnum mission);
const void** MissionEnumToFunctionList(MissionEnum mission);

#if SP_NISLET_TEST
void spNISletTestAttempt(sdword index);
#endif

bool32 singlePlayerNISletNamesGet(char **nisname, char **scriptname, sdword nisletNumber);
bool32 spFindCameraAttitude(vector *position);

void UpdateMidLevelHyperspacingShips(void);

void singlePlayerPreLoadCheck(void);

//stores the current mission filename
extern char CurrentLevelName[];

/*=============================================================================
    Data:
=============================================================================*/

extern sdword SINGLEPLAYER_STARTINGRUS;
extern char spMissionsDir[];
extern char spMissionsFile[];

extern real32 spFleetModifier;

extern real32 spHyperspaceDelay;
extern bool32   spHoldHyperspaceWindow;

extern real32 HYPERSPACEGATE_HEALTH;
extern real32 HYPERSPACEGATE_WIDTH;
extern real32 HYPERSPACEGATE_HEIGHT;
extern real32 HYPERSPACEGATE_WAYPOINTDIST;

extern real32 SINGLEPLAYER_MISSION14_SPHERE_OVERRIDE;

#endif
