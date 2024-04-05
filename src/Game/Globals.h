/*=============================================================================
    Name    : Globals.h
    Purpose : Definitions for Globals.c

    Created 7/30/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___GLOBALS_H
#define ___GLOBALS_H

#include "MaxMultiplayer.h"
#include "Queue.h"
#include "TitanInterfaceC.h"

//define to include a number of checks to see if regions are valid
#ifdef HW_BUILD_FOR_DEBUGGING
    #define DEBUG_STOMP
#endif

#ifdef HW_BUILD_FOR_DEBUGGING
#define GOD_LIKE_SYNC_CHECKING
//remove if we don't want this logging capability anymore
#define DEBUG_GAME_STATS
#endif

#define OPTIMIZE_VERBOSE            //certain optimizing features to help optimizing



/*=============================================================================
    Defines:
=============================================================================*/

#define MAX_RACES 2

// defines for startingGameState
#define CHECK_AUTODOWNLOAD_MAP          1
#define AUTODOWNLOAD_MAP                2
#define PROCESS_TEXTURES                4
#define GAME_STARTED                    5

#define MAX_SCENARIOSTR_LENGTH          50

#define MAX_MESSAGE_LENGTH              100
#define MAX_MESSAGES                    5

#define MAX_BIGMESSAGE_LENGTH           150
#define MAX_BIGMESSAGES                 2

/*=============================================================================
    Types:
=============================================================================*/

typedef struct
{
    real32 MessageExpire;
    char   message[MAX_MESSAGE_LENGTH];
} gMessageType;

typedef struct
{
    bool32 messageOn;
    char message[MAX_BIGMESSAGE_LENGTH];
} bMessageType;

/*=============================================================================
    Globals
=============================================================================*/

#ifdef OPTIMIZE_VERBOSE
extern sdword vecNormalizeCounter;
#endif

extern udword startingGameState;
extern bool32 HaveSentNonCaptainReadyPacket;

extern bool32 gameIsRunning;
extern bool32 multiPlayerGame;
extern bool32 multiPlayerGameUnderWay;

extern bool32 singlePlayerGame;
extern bool32 objectivesShown;

#define TUTORIAL_ONLY           1
#define TUTORIAL_SINGLEPLAYER   2
extern sdword tutorial;
extern bool32 tutorialdone;

#define PLAYER_QUIT             6
#define PLAYER_DROPPED_OUT      7
extern bool32 playersReadyToGo[MAX_MULTIPLAYER_PLAYERS];

#define playerHasDroppedOutOrQuit(index) ((playersReadyToGo[index] == PLAYER_DROPPED_OUT) || (playersReadyToGo[index] == PLAYER_QUIT))

extern uword numPlayers;
extern uword curPlayer;

extern bool32 startingGame;
//extern bool32 nisEnabled;

extern void *gMessageMutex;
extern gMessageType gMessage[MAX_MESSAGES+1];

extern bMessageType bMessage[MAX_BIGMESSAGES];

extern bool32 smSensorsActive;

extern udword logEnable;
extern char logFilePath[];


#ifdef GOD_LIKE_SYNC_CHECKING
extern bool32 syncDumpOn;
extern sdword syncDumpWindowSize;
extern sdword syncDumpGranularity;
extern sdword syncDumpWindowPos;
extern sdword syncDumpGranTrack;
#endif

#ifdef DEBUG_GAME_STATS
extern bool32 statLogOn;
#endif

extern bool32 smGhostMode;

extern char playerNames[MAX_MULTIPLAYER_PLAYERS][MAX_PERSONAL_NAME_LEN];

extern Queue ProcessSyncPktQ;
extern Queue ProcessRequestedSyncPktQ;
extern Queue ProcessCmdPktQ;
extern Queue ProcessCaptaincyPktQ;

extern sdword captainProposal;
extern sdword captainTransferState;
extern sdword receiveSyncPacketsFrom;

extern bool32 hrAbortLoadingGame;

extern bool32 explicitlyRequestingPackets;
extern udword explicitlyRequestingFrom;
extern udword explicitlyRequestingTo;

extern bool8 ComputerPlayerOn[MAX_MULTIPLAYER_PLAYERS];

extern bool32 startRecordingGameWhenSafe;

/*=============================================================================
    Public Macros:
=============================================================================*/

#define LOG_OFF     0
#define LOG_ON      1
#define LOG_VERBOSE 2

/*=============================================================================
    Functions:
=============================================================================*/

void globalsResetFunc(bool32 firstTime);
#define globalsReset()  globalsResetFunc(0)
#define globalsInit() globalsResetFunc(1)

void globalsClose(void);

#endif

