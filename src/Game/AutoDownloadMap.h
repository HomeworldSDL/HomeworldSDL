// =============================================================================
//  AutoDownloadMap.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
// =============================================================================

#ifndef ___AUTODOWNLOADMAP_H
#define ___AUTODOWNLOADMAP_H

#include "Types.h"
#ifdef __GNUC__
 #ifndef _STRING_H
  #include <string.h>
 #endif
#endif

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
void autodownloadmapPrintStatus();
real32 autodownloadmapPercentReceivedFiles();
real32 autodownloadmapPercentSentFiles();
bool autodownloadmapSendAFile(void);

#endif

