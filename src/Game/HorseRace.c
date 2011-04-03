/*=============================================================================
    Name    : HorseRace.c
    Purpose : Logic for the horse race and loading chat screens.

    Created 6/16/1998 by ddunlop
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "HorseRace.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "AutoDownloadMap.h"
#include "Chatting.h"
#include "CommandNetwork.h"
#include "Debug.h"
#include "Demo.h"
#include "devstats.h"
#include "ETG.h"
#include "FEFlow.h"
#include "FEReg.h"
#include "File.h"
#include "font.h"
#include "FontReg.h"
#include "glinc.h"
#include "interfce.h"
#include "LinkedList.h"
#include "main.h"
#include "Memory.h"
#include "mouse.h"
#include "MultiplayerGame.h"
#include "NIS.h"
#include "prim2d.h"
#include "Region.h"
#include "render.h"
#include "SinglePlayer.h"
#include "StringSupport.h"
#include "Task.h"
#include "Teams.h"
#include "texreg.h"
#include "TimeoutTimer.h"
#include "Titan.h"
#include "Tweak.h"
#include "UIControls.h"
#include "Universe.h"
#include "utility.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define strcasecmp _stricmp
    #include <direct.h>
    #include <io.h>
    #include <windows.h>
#else
    #include <dirent.h>
    #include <unistd.h>
#endif


#define ShouldHaveMousePtr (FALSE)

extern Uint32 utyTimerLast;
extern udword gDevcaps2;

/*=============================================================================
    defines:
=============================================================================*/

#ifdef _MACOSX
    #define HR_SCALE_MISSION_LOADING_SCREENS  TRUE
#else
    #define HR_SCALE_MISSION_LOADING_SCREENS  FALSE
#endif

#define HR_PlayerNameFont   "Arial_12.hff"
#define MAX_CHAT_TEXT       64
#define NUM_CHAT_LINES      10

real32 HorseRacePlayerDropoutTime = 10.0f;     // tweakable
color HorseRaceDropoutColor = colRGB(75,75,75);

/*=============================================================================
    data:
=============================================================================*/

/*extern HDC hGLDeviceContext;*/

static rectangle hrSinglePlayerPos;
static color hrSinglePlayerLoadingBarColor = colRGB(255,63,63);  // pastel red

static sdword JustInit;
static sdword localbar;

// Pixels and info about the background image chosen
static bool hrBackgroundInitFrame = 0;
static udword *hrBackgroundImage = NULL;
static long hrBackgroundDirty = 0;
bool hrBackgroundReinit = FALSE;
static long hrBackXSize, hrBackYSize;

static bool hrScaleMissionLoadingScreens = HR_SCALE_MISSION_LOADING_SCREENS;

static regionhandle hrDecRegion;

HorseStatus horseracestatus;
static udword hrProgressCounter = 0;

TTimer hrPlayerDropoutTimers[MAX_MULTIPLAYER_PLAYERS];

fibfileheader *hrScreensHandle = NULL;

fonthandle      playernamefont=0;
color           hrBackBarColor=HR_BackBarColor;
color           hrChatTextColor=HR_ChatTextColor;

bool            PlayersAlreadyDrawnDropped[MAX_MULTIPLAYER_PLAYERS];

textentryhandle ChatTextEntryBox = NULL;

regionhandle   hrBaseRegion       = NULL;
regionhandle   hrProgressRegion   = NULL;
regionhandle   hrChatBoxRegion    = NULL;
regionhandle   hrAbortLoadConfirm = NULL;

region horseCrapRegion =
{
    {-1,-1,-1,-1},             // rectangle
    NULL,                      // draw function
    NULL,                      // process function
    NULL, NULL,                // parent, child
    NULL, NULL,                // previous, next
    0, 0,                      // flags, status,
    0,                         // nKeys
    {0},                       // keys
    0,                         // userID
#if REG_ERROR_CHECKING
    REG_ValidationKey,         // validation key
#endif
    0,                         // tabstop
    UNINITIALISED_LINKED_LIST, // cutouts;
    {0,0},                     // drawstyle[2];
    0,                         // lastframedrawn;
    NULL                       // atom;
};

bool hrRunning=FALSE;

ChatPacket chathistory[NUM_CHAT_LINES];

udword      chatline=0;

void horseRaceRender(void);
void hrDrawPlayersProgress(featom *atom, regionhandle region);
void hrDrawChatBox(featom *atom, regionhandle region);
void hrDrawFile(char* filename, sdword x, sdword y);
void hrChatTextEntry(char *name, featom *atom);
void hrAbortLoadingYes(char *name, featom *atom);
void hrAbortLoadingNo(char *name, featom *atom);

real32 horseRaceGetPacketPercent(real32 barPercent);

//HR_ChatTextEntry
//extern nisheader *utyTeaserHeader;

/*=============================================================================
    Functions
=============================================================================*/
fedrawcallback hrDrawCallback[] =
{
    {hrDrawPlayersProgress,     "HR_DrawPlayersProgress"    },
    {hrDrawChatBox,             "HR_DrawChatBox"            },
    {NULL,          NULL}
};

fecallback hrCallBack[] =
{
    {hrChatTextEntry,           "HR_ChatTextEntry"          },
    {hrAbortLoadingYes,         "HR_AbortLoadingYes"        },
    {hrAbortLoadingNo,          "HR_AbortLoadingNo"         },
    {NULL,          NULL}
};

void hrDirtyProgressBar()
{
    if (hrRunning)
    {
        if (hrProgressRegion != NULL)
        {
#ifdef DEBUG_STOMP
            regVerify(hrProgressRegion);
#endif
            hrProgressRegion->status |= RSF_DrawThisFrame;
        }
    }
}

void hrDirtyChatBox()
{
    if (hrRunning)
    {
        if (hrChatBoxRegion != NULL)
        {
#ifdef DEBUG_STOMP
            regVerify(hrChatBoxRegion);
#endif
            hrChatBoxRegion->status |= RSF_DrawThisFrame;
        }
    }
}

void hrBarDraw(rectangle *rect, color back, color fore, real32 percent)
{
    rectangle temp;
//  primRectSolid2(rect, back);

    if (percent > 1.0)
    {
        percent = 1.0;
    }

    temp.x0 = rect->x0;
    temp.y0 = rect->y0;
    temp.x1 = rect->x0 + (sdword)((rect->x1-rect->x0)*percent);
    temp.y1 = rect->y1;

    primRectSolid2(&temp, fore);
}

