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

void autodownloadmapStartup();
void autodownloadmapShutdown();
void autodownloadmapReset();
void autodownloadmapGetFilesOfMap(void);
void autodownloadmapGotMapName(char *mapname, sdword numPlayers, sdword minPlayers, sdword maxPlayers);
bool autodownloadmapRequired();
void receivedFilePacketCB(ubyte *packet,udword sizeofPacket);
bool autodownloadmapReceivedAllFiles();
void autodownloadmapSendAllFiles(void);
void autodownloadmapPrintStatusStart();
real32 autodownloadmapPercentReceivedFiles();
real32 autodownloadmapPercentSentFiles();
bool autodownloadmapSendAFile(void);

#endif

