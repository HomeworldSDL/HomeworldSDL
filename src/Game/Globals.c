/*=============================================================================
    Name    : Globals.c
    Purpose : Contains game global variables

    Created 7/30/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "Globals.h"
#include "mainswitches.h"
#include "Universe.h"
#include "AIPlayer.h"
#include "CommandNetwork.h"
#include "NetCheck.h"
#include "MultiplayerGame.h"
#include "ProfileTimers.h"
#include "mainrgn.h"

/*=============================================================================
    Global variables
=============================================================================*/

// variables initialized here should also be initialized in globalsReset()

bool32 gameIsRunning = FALSE;

bool32 multiPlayerGame = FALSE;
bool32 multiPlayerGameUnderWay = FALSE;

bool32 singlePlayerGame = FALSE;
bool32 objectivesShown = TRUE;

sdword tutorial = 0;
bool32 tutorialdone = FALSE;

bool32 playersReadyToGo[MAX_MULTIPLAYER_PLAYERS];

int sigsPressedStartGame = FALSE;

uword numPlayers = 0;
uword curPlayer = 0;

udword startingGameState = 0;
bool32 HaveSentNonCaptainReadyPacket = FALSE;

bool32 startingGame = FALSE;

void *gMessageMutex = NULL;
gMessageType gMessage[MAX_MESSAGES+1];
bMessageType bMessage[MAX_BIGMESSAGES];

#ifdef HW_BUILD_FOR_DISTRIBUTION
udword logEnable = LOG_OFF;
#else
udword logEnable = LOG_ON;
#endif
char logFilePath[300] = "";

#ifdef GOD_LIKE_SYNC_CHECKING
sdword syncDumpWindowSize;
sdword syncDumpGranularity;
bool32 syncDumpOn=FALSE;
sdword syncDumpWindowPos=0;
sdword syncDumpGranTrack=0;
#endif

#ifdef OPTIMIZE_VERBOSE
sdword vecNormalizeCounter;
#endif


#ifdef DEBUG_GAME_STATS
bool32 statLogOn=FALSE;
#endif


Queue ProcessSyncPktQ;
Queue ProcessRequestedSyncPktQ;
Queue ProcessCmdPktQ;
Queue ProcessCaptaincyPktQ;

int sigsNumPlayers = 0;
int sigsPlayerIndex = 0;

char playerNames[MAX_MULTIPLAYER_PLAYERS][MAX_PERSONAL_NAME_LEN];

signed int captainIndex = -1;
sdword captainProposal = 1;
sdword captainTransferState = 0;

sdword receiveSyncPacketsFrom = 0;

bool32 hrAbortLoadingGame = FALSE;

bool32 explicitlyRequestingPackets = FALSE;
udword explicitlyRequestingFrom = 0;
udword explicitlyRequestingTo = 0;

bool8 ComputerPlayerOn[MAX_MULTIPLAYER_PLAYERS] = { -1,-1,-1,-1,-1,-1,-1,-1 };

sdword numPendingIn = 0;
sdword numPendingOut = 0;

bool32 startRecordingGameWhenSafe = FALSE;

/*-----------------------------------------------------------------------------
    Name        : globalsReset
    Description : resets the globals.c variables
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void globalsResetFunc(bool32 firstTime)
{
    udword i = 0;

    if (firstTime)
    {
        gMessageMutex = gameCreateMutex();
    }

    gameIsRunning = FALSE;
    multiPlayerGame = FALSE;
    multiPlayerGameUnderWay = FALSE;
    singlePlayerGame = FALSE;
    tutorial = 0;
    tutorialdone = FALSE;
    sigsPressedStartGame = FALSE;
    sigsNumPlayers = 0;
    sigsPlayerIndex = 0;
    captainIndex = -1;
    captainProposal = 1;
    captainTransferState = 0;
    receiveSyncPacketsFrom = 0;

    startRecordingGameWhenSafe = FALSE;

    aiCurrentAIPlayer = NULL;

    hrAbortLoadingGame = FALSE;

    mrRenderMainScreen = TRUE;

    explicitlyRequestingPackets = FALSE;
    explicitlyRequestingFrom = 0;
    explicitlyRequestingTo = 0;

    startingGameState = 0;
    startingGame = FALSE;
    HaveSentNonCaptainReadyPacket = FALSE;
    numPlayers = 0;
    curPlayer = 0;
    universePause = FALSE;

    LockMutex(gMessageMutex);
    while (i < MAX_MESSAGES)
    {
        gMessage[i].MessageExpire = (real32)0;
        gMessage[i].message[0]    = '\0';
        i++;
    }
    UnLockMutex(gMessageMutex);

    for (i=0;i<MAX_BIGMESSAGES;i++)
    {
        bMessage[i].messageOn = FALSE;
        bMessage[i].message[0] = '\0';
    }

    for (i=0;i<MAX_MULTIPLAYER_PLAYERS;i++)
    {
        playersReadyToGo[i] = FALSE;
    }

    CommandNetworkReset();
    netcheckReset();

    if (!firstTime)
    {
        profReset();
        for (i=0;i<MAX_MULTIPLAYER_PLAYERS;i++)
        {
            ComputerPlayerEnabled[i] = FALSE;
            ComputerPlayerOn[i] = -1;
        }

        recordPackets = FALSE;
        playPackets = FALSE;
        recordplayPacketsInGame = FALSE;
        recordFakeSendPackets = FALSE;

        smGhostMode = FALSE;
    }
    else
    {
        profInit();
    }
}

void globalsClose(void)
{
    if (gMessageMutex)
    {
        gameCloseMutex(gMessageMutex);
    }
    profClose();
}

