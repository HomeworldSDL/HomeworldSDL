/*============================================================================
    MAIN.C: Main Windows interface for Homeworld game code, including message
        handling and window set-up.

        Created June 1997 by Luke Moloney.
============================================================================*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winreg.h>
#endif

#include "SDL.h"

#include "regkey.h"
                // guess what?  The game code defines HKEY to 'H' which messes up the registry code.  So the
                // registry code gets to go here
#if 0	/* Not registering command line... */
int RegisterCommandLine(char *commandLine)
{
    HKEY key;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, BASEKEYNAME,
                        0, KEY_SET_VALUE, &key) != ERROR_SUCCESS)
    {
        return FALSE;
    }

    if ((commandLine == NULL) || (commandLine[0] == 0))
    {
        if (RegSetValueEx(key, "CmdLine", 0, REG_SZ, "", 1) != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return FALSE;
        }
    }
    else
    {
        if (RegSetValueEx(key, "CmdLine", 0, REG_SZ, (BYTE *)commandLine, strlen(commandLine)+1) != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return FALSE;
        }
    }

    RegCloseKey(key);

    return TRUE;
}
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "glinc.h"
#include "resource.h"
#include "utility.h"
#include "Key.h"
#include "mouse.h"
#include "debugwnd.h"
#include "Debug.h"
#include "Memory.h"
#include "File.h"
#include "Camera.h"
#include "Task.h"
#include "mainrgn.h"
#include "main.h"
#include "Globals.h"
#include "SoundEvent.h"
#include "soundlow.h"
#include "NIS.h"
#include "AIPlayer.h"
#include "FontReg.h"
#include "FEReg.h"
#include "Demo.h"
#include "ResearchAPI.h"
#include "ObjTypes.h"
#include "Formation.h"
#include "glcaps.h"
#include "Tactics.h"
#include "render.h"
#include "AutoLOD.h"
#include "Captaincy.h"
#include "Options.h"
#include "Sensors.h"
#include "BTG.h"
#include "ResearchGUI.h"
#include "ConsMgr.h"
#include "rinit.h"
#include "avi.h"
/*#include "bink.h"*/
#include "TitanNet.h"
#include "MultiplayerGame.h"
#include "Subtitle.h"
#include "dxdraw.h"
#include "LaunchMgr.h"
#include "ColPick.h"
#include "HorseRace.h"
#include "glcompat.h"
#include "sstglide.h"
#include "Particle.h"
#include "CommandLayer.h"
#include "Key.h"
#include "Strings.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

/*=============================================================================
    Data:
=============================================================================*/

extern udword gDevcaps, gDevcaps2;

bool mainSafeGL = FALSE;

udword* devTable = NULL;
sdword  devTableLength = 0;

static bool mainActuallyQuit = TRUE;

bool selectedRES = FALSE;
bool selectedDEVICE = FALSE;
bool selectedGL = FALSE;

sdword mainReinitRenderer = 0;
static bool reinitInProgress = FALSE;

bool bMustFree = TRUE;

bool mainPlayAVIs;

bool windowNeedsDeleting = FALSE;

bool mainNoPerspective = FALSE;

bool systemActive = FALSE;              //active flag for the program

#ifndef _MACOSX
static char windowTitle[] = "Homeworld";//name of window
#endif

//error strings
char ersWindowInit[] = "Error creating window";

//screen width, height
sdword MAIN_WindowWidth = 640;
sdword MAIN_WindowHeight = 480;
sdword MAIN_WindowDepth = 16;

sdword mainWidthAdd = 0;
sdword mainHeightAdd = 0;

sdword mainWindowWidth = 640;
sdword mainWindowHeight = 480;
sdword mainWindowDepth = 16;
#ifdef _WIN32
void *ghMainWindow = NULL;
void *ghInstance = NULL;
#endif

extern bool CompareBigfiles;
extern bool IgnoreBigfiles;
extern bool LogFileLoads;

//command-line switches and parameters
bool mainNoDrawPixels = FALSE;
bool mainOutputCRC = FALSE;
bool mainNoPalettes = FALSE;
bool mainSoftwareDirectDraw = TRUE;
bool mainDirectDraw = TRUE;
bool mainRasterSkip = FALSE;
bool mainDoubleIsTriple = FALSE;
bool mainFastFrontend = TRUE;
bool mainForceSoftware = FALSE;
bool mainAutoRenderer = TRUE;
bool mainForceKatmai = FALSE;
bool mainAllowKatmai = FALSE;
bool mainAllow3DNow = FALSE;
bool enableAVI = TRUE;
bool mainAllowPacking = TRUE;
bool mainOnlyPacking = FALSE;
bool gShowDamage = TRUE;
bool DebugWindow = FALSE;
sdword MemoryHeapSize = MEM_HeapSizeDefault;
bool FilePathPrepended = FALSE;
bool CDROMPathPrepended = FALSE;
bool UserSettingsPathPrepended = FALSE;
#if MAIN_MOUSE_FREE
bool startupClipMouse = TRUE;
#endif
bool mouseClipped = FALSE;
sdword showBackgrounds = TRUE;
bool showBorder = TRUE;
sdword enableTextures = TRUE;
sdword enableSmoothing = TRUE;
sdword enableStipple = FALSE;
sdword enableTrails = TRUE;
#if TR_NIL_TEXTURE
bool GLOBAL_NO_TEXTURES = FALSE;
#endif
bool fullScreen = TRUE;
bool slowBlits = FALSE;
#if RND_VISUALIZATION
bool dockLines = FALSE;
bool gunLines = FALSE;
bool8 RENDER_BOXES;
bool8 RENDER_LIGHTLINES;
#endif
bool enableSFX = TRUE;
bool enableSpeech = TRUE;
bool reverseStereo = FALSE;
bool useWaveout = FALSE;
bool useDSound = FALSE;
bool coopDSound = FALSE;
bool accelFirst = FALSE;
char mainDeviceToSelect[128] = "";
char mainGLToSelect[512] = "";
char mainD3DToSelect[128] = "";
char deviceToSelect[128] = "";
#ifdef _WIN32
char glToSelect[512] = "librgl.dll";
#else
char glToSelect[512] = "librgl.so";
#endif
bool8 RENDER_BOXES = FALSE;
bool8 RENDER_LIGHTLINES = FALSE;
#if CL_TEXTFEEDBACK
bool enableTextFeedback = FALSE;
#endif

#if FE_TEXTURES_DISABLABLE
bool fetEnableTextures = TRUE;
#endif
bool noDefaultComputerPlayer = FALSE;
bool8 ComputerPlayerEnabled[MAX_MULTIPLAYER_PLAYERS] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
//udword ComputerPlayerLevel[MAX_MULTIPLAYER_PLAYERS] = {2,2,2,2,2,2,2,2};
bool gatherStats = FALSE;
bool showStatsFight = FALSE;
udword showStatsFightI = 0;
udword showStatsFightJ = 0;
bool showStatsFancyFight = FALSE;
char showStatsFancyFightScriptFile[50];
bool SecretWON = FALSE;
bool forceLAN = FALSE;
bool ShortCircuitWON = FALSE;
#if DBW_TO_FILE
bool debugToFile = FALSE;
#endif

bool debugPacketRecord = FALSE;

bool recordPackets = FALSE;
bool playPackets = FALSE;
bool recordplayPacketsInGame = FALSE;
bool recordFakeSendPackets = FALSE;
char recordPacketFileName[MAX_RECORDPACKETFILENAME_STRLEN];
char recordPacketSaveFileName[MAX_RECORDPACKETFILENAME_STRLEN];

bool autoSaveDebug = FALSE;

#ifdef DEBUG_TACTICS
    bool tacticsOn = TRUE;
#endif
bool noRetreat = FALSE;

bool noPauseAltTab = FALSE;
bool noMinimizeAltTab = FALSE;

//options altered by a password function:
bool mainSinglePlayerEnabled = FALSE;
bool mainEnableSpecialMissions = FALSE;
bool mainScreenShotsEnabled = FALSE;
bool mainCDCheckEnabled = TRUE;

//char versionString[MAX_VERSION_STRING_LEN] = "M23bCGWRC1";
//char versionString[MAX_VERSION_STRING_LEN] = "M23dFinalRC1";
//char versionString[MAX_VERSION_STRING_LEN] = "M24aFinalRC3";
//char versionString[MAX_VERSION_STRING_LEN] = "M24bPublicBeta2";
//char versionString[MAX_VERSION_STRING_LEN] = "M24cFinalRC4";
//char versionString[MAX_VERSION_STRING_LEN] = "M24ePublicBeta2";
//char versionString[MAX_VERSION_STRING_LEN] = "M24fFinalRC5";

#if defined(HW_DEMO)
//char networkVersion[] =   "DownloadableRC1";
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "DLD_05";
#elif defined(HW_COMPUTER_GAMING_WORLD_DEMO)
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "CGWtronRC1";
#elif defined(HW_RAIDER_RETREAT)
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "OEMV1";
#else
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "HomeworldV1C";
#endif

#if defined(HW_RAIDER_RETREAT)
char minorBuildVersion[] = "051";
#else
char minorBuildVersion[] = "06.1";
#endif
char languageVersion[50] = "";    // constructed at beginning of program

// The version string is now constructed as follows:
//
// Network Version TAB Language TAB MinorBuild Version
//
// for example
// "DownloadableRC2\tEnglish\tBuild1"
//
//

// For purposes of games able to play each other on the network, just the networkVersion is consulted
//
// For purposes of whether a patch is required (valid version), the entire versionString (network language build) is consulted
//
// For finding out what directory to get the patch from, only the languageVersion is consulted

char versionString[MAX_VERSION_STRING_LEN] = "";        // constructed at beginning of program


sdword mainWindowTotalWidth = 0;
sdword mainWindowTotalHeight = 0;

bool noAuthorization = FALSE;

bool determCompPlayer = FALSE;

#if MAIN_SENSOR_LEVEL
udword initialSensorLevel = 0;
#endif

bool pilotView = FALSE;

//data for password-protecting certain options
#if MAIN_Password
char *mainMonthStrings[12] =
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};
char mainCompileDate[] = __DATE__;

//password: each character is xor'd with the corresponding character in this string:
ubyte mainOperatorString[] = "132AS sdFmfm na5r\x0\\6234asSDFG,m.";
char mainPassword0[] =
{
    //"BravoCharlie": unlocks single player game only
    '1' ^ 'B',
    '3' ^ 'r',
    '2' ^ 'a',
    'A' ^ 'v',
    'S' ^ 'o',
    ' ' ^ 'C',
    's' ^ 'h',
    'd' ^ 'a',
    'F' ^ 'r',
    'm' ^ 'l',
    'f' ^ 'i',
    'm' ^ 'e',
    ' ' ^ 0,
/*
    'n' ^ '',
    'a' ^ '',
    '5' ^ '',
    'r' ^ '',
    0]  '',
    '\\] ^ '',
    '6' ^ '',
    '2' ^ '',
    '3' ^ '',
    '4' ^ '',
    'a' ^ '',
    's' ^ '',
    'S' ^ '',
    'D' ^ '',
    'F' ^ '',
    'G' ^ '',
    ',' ^ '',
    'm' ^ '',
    '.' ^ '',
*/
};
char mainPassword1[] =
{
    //"Cheeky-Monkey": unlocks all features
    '1' ^ 'C',
    '3' ^ 'h',
    '2' ^ 'e',
    'A' ^ 'e',
    'S' ^ 'k',
    ' ' ^ 'y',
    's' ^ '-',
    'd' ^ 'M',
    'F' ^ 'o',
    'm' ^ 'n',
    'f' ^ 'k',
    'm' ^ 'e',
    ' ' ^ 'y',
    'n' ^ 0,
/*
    'a' ^ '',
    '5' ^ '',
    'r' ^ '',
    0]  '',
    '\\] ^ '',
    '6' ^ '',
    '2' ^ '',
    '3' ^ '',
    '4' ^ '',
    'a' ^ '',
    's' ^ '',
    'S' ^ '',
    'D' ^ '',
    'F' ^ '',
    'G' ^ '',
    ',' ^ '',
    'm' ^ '',
    '.' ^ '',
*/
};
udword mainPasswordChecksum0 =
    ((udword)0x0 ^ (udword)'B') +
    ((udword)0x1 ^ (udword)'r') +
    ((udword)0x2 ^ (udword)'a') +
    ((udword)0x3 ^ (udword)'v') +
    ((udword)0x4 ^ (udword)'o') +
    ((udword)0x5 ^ (udword)'C') +
    ((udword)0x6 ^ (udword)'h') +
    ((udword)0x7 ^ (udword)'a') +
    ((udword)0x8 ^ (udword)'r') +
    ((udword)0x9 ^ (udword)'l') +
    ((udword)0xa ^ (udword)'i') +
    ((udword)0xb ^ (udword)'e');
