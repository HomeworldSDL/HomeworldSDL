/*=============================================================================
    Name    : titannet.h
    Purpose : Definitions for the titan game networking stuff.

    Created 6/23/1998 by ddunlop
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___TITANNET_H
#define ___TITANNET_H

#include "TitanInterfaceC.h"
#include "Types.h"

extern real32 TITAN_PICKER_REFRESH_TIME;

void generateDirectoryCustomInfo(DirectoryCustomInfoMax *buildDirectoryCustomInfo);

void captainGameStartCB(void);
void titanGameDisolved(bool32 shutdown);
void titanUpdatePlayer(bool32 captain);

void titanResetGameCreated(void);

void titanGameStartup(void);
void titanGameShutdown(void);

void titanGameEnded(void);

extern wchar_t RemoveGameRequest[];

extern wchar_t CurrentChannel[];
extern wchar_t CurrentChannelDescription[];

extern real32 KEEPALIVE_SEND_IAMALIVE_TIME;
extern real32 KEEPALIVE_IAMALIVE_TIMEOUT;
extern real32 KEEPALIVE_SEND_ALIVESTATUS_TIME;

bool32 titanCheckValidVersion(char *myversion);
bool32 CheckNetworkVersionCompatibility(char *netversion);

extern bool32 HaveValidVersions;
extern char ValidVersions[];

extern udword ROOM_MIN_THRESHOLD;
extern udword ROOM_MAX_THRESHOLD;

bool32 titanKickPlayer(udword i);

#endif