void hrDrawPlayersProgress(featom *atom, regionhandle region)
{
    sdword     index;
    rectangle pos;
    rectangle outline;
    real32 percent;
    fonthandle currentfont;
    bool droppedOut;

    hrProgressRegion = region;

    pos = region->rect;

//  primRectSolid2(&pos, colRGBA(0, 0, 0, 64));

    pos.y0+=fontHeight(" ");
    pos.y1=pos.y0+8;

    currentfont = fontMakeCurrent(playernamefont);

    if (multiPlayerGame)
    {
        dbgAssertOrIgnore(sigsNumPlayers == tpGameCreated.numPlayers);
        for (index=0;index<sigsNumPlayers;index++)
        {
            droppedOut = playerHasDroppedOutOrQuit(index);

            outline = pos;
            outline.x0 -= 5;
            outline.x1 += 10;
            outline.y0 -= 3;
            outline.y1 = outline.y0 + fontHeight(" ")*2 - 2;

            if ((hrBackgroundDirty) || (!PlayersAlreadyDrawnDropped[index]))
            {
                PlayersAlreadyDrawnDropped[index] = droppedOut;

                primRectSolid2(&outline, colBlack);

                if (droppedOut)
                {
                    fontPrintf(pos.x0,pos.y0,colBlack,"%s",tpGameCreated.playerInfo[index].PersonalName);
                    fontPrintf(pos.x0,pos.y0,HorseRaceDropoutColor,"%s",
                               (playersReadyToGo[index] == PLAYER_QUIT) ? strGetString(strQuit) : strGetString(strDroppedOut));
                }
                else
                {
                    fontPrintf(pos.x0,pos.y0,tpGameCreated.playerInfo[index].baseColor,"%s",tpGameCreated.playerInfo[index].PersonalName);

                    if (horseracestatus.hrstatusstr[index][0])
                    {
                        fontPrintf(pos.x0+150,pos.y0,tpGameCreated.playerInfo[index].baseColor,"%s",horseracestatus.hrstatusstr[index]);
                    }
                }
            }

            primRectOutline2(&outline, 1, (droppedOut) ? HorseRaceDropoutColor : tpGameCreated.playerInfo[index].stripeColor);

            pos.y0+=fontHeight(" ");
            pos.y1=pos.y0+4;

            percent = horseracestatus.percent[index];

            hrBarDraw(&pos,hrBackBarColor,(droppedOut) ? HorseRaceDropoutColor : tpGameCreated.playerInfo[index].baseColor,percent);

            pos.y0+=fontHeight(" ");
        }
    }
    else if (singlePlayerGame)
    {
        pos = hrSinglePlayerPos;

        // progress bar
        if (pos.x0 != 0)
        {
            percent = horseracestatus.percent[0];

            //dbgMessagef("percent %f",percent);

            hrBarDraw(&pos, colBlack, hrSinglePlayerLoadingBarColor/*teColorSchemes[0].textureColor.base*/, percent);
        }

        // blinking hyperspace destination (render every other call)
        if (++hrProgressCounter % 2 == 0)
        {
            hrBackgroundDirty = 3;  // 1 - nothing happens as decremented before rendered
                                    // 2 - background is cleared but not redrawn
                                    // 3 - 3's the charm
            
            // hyperspace destination circled in first-person view  
            #define SP_LOADING_HYPERSPACE_DEST_CIRCLE_X  115
            #define SP_LOADING_HYPERSPACE_DEST_CIRCLE_Y  342
            
            // hyperspace destination arrowed in "as the bird flies" view 
            #define SP_LOADING_HYPERSPACE_DEST_ARROWS_X  195
            #define SP_LOADING_HYPERSPACE_DEST_ARROWS_Y  134

            // NB: hrDrawFile deals with coordinate mapping

#ifdef _WIN32
            hrDrawFile("feman\\loadscreen\\ring.lif",
                SP_LOADING_HYPERSPACE_DEST_CIRCLE_X, SP_LOADING_HYPERSPACE_DEST_CIRCLE_Y
            );
            hrDrawFile("feman\\loadscreen\\arrows.lif",
                SP_LOADING_HYPERSPACE_DEST_ARROWS_X, SP_LOADING_HYPERSPACE_DEST_ARROWS_Y
            );
#else
            hrDrawFile("feman/loadscreen/ring.lif",
                SP_LOADING_HYPERSPACE_DEST_CIRCLE_X, SP_LOADING_HYPERSPACE_DEST_CIRCLE_Y
            );
            hrDrawFile("feman/loadscreen/arrows.lif",
                SP_LOADING_HYPERSPACE_DEST_ARROWS_X, SP_LOADING_HYPERSPACE_DEST_ARROWS_Y
            );
#endif
        }
    }
    else
    {
        if (hrBackgroundDirty)
        {
            outline = pos;
            outline.x0 -= 5;
            outline.x1 += 10;
            outline.y0 -= 3;
            outline.y1 = outline.y0 + fontHeight(" ")*2 - 2;

            primRectTranslucent2(&outline, colRGBA(0,0,0,64));
            primRectOutline2(&outline, 1, teColorSchemes[0].textureColor.detail);

            fontPrintf(pos.x0,pos.y0,teColorSchemes[0].textureColor.base,"%s",playerNames[0]);
        }

        pos.y0+=fontHeight(" ");
        pos.y1=pos.y0+4;

        percent = horseracestatus.percent[0];

        hrBarDraw(&pos,hrBackBarColor,teColorSchemes[0].textureColor.base,percent);
    }
    fontMakeCurrent(currentfont);
}

void hrDrawChatBox(featom *atom, regionhandle region)
{
    fonthandle currentfont;
    sdword     x,y,i;
    char       name[128];

    hrChatBoxRegion = region;

    currentfont = fontMakeCurrent(playernamefont);

    primRectSolid2(&region->rect,colRGB(0,0,0));
    feStaticRectangleDraw(region);

    x = region->rect.x0+10;
    y = region->rect.y0;

    for (i=0;i<NUM_CHAT_LINES;i++)
    {
        if (chathistory[i].message[0]!=0)
        {
            x = region->rect.x0;
            sprintf(name,"%s >",tpGameCreated.playerInfo[chathistory[i].packetheader.frame].PersonalName);
            fontPrintf(x,y,tpGameCreated.playerInfo[chathistory[i].packetheader.frame].baseColor,"%s",name);
            x+=fontWidth(name)+10;
            //fontShadowSet(FS_E | FS_SE | FS_S);
            fontPrintf(x,y,hrChatTextColor,"%s",chathistory[i].message);
            //fontShadowSet(FS_NONE);
            y+= fontHeight(" ");
        }
    }

    fontMakeCurrent(currentfont);
}

