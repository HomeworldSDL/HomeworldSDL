/*============================================================================
    MAIN.C: Main Windows interface for Homeworld game code, including message
        handling and window set-up.

        Created June 1997 by Luke Moloney.
============================================================================*/

#include <limits.h> // for PATH_MAX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "AIPlayer.h"
#include "AutoLOD.h"
#include "avi.h"
// #include "bink.h"
#include "BTG.h"
#include "Camera.h"
#include "Captaincy.h"
#include "ColPick.h"
#include "CommandLayer.h"
#include "ConsMgr.h"
#include "Debug.h"
#include "Demo.h"
#include "FEReg.h"
#include "File.h"
#include "FontReg.h"
#include "Formation.h"
#include "glinc.h"
#include "Globals.h"
#include "HorseRace.h"
#include "Key.h"
#include "LaunchMgr.h"
#include "main.h"
#include "mainrgn.h"
#include "Memory.h"
#include "mouse.h"
#include "MultiplayerGame.h"
#include "NIS.h"
#include "ObjTypes.h"
#include "Options.h"
#include "Particle.h"
#include "PiePlate.h"
#include "regkey.h"
#include "render.h"
#include "ResearchAPI.h"
#include "ResearchGUI.h"
#include "resource.h"
#include "rinit.h"
#include "Sensors.h"
#include "SoundEvent.h"
#include "soundlow.h"
#include "StringSupport.h"
#include "Subtitle.h"
#include "Tactics.h"
#include "Task.h"
#include "TitanNet.h"
#include "utility.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define strcasecmp _stricmp
    #include <windows.h>
    #include <winreg.h>
    #include "debugwnd.h"
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

#ifndef __APPLE__
static char windowTitle[] = "Homeworld";//name of window
#endif

//error strings
char ersWindowInit[] = "Error creating window";

//screen width, height
int MAIN_WindowWidth = 640;
int MAIN_WindowHeight = 480;
int MAIN_WindowDepth = 16;

sdword mainWidthAdd = 0;
sdword mainHeightAdd = 0;

int mainWindowWidth;
int mainWindowHeight;
int mainWindowDepth;
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
bool mainNoPalettes = TRUE;
bool mainSoftwareDirectDraw = FALSE;
bool mainDirectDraw = FALSE;
bool mainRasterSkip = FALSE;
bool mainDoubleIsTriple = FALSE;
#ifdef __GNUC__
bool mainFastFrontend = FALSE;
#else
bool mainFastFrontend = TRUE;
#endif
bool mainForceSoftware = FALSE;
bool mainAutoRenderer = TRUE;
bool mainForceKatmai = FALSE;
#ifdef _WINDOWS
bool mainAllowKatmai = TRUE;
#else
bool mainAllowKatmai = FALSE;
#endif
bool mainAllow3DNow = FALSE;
bool enableAVI = TRUE;
bool mainAllowPacking = TRUE;
bool mainOnlyPacking = FALSE;
bool gShowDamage = TRUE;
bool DebugWindow = FALSE;
sdword MemoryHeapSize = MEM_HeapSizeDefault;
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

// turn fullscreen off when debugging so that if the debugger kicks in
// after a crash you don't find yourself locked out and have to reboot...
#if defined(__APPLE__) && defined(HW_BUILD_FOR_DEBUGGING) 
bool fullScreen = FALSE;
#else
bool fullScreen = TRUE;
#endif

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
bool8 RENDER_BOXES = FALSE;
bool8 RENDER_LIGHTLINES = FALSE;
#if CL_TEXTFEEDBACK
bool enableTextFeedback = FALSE;
#endif

#if FEF_TEXTURES_DISABLABLE
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
bool mainCDCheckEnabled = TRUE;

#if defined(HW_GAME_DEMO)
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "DLD_05";
#elif defined(HW_GAME_RAIDER_RETREAT)
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "OEMV1";
#else
char networkVersion[MAX_NETWORKVERSION_STRING_LEN] =   "HomeworldV1C";
#endif

