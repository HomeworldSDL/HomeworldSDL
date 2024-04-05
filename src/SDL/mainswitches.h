
#ifndef ___MAINSWITCHES_H
#define ___MAINSWITCHES_H

#include "MaxMultiplayer.h"
#include "Types.h"

#ifndef STATVIEWER_PROGRAM
//statviewer program is compiling this bitch...lets not do this
#endif

//command-line switches and parameters
extern bool32 DebugWindow;
extern sdword MemoryHeapSize;
extern bool32 mouseClipped;
extern bool32 startupClipMouse;
extern bool32 showFrontEnd;
extern bool32 enableSFX;
extern bool32 enableSpeech;
extern bool32 reverseStereo;
extern bool32 useWaveout;
extern bool32 useDSound;
extern bool32 coopDSound;
extern bool32 noDefaultComputerPlayer;
extern bool8 ComputerPlayerEnabled[MAX_MULTIPLAYER_PLAYERS];
extern udword ComputerPlayerLevel[MAX_MULTIPLAYER_PLAYERS];
extern bool32 gatherStats;

extern bool32 showStatsFight;
extern udword showStatsFightI;
extern udword showStatsFightJ;

extern bool32 showStatsFancyFight;
extern char showStatsFancyFightScriptFile[50];

extern sdword GiveAllTechnology;

#ifdef DEBUG_TACTICS
    extern bool32 tacticsOn;
#endif
extern bool32 noRetreat;

extern bool32 recordPackets;
extern bool32 playPackets;
extern bool32 recordplayPacketsInGame;
extern bool32 recordFakeSendPackets;
#define MAX_RECORDPACKETFILENAME_STRLEN 50
extern char recordPacketFileName[MAX_RECORDPACKETFILENAME_STRLEN];
extern char recordPacketSaveFileName[MAX_RECORDPACKETFILENAME_STRLEN];

extern bool32 noAuthorization;

extern bool32 mainCDCheckEnabled;

extern bool32 SecretWON;
extern bool32 forceLAN;

extern bool32 ShortCircuitWON;

extern char versionString[];
extern char networkVersion[];
extern char languageVersion[];
extern char minorBuildVersion[];

extern bool32 autoSaveDebug;

extern bool32 determCompPlayer;

extern bool32 bkDisableKeyRemap;

extern bool32 debugPacketRecord;

extern bool32 pilotView;

#endif

