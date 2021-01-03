// =============================================================================
//  Animatic.c
//  - playback of animatics via OpenGL
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 2/11/1999 by khent
// =============================================================================

#include "Animatic.h"

#include "avi.h"
#include "File.h"
#include "glinc.h"
#include "mouse.h"
#include "NIS.h"
#include "rglu.h"
#include "SinglePlayer.h"
#include "SoundEvent.h"
#include "soundlow.h"
#include "StringSupport.h"
#include "Subtitle.h"
#include "Universe.h"


sdword animaticJustPlayed = 0;  // actually a decremented counter for delaying purposes

static sdword g_frame;

#ifdef HW_ENABLE_MOVIES
static sdword g_displayFrame;
static bool   g_cleared;
#endif

typedef struct
{
    char filename[32];
} animlst;

static animlst animlisting[NUMBER_SINGLEPLAYER_MISSIONS];

static nisheader* animScriptHeader = NULL;
static sdword animCurrentEvent;

static real32 g_timeElapsed;

real32 animPreviousSFXVolume, animPreviousSpeechVolume, animPreviousMusicVolume;

//for localization
extern char *nisLanguageSubpath[];

static sdword subY1;


/* TC 2003-10-01:
 * The animStartup(), animShutdown(), and animBinkPlay() have been replaced
 * with dummy versions (after the block ignored by #if 0) so the game code can
 * stay the same. */

// Whilst the Linux folks want to zap most of this file for the moment and replace
// the original code with dummy functions (see above), the MacOSX folks don't.
// Please replace with a suitable #if you want to turn it back on. Thanks.

#ifdef NEVER_DEFINE_THIS  // Just spotted this. we don't want this code at all.
                          // Left it in for reference. Sorry.

// NB: Bink players available here: http://www.radgametools.com/bnkdown.htm
// I'm not sure how easy it will be to incorporate them (rather than SDK), nor
// whether they'd be happy if we did.

/* sound volume stuff */



#ifndef _MACOSX_FIX_ANIM
extern HDC hGLDeviceContext;
#endif




#define FORM_RGB32(R,G,B) \
        ((R << 16) | (G << 8) | B)
#define FORM_RGB565(R,G,B) \
        (((R) & 0xF8) << 8) | (((G) & 0xFC) << 3) | ((B) >> 3)
#define FORM_RGB555(R,G,B) \
        (((R) & 0xF8) << 7) | (((G) & 0xF8) << 2) | ((B) >> 3)