udword mainPasswordChecksum1 =
    ((udword)0x0 ^ (udword)'C') +
    ((udword)0x1 ^ (udword)'h') +
    ((udword)0x2 ^ (udword)'e') +
    ((udword)0x3 ^ (udword)'e') +
    ((udword)0x4 ^ (udword)'k') +
    ((udword)0x5 ^ (udword)'y') +
    ((udword)0x6 ^ (udword)'-') +
    ((udword)0x7 ^ (udword)'M') +
    ((udword)0x8 ^ (udword)'o') +
    ((udword)0x9 ^ (udword)'n') +
    ((udword)0xa ^ (udword)'k') +
    ((udword)0xb ^ (udword)'e') +
    ((udword)0xc ^ (udword)'y');
char *mainPasswordPtr = NULL;
#endif //MAIN_Password


/*=============================================================================
    Functions:
=============================================================================*/
/*-----------------------------------------------------------------------------
    Command-line parsing functions called when a certain flags are set
-----------------------------------------------------------------------------*/
bool HeapSizeSet(char *string)
{
    sscanf(string, "%d", &MemoryHeapSize);
    if (MemoryHeapSize <= MEM_BlockSize * 2)
    {
        MemoryHeapSize = MEM_HeapSizeDefault;
    }
    return TRUE;
}

bool PrependPathSet(char *string)
{
    filePrependPathSet(string);
    FilePathPrepended = TRUE;
    return TRUE;
}

bool CDROMPathSet(char *string)
{
#ifdef _WIN32
    char message[80];

    if (GetDriveType(string) != DRIVE_CDROM)
    {
        sprintf(message, "'%s' Is not a valid CD-ROM; path ignored.", string);
        MessageBox(NULL, message, "Invalid CD-ROM path", MB_OK | MB_APPLMODAL);
        return FALSE;
    }
#endif
    fileCDROMPathSet(string);
    CDROMPathPrepended = TRUE;
    return TRUE;
}

bool UserSettingsPathSet(char *string)
{
	fileUserSettingsPathSet(string);
	UserSettingsPathPrepended = TRUE;
	return TRUE;
}

bool EnableFileLoadLog(char *string)
{
    logfileClear(FILELOADSLOG);
    return TRUE;
}

bool SelectDevice(char* string)
{
    memStrncpy(deviceToSelect, string, 16 - 1);
    if (strcasecmp(deviceToSelect, "d3d") == 0)
    {
        mainReinitRenderer = 2;
    }
    selectedDEVICE = TRUE;
    return TRUE;
}

bool SelectMSGL(char* string)
{
#ifdef _WIN32
    memStrncpy(glToSelect, "opengl32.dll", 512 - 1);
#else
    memStrncpy(glToSelect, "libGL.so", 512 - 1);
#endif
    return TRUE;
}

bool SelectD3D(char* string)
{
#ifdef _WIN32
    memStrncpy(glToSelect, "librgl.dll", 512 - 1);
#else
    memStrncpy(glToSelect, "librgl.so", 512 - 1);
#endif
    memStrncpy(deviceToSelect, "d3d", 16 - 1);
    selectedGL = TRUE;
    selectedDEVICE = TRUE;
    mainReinitRenderer = 2;
    return TRUE;
}

bool EnableRasterSkip(char* string)
{
    mainRasterSkip = TRUE;
    return TRUE;
}

bool EnableDoubleIsTriple(char* string)
{
    mainDoubleIsTriple = TRUE;
    return TRUE;
}

bool DisableFastFrontend(char* string)
{
    mainFastFrontend = !mainFastFrontend;
    return TRUE;
}

bool EnableGatherStats(char *string)
{
    noDefaultComputerPlayer = TRUE;
    return TRUE;
}

bool EnableShowStatsFancyFight(char *string)
{
    showStatsFancyFight = TRUE;
    strcpy(showStatsFancyFightScriptFile,string);
    noDefaultComputerPlayer = TRUE;
    return TRUE;
}

bool EnableShowStatsFight(char *string)
{
    sscanf(string, "%d", &showStatsFightI);

    if ((string = strtok(NULL, TS_Delimiters)) == NULL)
        return TRUE;        //error??
    sscanf(string, "%d", &showStatsFightJ);

    showStatsFight = TRUE;

    noDefaultComputerPlayer = TRUE;
    return TRUE;
}

bool SpecifyLogFilePath(char *string)
{
    strcpy(logFilePath,string);
    return TRUE;
}

/*
void EnableComputerPlayers(char *string)
{
    char playerStr[20];
    char *scanning = playerStr;
    char index;

    if ((string = strtok(NULL, TS_Delimiters)) == NULL)
        return;
    sscanf(string, "%s", playerStr);

    while (*scanning != '\0')
    {
        if ((*scanning >= '0') && (*scanning <= '9'))
        {
            index = (char)(*scanning - '0');
            dbgAssert(index < MAX_MULTIPLAYER_PLAYERS);
            dbgAssert(index >= 0);

            ComputerPlayerEnabled[index] = TRUE;
        }

        scanning++;
    }

    noDefaultComputerPlayer = TRUE;
}
*/
#if 0
bool SetAIPlayerLevels(char *string)
{
    char playerStr[20];
    char *scanning = playerStr;
    char level, i;


    if ((string = strtok(NULL, TS_Delimiters)) == NULL)
        return TRUE;    //error?
    sscanf(string, "%s", playerStr);

    for (i=0;(*scanning != '\0') || (i > MAX_MULTIPLAYER_PLAYERS); )
    {
        if ((*scanning >= '0') && (*scanning <= '9'))
        {
            level = (char)(*scanning - '0');
            dbgAssert(level < (char)AI_NUM_LEVELS);
            dbgAssert(level >= 0);

            ComputerPlayerLevel[i] = level;
            i++;
        }

        scanning++;
    }
    return TRUE;
}
#endif

#if NIS_TEST
extern char *nisTestNIS;
extern char *nisTestScript;
bool TestNISSet(char *string)
{
    static char staticString[256];

    if ((string = strtok(NULL, TS_Delimiters)) == NULL)
        return TRUE;
    memStrncpy(staticString, string, 255);
    nisTestNIS = staticString;
    return TRUE;
}
bool TestNISScriptSet(char *string)
{
    static char staticString[256];

    if ((string = strtok(NULL, TS_Delimiters)) == NULL)
        return TRUE;
    memStrncpy(staticString, string, 255);
    nisTestScript = staticString;
    return TRUE;
}
#endif

#if MAIN_SENSOR_LEVEL
bool InitialSensorLevelSet(char *string)
{
    sscanf(string, "%d", &initialSensorLevel);
    return TRUE;
}
#endif

#if LOD_SCALE_DEBUG
bool EnableLodScaleDebug(char *string)
{
    sscanf(string, "%f", &lodDebugScaleFactor);
    return TRUE;
}
#endif

bool EnableDemoRecord(char *string)
{
    if (!demDemoPlaying)
    {
        demDemoRecording = TRUE;
        strcpy(demDemoFilename, string);
    }
    return TRUE;
}

bool EnableDemoPlayback(char *string)
{
    if (!demDemoRecording)
    {
        wasDemoPlaying = demDemoPlaying = TRUE;
        noPauseAltTab = TRUE;
        strcpy(demDemoFilename, string);
    }
    return TRUE;
}

#if DEM_AUTO_DEMO
bool AutoDemoWaitSet(char *string)
{
    sscanf(string , "%f", &demAutoDemoWaitTime);
    return TRUE;
}
#endif

bool EnablePacketPlay(char *string)
{
    transferCaptaincyDisabled = TRUE;
    strcpy(recordPacketFileName, strtok(NULL, TS_Delimiters));
    return TRUE;
}

bool EnablePacketRecord(char *string)
{
    debugPacketRecord = TRUE;
    return TRUE;
}

bool EnableDebugSync(char *string)
{
    recordPackets = TRUE;
    logEnable = LOG_VERBOSE;
    autoSaveDebug = TRUE;
    return TRUE;
}

bool EnableAutoSaveDebug(char *string)
{
    autoSaveDebug = TRUE;
    return TRUE;
}

bool EnableMiniRes(char* string)
{
    selectedRES = TRUE;
    mainWindowWidth  = 320;
    mainWindowHeight = 240;
    return TRUE;
}

bool EnableLoRes(char *string)
{
    selectedRES = TRUE;
    mainWindowWidth  = 640;
    mainWindowHeight = 480;
    return TRUE;
}

bool EnableHiRes(char *string)
{
    selectedRES = TRUE;
    mainWindowWidth  = 800;
    mainWindowHeight = 600;
    return TRUE;
}

bool EnableMegaRes(char *string)
{
    selectedRES = TRUE;
    mainWindowWidth  = 1024;
    mainWindowHeight = 768;
    return TRUE;
}

bool EnableUltraRes(char* string)
{
    selectedRES = TRUE;
    mainWindowWidth  = 1280;
    mainWindowHeight = 1024;
    return TRUE;
}

bool EnableInsaneRes(char* string)
{
    selectedRES = TRUE;
    mainWindowWidth  = 1600;
    mainWindowHeight = 1200;
    return TRUE;
}

bool Enable32Bit(char* string)
{
    selectedRES = TRUE;
    MAIN_WindowDepth = 32;
    return TRUE;
}

bool Enable16Bit(char* string)
{
    selectedRES = TRUE;
    MAIN_WindowDepth = 16;
    return TRUE;
}

bool Enable24Bit(char* string)
{
    selectedRES = TRUE;
    MAIN_WindowDepth = 24;
    return TRUE;
}

#ifdef GOD_LIKE_SYNC_CHECKING

#define GUESS_NUM_SHIPS     400
bool syncDumpInit(char *string1)
{
    if(sscanf(string1,"%d!%d",&syncDumpWindowSize,&syncDumpGranularity) != 2)
    {
        return FALSE;
    }
    syncDumpOn = TRUE;
    syncDumpWindowPos=0;
    syncDumpGranTrack=0;

    //MemoryHeapSize += sizeof(Ship)*400*syncDumpWindowSize;
    return TRUE;
}
#endif

#if MAIN_Password
bool SetPassword(char *string)
{
    mainPasswordPtr = string;
    return TRUE;
}