void hrChooseSinglePlayerBitmap(char* pFilenameBuffer)
{
    char fname[128];
    sdword x, y, width, height;

    memset(&hrSinglePlayerPos, 0, sizeof(hrSinglePlayerPos));

    //image itself
#if defined(HW_GAME_RAIDER_RETREAT)
    if (spGetCurrentMission() == MISSION_5B_TURANIC_RAIDER_PLANETOID)
    {
#ifdef _WIN32
        sprintf(fname, "SinglePlayer\\mission05_OEM\\loading.jpg");
#else
        sprintf(fname, "SinglePlayer/mission05_OEM/loading.jpg");
#endif
    }
    else
#endif
    {
#ifdef _WIN32
        sprintf(fname, "SinglePlayer\\mission%02d\\loading.jpg", spGetCurrentMission());
#else
        sprintf(fname, "SinglePlayer/mission%02d/loading.jpg", spGetCurrentMission());
#endif
    }

    if (!fileExists(fname, 0))
    {
        pFilenameBuffer[0] = '\0';
        return;
    }

    strcpy(pFilenameBuffer, fname);

    // loading bar position
    // NB: the single player loading images are not all aligned properly...
    #define SP_LOADING_IMAGE_PROGRESS_BAR_X        43
    #define SP_LOADING_IMAGE_PROGRESS_BAR_Y       133
    #define SP_LOADING_IMAGE_PROGRESS_BAR_WIDTH   151
    #define SP_LOADING_IMAGE_PROGRESS_BAR_HEIGHT    2

    x      = hrScaleMissionLoadingScreens
           ? feResRepositionScaledX (SP_LOADING_IMAGE_PROGRESS_BAR_X)
           : feResRepositionCentredX(SP_LOADING_IMAGE_PROGRESS_BAR_X);
      
    y      = hrScaleMissionLoadingScreens
           ? feResRepositionScaledY (SP_LOADING_IMAGE_PROGRESS_BAR_Y)
           : feResRepositionCentredY(SP_LOADING_IMAGE_PROGRESS_BAR_Y);
    
    width  = hrScaleMissionLoadingScreens
           ? (SP_LOADING_IMAGE_PROGRESS_BAR_WIDTH  * FE_SCALE_TO_FIT_FACTOR_RELIC_SCREEN)
           :  SP_LOADING_IMAGE_PROGRESS_BAR_WIDTH;
    
    height = hrScaleMissionLoadingScreens
           ? (SP_LOADING_IMAGE_PROGRESS_BAR_HEIGHT * FE_SCALE_TO_FIT_FACTOR_RELIC_SCREEN)
           :  SP_LOADING_IMAGE_PROGRESS_BAR_HEIGHT;
    
    hrSinglePlayerPos.x0 = x;
    hrSinglePlayerPos.y0 = y;
    hrSinglePlayerPos.x1 = x + width;
    hrSinglePlayerPos.y1 = y + height;
}

void hrChooseRandomBitmap(char *pFilenameBuffer)
{
#ifdef _WIN32
    struct _finddata_t  FindData;
    long hFind;
#else
    DIR *dp;
    struct dirent* dir_entry;
    FILE* fp;
#endif
    filehandle handle;
    long userScreenShotCount = 0, BigFileCount = 0,
         chosenFileIndex = 0, currentFileIndex = 0, Result;
    char BigName[PATH_MAX], CurDir[PATH_MAX], NewDir[PATH_MAX];

    // Remember the current directory
    getcwd(CurDir, PATH_MAX);

    NewDir[0] = 0;
    strcpy(NewDir, filePathPrepend("ScreenShots", FF_UserSettingsPath));

    // Prefer user screenshots over pre-saved ones; they're more likely to be 
    // at a higher resolution, not to mention more interesting...

    // Switch to the screenshots directory and count the ones in there
    /*SetCurrentDirectory(NewDir);*/
    chdir(NewDir);
#ifdef _WIN32
    hFind = _findfirst("*.jpg", &FindData);
    if(hFind != -1)
    {
        do {
            if( ((FindData.attrib & _A_SUBDIR) == 0) &&
                ((FindData.attrib & _A_HIDDEN) == 0) )
                userScreenShotCount++;
        } while (_findnext(hFind, &FindData) == 0);
        _findclose(hFind);
    }
#else
    dp = opendir(".");

    if (dp)
    {
        unsigned int dir_str_len;

        while ((dir_entry = readdir(dp)))
        {
            if (dir_entry->d_name[0] == '.')
                continue;
            dir_str_len = strlen(dir_entry->d_name);
            if (dir_str_len < 4 ||
                strcasecmp(dir_entry->d_name + dir_str_len - 4, ".jpg"))
                continue;

            /* See if the current process can actually open the file (simple
               check for read permissions and if it's a directory). */
            if (!(fp = fopen(dir_entry->d_name, "rb")))
                continue;
            fclose(fp);

            userScreenShotCount++;
        }

        closedir(dp);
    }
#endif

    if (userScreenShotCount > 0)
    {
        chosenFileIndex = (utyTimerLast % 32777) % userScreenShotCount;

#ifdef _WIN32
        hFind = _findfirst("*.jpg", &FindData);
        if(hFind != -1)
        {
            do {
                if( ((FindData.attrib & _A_SUBDIR) == 0) &&
                    ((FindData.attrib & _A_HIDDEN) == 0) )
                {
                    if(currentFileIndex == chosenFileIndex)
                    {
                        _findclose(hFind);
                        strcpy(pFilenameBuffer, filePathPrepend("ScreenShots\\", FF_UserSettingsPath));
                        strcat(pFilenameBuffer, FindData.name);
                        break;
                    }
                    else
                        currentFileIndex++;
                }
            } while (_findnext(hFind, &FindData) == 0);
        }
#else
        dp = opendir(".");

        if (dp)
        {
            unsigned int dir_str_len;

            while ((dir_entry = readdir(dp)))
            {
                if (dir_entry->d_name[0] == '.')
                    continue;
                dir_str_len = strlen(dir_entry->d_name);
                if (dir_str_len < 4 ||
                    strcasecmp(dir_entry->d_name + dir_str_len - 4, ".jpg"))
                    continue;
                if (!(fp = fopen(dir_entry->d_name, "rb")))
                    continue;
                fclose(fp);

                if (currentFileIndex != chosenFileIndex)
                {
                    currentFileIndex++;
                    continue;
                }

                strcat(pFilenameBuffer, filePathPrepend("ScreenShots/", FF_UserSettingsPath));
                strcat(pFilenameBuffer, dir_entry->d_name);
                
                break;
            }

            closedir(dp);
        }
#endif
    }
    else // look in the big file for fallback images
    {   
        // First, find screen shots listed in the BigFile
#ifdef _WIN32
        handle = fileOpen("ScreenShots\\ShotList.script", FF_ReturnNULLOnFail | FF_TextMode);
#else
        handle = fileOpen("ScreenShots/ShotList.script", FF_ReturnNULLOnFail | FF_TextMode);
#endif
        if(handle)
        {
            do {
                Result = fileLineRead(handle, BigName, 512);

                if(Result != FR_EndOfFile && Result > 0)    // Found one!
                    BigFileCount++;

            } while(Result != FR_EndOfFile);

            fileClose(handle);
        }
        
        if (BigFileCount > 0)
        {
            chosenFileIndex = (utyTimerLast % 32777) % BigFileCount;

#ifdef _WIN32
            handle = fileOpen("ScreenShots\\ShotList.script", FF_ReturnNULLOnFail | FF_TextMode);
#else
            handle = fileOpen("ScreenShots/ShotList.script", FF_ReturnNULLOnFail | FF_TextMode);
#endif
            if(handle)
            {
                do {
                    Result = fileLineRead(handle, BigName, 512);

                    if(Result != FR_EndOfFile && Result > 0)    // Found one!
                        chosenFileIndex--;

                } while( (chosenFileIndex >= 0) && (Result != FR_EndOfFile));
#ifdef _WIN32
                strcpy(pFilenameBuffer, "ScreenShots\\");
#else
                strcpy(pFilenameBuffer, "ScreenShots/");
#endif
                strcat(pFilenameBuffer, BigName);
            }
        }
    }

    // Did we find any at all?
    if(userScreenShotCount == 0 && BigFileCount == 0)
    {
        pFilenameBuffer[0] = 0;
    }

    /*SetCurrentDirectory(CurDir);*/
    chdir(CurDir);
}

