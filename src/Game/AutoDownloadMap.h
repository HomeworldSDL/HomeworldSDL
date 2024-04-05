// =============================================================================
//  AutoDownloadMap.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 99/03/29 by gshaw
// =============================================================================

#ifndef ___AUTODOWNLOADMAP_H
#define ___AUTODOWNLOADMAP_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------

void autodownloadmapStartup(void);
void autodownloadmapShutdown(void);
void autodownloadmapReset(void);
void autodownloadmapGetFilesOfMap(void);
void autodownloadmapGotMapName(char *mapname, sdword numPlayers, sdword minPlayers, sdword maxPlayers);
bool32 autodownloadmapRequired(void);
void receivedFilePacketCB(ubyte *packet,udword sizeofPacket);
bool32 autodownloadmapReceivedAllFiles(void);
void autodownloadmapSendAllFiles(void);
void autodownloadmapPrintStatusStart(void);
real32 autodownloadmapPercentReceivedFiles(void);
real32 autodownloadmapPercentSentFiles(void);
bool32 autodownloadmapSendAFile(void);

#endif