/*-----------------------------------------------------------------------------
    Name        : mainPasswordVerify
    Description : Verifies the system password and unlocks some features, or if
                    there is no password, times the game out 30 days after compile.
    Inputs      : string - password string or NULL of none specified.
    Outputs     :
    Return      : NULL if no errors, or error string pointer.
----------------------------------------------------------------------------*/
char *mainPasswordVerify(char *string)
{
    udword index;
    udword checksum;
    time_t time0;
    struct tm *time1;
    char dayString0[16];
    char monthString0[16];
    char monthString1[16];
    sdword day0, hour0, min0, sec0, year0, month0;
    sdword day1, year1, month1;
    sdword nScanned;
    sdword daysOld;

    if (string != NULL)
    {
        //tamper-detect the password
        for (index = checksum = 0; (mainPassword1[index] ^ mainOperatorString[index]) != 0; index++)
        {
            checksum += (udword)index ^ (udword)((ubyte)(mainPassword1[index] ^ mainOperatorString[index]));
        }
        if (checksum != mainPasswordChecksum1)
        {
            //... error: wrong password checksum (tampering?)
            return("Invalid binaries.");
        }
        for (index = checksum = 0; mainPassword1[index] != 0; index++)
        {
            if (mainPassword1[index] != (string[index] ^ mainOperatorString[index]))
            {
                //... error: invalid password
                goto checkNextPassword;
            }
        }
        //... enable screen shots, single player and disable CD check
        mainScreenShotsEnabled = TRUE;
        mainSinglePlayerEnabled = TRUE;
        mainEnableSpecialMissions = TRUE;
        mainCDCheckEnabled = FALSE;
        return(NULL);                                       //they've got a password
checkNextPassword:
        //tamper-detect the password
        for (index = checksum = 0; (mainPassword0[index] ^ mainOperatorString[index]) != 0; index++)
        {
            checksum += (udword)index ^ (udword)((ubyte)(mainPassword0[index] ^ mainOperatorString[index]));
        }
        if (checksum != mainPasswordChecksum0)
        {
            //... error: wrong password checksum (tampering?)
            return("Invalid binaries.");
        }
        for (index = checksum = 0; mainPassword0[index] != 0; index++)
        {
            if (mainPassword0[index] != (string[index] ^ mainOperatorString[index]))
            {
                //... error: invalid password
                return("Invalid password.");
            }
        }
        //... enable single player only
        mainSinglePlayerEnabled = TRUE;
        mainEnableSpecialMissions = TRUE;
        return(NULL);                                       //they've got a password
    }
    else
    {                                                       //no password specified: see if it times out
        //tamper-detect the date
/*
        for (index = checksum = 0; index < 10; index++)
        {
            checksum += mainCompileDate[index] ^ index;
        }
        if (checksum != mainCompileDateChecksum)
        {
            //... error: wrong date (tampering?)
            return("Invalid binaries/data.");
        }
*/
        //compare current date to compile date and determine time expired
        time(&time0);
        time1 = gmtime(&time0);
        nScanned = sscanf(asctime(time1), "%s %s %d %d:%d:%d %d", dayString0, monthString0, &day0, &hour0, &min0, &sec0, &year0);
        if (nScanned != 7)
        {
            //... error: bad scan
            return("Bad scan xx(1)");
        }
        nScanned = sscanf(mainCompileDate, "%s %d %d", monthString1, &day1, &year1);
        if (nScanned != 3)
        {
            //... error: bad scan
            return("Bad scan xx(2)");
        }
        month0 = month1 = -1;
        for (index = 0; index < 12; index++)
        {
            if (!_stricmp(monthString0, mainMonthStrings[index]))
            {
                month0 = index;
            }
            if (!_stricmp(monthString1, mainMonthStrings[index]))
            {
                month1 = index;
            }
        }
        if (month0 == -1 || month1 == -1)
        {
            //... error: bad month string
            return("I hate this month");
        }
        daysOld = 365 * (year1 - year0) + 30 * (month1 - month0) + (day1 - day0);
        //        ^^^                     ^^
        // I know 30 isn't the length of all months, but let's just assume for now
        //
        if (daysOld > MAIN_ExpiryTime)
        {
            //... error: Evaluation copy expired
            return("Skynyrd!");
        }
    }
    return(NULL);
}
#endif //MAIN_Password