typedef struct
{
    unsigned char v[2][2];
} ColorQuad;


//
// Takes X & Y values normalized from 0 to 1
// returns a bilinear interpolated pixel value
//
unsigned char hrBilinear(ColorQuad *pQuad, double X, double Y)
{
    float Top, Bot, Final;
    long FinalLong;

    Top = (float)pQuad->v[0][0] + ((float)pQuad->v[1][0] - (float)pQuad->v[0][0]) * X;
    Bot = (float)pQuad->v[0][1] + ((float)pQuad->v[1][1] - (float)pQuad->v[0][1]) * X;

    Final = (Top + (Bot - Top) * Y) + 0.5f;

    FinalLong = (long)Final;

    // Clamp it to 0 - 255
    if(FinalLong > 255) FinalLong = (FinalLong >> 31) & 0xff;
    return (unsigned char)FinalLong;
}

unsigned long hrGetInterpPixel(unsigned char *pSrcImg, long XSize, float Xf, float Yf)
{
    unsigned long Result, x, y, xx, yy;
    unsigned char *pCol;
    double xfrac, yfrac, xint, yint;
    ColorQuad r, g, b;

    x = (unsigned long)Xf;
    y = (unsigned long)Yf;

    xfrac = modf(Xf, &xint);
    yfrac = modf(Yf, &yint);

    for(yy=0; yy<2; yy++)
    {
        for(xx=0; xx<2; xx++)
        {
            pCol = &pSrcImg[ (((y+yy) * XSize) + x+xx) * 3 ];
            r.v[xx][yy] = pCol[0];
            g.v[xx][yy] = pCol[1];
            b.v[xx][yy] = pCol[2];
        }
    }

    Result  = (unsigned long)hrBilinear(&r, xfrac, yfrac);
    Result |= (unsigned long)hrBilinear(&g, xfrac, yfrac) << 8;
    Result |= (unsigned long)hrBilinear(&b, xfrac, yfrac) << 16;
    Result |= 0xff000000;

    return Result;
}


long hrShipsToLoadForRace(ShipRace shiprace)
{
    long ShipsToLoad = 0;
    ShipType shiptype;
    ShipType firstshiptype;
    ShipType lastshiptype;
    ShipStaticInfo *shipstaticinfo;

    firstshiptype = FirstShipTypeOfRace[shiprace];
    lastshiptype = LastShipTypeOfRace[shiprace];
    for (shiptype=firstshiptype;shiptype<=lastshiptype;shiptype++)
    {
        shipstaticinfo = GetShipStaticInfo(shiptype,shiprace);

        if( bitTest(shipstaticinfo->staticheader.infoFlags, IF_InfoNeeded) && !bitTest(shipstaticinfo->staticheader.infoFlags, IF_InfoLoaded) )
            ShipsToLoad++;
    }

    return ShipsToLoad;
}


static bool hrDrawPixelsSupported(void)
{
    extern bool mainNoDrawPixels;

    if (mainNoDrawPixels)
    {
        //commandline option
        return FALSE;
    }
    if (mainSafeGL)
    {
        //running in "safe mode"
        return FALSE;
    }
    if (bitTest(gDevcaps2, DEVSTAT2_NO_DRAWPIXELS))
    {
        //bit in devcaps2
        return FALSE;
    }
    else
    {
        //assume support
        return TRUE;
    }
}

