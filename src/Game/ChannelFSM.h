// =============================================================================
//  ChannelFSM.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
// =============================================================================

#ifndef ___CHANNELFSM_H
#define ___CHANNELFSM_H

#include "Types.h"

bool32 cCreateChannel(wchar_t *name, wchar_t *desc);
bool32 cJoinChannelRequest(wchar_t *name, wchar_t *desc);
void cNotifyChatConnected(void);
void cResetFSM(void);
bool32 cJoinADefaultRoom(void);
void cNotifyChatBadResponse(void);

#endif