/*-----------------------------------------------------------------------------
    Name        : animBinkSetup
    Description : setup / reset the GL for video playback
    Inputs      : on - TRUE or FALSE, setup or reset
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animBinkSetup(bool on)
{
    static GLint matrixMode;
    static GLfloat projection[16];

    if (on)
    {
        glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
        glGetFloatv(GL_PROJECTION_MATRIX, projection);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        rgluOrtho2D(0.0f, (GLfloat)MAIN_WindowWidth, 0.0f, (GLfloat)MAIN_WindowHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    }
    else
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projection);

        glMatrixMode(matrixMode);
    }
}


#if 0
/*-----------------------------------------------------------------------------
    Name        : animBinkReverseRGBA
    Description : inplace y-flip an RGBA image (640x480x32)
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animBinkReverseRGBA(ubyte* surf)
{
    ubyte  line[4*640];
    sdword y, top, bot;
    sdword pitch;

    pitch = 4*640;

    for (y = 0; y < (480/2); y++)
    {
        top = y;
        bot = 479 - y;

        memcpy(line, surf + pitch*top, pitch);
        memcpy(surf + pitch*top, surf + pitch*bot, pitch);
        memcpy(surf + pitch*bot, line, pitch);
    }
}
#endif

/*-----------------------------------------------------------------------------
    Name        : animBinkDisplay
    Description : display callback for a frame of Bink video
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
#ifdef _WIN32
void animBinkDisplay(binkDisplayCallback_proc callback)
#else
void animBinkDisplay()
#endif
{
    sdword xOfs, yOfs;
#if 0
    ubyte* binkSurface = (ubyte*)binkGetSurface();

    if (g_frame <= g_displayFrame)
    {
        return;
    }
    g_displayFrame = g_frame;

    //prepare binkSurface
    callback(NULL, 0, 0, 0);
#endif
    xOfs = (MAIN_WindowWidth  - 640) / 2;
    yOfs = (MAIN_WindowHeight - 480) / 2;

    animBinkSetup(TRUE);
    glRasterPos2f((real32)xOfs, (real32)yOfs);

#ifndef _MACOSX_FIX_ANIM
    switch (RGLtype)
    {
    case GLtype:
        glcDisplayRGBABackgroundScaled(binkSurface);
        break;
    case SWtype:
        glDrawPixels(640, 480, GL_RGBA16, GL_UNSIGNED_BYTE, binkSurface);
        break;
    default:
        dbgFatalf(DBG_Loc, "what's this RGLtype: %d [binkSimpleDisplayProc]", RGLtype);
    }
#endif

    animBinkSetup(FALSE);

    animSubtitlesDraw();
    rndFlush();

    animSubtitlesClear();
}

#if 0
/*-----------------------------------------------------------------------------
    Name        : animGenericDecode
    Description : callback once per frame decode
    Inputs      : frame - current frame number of animation
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animBinkDecode(sdword frame)
{
    real32 framerate, timeElapsed;
    nisheader* header;
    nisevent* event;

    g_frame = frame;

    if (animScriptHeader == NULL)
    {
        return;
    }

    header = animScriptHeader;

    framerate = binkFrameRate;
    timeElapsed = (real32)frame / framerate;

    event = &animScriptHeader->events[animCurrentEvent];
    while (animCurrentEvent < header->nEvents &&
           event->time <= timeElapsed)
    {
        dbgAssertOrIgnore(nisEventDispatch[event->code] != NULL);
        nisEventDispatch[event->code](NULL, event);
        animCurrentEvent++;
        event++;
    }

    universe.totaltimeelapsed = timeElapsed;
    subTitlesUpdate();
}

/*-----------------------------------------------------------------------------
    Name        : animBinkEnd
    Description : cleanup after playing a Bink video file as an animatic
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animBinkEnd(void)
{
    universe.totaltimeelapsed = g_timeElapsed;
    soundEventStopMusic(0.0f);

    mouseCursorShow();

    rndClear();

    binkCleanup();
    soundstopall(0.0);
    speechEventCleanup();
    subReset();

    soundEventMusicVol(animPreviousMusicVolume);
    soundEventSpeechVol(animPreviousSpeechVolume);
    soundEventSFXVol(animPreviousSFXVolume);

    animaticJustPlayed = 8;
}

#endif	/* I hear Bink, but I don't see Bink... */

#endif  // #ifdef NEVER_DEFINE_THIS