void hrInitBackground(void)
{
    char CurDir[PATH_MAX], NewDir[PATH_MAX];
    char hrImageName[PATH_MAX];
    filehandle handle;
    JPEGDATA    jp;

    udword imageWidth, imageHeight, i;
    udword pixelX, pixelY;
    udword screenWidth, screenHeight, Size, Top, Bottom;
    udword scaledImageGapSizeX, scaledImageGapSizeY;

    udword *pDest;
    unsigned char *pTempImage, *pTempLine, *pRGB;

    real32 subPixelX, subPixelY, scaleFactor;
    real32 subPixelIncrement;
    
    // used when scanning across the image for rescaling
    bool interpolatingImage    = FALSE;    // at screen position that rescaled image covers
    bool interpolatedImageLine = FALSE;    // used image information during horizontal scan line

    /*GetCurrentDirectory(511, CurDir);*/
    getcwd(CurDir, PATH_MAX);

    hrImageName[0] = 0;
    if (singlePlayerGame)
    {
        hrChooseSinglePlayerBitmap(hrImageName);
    }
    else
    {
        hrChooseRandomBitmap(hrImageName);
    }

    /*GetCurrentDirectory(511, NewDir);*/
    getcwd(NewDir, PATH_MAX);

    dbgAssertOrIgnore(strcasecmp(CurDir,NewDir) == 0);

    // Load the bitmap image
    handle = fileOpen(hrImageName, FF_ReturnNULLOnFail|FF_IgnorePrepend);
    if (handle)
    {
        memset(&jp, 0, sizeof(jp));
        jp.input_file = handle;
        JpegInfo(&jp);

        fileSeek(handle, 0, SEEK_SET);

        imageWidth = jp.width;
        imageHeight = jp.height;

        pTempImage = (unsigned char *)memAllocAttempt((imageWidth+1) * (imageHeight+1) * 3, "BackgroundTemp", NonVolatile);
        if (pTempImage == NULL)
        {
            return;
        }
        
        jp.ptr = pTempImage;
        JpegRead(&jp);

        fileClose(handle);

        Size = imageWidth*3;
        pTempLine = (unsigned char *)malloc(Size);
        for(i=0; i<(imageHeight/2); i++)
        {
            Top    = i;
            Bottom = (imageHeight - 1) - i;
 
            memcpy(pTempLine, pTempImage + (Size * Top), Size);
            memcpy(pTempImage + (Size * Top),   pTempImage + (Size * Bottom), Size);
            memcpy(pTempImage + (Size * Bottom), pTempLine, Size);
        }
        free(pTempLine);

        // Replicate the last line to appease the filter algorithm
        memcpy(&pTempImage[imageHeight * imageWidth * 3], &pTempImage[(imageHeight-1) * imageWidth * 3], imageWidth*3);

        if (!hrScaleMissionLoadingScreens
        ||  !hrDrawPixelsSupported())
        {
            //no DrawPixels support, must use glcompat 640x480 quilting
            screenWidth  = 640;
            screenHeight = 480;
            hrBackgroundImage = (udword*)malloc(screenWidth * screenHeight * 4);
        }
        else
        {
            scaleFactor = 1.1f;
            do {
                scaleFactor -= 0.1f;
                screenWidth  = (udword)((real32)MAIN_WindowWidth  * scaleFactor);
                screenHeight = (udword)((real32)MAIN_WindowHeight * scaleFactor);

                hrBackgroundImage = (udword *)malloc(screenWidth * screenHeight * 4);
            } while((hrBackgroundImage == NULL) && (scaleFactor > 0.4f));
        }

        // if the memory was not succesfully allocated
        if (hrBackgroundImage == NULL)
        {
            memFree(pTempImage);
            return;
        }

        // scale (not stretch) the image to fit the current display size
        scaleFactor = (hrScaleMissionLoadingScreens
                       || imageWidth  > screenWidth
                       || imageHeight > screenHeight)
                    ? FE_SCALE_TO_FIT_FACTOR(screenWidth, screenHeight, imageWidth, imageHeight)
                    : 1;
                          
        subPixelIncrement = 1 / scaleFactor;

        scaledImageGapSizeX
            = (screenWidth  - (imageWidth  * scaleFactor)) / 2;
        scaledImageGapSizeY
            = (screenHeight - (imageHeight * scaleFactor)) / 2;

        pDest = (udword*)hrBackgroundImage;
        
        if (pDest != NULL)
        {
            subPixelY = 0.0f;
            for (pixelY = 0; pixelY < screenHeight; pixelY++)
            {
                interpolatedImageLine = FALSE;

                subPixelX = 0.0f;
                for (pixelX = 0; pixelX < screenWidth; pixelX++)
                {
                    interpolatingImage = TRUE;
            
                    // blank area that a proportional resize won't cover
                    if (pixelX < (scaledImageGapSizeX)
                    ||  pixelY < (scaledImageGapSizeY)
                    ||  pixelX > (screenWidth  - scaledImageGapSizeX)
                    ||  pixelY > (screenHeight - scaledImageGapSizeY))
                    {
                        interpolatingImage = FALSE;
                        
                        pDest[pixelX] = 0xff000000;  // AGBR (black)
                    }
                    
                    // upscaling image (common case)
                    else if (subPixelIncrement < 1.0f)
                    {
                        interpolatedImageLine = TRUE;

                        pDest[pixelX] = hrGetInterpPixel(pTempImage, imageWidth, subPixelX, subPixelY);
                    }
                    
                    // downscaling image (direct pixel sampling so won't be particularly smooth...) 
                    else
                    {
                        interpolatedImageLine = TRUE;

                        pRGB = &pTempImage[(((unsigned long)subPixelY * imageWidth) + (unsigned long)subPixelX) * 3 ];

                        // RGBA -> ABGR
                        pDest[pixelX] = 0xff000000                       // A
                                      + ((unsigned long)pRGB[2] << 16)   // B
                                      + ((unsigned long)pRGB[1] << 8 )   // G
                                      + ((unsigned long)pRGB[0]      );  // R
                    }

#if FIX_ENDIAN
                    pDest[pixelX] = FIX_ENDIAN_INT_32( pDest[pixelX] );
#endif

                    if (interpolatingImage)
                    {
                        subPixelX += subPixelIncrement;
                    }
                }
                
                if (interpolatedImageLine)
                {
                    subPixelY += subPixelIncrement;
                }

                pDest += screenWidth;
            }
        }

        hrBackXSize = screenWidth;
        hrBackYSize = screenHeight;
        
        memFree(pTempImage);
    }
}

#define COORD(S,T,X,Y) \
    glTexCoord2f(S, T); \
    glVertex2f(primScreenToGLX(X), primScreenToGLY(Y));
void hrRectSolidTextured2(rectangle *rect)
{
    glColor3ub(255, 255, 255);

    rndTextureEnvironment(RTE_Replace);
    rndTextureEnable(TRUE);

    glBegin(GL_QUADS);
    COORD(0.0f, 0.0f, rect->x0, rect->y0);
    COORD(0.0f, 1.0f, rect->x0, rect->y1 - 1);
    COORD(1.0f, 1.0f, rect->x1, rect->y1 - 1);
    COORD(1.0f, 0.0f, rect->x1, rect->y0);
    glEnd();

    rndTextureEnable(FALSE);
    rndTextureEnvironment(RTE_Modulate);
}
#undef COORD

//don't mind if this is inefficient as it only
//gets called once per image anyway
void hrDrawFile(char* filename, sdword x, sdword y)
{
    udword handle;
    rectangle rect;
    lifheader* lif = trLIFFileLoad(filename, Pyrophoric);

    rndTextureEnable(TRUE);
    rndAdditiveBlends(FALSE);
    glEnable(GL_BLEND);

    glGenTextures(1, &handle);
    trClearCurrent();
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lif->width, lif->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, lif->data);
    
    // FIXME: there's no LiF scaling here, just translation of the image
    x = hrScaleMissionLoadingScreens
      ? feResRepositionScaledX(x)
      : feResRepositionCentredX(x);
      
    y = hrScaleMissionLoadingScreens
      ? feResRepositionScaledY(y)
      : feResRepositionCentredY(y);
    
    x -= lif->width  >> 1;
    y -= lif->height >> 1;
    rect.x0 = x;
    rect.y0 = y;
    rect.x1 = x + lif->width;
    rect.y1 = y + lif->height;
    hrRectSolidTextured2(&rect);

    glDeleteTextures(1, &handle);
    memFree(lif);

    glDisable(GL_BLEND);
}