/*-----------------------------------------------------------------------------
    Structures used for command paramters and help
-----------------------------------------------------------------------------*/
#define entryFn(command, func, help)        {COF_Visible, command, func, NULL, 0, help}
#define entryVr(command, var, value, help)  {COF_Visible, command, NULL, &var, value, help}
#define entryFV(command, func, var, value, help)   {COF_Visible, command, func, &var, value, help}
#define entryFnParam(command, func, help)        {COF_Visible | COF_NextToken, command, func, NULL, 0, help}
#define entryFVParam(command, func, var, help)   {COF_Visible | COF_NextToken, command, func, &var, value, help}
#define entryFnHidden(command, func, help)        {0, command, func, NULL, 0, help}
#define entryVrHidden(command, var, value, help)  {0, command, NULL, &var, value, help}
#define entryFVHidden(command, func, var, value, help)   {0, command, func, &var, value, help}
#define entryFnParamHidden(command, func, help)        {COF_NextToken, command, func, NULL, 0, help}
#define entryFVParamHidden(command, func, var, help)   {COF_NextToken, command, func, &var, value, help}
#define entryComment(comment)               {COF_Visible, comment, NULL, NULL, 0, NULL}
commandoption commandOptions[] =
{
#ifndef HW_Release
    entryComment("DEBUGGING OPTIONS"),//-----------------------------------------------------
    entryVr("/debug",               DebugWindow, TRUE,                  " - Enable debug window."),
    entryVr("/nodebugInt",          dbgInt3Enabled,FALSE,               " - Fatal errors don't genereate an int 3 before exiting."),
#if DBW_TO_FILE
    entryVr("/debugToFile",         debugToFile, TRUE,                  " - output debugging info to a file."),
#endif
#else
    entryVrHidden("/debug",         DebugWindow, TRUE,                  " - Enable debug window."),
    entryVrHidden("/nodebugInt",    dbgInt3Enabled,FALSE,               " - Fatal errors don't genereate an int 3 before exiting."),
#if DBW_TO_FILE
    entryVrHidden("/debugToFile",   debugToFile, TRUE,                  " - output debugging info to a file."),
#endif
#endif
#if RAN_DEBUG_CALLER
    entryVr("/ranCallerDebug",      ranCallerDebug, TRUE,               " - debug non-deterministic calling of random numbers."),
#endif
#ifndef HW_Release
    entryFn("/autosavedebug",       EnableAutoSaveDebug,                " autosaves game frequently"),
#endif

    entryComment("SYSTEM OPTIONS"), //-----------------------------------------------------
    entryFnParam("/heap",           HeapSizeSet,                        " <n> - Sets size of global memory heap to [n]."),
    entryFnParam("/prepath",        PrependPathSet,                     " <path> - Sets path to search for opening files."),
    entryFnParam("/CDpath",         CDROMPathSet,                       " <path> - Sets path to CD-ROM in case of ambiguity."),
    entryFnParam("/settingspath",   UserSettingsPathSet,                " <path> - Sets the path to store settings, saved games, and screenshots (defaults to ~/.homeworld)."),
#if MAIN_MOUSE_FREE
#ifndef HW_Release
    entryVr("/freemouse",           startupClipMouse, FALSE,            " - Mouse free to move about entire screen at startup.  Use <CTRL>F11 to toggle during play."),
#else
    entryVrHidden("/freemouse",     startupClipMouse, FALSE,            " - Mouse free to move about entire screen at startup.  Use <CTRL>F11 to toggle during play."),
#endif
#endif
#ifndef HW_Release
    entryVr("/ignoreBigfiles",      IgnoreBigfiles, TRUE,               " - don't use anything from bigfile(s)"),
    entryFV("/logFileLoads",        EnableFileLoadLog,LogFileLoads,TRUE," - create log of data files loaded"),
#endif

    entryComment("PROCESSOR OPTIONS"),//-----------------------------------------------------
    entryVr("/enableSSE",           mainAllowKatmai, TRUE,              " - allow use of SSE if support is detected."),
    entryVr("/forceSSE",            mainForceKatmai, TRUE,              " - force usage of SSE even if determined to be unavailable."),
    entryVr("/enable3DNow",         mainAllow3DNow, TRUE,               " - allow use of 3DNow! if support is detected."),

    entryComment("SOUND OPTIONS"),  //-----------------------------------------------------
#if SE_DEBUG
    entryVr("/noSound",             enableSFX, FALSE,                   " - turn all sound effects off."),
    entryVr("/noSpeech",            enableSpeech, FALSE,                " - turn all speech off."),
#endif
    entryVr("/dsound",              useDSound, TRUE,                    " - forces mixer to write to DirectSound driver, even if driver reports not certified."),
    entryVr("/dsoundCoop",          coopDSound, TRUE,                   " - switches to co-operative mode of DirectSound (if supported) to allow sharing with other applications."),
    entryVr("/waveout",             useWaveout, TRUE,                   " - forces mixer to write to Waveout even if a DirectSound supported object is available."),
    entryVr("/reverseStereo",       reverseStereo, TRUE,                " - swap the left and right audio channels."),

    entryComment("DETAIL OPTIONS"), //-----------------------------------------------------
    entryFn("/rasterSkip",          EnableRasterSkip,                   " - enable interlaced display with software renderer."),
    entryVr("/noBG",                showBackgrounds, FALSE,             " - disable display of galaxy backgrounds."),
    entryVr("/noFilter",            texLinearFiltering,FALSE,           " - disable bi-linear filtering of textures."),
    entryVr("/noSmooth",            enableSmoothing, FALSE,             " - do not use polygon smoothing."),
#if TR_NIL_TEXTURE
    entryVr("/nilTexture",          GLOBAL_NO_TEXTURES,TRUE,            " - don't ever load textures at all."),
#endif
#if ETG_DISABLEABLE
    entryVr("/noEffects",           etgEffectsEnabled,FALSE,            " - disable all effects (Debug only)."),
#endif
#if FE_TEXTURES_DISABLABLE
    entryVr("/NoFETextures",        fetEnableTextures, FALSE,           " - turns off front end textures"),
#endif
    entryVr("/stipple",             enableStipple, TRUE,                " - enable stipple alpha with software renderer."),
    entryVr("/noShowDamage",        gShowDamage, FALSE,                 " - Disables showing ship damage effects."),

    entryComment("VIDEO MODE OPTIONS"),//-----------------------------------------------------
    entryVr("/safeGL",              mainSafeGL, TRUE,                   " - don't use possibly buggy optimized features of OpenGL for rendering."),
    entryFn("/triple",              EnableDoubleIsTriple,               " - use when frontend menus are flickering madly."),
    entryVr("/nodrawpixels",        mainNoDrawPixels, TRUE,             " - use when background images don't appear while loading."),
    entryVr("/noswddraw",           mainSoftwareDirectDraw, FALSE,      " - don't use DirectDraw for the software renderer."),
    entryVr("/noglddraw",           mainDirectDraw, FALSE,              " - don't use DirectDraw to setup OpenGL renderers."),
    entryVr("/sw",                  mainForceSoftware, TRUE,            " - reset rendering system to defaults at startup."),
    entryVrHidden("/noSavedMode",   mainAutoRenderer, FALSE,            " - disable recovery of previous display mode."),
    entryFn("/noFastFE",            DisableFastFrontend,                " - disable fast frontend rendering."),
    entryVr("/fullscreen",          fullScreen, TRUE,                   " - display fullscreen with software renderer (default)."),
    entryVr("/window",              fullScreen, FALSE,                  " - display in a window."),
    entryVr("/noBorder",            showBorder, FALSE,                  " - no border on window."),
    entryVrHidden("/d3dDeviceCRC",  mainOutputCRC, TRUE,                " - generate d3dDeviceCRC.txt for video troubleshooting."),
    entryFnHidden("/minny",           EnableMiniRes,                      " - run at 320x240 resolution."),
    entryFn("/640",                 EnableLoRes,                        " - run at 640x480 resolution (default)."),
    entryFn("/800",                 EnableHiRes,                        " - run at 800x600 resolution."),
    entryFn("/1024",                EnableMegaRes,                      " - run at 1024x768 resolution."),
    entryFn("/1280",                EnableUltraRes,                     " - run at 1280x1024 resolution."),
    entryFn("/1600",                EnableInsaneRes,                    " - run at 1600x1200 resolution."),
//    entryFn("/d16",                 Enable16Bit,                        " - run in 16 bits of colour."),
//    entryFn("/d24",                 Enable24Bit,                        " - run in 24 bits of colour."),
//    entryFn("/d32",                 Enable32Bit,                        " - run in 32 bits of colour."),
//    entryVr("/truecolor",           trueColor, TRUE,                    " - try 24bit modes before 15/16bit."),
//    entryVr("/slowBlits",           slowBlits, TRUE,                    " - use slow screen blits if the default is buggy."),
    entryFnParam("/device",         SelectDevice,                       " <dev> - select an rGL device by name, eg. sw, fx, d3d."),
//    entryFV("/gl",                  SelectMSGL, selectedGL, TRUE,       " - select default OpenGL as renderer."),
//    entryFn("/d3d",                 SelectD3D,                          " - select Direct3D as renderer."),
    entryVr("/nohint",              mainNoPerspective, TRUE,            " - disable usage of OpenGL perspective correction hints."),
    entryVrHidden("/noPause",             noPauseAltTab, TRUE,                " - don't pause when you alt-tab."),
    entryVrHidden("/noMinimize",          noMinimizeAltTab, TRUE,             " - don't minimize when you alt-tab."),

#ifndef HW_Release
    entryComment("CHEATS AND SHORTCUTS"),         //-----------------------------------------------------
#if CM_CHEAP_SHIPS
    entryVr("/cheapShips",          cmCheapShips, TRUE,                 " - ships only cost 1 RU."),
#endif
#if MAIN_SENSOR_LEVEL
    entryFnParam("/sensorLevel" ,   InitialSensorLevelSet,              " <n> - set initial sensors level (0.. 2).  Default is 0."),
#endif
    entryVr("/noCompPlayer",        noDefaultComputerPlayer, TRUE,      " - disable default computer players"),
#ifdef DEBUG_TACTICS
    entryVr("/notactics",           tacticsOn, FALSE,                   " - Disables tactics."),
#endif
    entryVr("/noretreat",           noRetreat, TRUE,                    " - disables the 'retreat' feature of tactics"),
#endif
    entryVrHidden("/disableAVI",    enableAVI,FALSE,                    " - don't display intro sequences."),

#if RENDER_LIGHTLINES
    entryComment("VISUALIZATION"),  //-----------------------------------------------------
    entryVr("/dockLines",           dockLines, TRUE,                    " - show dock lines."),
    entryVr("/gunLines",            gunLines, TRUE,                     " - show gun lines."),
    entryVr("/lightLines",          RENDER_LIGHTLINES, TRUE,            " - show light lines (Debug only)."),
    entryVr("/boxes",               RENDER_BOXES, TRUE,                 " - render bounding bowties on the ships."),
#endif
#if CL_TEXTFEEDBACK
    entryVr("/textFeedback",        enableTextFeedback, TRUE,           " - enable text feedback for in game commands."),
#endif
#if TR_DEBUG_TEXTURES
    entryVr("/specialTextures",     trSpecialTextures, TRUE,            " - enable special debugging textures."),
#endif
#if MESH_MORPH_DEBUG
    entryVr("/morphDebug",          meshMorphDebug, TRUE,               " - enable debugging of morphed mesh rendering code."),
#endif
#if LOD_SCALE_DEBUG
    entryFnParam("/lodScaleDebug",  EnableLodScaleDebug,                " - enable fixing a LOD scale factor."),
#endif
#if MR_CAN_FOCUS_ROIDS
    entryVr("/focusRoids",          mrCanFocusRoids, TRUE,              " - enable focussing on asteroids and dust clouds."),
#endif
#if PIE_VISUALIZE_EXTENTS
    entryVr("/showExtents",         pieVisualizeExtents, TRUE,          " - draw elliptical universe extents."),
#endif
#if UNIV_SHIP_LOADFREE_LOG
    entryVr("/loadFreeLog",         univLoadFreeLog, TRUE,              " - enable logging of what was loaded and freed between missions."),
#endif
#ifndef HW_Release
    entryVr("/NoBind",              bkDisableKeyRemap, TRUE,            " - disable key bindings so that debug keys work."),
#else
    entryVrHidden("/NoBind",        bkDisableKeyRemap, TRUE,            " - disable key bindings so that debug keys work."),
#endif

#ifndef HW_Release
    entryComment("COMPUTER PLAYER AND STATS"),//-----------------------------------------------------
//    {"/compPlayer",  EnableComputerPlayers, "=01234567 to enable all computer players"},
    entryVr("/aiplayerLog",         aiplayerLogEnable, TRUE,            " - enable AI Player Logging"),
    entryVr("/determCompPlayer",    determCompPlayer, TRUE,             " - makes computer players deterministic"),
    entryFV("/gatherStats",         EnableGatherStats, gatherStats, TRUE,"- enable gathering of stats"),
    entryFnParam("/showStatsFight", EnableShowStatsFight,               "=<i,j> to show stats fight i,j"),
    entryFnParam("/showStatsFancyFight", EnableShowStatsFancyFight,     "=filename.script"),
#endif

#ifndef HW_Release
    entryComment("NETWORK PLAY"),   //-----------------------------------------------------
    //entryVr("/captaincyLogOff",     captaincyLogEnable, FALSE,          " - turns off captaincy log file" ),
    //entryVr("/captaincyLogOn",      captaincyLogEnable, TRUE,           " - turns on captaincy log file" ),
    entryVr("/logOff",              logEnable, LOG_OFF,                 " - turns of network logging file"),
    entryVr("/logOn",               logEnable, LOG_ON,                  " - turns network logging file on"),
    entryVr("/logOnVerbose",        logEnable, LOG_VERBOSE,             " - turns verbose network logging file on"),
    entryFnParam("/logFilePath",    SpecifyLogFilePath,                 "=filepath.txt"),
    entryFn("/debugSync",           EnableDebugSync,                    " autosaves game frequently, records packets, logonverbose" ),
    entryVrHidden("/noWon",         SecretWON, TRUE,                    " - no WON stuff" ),
    entryVr("/forceLAN",            forceLAN, TRUE,                     " - allow LAN play regardless of version" ),
    entryVrHidden("/noAuth",        noAuthorization, TRUE,              " - Disables WON Login"),
    entryVrHidden("/shortWon",      ShortCircuitWON, TRUE,              " - short circuit WON stuff" ),
#else
    entryVrHidden("/logOff",        logEnable, LOG_OFF,                 " - turns of network logging file"),
    entryVrHidden("/logOn",         logEnable, LOG_ON,                  " - turns network logging file on"),
    entryVrHidden("/logOnVerbose",  logEnable, LOG_VERBOSE,             " - turns verbose network logging file on"),
    entryFnParamHidden("/logFilePath",SpecifyLogFilePath,               "=filepath.txt"),
//!!!shortwon in release mode!!!    entryVrHidden("/shortWon",      ShortCircuitWON, TRUE,              " - short circuit WON stuff" ),
#endif
#ifdef DEBUG_GAME_STATS
    entryVr("/statLogOn",           statLogOn, TRUE,                    " - generates game stats log file"),
#endif
#ifdef GOD_LIKE_SYNC_CHECKING
    entryFnParam("/BryceAndDrewAreGods", syncDumpInit,                  "=<X>!<Y>   X = size of SyncDumpWindow  Y = granularity in universe Frames"),
#endif

#if NIS_PRINT_INFO
    entryComment("NIS OPTIONS"),    //-----------------------------------------------------
#endif
#if NIS_TEST
    entryFn("/testNIS" ,            TestNISSet,                         " <nisFile> - enables NIS testing mode using [nisFile]."),
    entryFn("/testNISScript",       TestNISScriptSet,                   " <scriptFile> - enables NIS testing mode using [scriptFile]."),
#endif
#if NIS_PRINT_INFO
    entryVr("/nisCounter",          nisPrintInfo,TRUE,                  " - display nis time index info by default."),
    entryVr("/nisNoLockout",        nisNoLockout, TRUE,                 " - don't lock out the interface when playing an NIS."),
#endif

/*
    entryComment("RECORDED DEMOS"), //-----------------------------------------------------
    entryFnParam("/demoRecord",     EnableDemoRecord,                   " <fileName> - record a demo."),
    entryFnParam("/demoPlay",       EnableDemoPlayback,                 " <fileName> - play a demo."),
*/
#ifndef HW_Release
    entryFV("/packetRecord",        EnablePacketRecord, recordPackets, TRUE, " - record packets of this multiplayer game"),
    entryFV("/packetPlay",          EnablePacketPlay, playPackets, TRUE," <fileName> - play back packet recording"),
#else
    entryFVHidden("/packetRecord",  EnablePacketRecord, recordPackets, TRUE, " - record packets of this multiplayer game"),
    entryFVHidden("/packetPlay",    EnablePacketPlay, playPackets, TRUE," <fileName> - play back packet recording"),
#endif

    //entryVr("/compareBigfiles",     CompareBigfiles, TRUE,              " - file by file, use most recent (bigfile/filesystem)"),
#if DEM_AUTO_DEMO
    entryVr("/disableAutoDemos",    demAutoDemo,FALSE,                  " - don't automatically play demos."),
    entryFnParam("/autoDemoWait",   AutoDemoWaitSet,                    " <seconds> - time to wait on main screen before starting a demo."),
#endif
#if DEM_FAKE_RENDER_SWITCH
    entryVr("/disableFakeRenders",  demFakeRenders,FALSE,               " - disable feature where playback will try to keep up with recorded demo."),
#endif

    entryComment("TEXTURES"),       //-----------------------------------------------------
    entryVr("/nopal",               mainNoPalettes, TRUE,               " - disable paletted texture support."),
/*
    entryVrHidden("/allowPacking",  mainAllowPacking, TRUE,             " - use the packed textures if available (default)."),
    entryVr("/disablePacking",      mainAllowPacking, FALSE,            " - don't use the packed textures if available."),
#ifndef HW_Release
    entryVr("/onlyPacking",         mainOnlyPacking, TRUE,              " - only display packed textures."),
#endif
*/

    entryComment("MISC OPTIONS"),   //-----------------------------------------------------
    entryVrHidden("/smCentreCamera",      smCentreWorldPlane, FALSE,          " - centres the SM world plane about 0,0,0 rather than the camera."),
#if MAIN_Password
    entryFnParam("/password",       SetPassword,                        " <password> - specify password to enable certain features."),
#endif
#if RND_PLUG_DISABLEABLE
    entryVr("/noPlug",              rndShamelessPlugEnabled, FALSE,     " - don't display relic logo on pause."),
#endif

    entryVrHidden("/closeCaptioned",      subCloseCaptionsEnabled, TRUE,      " - close captioned for the hearing impared."),
    entryVr("/pilotView",           pilotView, TRUE, " - enable pilot view.  Focus on single ship and hit Q to toggle."),
    {0, NULL, NULL}
};

/*-----------------------------------------------------------------------------
    Name        : DebugHelpDefault
    Description : Print out command-line usage help to a dialog box
    Inputs      : string - the unrecognized command-line switch that causes this help.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
char *gHelpString;
void DebugHelpDefault(char *string)
{
    sdword index, length;

    //calc length of help string
    length = strlen("Invalid or unrecognised command line option: '%s'\n");
    length += strlen(string);
    for (index = 0; commandOptions[index].parameter; index++)
    {
        if (!bitTest(commandOptions[index].flags, COF_Visible))
        {
            continue;
        }
        if (commandOptions[index].helpString == NULL)
        {                                                   //no help string: it's a comment
            length += strlen(commandOptions[index].parameter) + 2;//just length of comment
        }
        else
        {                                                   //else it's a real command line option
            length += strlen(commandOptions[index].helpString); //length of help string
            length += strlen(commandOptions[index].parameter) + MCL_IndentSpace;//parameter string
        }
        length += 2;                                        //and a newline
    }

    if ((gHelpString = malloc(length)) == NULL)              //allocate string
    {
        /*MessageBox(NULL, "Cannot allocate memory for help.", "Command-Line Usage", MB_OK | MB_APPLMODAL);*/
        fprintf(stderr, "Cannot allocate memory for help.\n");
        return;
    }

    //copy all help strings into one
    sprintf(gHelpString, "Invalid or unrecognised command line option: '%s'\n", string);
    for (index = 0; commandOptions[index].parameter; index++)
    {
        if (!bitTest(commandOptions[index].flags, COF_Visible))
        {
            continue;
        }
        if (commandOptions[index].helpString == NULL)
        {                                                   //no help string: it's a comment
            strcat(gHelpString, "\n");
            strcat(gHelpString, commandOptions[index].parameter);
            strcat(gHelpString, "\n");
        }
        else
        {                                                   //else it's a real command line option
            sprintf(gHelpString + strlen(gHelpString), "    %s%s\n", commandOptions[index].parameter, commandOptions[index].helpString);
            //                                            ^ MCL_IndentSpace spaces
        }
    }

    /*DialogBox(ghInstance, MAKEINTRESOURCE(IDD_CommandLine), NULL, CommandLineFunction);*/
    printf(gHelpString);
    free(gHelpString);                                       //done with string, free it
}