#if defined(HW_GAME_RAIDER_RETREAT)
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


/*=============================================================================
    Functions:
=============================================================================*/

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


bool EnableFileLoadLog(char *string)
{
    logfileClear(FILELOADSLOG);
    return TRUE;
}

bool SelectDevice(char* string)
{
    selectedDEVICE = TRUE;
    return TRUE;
}

bool SelectMSGL(char* string)
{
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
    mainFastFrontend = FALSE;
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
#ifdef HW_BUILD_FOR_DEBUGGING
    entryComment("DEBUGGING OPTIONS"),//-----------------------------------------------------
    entryVr("/debug",               DebugWindow, TRUE,                  " - Enable debug window."),
    entryVr("/nodebugInt",          dbgAllowInterrupts, FALSE,          " - Fatal errors don't generate an interrupt before exiting."),
#if DBW_TO_FILE
    entryVr("/debugToFile",         debugToFile, TRUE,                  " - output debugging info to a file."),
#endif
#else
    entryVrHidden("/debug",         DebugWindow, TRUE,                  " - Enable debug window."),
    entryVrHidden("/nodebugInt",    dbgAllowInterrupts, FALSE,          " - Fatal errors don't generate an interrupt before exiting."),
#if DBW_TO_FILE
    entryVrHidden("/debugToFile",   debugToFile, TRUE,                  " - output debugging info to a file."),
#endif
#endif
#if RANDOM_DEBUG_CALL_SEQ
    entryVr("/ranCallerDebug",      ranCallerDebug, TRUE,               " - debug non-deterministic calling of random numbers."),
#endif
#ifdef HW_BUILD_FOR_DEBUGGING
    entryFn("/autosavedebug",       EnableAutoSaveDebug,                " autosaves game frequently"),
#endif

    entryComment("SYSTEM OPTIONS"), //-----------------------------------------------------
    entryFnParam("/heap",           HeapSizeSet,                        " <n> - Sets size of global memory heap to [n]."),
    entryFnParam("/bigoverride",    fileOverrideBigPathSet,             " <path> - Sets path to search for opening files."),
    entryFnParam("/CDpath",         fileCDROMPathSet,                   " <path> - Sets path to CD-ROM in case of ambiguity."),
    entryFnParam("/settingspath",   fileUserSettingsPathSet,            " <path> - Sets the path to store settings, saved games, and screenshots (defaults to ~/.homeworld)."),
#if MAIN_MOUSE_FREE
#ifdef HW_BUILD_FOR_DEBUGGING
    entryVr("/freemouse",           startupClipMouse, FALSE,            " - Mouse free to move about entire screen at startup.  Use <CTRL>F11 to toggle during play."),
#else
    entryVrHidden("/freemouse",     startupClipMouse, FALSE,            " - Mouse free to move about entire screen at startup.  Use <CTRL>F11 to toggle during play."),
#endif
#endif
    entryVr("/ignoreBigfiles",      IgnoreBigfiles, TRUE,               " - don't use anything from bigfile(s)"),
#ifdef HW_BUILD_FOR_DEBUGGING
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
#if FEF_TEXTURES_DISABLABLE
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
    entryFnParam("/device",         SelectDevice,                       " <dev> - select an rGL device by name, eg. sw, fx."),
//    entryFV("/gl",                  SelectMSGL, selectedGL, TRUE,       " - select default OpenGL as renderer."),
    entryVr("/nohint",              mainNoPerspective, TRUE,            " - disable usage of OpenGL perspective correction hints."),
    entryVrHidden("/noPause",             noPauseAltTab, TRUE,                " - don't pause when you alt-tab."),
    entryVrHidden("/noMinimize",          noMinimizeAltTab, TRUE,             " - don't minimize when you alt-tab."),

#ifdef HW_BUILD_FOR_DEBUGGING
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

#if RND_VISUALIZATION
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
#ifdef HW_BUILD_FOR_DEBUGGING
    entryVr("/NoBind",              bkDisableKeyRemap, TRUE,            " - disable key bindings so that debug keys work."),
#else
    entryVrHidden("/NoBind",        bkDisableKeyRemap, TRUE,            " - disable key bindings so that debug keys work."),
#endif

#ifdef HW_BUILD_FOR_DEBUGGING
    entryComment("COMPUTER PLAYER AND STATS"),//-----------------------------------------------------
//    {"/compPlayer",  EnableComputerPlayers, "=01234567 to enable all computer players"},
    entryVr("/aiplayerLog",         aiplayerLogEnable, TRUE,            " - enable AI Player Logging"),
    entryVr("/determCompPlayer",    determCompPlayer, TRUE,             " - makes computer players deterministic"),
    entryFV("/gatherStats",         EnableGatherStats, gatherStats, TRUE,"- enable gathering of stats"),
    entryFnParam("/showStatsFight", EnableShowStatsFight,               "=<i,j> to show stats fight i,j"),
    entryFnParam("/showStatsFancyFight", EnableShowStatsFancyFight,     "=filename.script"),
#endif

#ifdef HW_BUILD_FOR_DEBUGGING
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
#ifdef HW_BUILD_FOR_DEBUGGING
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
#ifdef HW_BUILD_FOR_DEBUGGING
    entryVr("/onlyPacking",         mainOnlyPacking, TRUE,              " - only display packed textures."),
#endif
*/

    entryComment("MISC OPTIONS"),   //-----------------------------------------------------
    entryVrHidden("/smCentreCamera",      smCentreWorldPlane, FALSE,          " - centres the SM world plane about 0,0,0 rather than the camera."),

#if RND_PLUG_DISABLEABLE
    entryVr("/noPlug",              rndShamelessPlugEnabled, FALSE,     " - don't display relic logo on pause."),
#endif

    entryVrHidden("/closeCaptioned",      subCloseCaptionsEnabled, TRUE,      " - close captioned for the hearing impared."),
    entryVr("/pilotView",           pilotView, TRUE, " - enable pilot view.  Focus on single ship and hit Q to toggle."),

    END_COMMAND_OPTION,
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
    printf("%s", gHelpString);
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
                dbgAssertOrIgnore(commandOptions[index].variableToModify || commandOptions[index].function);
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
        case CID_ExitError:
            e.quit.type = SDL_QUIT;
            SDL_PushEvent(&e);

            break;
        case CID_ExitOK:
            e.quit.type = SDL_QUIT;
            SDL_PushEvent(&e);

            break;
    }
}