void hrDrawBackground(void)
{
    real32 x, y;

    rndClearToBlack();

    // Draw the cached background bitmap using glDrawPixels
    if (hrBackgroundImage)
    {
        x = -((real32)hrBackXSize / (real32)MAIN_WindowWidth);
        y = -((real32)hrBackYSize / (real32)MAIN_WindowHeight);

        rndTextureEnable(FALSE);
        rndLightingEnable(FALSE);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glRasterPos2f(x, y);
        if (hrDrawPixelsSupported())
        {
            glDrawPixels(hrBackXSize, hrBackYSize, GL_RGBA, GL_UNSIGNED_BYTE, hrBackgroundImage);
        }
    }
}

void hrShutdownBackground(void)
{
    // free the memory required by the cached background bitmap

    if (hrBackgroundImage != NULL)
    {
        free(hrBackgroundImage);
        hrBackgroundImage = NULL;
    }
    hrBackgroundInitFrame = 0;
    hrBackgroundReinit = FALSE;
}

void hrAbortLoadingYes(char *name, featom *atom)
{
    hrAbortLoadingGame = TRUE;

    if (multiPlayerGame)
        SendDroppingOutOfLoad(sigsPlayerIndex);

    if (hrAbortLoadConfirm)
    {
        feScreenDelete(hrAbortLoadConfirm);
        hrAbortLoadConfirm = NULL;
    }
}

void hrAbortLoadingNo(char *name, featom *atom)
{
    if (hrAbortLoadConfirm)
    {
        feScreenDelete(hrAbortLoadConfirm);
        hrAbortLoadConfirm = NULL;
    }
}

void hrChatTextEntry(char *name, featom *atom)
{
    char *string;
    ChatPacket temp;
    sdword     width;
    fonthandle fhsave;
    char  testwidth[MAX_CHATSTRING_LENGTH+40];

    if (FEFIRSTCALL(atom))
    {
        // initialize button here
        ChatTextEntryBox = (textentryhandle)atom->pData;
        uicTextEntryInit(ChatTextEntryBox,UICTE_NoLossOfFocus|UICTE_ChatTextEntry);
        uicTextBufferResize(ChatTextEntryBox,MAX_CHATSTRING_LENGTH-2);
        return;
    }

    switch (uicTextEntryMessage(atom))
    {
        case CM_AcceptText :
            string = ((textentryhandle)atom->pData)->textBuffer;
            sendChatMessage(ALL_PLAYER_MASK^PLAYER_MASK(sigsPlayerIndex),string,(uword)sigsPlayerIndex);
            dbgMessagef("text entry: %s",string);
            strcpy(temp.message,string);
            temp.packetheader.frame = (uword)sigsPlayerIndex;
            hrProcessPacket((struct ChatPacket *)&temp);
            uicTextEntrySet(ChatTextEntryBox,"",0);
        break;
        case CM_KeyPressed :
            fhsave = fontMakeCurrent(((textentryhandle)atom->pData)->currentFont); //select the appropriate font
            sprintf(testwidth, "%s >  %s", playerNames[sigsPlayerIndex], ((textentryhandle)atom->pData)->textBuffer);
            width = fontWidth(testwidth);
            fontMakeCurrent(fhsave);
            if (width > (atom->width-30))
            {
                uicBackspaceCharacter((textentryhandle)atom->pData);
            }
        break;
    }
}

HorseRaceBars horseBarInfo;
extern bool8 etgHasBeenStarted;

void horseGetNumBars(HorseRaceBars *horsebars)
{
    sdword i;
    bool enablebar[MAX_POSSIBLE_NUM_BARS];
    real32 totalperc;

    dbgAssertOrIgnore(horseTotalNumBars.numBars > 0);
    for (i=0;i<horseTotalNumBars.numBars;i++)
    {
        switch (i)
        {
            case DOWNLOADMAP_BAR:
                enablebar[i] = autodownloadmapRequired();
                break;

            case UNIVERSE_BAR:
                enablebar[i] = TRUE;
                break;

            case ETG_BAR:
                enablebar[i] = etgHasBeenStarted ? FALSE : TRUE;
                break;

            case TEXTURE1_BAR:
            case TEXTURE2_BAR:
#if TR_NIL_TEXTURE
                enablebar[i] = GLOBAL_NO_TEXTURES ? FALSE : TRUE;
#else
                enablebar[i] = TRUE;
#endif
                break;

            default:
                dbgFatalf(DBG_Loc,"You must specify whether or not to load bar %i here",i);
                break;
        }
    }

    horsebars->numBars = 0;

    // now, based on enablebar[i], selectively add bars that will be loaded:

    for (i=0;i<horseTotalNumBars.numBars;i++)
    {
        if (enablebar[i])
        {
            horsebars->perc[horsebars->numBars++] = horseTotalNumBars.perc[i];
        }
    }

    dbgAssertOrIgnore(horsebars->numBars > 0);

    // now renormalize percentages to 1.0
    for (totalperc = 0.0f,i=0;i<horsebars->numBars;i++)
    {
        totalperc += horsebars->perc[i];
    }

    dbgAssertOrIgnore(totalperc > 0.0f);
    if (totalperc != 1.0f)
    {
        for (i=0;i<horsebars->numBars;i++)
        {
            horsebars->perc[i] /= totalperc;
        }
    }
}

void horseRaceInit()
{
    udword i;
    horseGetNumBars(&horseBarInfo);

    //initialize current bar to the 0th bar
    localbar=0;

    for(i=0;i<MAX_MULTIPLAYER_PLAYERS;i++)
    {
        horseracestatus.barnum[i] = 0;
        horseracestatus.percent[i] = 0;
        horseracestatus.hrstatusstr[i][0] = 0;
        if (autodownloadmapRequired())
            TTimerStart(&hrPlayerDropoutTimers[i],HorseRacePlayerDropoutTime*2.0f);     // give double time for autodownloads
        else
            TTimerStart(&hrPlayerDropoutTimers[i],HorseRacePlayerDropoutTime);
        PlayersAlreadyDrawnDropped[i] = FALSE;
    }

    for (i=0;i<NUM_CHAT_LINES;i++)
    {
        chathistory[i].message[0] = 0;
    }

    listInit(&horseCrapRegion.cutouts);

    JustInit = TRUE;

    if (!hrScreensHandle)
    {
        feCallbackAddMultiple(hrCallBack);
        feDrawCallbackAddMultiple(hrDrawCallback);
        hrScreensHandle = feScreensLoad(HR_FIBFile);
    }

    if (!ShouldHaveMousePtr) mouseCursorHide();

    if (singlePlayerGame)
    {
        hrBaseRegion = feScreenStart(&horseCrapRegion, HR_SingleRaceScreen);
    }
    else
    {
        hrBaseRegion = feScreenStart(&horseCrapRegion, (multiPlayerGame) ? HR_RaceScreen : HR_RaceScreenNotNetwork);
    }

    playernamefont = frFontRegister(HR_PlayerNameFont);

    hrRunning=TRUE;
    hrBackgroundDirty = 3;
    hrBackgroundReinit = FALSE;
}