/*-----------------------------------------------------------------------------
    Name        : animAviSetup
    Description : setup / reset the GL for video playback
    Inputs      : on - TRUE or FALSE, setup or reset
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animAviSetup(bool on)
{
    static GLint matrixMode;
    static GLfloat projection[16];

    if (on)
    {
        glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
        glGetFloatv(GL_PROJECTION_MATRIX, projection);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        rgluOrtho2D(0.0f, (GLfloat)MAIN_WindowWidth, 0.0f, (GLfloat)MAIN_WindowHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    }
    else
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projection);

        glMatrixMode(matrixMode);
    }
}

/*-----------------------------------------------------------------------------
    Name        : animAviDecode
    Description : callback once per frame decode
    Inputs      : frame - current frame number of animation
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animAviDecode(sdword frame)
{
    real32 framerate, timeElapsed;
    nisheader* header;
    nisevent* event;

    g_frame = frame;

    if (animScriptHeader == NULL)
    {
        return;
    }

    header = animScriptHeader;

    framerate = 15.0f;
    timeElapsed = (real32)frame / framerate;

    event = &animScriptHeader->events[animCurrentEvent];
    while (animCurrentEvent < header->nEvents &&
           event->time <= timeElapsed)
    {
        dbgAssertOrIgnore(nisEventDispatch[event->code] != NULL);
        nisEventDispatch[event->code](NULL, event);
        animCurrentEvent++;
        event++;
    }

    universe.totaltimeelapsed = timeElapsed;
    subTitlesUpdate();
}

/*-----------------------------------------------------------------------------
    Name        : animAviEnd
    Description : cleanup after playing a Avi video file as an animatic
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animAviEnd(void)
{
    universe.totaltimeelapsed = g_timeElapsed;
    soundEventStopMusic(0.0f);

    mouseCursorShow();

    rndClear();

    aviCleanup();
    soundstopall(0.0);
    speechEventCleanup();
    subReset();

    soundEventMusicVol(animPreviousMusicVolume);
    soundEventSpeechVol(animPreviousSpeechVolume);
    soundEventSFXVol(animPreviousSFXVolume);

    animaticJustPlayed = 8;
}

/*-----------------------------------------------------------------------------
    Name        : animSubtitlesSetup
    Description : setup / reset the GL for displaying subtitles
    Inputs      : on - TRUE or FALSE, setup or reset
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animSubtitlesSetup(bool on)
{
    static GLint matrixMode;
    static GLfloat projection[16];

    if (on)
    {
        glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
        glGetFloatv(GL_PROJECTION_MATRIX, projection);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    }
    else
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projection);

        glMatrixMode(matrixMode);
    }
}

/*-----------------------------------------------------------------------------
    Name        : animSubtitlesClear
    Description : clear (to black) the region that subtitles appear in
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animSubtitlesClear(void)
{
    rectangle r;

    if (subY1 < 0 || subY1 > (MAIN_WindowHeight >> 1))
    {
        return;
    }

    animSubtitlesSetup(TRUE);

    r.x0 = -1;
    r.y0 = 0;
    r.x1 = MAIN_WindowWidth;
    r.y1 = (subY1 > 128) ? subY1 : 128;
    primRectSolid2(&r, colBlack);

    animSubtitlesSetup(FALSE);
}

/*-----------------------------------------------------------------------------
    Name        : animSubtitlesDraw
    Description : render any subtitles
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animSubtitlesDraw(void)
{
    sdword index;

    animSubtitlesSetup(TRUE);

    subY1 = 0;

    for (index = 0; index < SUB_NumberRegions; index++)
    {
        if (subRegion[index].bEnabled && subRegion[index].cardIndex > 0)
        {
            subTimeElapsed = &universe.totaltimeelapsed;
            subTitlesDraw(&subRegion[index]);
            if (subRegion[index].rect.y1 > subY1)
            {
                subY1 = subRegion[index].rect.y1;
            }
        }
    }

    animSubtitlesSetup(FALSE);
}


/*-----------------------------------------------------------------------------
    Name        : animAviDisplay
    Description : display callback for a frame of Bink video
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
//#ifdef _WIN32
//void animAviDisplay(binkDisplayCallback_proc callback)
void animAviDisplay()
{
    //sdword xOfs, yOfs;
    int xOfs, yOfs;
#if 0
    ubyte* binkSurface = (ubyte*)binkGetSurface();

    if (g_frame <= g_displayFrame)
    {
        return;
    }
    g_displayFrame = g_frame;

    //prepare binkSurface
    callback(NULL, 0, 0, 0);
#endif
    xOfs = (MAIN_WindowWidth  - 640) / 2;
    yOfs = (MAIN_WindowHeight - 480) / 2;

    animAviSetup(TRUE);
//    glRasterPos2f((real32)xOfs, (real32)yOfs);
//    glRasterPos2i(xOfs, yOfs);

#ifdef NEVER_DEFINE_THIS
    switch (RGLtype)
    {
    case GLtype:
        glcDisplayRGBABackgroundScaled(binkSurface);
        break;
    case SWtype:
        glDrawPixels(640, 480, GL_RGBA16, GL_UNSIGNED_BYTE, binkSurface);
        break;
    default:
        dbgFatalf(DBG_Loc, "what's this RGLtype: %d [binkSimpleDisplayProc]", RGLtype);
    }
#endif

    animAviSetup(FALSE);

    animSubtitlesDraw();
    rndFlush();

    animSubtitlesClear();
}


/*-----------------------------------------------------------------------------
    Name        : animLoadNISScript
    Description : process an NIS script file containing time-keyed events
    Inputs      : scriptname - name of script file to process
    Outputs     :
    Return      : nisheader
----------------------------------------------------------------------------*/
nisheader* animLoadNISScript(char* scriptname)
{
    // for localization
	char *pString;
    char localisedPath[256];
    char string[256];
    nisheader* newHeader;

#if AVI_VERBOSE_LEVEL >= 2
    dbgMessage("animLoadNISScript: Entering");
#endif

    animCurrentEvent = 0;

    newHeader = memAlloc(sizeof(nisheader), "animatic NIS header", NonVolatile);
    memset(newHeader, 0, sizeof(nisheader));

    newHeader->iLookyObject = -1;
    newHeader->length = NIS_FrameRate * 10000;
//    if (scriptname != NULL && fileExists(scriptname, 0))
    if (scriptname != NULL)
    {
        // for localization
		strcpy(localisedPath, scriptname);
        if (strCurLanguage >= 1)
        {
            for (pString = localisedPath + strlen(localisedPath); pString > localisedPath; pString--)
            {                                               //find the end of the path
#ifdef _WIN32
                if (*pString == '\\')
#else
                if (*pString == '/')
#endif
                {
                    strcpy(string, pString + 1);            //save the file name
                    strcpy(pString + 1, nisLanguageSubpath[strCurLanguage]);//add the language sub-path
                    strcat(pString, string);                //put the filename back on
                    break;
                }
            }
        }

		if (fileExists(localisedPath, 0))
		{
			nisEventIndex = 0;
			nisCurrentHeader = newHeader;
			scriptSet(NULL, localisedPath, nisScriptTable);     //load in the script file
//        scriptSet(NULL, scriptname, nisScriptTable);
			nisCurrentHeader = NULL;
			newHeader->nEvents = nisEventIndex;
			if (newHeader->nEvents != 0)
			{
				newHeader->events = nisEventList;
				qsort(newHeader->events, newHeader->nEvents, sizeof(nisevent), nisEventSortCB);
				nisEventList = NULL;
			}
			else
			{
				newHeader->events = NULL;
			}
		}
		else
		{
			// file doesn't exist
			newHeader->nEvents = 0;
			newHeader->events = NULL;
		}
    }
    else
    {
		// script name = null
        newHeader->nEvents = 0;
        newHeader->events = NULL;
    }
    return newHeader;
}