filehandle mainGetDevStatsHandle(char *filepath) {
    if (!fopen (filepath, "r")) {
        return 0;
    }
    return fileOpen(filepath, FF_IgnorePrepend | FF_TextMode | FF_IgnoreBIG);
}

/*-----------------------------------------------------------------------------
devstat init startup removed
----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------

    Name        : mainDevStatsShutdown
    Description : release memory used by the devstats table
    Inputs      :
    Outputs     :
    Return      :

fixme: This section was called devstats shutdown but seems to be more. Cleanup required...
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
    sounddeactivate(TRUE);

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

    feRenderEverything = TRUE;

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

/*-----------------------------------------------------------------------------
    Name        : mainFreeLibraries
    Description : Release references to possible GL libraries.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainFreeLibraries(void)
{
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

    renderData.width = MAIN_WindowWidth;
    renderData.height = MAIN_WindowHeight;
#ifdef _WIN32
    renderData.hWnd = ghMainWindow;
#endif
    renderData.hWnd = 0;
    if (!rndSmallInit(&renderData, TRUE))
    {
        return FALSE;
    }

    utyForceTopmost(fullScreen);

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

    utyForceTopmost(fullScreen);

    return TRUE;
}

void mainDestroyWindow(void)
{
    mainActuallyQuit = FALSE;

    mainWindowTotalWidth  = MAIN_WindowWidth  + mainWidthAdd;
    mainWindowTotalHeight = MAIN_WindowHeight + mainHeightAdd;

#ifdef _WIN32
    if (DebugWindow)
    {
        //dbwClose();
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
        //dbwStart((udword)ghInstance, (udword)ghMainWindow);
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
    rndResetGLState();
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
    rmGUIStartup();
    cmLoadTextures();
    btgLoadTextures();
    lmLoadTextures();
    //shameless plug handles reloading itself
    frReloadGL();
}

sdword saveMAIN_WindowWidth;
sdword saveMAIN_WindowHeight;
sdword saveMAIN_WindowDepth;

/*-----------------------------------------------------------------------------
    Name        : mainSaveRender
    Description : save current renderer info in case a mode switch doesn't work
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void mainSaveRender(void)
{
    saveMAIN_WindowWidth  = MAIN_WindowWidth;
    saveMAIN_WindowHeight = MAIN_WindowHeight;
    saveMAIN_WindowDepth  = MAIN_WindowDepth;
}

void mainSetupSoftware(void)
{
    mainWindowWidth  = MAIN_WindowWidth;
    mainWindowHeight = MAIN_WindowHeight;
    mainWindowDepth  = MAIN_WindowDepth;

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
    mainWindowWidth  = MAIN_WindowWidth  = saveMAIN_WindowWidth;
    mainWindowHeight = MAIN_WindowHeight = saveMAIN_WindowHeight;
    mainWindowDepth  = MAIN_WindowDepth  = saveMAIN_WindowDepth;

    mainRescaleMainWindow();

    bMustFree = FALSE;
    if (!mainLoadGL(NULL))
    {
        //couldn't restore, try basic software
        mainRestoreSoftware();
    }
    bMustFree = TRUE;

    feRenderEverything = TRUE;
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
    dbgMessage("mainShutdownRenderer");

    mainCloseRender();
    mainShutdownGL();
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
    dbgMessage("-- load OpenGL --");

    if (bMustFree)
    {
        mainCloseRender();
        mainShutdownGL();
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
    Inputs      : device - device name {sw, fx}
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool mainLoadParticularRGL(char* device, char* data)
{
    dbgMessagef("-- load rGL device %s --", device);

    if (bMustFree)
    {
        mainCloseRender();
        mainShutdownGL();
        mainFreeLibraries();
    }

    if (!mainStartupParticularRGL(device, data))
    {
        return FALSE;
    }

    mainOpenRender();

    lodScaleFactor = 1.0f;
    alodStartup();

    mainReinitRenderer = 2;

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

    dbgAssertOrIgnore(pEvent);

#if MAIN_PRINT_MESSAGES
    dbgMessagef("Event type = 0x%hhx", pEvent->type);
#endif //MAIN_PRINT_MESSAGES

    switch (pEvent->type)
    {
        case SDL_APP_DIDENTERFOREGROUND:
            if (systemActive == FALSE)
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
            else
                return 0;
        case SDL_APP_WILLENTERBACKGROUND:
            if (systemActive == TRUE)
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
                return 0;                                   //per documentation

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
                        keyPressUp(pEvent->key.keysym.scancode);
                    }
#if MAIN_MOUSE_FREE
                case SDLK_F11:                              //toggle clipped cursor
                    if (keyIsHit(CONTROLKEY))
                    {
                        utyClipMouse(mouseClipped ^ TRUE);
                    }
                    else
                    {
                        keyPressUp(pEvent->key.keysym.scancode);
                    }
                    break;
#endif
                case SDLK_F12:
                    if (keyIsHit(SHIFTKEY) && keyIsHit(CONTROLKEY))
                    {
                        mainCloseRender();
                        mainShutdownGL();
                        mainRestoreSoftware();
                        mainOpenRender();
                        lodScaleFactor = LOD_ScaleFactor;
                        alodStartup();
                    }
                    break;
                default:
                    keyPressUp(keyLanguageTranslate(pEvent->key.keysym.scancode));//keyPressUp(KeyMapFromWindows(wParam));
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
            keyPressDown(keyLanguageTranslate(pEvent->key.keysym.scancode));
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

               }
            }
            break;

        case SDL_MOUSEWHEEL:
            if (!mouseDisabled)
            {
                if(pEvent->wheel.y == 1)
                    keyPressDown(FLYWHEEL_UP);
                else if(pEvent->wheel.y == -1)
                    keyPressDown(FLYWHEEL_DOWN);
                break;
            }
 
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
            
        // Currently written with use of SpaceNavigator in mind (since that's
        // what I've got). Needs to be rewritten to be a little more generic
        // using mappings of registered joystick axes to functionality.
        case SDL_JOYAXISMOTION:
            #define DEBUG_JOYSTICK_CAMERA            FALSE
            #define JOYSTICK_SENSITIVITY_THRESHOLD  (10 * SWORD_Max / 100)

            if (pEvent->jaxis.value > -JOYSTICK_SENSITIVITY_THRESHOLD
            &&  pEvent->jaxis.value <  JOYSTICK_SENSITIVITY_THRESHOLD)
            {
                break;
            }
            
            switch (pEvent->jaxis.axis) 
            {
                // zoom: +ve multiplier = zoom out
                
                case 1: // translation (+/-) forwards-backwards 
                    // camJoyZoom = -1 * pEvent->jaxis.value;
                    break;
                    
                case 2: // translation (+/-) up-down
                    camJoyZoom = -1 * pEvent->jaxis.value;
                    break;

                // declination: +ve multiplier = move toward north pole
                
                case 3: // pitch (+/-) back-forward  - declination
                    camJoyDeclination = +1 * pEvent->jaxis.value;
                    break;

                // right ascension: +ve multiplier = anti clockwise about z-axis

                case 0: // translation (+/-) left-right 
                    // camJoyRightAscension = -1 * pEvent->jaxis.value;
                    break;

                case 4: // roll  (+/-) left-right    - rotate
                    // camJoyRightAscension = -1 * pEvent->jaxis.value;
                    break;

                case 5: // yaw   (+/-) right-left    - rotate
                    camJoyRightAscension = -1 * pEvent->jaxis.value;
                    break;

                default: 
                    break; 
            }

#if DEBUG_JOYSTICK_CAMERA            
            dbgMessagef("joystick: dec(%6d) asc(%6d) zoom(%6d)", camJoyDeclination, camJoyRightAscension, camJoyZoom);
#endif
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

    mainWindowTotalWidth  = MAIN_WindowWidth  + mainWidthAdd;
    mainWindowTotalHeight = MAIN_WindowHeight + mainHeightAdd;

    /* Window created in renderer initialization.. */

    rinEnumerateDevices();

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

    lodScaleFactor = 1.0f;

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
    }
}

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

    if (selectedRES)
    {
        MAIN_WindowWidth  = mainWindowWidth;
        MAIN_WindowHeight = mainWindowHeight;
        MAIN_WindowDepth  = mainWindowDepth;
    }

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
            //dbwClose();
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
            //dbwStart((udword)ghInstance, (udword)ghMainWindow);
            utySet(SSA_DebugWindow);
        }
#endif
    }

    mainPlayAVIs = FALSE;
    if (errorString == NULL)
    {
#ifndef HW_GAME_DEMO
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
        else
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
        bool breakMainLoop = FALSE;

        while (TRUE)
        {
            // Give sound a break :)
            SDL_Delay(0);

            while (SDL_PollEvent(&e))
            {
                event_res = HandleEvent(&e);

                if (e.type == SDL_QUIT) {
                    breakMainLoop = TRUE;
                    break;
                }
            }
            if (breakMainLoop) break;

            utyTasksDispatch();                         //execute all tasks

            if (opTimerActive)
            {
                if (taskTimeElapsed > (opTimerStart + opTimerLength))
                {
                    opTimerExpired();
                }
            }
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
    {
        SDL_Quit();
    }

    return event_res;
} /* WinMain */
