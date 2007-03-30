// =============================================================================
//  ChannelFSM.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
// =============================================================================

#ifndef ___CHANNELFSM_H
#define ___CHANNELFSM_H

#include "Types.h"

bool cCreateChannel(wchar_t *name, wchar_t *desc);
bool cJoinChannelRequest(wchar_t *name, wchar_t *desc);
void cNotifyChatConnected(void);
void cResetFSM(void);
bool cJoinADefaultRoom(void);
void cNotifyChatBadResponse(void);

#endif