/*-----------------------------------------------------------------------------
    Name        : animStartup
    Description : reads animatics.lst from the Movies directory
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animStartup(void)
{
    filehandle lst;
    char   line[128], temp[64];
    sdword level;

    memset(animlisting, 0, NUMBER_SINGLEPLAYER_MISSIONS*sizeof(animlst));

#ifdef _WIN32
    if (!fileExists("Movies\\animatics.lst", 0))
#else
    if (!fileExists("Movies/animatics.lst", 0))
#endif
    {
        return;
    }

#ifdef _WIN32
    lst = fileOpen("Movies\\animatics.lst", FF_TextMode);
#else
    lst = fileOpen("Movies/animatics.lst", FF_TextMode);
#endif
    while (fileLineRead(lst, line, 127) != FR_EndOfFile)
    {
        if (strlen(line) < 5)
        {
            continue;
        }
        if (line[0] == ';' || line[0] == '/')
        {
            continue;
        }
        sscanf(line, "%d %s", &level, temp);
        dbgAssertOrIgnore(level >= 0 && level < NUMBER_SINGLEPLAYER_MISSIONS);
        memStrncpy(animlisting[level].filename, temp, 31);
    }
    fileClose(lst);
}

/*-----------------------------------------------------------------------------
    Name        : animShutdown
    Description : releases memory required by the animatics listing file
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void animShutdown(void)
{
#ifdef _MACOSX_FIX_ANIM	/* BINK!@#$3 */
    memset(animlisting, 0, NUMBER_SINGLEPLAYER_MISSIONS*sizeof(animlst));
#endif	/* BONK!@#$1$ */
}