/*-----------------------------------------------------------------------------
    Name        : ProcessCommandLine
    Description : Process all defined command-lien flags
    Inputs      : argc - Number of command-line arguments
                  argv - Array of command-line arguments
    Outputs     : sets command-line switch variables
    Return      : void
    Remarks:
        Here are the command-line switches for Homeworld:
            -debug: Enable debug window by setting the DebugWindow flag
----------------------------------------------------------------------------*/
sdword ProcessCommandLine (int argc, char* argv[])
{
    char* string;
    sdword index;
    int i;

    for (i = 1; i < argc; i++)  // Skip the executable name.
    {
        string = argv[i];
        if (string[0] == '-')
        {
            string[0] = '/';
        }
        for (index = 0; commandOptions[index].parameter; index++)
        {
            if (commandOptions[index].helpString == NULL)
            {                                               //don't compare against comment lines
                continue;
            }
            if (!strcasecmp(string, commandOptions[index].parameter))
            {                                               //if this is the correct option
                dbgAssert(commandOptions[index].variableToModify || commandOptions[index].function);
                if (commandOptions[index].variableToModify != NULL)
                {                                           //set a variable if applicable
                    *((udword *)(commandOptions[index].variableToModify)) = commandOptions[index].valueToSet;
                }
                if (commandOptions[index].function != NULL)
                {                                           //call the function associated, if applicable
                    if (bitTest(commandOptions[index].flags, COF_NextToken))
                    {                                       //does the function take next token as a parameter?
                        if (++i == argc)                    //get next token
                        {                                   //if no next token
                            break;                          //print usage
                        }
                        string = argv[i];
                    }
                    if(!commandOptions[index].function(string))
                    {
                        //error occured in parsing function
                        break;
                    }
                }
                break;
            }
        }

        if (!commandOptions[index].parameter)
        {
            DebugHelpDefault(string);                       //no string found, print help
            return(-1);
        }
    }
    return(OKAY);
}

/*-----------------------------------------------------------------------------
    Name        : CommandProcess
    Description : processes command messages sent to WindowProc
    Inputs      : Same as WindowProc without the message WM_COMMAND integer
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
/*void CommandProcess(HWND hWnd, WPARAM wParam, LPARAM lParam)*/
void CommandProcess(int code)
{
    SDL_Event e;

    switch (code)
    {
        case CID_ExitError:                                 //error message posted, exit with a message box
            fprintf(stderr, "Fatal Error: %s\n", dbgFatalErrorString);

            e.quit.type = SDL_QUIT;
            SDL_PushEvent(&e);

            break;
        case CID_ExitOK:
            e.quit.type = SDL_QUIT;
            SDL_PushEvent(&e);

            break;
    }
}

/*-----------------------------------------------------------------------------
    Name        : mainDevStatsInit
    Description : initialize the devstats table.  this table contains features
                  that need to be disabled on particular (D3D) devices
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainDevStatsInit(void)
{
    filehandle handle;
    char string[512];
    crc32 crc;
    udword flags0, flags1, flags2;
    sdword size, index;

    handle = fileOpen("devstats.dat", FF_IgnorePrepend | FF_TextMode | FF_IgnoreBIG);
    for (devTableLength = 0;;)
    {
        if (fileLineRead(handle, string, 511) == FR_EndOfFile)
        {
            break;
        }
        if (string[0] == ';')
        {
            continue;
        }
        if (strlen(string) < 4)
        {
            continue;
        }

        //xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx ...
        sscanf(string, "%X %X %X %X", &crc, &flags0, &flags1, &flags2);
        if (crc != 0)
        {
            devTableLength++;
        }
    }
    fileClose(handle);

    if (devTableLength > 0)
    {
        size = 4 * sizeof(udword) * devTableLength;
        devTable = (udword*)malloc(size);
        if (devTable == NULL)
        {
            dbgFatal(DBG_Loc, "mainDevStatsInit couldn't allocate memory for devTable");
        }
        memset(devTable, 0, size);

        handle = fileOpen("devstats.dat", FF_IgnorePrepend | FF_TextMode | FF_IgnoreBIG);
        for (index = 0;;)
        {
            if (fileLineRead(handle, string, 511) == FR_EndOfFile)
            {
                break;
            }
            if (string[0] == ';')
            {
                continue;
            }
            if (strlen(string) < 4)
            {
                continue;
            }

            //xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx ...
            sscanf(string, "%X %X %X %X", &crc, &flags0, &flags1, &flags2);
            if (crc != 0)
            {
                devTable[index+0] = crc;
                devTable[index+1] = flags0;
                devTable[index+2] = flags1;
                devTable[index+3] = flags2;
                index += 4;
            }
        }
        fileClose(handle);
    }
}

/*-----------------------------------------------------------------------------
    Name        : mainDevStatsShutdown
    Description : release memory used by the devstats table
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainDevStatsShutdown(void)
{
    if (devTable != NULL)
    {
        free(devTable);
        devTable = NULL;
    }
}

void IntroDeactivateMe()
{
    if (gl95)
    {
        return;                                             //don't allow the program to deactivate under Windows 95
    }
    sounddeactivate(TRUE);

#if 0	/* Bink, no? */
    if (!binkDonePlaying)
    {
        binkPause(TRUE);
        hwSetRes(0, 0, 0);
    }
#endif

    wasDemoPlaying = demDemoPlaying;                        //save demo playback state
    demDemoPlaying = FALSE;                                 //stop the demo playback for now

#if MAIN_MOUSE_FREE
    if (utySystemStarted)
    {
        utyClipMouse(FALSE);
    }
#endif
    utyTaskTimerClear();                                    //clear pending timer ticks
    keyClearAll();
    keyBufferClear();
    systemActive = FALSE;
}

void IntroActivateMe()
{
    sounddeactivate(FALSE);

    /*hwSetRes(-1, -1, -1);*/

    demDemoPlaying = wasDemoPlaying;                        //keep playing demo if it was playing when we minimized
#if MAIN_MOUSE_FREE
    if (utySystemStarted)
    {
        utyClipMouse(startupClipMouse);                     //optionally trap the mouse
    }
#endif
    systemActive = TRUE;

    utyForceTopmost(fullScreen);
    SDL_ShowCursor(SDL_DISABLE);
    //make sure that the mouse is rotating properly when we come back
    utyMouseButtonsClear();

    keyClearAll();
    keyBufferClear();
    mrTacticalOverlayState(utyCapsLockToggleState());

#if 0	/* More bink... */
    if (!binkDonePlaying)
    {
        binkPause(FALSE);
    }
#endif
}

/*-----------------------------------------------------------------------------
    Name        : DeactivateMe
    Description : deactivates this application and suspends processing
    Inputs      : hWnd
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void DeactivateMe()
{
    if (gl95)
    {
        return;                                             //don't allow the program to deactivate under Windows 95
    }
    sounddeactivate(TRUE);

    if (RGL)
    {
        rglFeature(RGL_DEACTIVATE);
    }
    /*
    else
    {
        (void)hwActivate(FALSE);
    }
    */
    wasDemoPlaying = demDemoPlaying;                        //save demo playback state
    demDemoPlaying = FALSE;                                 //stop the demo playback for now

#if MAIN_MOUSE_FREE
    utyClipMouse(FALSE);
#endif
    utyTaskTimerClear();                                    //clear pending timer ticks
    if (multiPlayerGame || noPauseAltTab)
    {                                                       //only stop rendering if it's single player
        taskSavePauseStatus();
        taskPause(utyRenderTask);
        taskPause(regTaskHandle);
    }
    else
    {
        taskFreezeAll();                                    //stop all tasks
    }
    keyClearAll();
    keyBufferClear();
    systemActive = FALSE;

#if 0	/* Bink stuff... */
    if (!binkDonePlaying)
    {
        binkPause(TRUE);
    }
#endif
    /*
    if (utyTest2(SS2_ToggleKeys))
    {
        utyToggleKeyStatesRestore();
    }
    */
}

/*-----------------------------------------------------------------------------
    Name        : ActivateMe
    Description : activates this application and resumes processing
    Inputs      : hWnd
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ActivateMe()
{
    sounddeactivate(FALSE);

    demDemoPlaying = wasDemoPlaying;                        //keep playing demo if it was playing when we minimized
    if (RGL)
    {
        rglFeature(RGL_ACTIVATE);
        mainReinitRenderer = 2;
    }
    /*
    else
    {
        (void)hwActivate(TRUE);
    }
    */
    if (glcActive())
    {
        glcRenderEverythingALot();
    }
    else
    {
        feRenderEverything = TRUE;
    }
#if MAIN_MOUSE_FREE
    if (utySystemStarted)
    {
        utyClipMouse(startupClipMouse);                     //optionally trap the mouse
    }
#endif
    if (utySystemStarted)
    {                                                       //if game has started
        taskResumeAll();                                    //resume all tasks
    }
    systemActive = TRUE;

    utyForceTopmost(fullScreen);
    SDL_ShowCursor(SDL_DISABLE);
    //make sure that the mouse is rotating proper when we come back
    utyMouseButtonsClear();

    keyClearAll();
    keyBufferClear();

    hrBackgroundReinit = TRUE;
    /*
    if (utyTest2(SS2_ToggleKeys))
    {
        utyToggleKeyStatesSave();
    }
    */
    mrTacticalOverlayState(utyCapsLockToggleState());

#if 0	/* Bink stuff... */
    if (!binkDonePlaying)
    {
        binkPause(FALSE);
    }
#endif
}

static bool mainFileExists(char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        return FALSE;
    }
    else
    {
        fclose(file);
        return TRUE;
    }
}

static bool mainFind3DfxGL(char* path)
{
    char* dir;
    char  subdir[8];

    if (glCapNT())
    {
        strcpy(subdir, "WinNT");
    }
    else
    {
        strcpy(subdir, "Win9x");
    }

    //try cwd\3dfx\opengl32.dll first
#ifdef _WIN32
    sprintf(path, "3dfx\\%s\\opengl32.dll", subdir);
#else
    sprintf(path, "3dfx/%s/libGL.so", subdir);
#endif
    if (mainFileExists(path))
    {
        return TRUE;
    }

    //try $HW_Root\dll\opengl32.dll next
    dir = getenv("HW_Root");
    if (dir == NULL)
    {
        //not found, use default opengl
#ifdef _WIN32
        strcpy(path, "opengl32.dll");
#else
        strcpy(path, "libGL.so");
#endif
        return FALSE;
    }
    else
    {
        char concatdir[128];
        strcpy(path, dir);
#ifdef _WIN32
        if (path[strlen(path)-1] == '\\')
        {
            sprintf(concatdir, "dll\\%s\\opengl32.dll", subdir);
        }
        else
        {
            sprintf(concatdir, "\\dll\\%s\\opengl32.dll", subdir);
        }
#else
        if (path[strlen(path)-1] == '/')
        {
            sprintf(concatdir, "dll/%s/libGL.so", subdir);
        }
        else
        {
            sprintf(concatdir, "/dll/%s/libGL.so", subdir);
        }
#endif
        strcat(path, concatdir);
        return TRUE;
    }
}