void horseRaceShutdown()
{
    sdword i;

    if (!ShouldHaveMousePtr) mouseCursorShow();

    for(i=0;i<MAX_MULTIPLAYER_PLAYERS;i++)
    {
        TTimerClose(&hrPlayerDropoutTimers[i]);
    }

    hrRunning=FALSE;
    if (hrAbortLoadConfirm)
    {
        feScreenDelete(hrAbortLoadConfirm);
        hrAbortLoadConfirm = NULL;
    }

    feScreenDelete(hrBaseRegion);

    hrShutdownBackground();
    hrProgressCounter = 0;
    
    hrBaseRegion = NULL;
    hrProgressRegion = NULL;
    hrChatBoxRegion = NULL;
    ChatTextEntryBox = NULL;
}

void horseRaceWaitForNetworkGameStartShutdown()
{
    if (multiPlayerGame)
    {
        while (!multiPlayerGameUnderWay) ;
    }
}

real32 lasttime;

void HorseRaceBeginBar(uword barnum)
{
    //udword i;
    HorsePacket packet;
    lasttime = 0.0f;
//    if(utyTeaserHeader) return;

    if(JustInit)
    {
        JustInit = FALSE;
    }
    else
    {
        localbar++;
    }

    //send packet
    packet.packetheader.type = PACKETTYPE_HORSERACE;
    packet.playerindex = (uword) sigsPlayerIndex;
    packet.packetheader.numberOfCommands = 0;                    //don't need probably
    packet.barnum = (uword) localbar;
    packet.percent = horseRaceGetPacketPercent(0.0f);

    if (multiPlayerGame)
    {
        SendHorseRacePacket((ubyte *)&packet,sizeof(HorsePacket));
    }
    else
    {
        recievedHorsePacketCB((ubyte *)&packet,sizeof(HorsePacket));
    }
}

extern void mousePoll();
extern udword regRegionProcess(regionhandle reg, udword mask);
extern sdword regProcessingRegions;
extern sdword regRenderEventIndex;

void hrUncleanDecorative(void)
{
    if (hrDecRegion != NULL)
    {
        hrDecRegion->drawFunction = ferDrawDecorative;
    }
}

