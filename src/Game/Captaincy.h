// =============================================================================
//  Captaincy.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 98/10/06 by gshaw
// =============================================================================

#ifndef ___CAPTAINCY_H
#define ___CAPTAINCY_H

#include "MaxMultiplayer.h"
#include "Types.h"

// INTERFACE -------------------------------------------------------------------

typedef enum CaptaincyEvent {
    CAPEVENT_STARTGAME,
    CAPEVENT_TIMEOUT1,
    CAPEVENT_TIMEOUT2,
    CAPEVENT_TWAITFORPAUSEACKS,
    CAPEVENT_GIVEUP_CAPTAINCY_NOTICE,
    CAPEVENT_CAPTAIN_PROPOSAL,
    CAPEVENT_PAUSE_TRANSFERRING_CAPTAIN,
    CAPEVENT_PAUSE_ACK,
    CAPEVENT_PAUSE_YOU_BE_CAPTAIN,
    CAPEVENT_I_AM_NEW_CAPTAIN,
    CAPEVENT_RECEIVED_SYNC_PKT
} CaptaincyEvent;

typedef struct CaptaincyCustomInfo
{
    udword custom[MAX_MULTIPLAYER_PLAYERS];
} CaptaincyCustomInfo;


void TransferCaptaincyGameEnded(void);
void TransferCaptaincyGameStarted(void);

void TransferCaptaincyStateMachine(CaptaincyEvent event, uword from, udword misc, CaptaincyCustomInfo *customInfo);
void TransferCaptaincySyncPacketReceivedNotify(void);

void TransferCaptaincyUpdate(void);

void TimeoutTimerInitAll(void);
void TimeoutTimerCloseAll(void);
void TimeoutTimerUpdateAll(void);

void GiveUpCaptaincy(void);

void captaincyLogInit(void);
void captaincyLog(bool32 echotoscreen, char *format, ...);


extern bool32 transferCaptaincyDisabled;
extern bool32 captaincyLogEnable;
extern bool32 printCaptainMessage;

extern real32 T1_Timeout;
extern real32 T2_Timeout;
extern real32 TWAITFORPAUSEACKS_Timeout;

#endif