/*-----------------------------------------------------------------------------
    Name        : mainFreeLibraries
    Description : Release references to possible GL libraries.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainFreeLibraries(void)
{
    void glCapResetRGLAddresses(void);

    /* No longer unload the library (SDL should handle this). */

    RGL = FALSE;
    glCapResetRGLAddresses();
}

/*-----------------------------------------------------------------------------
    Name        : mainRescaleMainWindow
    Description : rescale the main window (ghMainWindow) and call fn to
                  reinit game systems that require it
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainRescaleMainWindow(void)
{
    /* Maybe? */
#if 0
    bool wasClipped;

    mainWindowTotalWidth  = MAIN_WindowWidth  + mainWidthAdd;
    mainWindowTotalHeight = MAIN_WindowHeight + mainHeightAdd;

    wasClipped = mouseClipped;
    utyClipMouse(FALSE);

    SetWindowPos(ghMainWindow, HWND_TOP,
                 0, 0,
                 mainWindowTotalWidth, mainWindowTotalHeight,
                 (showBorder) ? SWP_DRAWFRAME : 0);

    if (wasClipped)
    {
        utyClipMouse(TRUE);
    }
#endif

    (void)utyChangeResolution(MAIN_WindowWidth, MAIN_WindowHeight, MAIN_WindowDepth);
}

/*-----------------------------------------------------------------------------
    Name        : mainStartupGL
    Description : startup an OpenGL renderer
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool mainStartupGL(char* data)
{
    rndinitdata renderData;

    mainRescaleMainWindow();

    if (opUsing3DfxGL)
    {
        if (!mainFind3DfxGL(glToSelect))
        {
#ifdef _WIN32
            memStrncpy(glToSelect, "opengl32.dll", 512 - 1);
#else
            memStrncpy(glToSelect, "libGL.so", 512 - 1);
#endif
        }
    }
    else
    {
#ifdef _WIN32
        memStrncpy(glToSelect, "opengl32.dll", 512 - 1);
#else
        memStrncpy(glToSelect, "libGL.so", 512 - 1);
#endif
    }

    if (!glCapLoadOpenGL(glToSelect))
    {
        return FALSE;
    }
    mainDeviceToSelect[0] = '\0';
    mainD3DToSelect[0] = '\0';
    memStrncpy(mainGLToSelect, glToSelect, 512 - 1);

    /* Window creation moved to rndSmallInit()/rndInit()...no more
       hwCreateWindow()... */

    renderData.width = MAIN_WindowWidth;
    renderData.height = MAIN_WindowHeight;
#ifdef _WIN32
    renderData.hWnd = ghMainWindow;
#endif
    renderData.hWnd = 0;
    if (!rndSmallInit(&renderData, TRUE))
    {
        /*(void)hwDeleteWindow();*/
        return FALSE;
    }

    if (!glCapValidGL())
    {
        /*(void)hwDeleteWindow();*/
        return FALSE;
    }

    utyForceTopmost(fullScreen);

    glCapStartup();

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : mainMemAlloc
    Description : memory allocation wrapper (for rGL's benefit)
    Inputs      : len - amount of memory to allocate (bytes)
                  name - name of the memory handle
                  flags - flags for allocation, currently ignored
    Outputs     :
    Return      : pointer to new memory, or NULL if none available
----------------------------------------------------------------------------*/
void* mainMemAlloc(GLint len, char* name, GLuint flags)
{
    return memAllocAttempt(len, name, NonVolatile | ExtendedPool);
}

/*-----------------------------------------------------------------------------
    Name        : mainMemFree
    Description : memory free wrapper (for rGL's benefit)
    Inputs      : pointer - the memory pointer to free
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainMemFree(void* pointer)
{
    memFree(pointer);
}

/*-----------------------------------------------------------------------------
    Name        : mainContinueRGL
    Description : set rGL's features
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainContinueRGL(char* data)
{
    rglSetAllocs((MemAllocFunc)mainMemAlloc, (MemFreeFunc)mainMemFree);
    if (fullScreen)
    {
        rglFeature(RGL_FULLSCREEN);
    }
    else
    {
        rglFeature(RGL_WINDOWED);
    }
    if (mainSoftwareDirectDraw)
    {
        rglFeature(RGL_HICOLOR);
    }
    else
    {
        rglFeature(RGL_TRUECOLOR);
    }
    if (slowBlits)
    {
        rglFeature(RGL_SLOWBLT);
    }
    else
    {
        rglFeature(RGL_FASTBLT);
    }
    if (accelFirst)
    {
        rglSelectDevice("fx", "");
        lodScaleFactor = 1.0f;
    }
    if (deviceToSelect[0] != '\0')
    {
        rglSelectDevice(deviceToSelect, data);
        if (strcmp(deviceToSelect, "sw"))
        {
            lodScaleFactor = 1.0f;
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : mainStartupParticularRGL
    Description : startup rGL as a renderer and activate a particular device
    Inputs      : device - rGL device name
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool mainStartupParticularRGL(char* device, char* data)
{
    rndinitdata renderData;

    mainRescaleMainWindow();

#ifdef _WIN32
    memStrncpy(glToSelect, "librgl.dll", 512 - 1);
#else
    memStrncpy(glToSelect, "librgl.so", 512 - 1);
#endif
    if (!glCapLoadOpenGL(glToSelect))
    {
        return FALSE;
    }
    memStrncpy(mainDeviceToSelect, device, 16 - 1);
    memStrncpy(mainD3DToSelect, data, 64 - 1);
    memStrncpy(mainGLToSelect, glToSelect, 512 - 1);

    mainContinueRGL(data);

    rglSelectDevice(device, data);

    renderData.width = MAIN_WindowWidth;
    renderData.height = MAIN_WindowHeight;
#ifdef _WIN32
    renderData.hWnd = ghMainWindow;
#endif
    renderData.hWnd = 0;
    if (!rndSmallInit(&renderData, FALSE))
    {
        return FALSE;
    }

    if (!glCapValidGL())
    {
        return FALSE;
    }

    utyForceTopmost(fullScreen);

    glCapStartup();

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : mainActiveRenderer
    Description : returns the type of currently active renderer
    Inputs      :
    Outputs     :
    Return      : GLtype, D3Dtype, SWtype, [GLIDEtype]
----------------------------------------------------------------------------*/
sdword mainActiveRenderer(void)
{
    return RGLtype;
}

void mainDestroyWindow(void)
{
    mainActuallyQuit = FALSE;

    mainWindowTotalWidth  = MAIN_WindowWidth  + mainWidthAdd;
    mainWindowTotalHeight = MAIN_WindowHeight + mainHeightAdd;

#ifdef _WIN32
    if (DebugWindow)
    {
        dbwClose();
    }
#endif

//    mainUnregisterClass(ghInstance);
//    mainRegisterClass(ghInstance);

    utyForceTopmost(fullScreen);

#ifdef _WIN32
    if (DebugWindow)
    {
        /* If porting this back to Windows, you'll need to get a hold of
           values for these (hWnd is in the SDL_SysWMinfo structure, hInst
           would need to be acquired through Windows API calls). */
        dbwStart((udword)ghInstance, (udword)ghMainWindow);
    }
#endif
}

/*-----------------------------------------------------------------------------
    Name        : mainShutdownGL
    Description : shutdown a GL renderer
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainShutdownGL(void)
{
    rndClose();

#ifndef _MACOSX_FIX_ME
    if (sstLoaded())
    {
        sstShutdown();
    }
#endif

    /*hwDeleteWindow();*/
    mainDestroyWindow();
}

/*-----------------------------------------------------------------------------
    Name        : mainShutdownRGL
    Description : shutdown rGL as a renderer
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainShutdownRGL(void)
{
    rndClose();
    mainDestroyWindow();
}

/*-----------------------------------------------------------------------------
    Name        : mainResetRender
    Description : inform necessary modules that the renderer is switching
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainResetRender(void)
{
    glDLLReset();
    rndResetGLState();
    glcRenderEverythingALot();
    feRenderEverything = TRUE;
    frReset();
    ferReset();
    mouseReset();
    feReset();
    if (!reinitInProgress)
    {
        trReload();
    }
}

/*-----------------------------------------------------------------------------
    Name        : mainCloseRender
    Description : free module textures, call reset render
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainCloseRender(void)
{
    partShutdown();
    glcFreeTextures();
    ferReset();
    cpTexturesPurge();
    lmFreeTextures();
    cpPreviewImageDelete();
    rndLoadShamelessPlug(FALSE);    //shameless plug handles reloading itself
    btgCloseTextures();
    cmCloseTextures();
    rmGUIShutdown();
    mainResetRender();

    if (!reinitInProgress)
    {
        trSetAllPending(FALSE);
        trNoPalShutdown();
#if TR_ERROR_CHECKING
        if (RGL)
        {
            rglFeature(RGL_TEXTURE_LOG);
        }
#endif
    }
}

/*-----------------------------------------------------------------------------
    Name        : mainOpenRender
    Description : call reset render
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainOpenRender(void)
{
    trNoPalStartup();
    mainResetRender();
    glcLoadTextures();
    rmGUIStartup();
    cmLoadTextures();
    btgLoadTextures();
    lmLoadTextures();
    //shameless plug handles reloading itself
    frReloadGL();
}

udword saveRGLtype;
sdword saveMAIN_WindowWidth;
sdword saveMAIN_WindowHeight;
sdword saveMAIN_WindowDepth;
char saveglToSelect[512];
char savedeviceToSelect[16];
char saved3dToSelect[64];

/*-----------------------------------------------------------------------------
    Name        : mainSaveRender
    Description : save current renderer info in case a mode switch doesn't work
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainSaveRender(void)
{
    saveRGLtype = RGLtype;
    saveMAIN_WindowWidth  = MAIN_WindowWidth;
    saveMAIN_WindowHeight = MAIN_WindowHeight;
    saveMAIN_WindowDepth  = MAIN_WindowDepth;
    memStrncpy(saveglToSelect, mainGLToSelect, 512 - 1);
    memStrncpy(savedeviceToSelect, mainDeviceToSelect, 16 - 1);
    memStrncpy(saved3dToSelect, mainD3DToSelect, 64 - 1);
}

void mainSetupSoftware(void)
{
#ifdef _WIN32
    strcpy(glToSelect, "librgl.dll");
    strcpy(mainGLToSelect, "librgl.dll");
    strcpy(deviceToSelect, "sw");
    strcpy(mainDeviceToSelect, "sw");
    strcpy(mainD3DToSelect, "");
#else
    strcpy(glToSelect, "librgl.so");
    strcpy(mainGLToSelect, "librgl.so");
    strcpy(deviceToSelect, "sw");
    strcpy(mainDeviceToSelect, "sw");
    strcpy(mainD3DToSelect, "");
#endif

    mainWindowWidth  = MAIN_WindowWidth  = 640;
    mainWindowHeight = MAIN_WindowHeight = 480;
    mainWindowDepth  = MAIN_WindowDepth  = 16;

    opDeviceIndex = -1;

    gDevcaps = gDevcaps2 = 0;
}

/*-----------------------------------------------------------------------------
    Name        : mainRestoreSoftware
    Description : load 640x480@16, rGL+software rendering system
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainRestoreSoftware(void)
{
    mainSetupSoftware();

    mainRescaleMainWindow();

    bMustFree = FALSE;
    if (!mainLoadParticularRGL("sw", ""))
    {
        SDL_Event e;
/*
        MessageBox(NULL,
                   "couldn't initialize default rendering system",
                   windowTitle, MB_APPLMODAL | MB_OK);
*/
        fprintf(stderr, "Fatal Error: Couldn't initialize default rendering system.\n");
        e.user.type = SDL_USEREVENT;
        e.user.code = CID_ExitError;
        SDL_PushEvent(&e);
    }
    bMustFree = TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : mainRestoreRender
    Description : restore saved rendering system (mainSaveRender())
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainRestoreRender(void)
{
    memStrncpy(glToSelect, saveglToSelect, 512 - 1);
    memStrncpy(mainGLToSelect, saveglToSelect, 512 - 1);
    memStrncpy(deviceToSelect, savedeviceToSelect, 16 - 1);
    memStrncpy(mainDeviceToSelect, savedeviceToSelect, 16 - 1);
    memStrncpy(mainD3DToSelect, saved3dToSelect, 64 - 1);

    mainWindowWidth  = MAIN_WindowWidth  = saveMAIN_WindowWidth;
    mainWindowHeight = MAIN_WindowHeight = saveMAIN_WindowHeight;
    mainWindowDepth  = MAIN_WindowDepth  = saveMAIN_WindowDepth;

    mainRescaleMainWindow();

    RGLtype = saveRGLtype;
    bMustFree = FALSE;
    if (RGLtype == GLtype)
    {
        if (!mainLoadGL(NULL))
        {
            //couldn't restore, try basic software
            mainRestoreSoftware();
        }
    }
    else
    {
        if (!mainLoadParticularRGL(savedeviceToSelect, saved3dToSelect))
        {
            //couldn't restore, try basic software
            mainRestoreSoftware();
        }
    }
    bMustFree = TRUE;

    feRenderEverything = TRUE;
    glcRenderEverythingALot();
}