void horseRaceRender()
{
    SDL_Event e;

    regRenderEventIndex = 0;

    //special-case code for double-clicks

/*    if (keyIsHit(LMOUSE_DOUBLE))
    {
        keyPressUp(LMOUSE_DOUBLE);
        utyDoubleClick();
    }
    if (demDemoRecording)
    {
        memcpy((ubyte *)&keyScanCode[0], (ubyte *)&keySaveScan[0], sizeof(keyScanCode));//freeze a snapshot of the key state
        demStateSave();
    }
    else if (demDemoPlaying)
    {
        demStateLoad();
    }*/

    if (hrBackgroundReinit)
    {
        if (hrBackgroundImage != NULL)
        {
            free(hrBackgroundImage);
            hrBackgroundImage = NULL;
        }
        hrBackgroundReinit = FALSE;
        hrBackgroundDirty = 3;
        hrBackgroundInitFrame = 0;
    }

    // Make sure the Homeworld text gets drawn on the correct frames
    if (hrBackgroundDirty)
    {
        regRecursiveSetDirty(&horseCrapRegion);
        hrDecRegion = NULL;
    }
    else
    {
        regionhandle reg;

        // "Clean" the region with the homeworld logo
        // in it so it doesn't re-draw all the time

        reg = horseCrapRegion.child;
        if (reg)
        {
            reg = reg->child;
        }

        while (reg && reg->drawFunction != &ferDrawDecorative)
        {
            reg = reg->next;
        }

        if (reg)
        {
            void regNULLRenderFunction(regionhandle region);
            hrDecRegion = reg;
            reg->drawFunction = regNULLRenderFunction;
        }
    }

    if (TitanActive)
        titanPumpEngine();

    SDL_Delay(0);

    if (!SDL_PollEvent(0))
    {
        regProcessingRegions = TRUE;
        regRegionProcess(horseCrapRegion.child, 0xffffffff);
        regProcessingRegions = FALSE;
        if (ChatTextEntryBox!=NULL)
        {
            bitSet(ChatTextEntryBox->reg.status,RSF_KeyCapture);
            keyBufferClear();
        }
    }
    else
    {
        regRegionProcess(horseCrapRegion.child, 0xffffffff);
        while (SDL_PollEvent(0))
        {
            if (SDL_WaitEvent(&e))
            {
                HandleEvent(&e);

                if (multiPlayerGame)
                {
                    if (keyIsStuck(ESCKEY))
                    {
                        keyClearSticky(ESCKEY);                      //clear the sticky bit
                        if (!hrAbortLoadConfirm)
                        {
                            if (!hrAbortLoadingGame)    // if not already aborting
                            {
                                hrAbortLoadConfirm = feScreenStart(&horseCrapRegion, "AbortLoadConfirm");
                            }
                        }
                        else
                        {
                            feScreenDelete(hrAbortLoadConfirm);
                            hrAbortLoadConfirm = NULL;
                        }
                    }
                }

                regProcessingRegions = TRUE;
                if (hrAbortLoadConfirm!=NULL)
                {
                    ;
                }
                else if (ChatTextEntryBox!=NULL)
                {
                    regRegionProcess(&ChatTextEntryBox->reg, 0xffffffff);
                }
                regProcessingRegions = FALSE;
            }
            if (ChatTextEntryBox!=NULL)
            {
                bitSet(ChatTextEntryBox->reg.status,RSF_KeyCapture);
                keyBufferClear();
            }

            if (TitanActive)
                titanPumpEngine();

            SDL_Delay(0);
        }
    }

    // All of the hacked stuff from the render task

    //glColor3ub(colRed(RND_StarColor), colGreen(RND_StarColor), colBlue(RND_StarColor));

    if (ShouldHaveMousePtr)
    {
        rndClearToBlack();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

//    primErrorMessagePrint();
    //default rendering scheme is primitives on any
    //functions which want it off should set it back on when done


    // I know this looks weird, but it's correct
    if(hrBackgroundInitFrame == 1)
    {
        hrInitBackground();
    }

    // When there's no background loaded yet, it fills the screen with black
    if (hrBackgroundDirty)
    {
        hrDrawBackground();
    }
    else
    {
        if (hrBackgroundImage != NULL)
        {
            free(hrBackgroundImage);
            hrBackgroundImage = NULL;
        }
    }

    regFunctionsDraw();                                //render all regions
    primErrorMessagePrint();

    hrUncleanDecorative();

    // We want the init code to be called on the 2nd pass.  That way, the screen erases,
    // Then we incur the delay of loading the background.
    // For two frames -after that- we'll draw the background,
    // then just draw the progress bars.
    if(hrBackgroundDirty && hrBackgroundInitFrame)
    {
        if (hrBackgroundDirty > 0)
        {
            hrBackgroundDirty--;
        }
    }

    hrBackgroundInitFrame++;

    if (ShouldHaveMousePtr)
    {
        // set the cursor type, reset the variables then draw the mouse cursor
        mouseSelectCursorSetting();
        mouseSetCursorSetting();
        if (feShouldSaveMouseCursor())
        {
            mouseStoreCursorUnder();
        }
        mousePoll();
        mouseDraw();                                        //draw mouse atop everything

        if (demDemoPlaying)
        {
            rndShamelessPlug();
        }
    }

    rndFlush();
    if (ShouldHaveMousePtr)
    {
        if (feShouldSaveMouseCursor())
        {
            mouseRestoreCursorUnder();
        }
    }
    primErrorMessagePrint();
}

//function that returns the doctored percentage for conversion of
//multiple bars to a single bar.

real32 horseRaceGetPacketPercent(real32 barPercent)
{
    real32 percent=0.0f;
    sdword i;
    for(i=0;i<localbar;i++)
    {
        //add previous bar percentiles...
        percent += horseBarInfo.perc[i];
    }
    //add on current bars contribution scaled by appropriate ammount
    percent += barPercent*horseBarInfo.perc[localbar];
    return percent;
}

bool HorseRaceNext(real32 percent)
{
    Uint32 lp;
    HorsePacket packet;
    real32 temptime;
    udword i;
    sdword dontrenderhack = FALSE;
    bool sendhrpackethack = FALSE;
//    static real32 lastper = 0.0f;
    static sdword modulusCounter = 0;

    if (percent > 1.0f) percent = 1.0f;
//    if(utyTeaserHeader)  return TRUE;

    /*glClear(GL_COLOR_BUFFER_BIT);
    regFunctionsDraw();                                 //render all regions
    glFlush();                                          //ensure all draws complete*/

    if (TitanActive)
        titanPumpEngine();

    packet.packetheader.type = PACKETTYPE_HORSERACE;
    packet.playerindex = (uword) sigsPlayerIndex;
    packet.packetheader.numberOfCommands = 0;                    //don't need probably
    packet.barnum = (uword) localbar;
    //packet.current = (uword) localcurrent;
    //packet.max = (uword) localmax;

    packet.percent = horseRaceGetPacketPercent(percent);

    //render first
    if (lasttime == 0.0f)
    {
        horseRaceRender();
        dontrenderhack = TRUE;
    }

    if (multiPlayerGame && (startingGameState == AUTODOWNLOAD_MAP) && (sigsPlayerIndex == 0))
    {
        sendhrpackethack = TRUE;
        SendHorseRacePacket((ubyte *)&packet,sizeof(HorsePacket));      // always send horse race packet when autouploading map
    }

    lp = SDL_GetTicks();
    temptime = (float)lp;
    if(temptime - lasttime < horseRaceRenderTime)
    {
        if(lasttime > temptime)
            lasttime = temptime;
        return FALSE;     //don't return
    }
    lasttime = temptime;

//    if(lastper != percent)
//    {

    if (multiPlayerGame)
    {
        if (!sendhrpackethack)
        {
            if (modulusCounter <= 0)
            {
                modulusCounter = 6;
            }
            modulusCounter--;
            if (modulusCounter == 0)
            {
                SendHorseRacePacket((ubyte *)&packet,sizeof(HorsePacket));
            }
        }
    }
    else
    {
        recievedHorsePacketCB((ubyte *)&packet,sizeof(HorsePacket));
    }

//    }
//    lastper = percent;

    if (!dontrenderhack)
    {
        horseRaceRender();
    }

    if(multiPlayerGame)
    {
        for(i=0;i<sigsNumPlayers;i++)
        {
            if (i != sigsPlayerIndex)       // don't check myself
            {
                if (!playerHasDroppedOutOrQuit(i))
                {
                    if (TTimerUpdate(&hrPlayerDropoutTimers[i]))
                    {
                        PlayerDroppedOut(i,TRUE);
                    }
                }
            }
        }

        if(localbar == (horseBarInfo.numBars - 1))
        {
            for(i=0;i<sigsNumPlayers;i++)
            {
                if (!playerHasDroppedOutOrQuit(i))
                {
                    if(horseracestatus.percent[i] < 0.999f)
                    {
                        return FALSE;
                    }
                }
            }
        }
        return TRUE;
    }

    return TRUE;
}

void hrProcessPacket(struct ChatPacket *packet)
{
    sdword  i;
    bool    done=FALSE;

    for (i=0;i<NUM_CHAT_LINES;i++)
    {
        if (chathistory[i].message[0]==0)
        {
            strcpy(chathistory[i].message,(*((ChatPacket *)packet)).message);
            chathistory[i].packetheader.frame = (*((ChatPacket *)packet)).packetheader.frame;
            done = TRUE;
            break;
        }
    }

    if (!done)
    {
        for (i=0;i<NUM_CHAT_LINES-1;i++)
        {
            chathistory[i] = chathistory[i+1];
            strcpy(chathistory[i].message,chathistory[i+1].message);
        }
        strcpy(chathistory[i].message,(*((ChatPacket *)packet)).message);
        chathistory[i].packetheader.frame = (*((ChatPacket *)packet)).packetheader.frame;
    }

    hrDirtyChatBox();
}

void recievedHorsePacketCB(ubyte *packet,udword sizeofpacket)
{
    //captain should have recieved a packet here...
    HorsePacket *hp = (HorsePacket *)packet;
    udword i;

    i = hp->playerindex;
    dbgAssertOrIgnore(i < MAX_MULTIPLAYER_PLAYERS);

    if (multiPlayerGame)
    {
        titanDebug("hr: recv hrpacket from %d status %d",i,playersReadyToGo[i]);
        if (!playerHasDroppedOutOrQuit(i))
        {
            TTimerReset(&hrPlayerDropoutTimers[i]);
        }
    }

    hrDirtyProgressBar();

    horseracestatus.barnum[i] = hp->barnum;
    horseracestatus.percent[i] = hp->percent;
}