/*-----------------------------------------------------------------------------
    Name        : animBinkPlay
    Description : plays a Bink video file
    Inputs      : a, b - levels this video is playing between
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool animBinkPlay(sdword a, sdword b)
{
#if 0 /* One more time...BINK!@#1 */
    bool rval;
    char filename[1024], scriptname[1024];
    void animBinkEnd(void);
    extern sdword trLitPaletteBits;

    if (tutorial == 1)
    {
        return FALSE;
    }

    g_cleared = FALSE;
    g_frame = 0;
    g_displayFrame = 0;
    animaticJustPlayed = 0;
    subReset();

    if (a < 0)
    {
        animScriptHeader = NULL;
        sprintf(filename, (char*)b);
    }
    else
    {
        dbgAssertOrIgnore(a >= 0 && b < NUMBER_SINGLEPLAYER_MISSIONS);
        if (animlisting[a].filename[0] == '\0')
        {
            return FALSE;
        }

#ifdef _WIN32
        sprintf(filename, "Movies\\%s.bik", animlisting[a].filename);
        sprintf(scriptname, "Movies\\%s.script", animlisting[a].filename);
#else
        sprintf(filename, "Movies/%s.bik", animlisting[a].filename);
        sprintf(scriptname, "Movies/%s.script", animlisting[a].filename);
#endif
        animScriptHeader = animLoadNISScript(scriptname);
    }

    soundEventStopMusic(0.0f);
    soundstopall(0.0f);
    rndSetClearColor(colBlack);
    rndClear();

    mouseCursorHide();

    g_timeElapsed = universe.totaltimeelapsed;
    universe.totaltimeelapsed = 0.0f;

    soundEventGetVolume(&animPreviousSFXVolume, &animPreviousSpeechVolume, &animPreviousMusicVolume);
#ifndef _MACOSX_FIX_ANIM
    rval = binkPlay(filename,
                    animBinkDisplay,
                    animBinkDecode,
                    (trLitPaletteBits == 15) ? S_RGB555 : S_RGB565,
                    FALSE, -1);
#endif

#ifndef _WIN32
	animBinkDisplay();
#endif
    //animBinkEnd();

    return rval;
#endif	/* bonk? */
	animaticJustPlayed = 8;

	return TRUE;  /* LIAR!! */
}


/*-----------------------------------------------------------------------------
    Name        : animAviPlay
    Description : plays a Bink video file
    Inputs      : a, b - levels this video is playing between
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool animAviPlay(sdword a, sdword b)
{
    bool rval = TRUE;

#if AVI_VERBOSE_LEVEL >= 2
    dbgMessage("animAviPlay: Entering");
#endif


#ifdef HW_ENABLE_MOVIES

    char filename[1024], scriptname[1024];
//    void animBinkEnd(void);
    extern sdword trLitPaletteBits;

    if (tutorial == 1)
    {
        return FALSE;
    }

    g_cleared = FALSE;
    g_frame = 0;
    g_displayFrame = 0;
    animaticJustPlayed = 0;
    subReset();

    if (a < 0)
    {
        animScriptHeader = NULL;
        sprintf(filename, (char*)b);
    }
    else
    {
        dbgAssertOrIgnore(a >= 0 && b < NUMBER_SINGLEPLAYER_MISSIONS);
        if (animlisting[a].filename[0] == '\0')
        {
            return FALSE;
        }

#ifdef _WIN32
        sprintf(filename, "Movies\\%s.avi", animlisting[a].filename);
        sprintf(scriptname, "Movies\\%s.script", animlisting[a].filename);
#else
        sprintf(filename, "Movies/%s.avi", animlisting[a].filename);
        sprintf(scriptname, "Movies/%s.script", animlisting[a].filename);
#endif
        animScriptHeader = animLoadNISScript(scriptname);
    }

    soundEventStopMusic(0.0f);
    soundstopall(0.0f);
    if (!aviInit())
    {
        dbgMessage("animAviPlay: aviInit failed.");
        return FALSE;
    }
    rndSetClearColor(colBlack);
    rndClear();

    mouseCursorHide();

    g_timeElapsed = universe.totaltimeelapsed;
    universe.totaltimeelapsed = 0.0f;

    soundEventGetVolume(&animPreviousSFXVolume, &animPreviousSpeechVolume, &animPreviousMusicVolume);
    rval = aviPlay(filename);
//                    animBinkDisplay,
//                    animBinkDecode,
//                    (trLitPaletteBits == 15) ? S_RGB555 : S_RGB565,
//                    FALSE, -1);

    animAviEnd();

#endif   // HW_ENABLE_MOVIES

	animaticJustPlayed = 8;

    return rval;
}