/*-----------------------------------------------------------------------------
    Name        : mainShutdownRenderer
    Description : shutdown the existing renderer
    Inputs      :
    Outputs     :
    Return      : TRUE or FALSE
----------------------------------------------------------------------------*/
bool mainShutdownRenderer(void)
{
    dbgMessage("\nmainShutdownRenderer");

    mainCloseRender();
    if (RGLtype == GLtype)
    {
        mainShutdownGL();
    }
    else
    {
        mainShutdownRGL();
    }
    mainFreeLibraries();

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : mainLoadGL
    Description : close existing renderer, startup a GL
    Inputs      : data - possible name of GL .DLL
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool mainLoadGL(char* data)
{
    dbgMessage("\n-- load OpenGL --");

    if (bMustFree)
    {
        mainCloseRender();

        if (RGLtype == GLtype)
        {
            mainShutdownGL();
        }
        else
        {
            mainShutdownRGL();
        }
        mainFreeLibraries();
    }

    if (!mainStartupGL(data))
    {
        return FALSE;
    }

    mainOpenRender();

    lodScaleFactor = 1.0f;
    alodStartup();

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : mainLoadParticularRGL
    Description : close existing render, startup rGL w/ specified device
    Inputs      : device - device name {sw, d3d, fx}
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool mainLoadParticularRGL(char* device, char* data)
{
    dbgMessagef("\n-- load rGL device %s --", device);

    if (bMustFree)
    {
        mainCloseRender();

        if (RGLtype == GLtype)
        {
            mainShutdownGL();
        }
        else
        {
            mainShutdownRGL();
        }
        mainFreeLibraries();
    }

    if (!mainStartupParticularRGL(device, data))
    {
        return FALSE;
    }

    mainOpenRender();

    lodScaleFactor = (RGLtype == SWtype) ? LOD_ScaleFactor : 1.0f;
    alodStartup();

    mainReinitRenderer = 2;

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : mainReinitRGL
    Description : reinitializes rGL, currently used to work around a disheartening
                  D3D problem wrt alpha textures
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool mainReinitRGL(void)
{
    if (RGLtype != D3Dtype)
    {
        return TRUE;
    }

    reinitInProgress = TRUE;

//    mainCloseRender();
    rglFeature(RGL_REINIT_RENDERER);
//    glCapStartup();
//    mainOpenRender();
//    glCapStartup();
//    lodScaleFactor = (RGLtype == SWtype) ? LOD_ScaleFactor : 1.0f;
//    alodStartup();
    dbgMessage("\n-- reinit rGL --");

    reinitInProgress = FALSE;

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : keyLanguageTranslate
    Description : This function translates the virtual key into the ASCII character.
    Inputs      : keysym
    Outputs     : converted character
    Return      : Uh-huh
----------------------------------------------------------------------------*/
udword keyLanguageTranslate(udword wParam)
{
    switch (strCurKeyboardLanguage)
    {
        case languageEnglish:
            return(wParam);
        break;
        case languageFrench:
            if (wParam < KEY_TOTAL_KEYS)
                return(keyFrenchToEnglish(wParam));
            else
                return(0);
        break;
        case languageGerman:
            if (wParam < KEY_TOTAL_KEYS)
                return(keyGermanToEnglish(wParam));
            else
                return(0);
        break;
        case languageSpanish:
            if (wParam < KEY_TOTAL_KEYS)
                return(keySpanishToEnglish(wParam));
            else
                return(0);
        break;
        case languageItalian:
            if (wParam < KEY_TOTAL_KEYS)
                return(keyItalianToEnglish(wParam));
            else
                return(0);
        break;
    }

    return wParam;
/*    char   buff[256], trankey[2];
    sdword i,ret;

    for (i=0;i<256;i++) buff[i] = 0;

    ret = ToAscii(wParam, 0, buff, (uword *)trankey, 0);

    if (ret==0)
    {
        return((udword)wParam);
    }
    else if (ret==1)
    {
        if ((trankey[0] >= 'a') && (trankey[0] <= 'z'))
        {
            trankey[0]-='a'-'A';
        }
        return ((udword)((ubyte)trankey[0]));
    }

    return (0);*/
}

/*-----------------------------------------------------------------------------
    Name        : HandleEvent
    Description : SDL event handling for the application
    Inputs      : pEvent - Pointer to the event to handle
    Outputs     :
    Return      : Result of event handling (based on the event)
----------------------------------------------------------------------------*/
//long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
sdword HandleEvent (const SDL_Event* pEvent)
{
    extern bool utilPlayingIntro;

    /* Mouse button press times for double-click support. */
    static Uint32 mbDownTime[3] = { 0, 0, 0 };
    static Uint8 mbDouble[3] = { 0, 0, 0 };

    dbgAssert(pEvent);

#if MAIN_PRINT_MESSAGES
    dbgMessagef("\nEvent type = 0x%hhx", pEvent->type);
#endif //MAIN_PRINT_MESSAGES

    switch (pEvent->type)
    {
        case SDL_ACTIVEEVENT:
#if 0
            if (!mainActuallyQuit)
            {
                return 0;
            }
#endif
            if (pEvent->active.gain && systemActive == FALSE)
            {                                               //we're being activated
                if (utilPlayingIntro)
                {
                    IntroActivateMe();
                }
                else
                {
                    ActivateMe();
                }
            }
            else if (!pEvent->active.gain && systemActive == TRUE)
            {                                               //we're being deactivated
                if (utilPlayingIntro)
                {
                    IntroDeactivateMe();
                }
                else
                {
                    DeactivateMe();
                }
            }
            else
                break;

            return 0;                                       //per documentation

        case SDL_KEYUP:                                     //keys up/down
            switch (pEvent->key.keysym.sym)
            {
                case SDLK_ESCAPE:
#if 0	/* Bink */
                    if (!binkDonePlaying)
                    {
                        binkStop();
                    }
                    else
#endif
                    {
                        keyPressUp(pEvent->key.keysym.sym);
                    }
#if MAIN_MOUSE_FREE
                case SDLK_F11:                              //toggle clipped cursor
                    if (keyIsHit(CONTROLKEY))
                    {
                        utyClipMouse(mouseClipped ^ TRUE);
                    }
                    else
                    {
                        keyPressUp(pEvent->key.keysym.sym);
                    }
                    break;
#endif
                case SDLK_F12:
                    if (!RGL)
                    {
                        if (keyIsHit(SHIFTKEY) && keyIsHit(CONTROLKEY))
                        {
                            glcFreeTextures();
                            mainCloseRender();
                            mainShutdownGL();
                            mainRestoreSoftware();
                            mainOpenRender();
                            glCapStartup();
                            glcLoadTextures();
                            lodScaleFactor = LOD_ScaleFactor;
                            alodStartup();
                        }
                    }
                    else
                    {
                        dbgMessagef("\nprevious GL RENDERER: %s", glGetString(GL_RENDERER));
                        if (keyIsHit(SHIFTKEY) && keyIsHit(CONTROLKEY))
                        {
                            mainCloseRender();
                            mainShutdownRGL();
                            mainRestoreSoftware();
                        }
                        else
                        {
                            break;
                        }
                        glCapStartup();
                        mainOpenRender();
                        glCapStartup();
                        if (RGLtype == SWtype)
                        {
                            lodScaleFactor = LOD_ScaleFactor;
                        }
                        else
                        {
                            lodScaleFactor = 1.0f;
                        }
                        alodStartup();
                        dbgMessagef("\nnew GL RENDERER: %s", glGetString(GL_RENDERER));
                    }
                    break;
                default:
                    keyPressUp(keyLanguageTranslate(pEvent->key.keysym.sym));//keyPressUp(KeyMapFromWindows(wParam));
            }
            return 0;

        case SDL_KEYDOWN:
            /*
            if (lParam & BIT30)
            {                                               //if it's a repeating key
                //keyRepeat(KeyMapFromWindows(wParam));
                keyRepeat(keyLanguageTranslate(wParam));
            }
            */
            keyPressDown(keyLanguageTranslate(pEvent->key.keysym.sym));
            //keyPressDown(KeyMapFromWindows(wParam));
            return 0;

        case SDL_USEREVENT:
            CommandProcess(pEvent->user.code);
            return 0;

        case SDL_MOUSEBUTTONDOWN:
            if (!mouseDisabled)
            {
                Uint32 curr_time = SDL_GetTicks();

                switch (pEvent->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        if (!mbDouble[0] && curr_time - mbDownTime[0] <= 500)
                        {
                            keyPressDown(LMOUSE_DOUBLE);
                            mbDouble[0] = 1;
                        }
                        else
                        {
                            mouseLClick();
                            keyPressDown(LMOUSE_BUTTON);
                            mbDouble[0] = 0;
                        }
                        mbDownTime[0] = curr_time;
                    break;

                    case SDL_BUTTON_MIDDLE:
                        if (!mbDouble[1] && curr_time - mbDownTime[1] <= 500)
                        {
                            keyPressDown(MMOUSE_DOUBLE);
                            mbDouble[1] = 1;
                        }
                        else
                        {
                            keyPressDown(MMOUSE_BUTTON);
                            mbDouble[1] = 0;
                        }
                        mbDownTime[1] = curr_time;
                    break;

                    case SDL_BUTTON_RIGHT:
                        if (!mbDouble[2] && curr_time - mbDownTime[2] <= 500)
                        {
                            keyPressDown(RMOUSE_DOUBLE);
                            mbDouble[2] = 1;
                        }
                        else
                        {
                            keyPressDown(RMOUSE_BUTTON);
                            mbDouble[2] = 0;
                        }
                        mbDownTime[2] = curr_time;
                    break;

                    case SDL_BUTTON_WHEELUP:
                        keyPressDown(FLYWHEEL_UP);
                    break;

                    case SDL_BUTTON_WHEELDOWN:
                        keyPressDown(FLYWHEEL_DOWN);
                    break;
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (!mouseDisabled)
            {
                switch (pEvent->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        keyPressUp(LMOUSE_BUTTON);
                        keyPressUp(LMOUSE_DOUBLE);
                    break;

                    case SDL_BUTTON_MIDDLE:
                        keyPressUp(MMOUSE_BUTTON);
                        keyPressUp(MMOUSE_DOUBLE);
                    break;

                    case SDL_BUTTON_RIGHT:
                        keyPressUp(RMOUSE_BUTTON);
                        keyPressUp(RMOUSE_DOUBLE);
                    break;
                }
            }
            break;

        case SDL_QUIT:
            if (mainActuallyQuit)
            {
                SDL_Event e;
                WindowsCleanup();
                e.quit.type = SDL_QUIT;
                SDL_PushEvent(&e);
            }
            else
            {
                mainActuallyQuit = TRUE;
            }
            return 0;
    }

    return 0;
} // WindowProc

/*-----------------------------------------------------------------------------
    Name        : InitWindow
    Description : Initialize main window...stuff...
    Inputs      :
    Outputs     :
    Return      : TRUE if successful, FALSE if not
----------------------------------------------------------------------------*/
static bool InitWindow ()
{
    char d3dToSelect[64];
	unsigned int rinDevCRC;

    /*
     * create a window
     */

    if (mainAutoRenderer)
    {
        if (selectedRES)
        {
            MAIN_WindowWidth  = mainWindowWidth;
            MAIN_WindowHeight = mainWindowHeight;
            MAIN_WindowDepth  = mainWindowDepth;
        }
        else
        {
            mainWindowWidth  = MAIN_WindowWidth;
            mainWindowHeight = MAIN_WindowHeight;
            mainWindowDepth  = MAIN_WindowDepth;
        }
    }

    mainWidthAdd  = 0;
    mainHeightAdd = 0;

    if (mainForceSoftware)
    {
        MAIN_WindowWidth  = 640;
        MAIN_WindowHeight = 480;
        MAIN_WindowDepth  = 16;
    }

    mainWindowTotalWidth  = MAIN_WindowWidth  + mainWidthAdd;
    mainWindowTotalHeight = MAIN_WindowHeight + mainHeightAdd;

    /* Window created in renderer initialization.. */

    mainDevStatsInit();
    rinEnumerateDevices();

    d3dToSelect[0] = '\0';

    if (mainAutoRenderer &&
        (strlen(mainGLToSelect) > 0))
    {
        if (selectedDEVICE)
        {
            d3dToSelect[0] = '\0';
        }
        else
        {
            memStrncpy(deviceToSelect, mainDeviceToSelect, 16 - 1);
            memStrncpy(d3dToSelect, mainD3DToSelect, 64 - 1);
        }
        if (!selectedGL)
        {
            memStrncpy(glToSelect, mainGLToSelect, 512 - 1);
        }
    }

	rinDevCRC = rinDeviceCRC();
    if (opDeviceCRC != rinDevCRC)
    {
        opDeviceIndex = -1;
        if (mainAutoRenderer)
        {
            opDeviceCRC = rinDeviceCRC();
            mainForceSoftware = TRUE;
        }
    }

    if (mainForceSoftware)
    {
        strcpy(deviceToSelect, "sw");
        strcpy(mainDeviceToSelect, "sw");
#ifdef _WIN32
        strcpy(glToSelect, "librgl.dll");
        strcpy(mainGLToSelect, "librgl.dll");
#else
        strcpy(glToSelect, "librgl.so");
        strcpy(mainGLToSelect, "librgl.so");
#endif
        d3dToSelect[0] = '\0';
        mainD3DToSelect[0] = '\0';
        MAIN_WindowWidth  = 640;
        MAIN_WindowHeight = 480;
        MAIN_WindowDepth  = 16;

        utyForceTopmost(fullScreen);

        mainRescaleMainWindow();

        opDeviceIndex = -1;

        gDevcaps = gDevcaps2 = 0;
    }
    else
    {
        extern udword loadedDevcaps, loadedDevcaps2;
        //successfully re-using previous device,
        //re-use previous devcaps
        gDevcaps = loadedDevcaps;
        if (gDevcaps == 0xFFFFFFFF)
        {
            gDevcaps = 0;
        }
        gDevcaps2 = loadedDevcaps2;
        if (gDevcaps2 == 0xFFFFFFFF)
        {
            gDevcaps2 = 0;
        }
    }

    if (!glCapLoadOpenGL(glToSelect))
    {
        mainSetupSoftware();

        utyForceTopmost(fullScreen);

        mainRescaleMainWindow();
        if (!glCapLoadOpenGL(glToSelect))
        {
            fprintf(stderr, "Fatal Error: Couldn't initialize default rendering system\n.");
            return FALSE;
        }
    }
    memStrncpy(mainDeviceToSelect, deviceToSelect, 16 - 1);
    memStrncpy(mainGLToSelect, glToSelect, 512 - 1);

    glCapStartup();

    if (strcasecmp(deviceToSelect, "d3d") == 0)
    {
        mainReinitRenderer = 2;
    }

    if (RGL)
    {
        rglSetAllocs((MemAllocFunc)mainMemAlloc, (MemFreeFunc)mainMemFree);
        if (fullScreen)
        {
            rglFeature(RGL_FULLSCREEN);
        }
        else
        {
            rglFeature(RGL_WINDOWED);
        }
        if (mainSoftwareDirectDraw)
        {
            rglFeature(RGL_HICOLOR);
        }
        else
        {
            rglFeature(RGL_TRUECOLOR);
        }
        if (slowBlits)
        {
            rglFeature(RGL_SLOWBLT);
        }
        else
        {
            rglFeature(RGL_FASTBLT);
        }
        if (accelFirst)
        {
            rglSelectDevice("fx", "");
            lodScaleFactor = 1.0f;                              //default scale factor is just right for 3dfx
        }
        if (deviceToSelect[0] != '\0')
        {
            rglSelectDevice(deviceToSelect, d3dToSelect);
            if (strcmp(deviceToSelect, "sw"))
            {
                lodScaleFactor = 1.0f;
            }
        }
    }
    else
    {
        lodScaleFactor = 1.0f;
    }

    return TRUE;
} /* doInit */

/*-----------------------------------------------------------------------------
    Name        : WindowsCleanup
    Description : Closes all windows, frees memory etc.
    Inputs      : void
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void WindowsCleanup(void)
{
    utyGameSystemsShutdown();
    if (!RGL)
    {
        /*hwDeleteWindow();*/
    }
    rinFreeDevices();
}

/*-----------------------------------------------------------------------------
    Name        : mainCleanupAfterVideo
    Description : possibly delete main DDraw window after playing an AVI if a renderer
                  is about to be initialized that wants to create its own
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainCleanupAfterVideo(void)
{
    if (windowNeedsDeleting)
    {
        windowNeedsDeleting = FALSE;
        //restore previous display mode
        /*hwSetRes(0, 0, 0);*/
    }
    if (!RGL)
    {
        //create a window at appropriate res
        /*(void)hwCreateWindow((int)ghMainWindow, MAIN_WindowWidth, MAIN_WindowHeight, MAIN_WindowDepth);*/
    }
}

#if 0
void RunPatcher(void)
{
    DeactivateMe();

    WinExec(PATCHNAME,SW_NORMAL);

//    WindowsCleanup();
    //PostQuitMessage(0);
}
#endif


/*-----------------------------------------------------------------------------
    Name        : WinMain
    Description : Entry point to the game
    Inputs      :
        hInstance       - instance handle for application
        hPrevInstance   - warning! does nothing under Win32!
        commandLine     - un-tokenized command line string
        nCmdShow        - run state (minimized etc.)
    Outputs     :
    Return      : Exit code (Windows provides an intelligible number)
----------------------------------------------------------------------------*/
/*
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR commandLine, int nCmdShow)
*/
int main (int argc, char* argv[])
{
    static char *errorString = NULL;
#ifdef _WIN32
    static HANDLE hMapping;
#endif
    static bool preInit;
    SDL_Event e;
    int event_res = 0;

#ifdef _WIN32
    //check to see if a copy of the program already running and just exit if so.
    //This messy hack is required because hPrevInstance is not reliable under Win32.
    hMapping = CreateFileMapping( (HANDLE) 0xffffffff,
                    NULL,
                    PAGE_READONLY,
                    0,
                    32,
                    "LSMap" );

    if( hMapping )
    {
        if( GetLastError() == ERROR_ALREADY_EXISTS )
        {
            HWND hWndPrev;

            // Already an instance running. If it has a window yet, restore it.
            hWndPrev = FindWindow(windowTitle, NULL);
            if (hWndPrev)
            {
                OutputDebugString("\r\nProgram already running.\r\n");
                ShowWindow(hWndPrev, SW_RESTORE);
                BringWindowToTop(hWndPrev);
                UpdateWindow(hWndPrev);
            }
            return 0;
        }
    }
#endif  /* Support for other platforms? */

    mainDeviceToSelect[0] = '\0';

    //load in options from the options file
    utyOptionsFileRead();

    //copy keyboard redefinitions
    opKeyboardLoad();

    /*
    RegisterCommandLine(commandLine);       // make sure you call this before ProcessCommandLine because
                                            // ProcessCommandLine modifies commandLine
    */

    //process the command line, setting flags to be used later
    if (ProcessCommandLine(argc, argv) != OKAY)
    {
        return 0;
    }

    glNT = glCapNT();
    gl95 = glCap95();

    if (selectedRES)
    {
        MAIN_WindowWidth  = mainWindowWidth;
        MAIN_WindowHeight = mainWindowHeight;
        MAIN_WindowDepth  = mainWindowDepth;
    }

#if MAIN_Password
    if ((errorString = mainPasswordVerify(mainPasswordPtr)) != NULL)
    {
        fprintf(stderr, "Homeworld Run-Time Error\n");
        return 0;
    }
#endif

    //initial game systems startup
    preInit = FALSE;
    if (errorString == NULL)
    {
        errorString = utyGameSystemsPreInit();
    }

    //startup the game window
    if (errorString == NULL)
    {
#ifdef _WIN32
        if (DebugWindow)
        {
            dbwClose();
        }
#endif
        preInit = TRUE;
        if (!InitWindow())
        {
            errorString = ersWindowInit;
        }
#ifdef _WIN32
        if (DebugWindow)
        {
            /* If porting this back to Windows, you'll need to get a hold of
               values for these (hWnd is in the SDL_SysWMinfo structure, hInst
               would need to be acquired through Windows API calls). */
            dbwStart((udword)ghInstance, (udword)ghMainWindow);
            utySet(SSA_DebugWindow);
        }
#endif
    }

    mainPlayAVIs = FALSE;
    if (errorString == NULL)
    {
#ifndef HW_DEMO
        if (enableAVI && fullScreen)
        {
            windowNeedsDeleting = TRUE;
            //set display mode
            /*
            if (!hwSetRes(640, 480, 32))
            {
                hwSetRes(640, 480, 16);
            }
            */
            utyForceTopmost(TRUE);
        }
        else if (!RGL)
        {
            windowNeedsDeleting = FALSE;
            /*(void)hwCreateWindow((int)ghMainWindow, MAIN_WindowWidth, MAIN_WindowHeight, MAIN_WindowDepth);*/
            utyForceTopmost(fullScreen);
        }
#else
        windowNeedsDeleting = FALSE;
        /*(void)hwCreateWindow((int)ghMainWindow, MAIN_WindowWidth, MAIN_WindowHeight, MAIN_WindowDepth);*/
        utyForceTopmost(fullScreen);
#endif
        //startup game systems
        if (errorString == NULL)
        {
            errorString = utyGameSystemsInit();
        }
    }

    if (errorString == NULL)
    {
        preInit = FALSE;

        while (TRUE)
        {
            // Give sound a break :)
            SDL_Delay(0);

            if (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    break;
                event_res = HandleEvent(&e);
            }
            else
            {
                utyTasksDispatch();                         //execute all tasks
            }

            if (opTimerActive)
            {
                if (taskTimeElapsed > (opTimerStart + opTimerLength))
                {
                    opTimerExpired();
                }
            }

#if 0
            if (patchComplete)
            {
                RunPatcher();
                patchComplete = 0;
            }
#endif
        }
    }
    else
    {                                                       //some error on startup, either from preInit or Init()
        if (preInit)
        {
            (void)utyGameSystemsPreShutdown();
        }
        fprintf(stderr, "%s\n", errorString);
        return ERR_ErrorStart;
    }

    /* Stay clean and fresh... */
    if (SDL_WasInit(SDL_INIT_EVERYTHING))
        SDL_Quit();

    return event_res;
} /* WinMain */
