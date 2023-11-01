// =============================================================================
//  screenshot.c
//  - take screenshots via OpenGL
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/15/1998 by khent
// =============================================================================

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "screenshot.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <sys/mman.h>
#endif

#include "Debug.h"
#include "glinc.h"
#include "File.h"
#include "main.h"

#ifdef _WIN32
	#define sleep(x) _sleep((x) * 1000)
#else
    #include <unistd.h>
#endif

static void _ssAppendScreenshotFilename(char* savePath);


// =============================================================================


static void _ssAppendScreenshotFilename(char* savePath)
{
    FILE        *imageFile;
    char         imagePath[PATH_MAX + 1],
                 imageName[256];
                 
    time_t       now;
    struct tm    timeStruct;
    
    unsigned int attempts          =  0,
                 sleep_time_secs   =  1,
                 max_attempts_secs = 10;

    while (1) {
        time(&now);
        timeStruct = *localtime(&now);
        
        strftime(imageName, sizeof(imageName), "shot_%Y%m%d_%H%M%S_%Z.jpg", &timeStruct);

        strcpy(imagePath, savePath);
        strcat(imagePath, imageName);
        
        // unable to open filename so presumably good name to use
        if ((imageFile = fopen(imagePath, "r")) == NULL) {
            break;
        }
        
        fclose(imageFile);
        attempts++;
        sleep(sleep_time_secs);    // rapid screenshots possibly: wait and try again

        // don't try for more than the allowed time
        if (attempts > max_attempts_secs / sleep_time_secs) {
            strcpy(imageName, "most_recent_screenshot.jpg");
            break;
        }
    }

    strcat(savePath, imageName);
}


static void _ssSaveScreenshot(ubyte* buf, const char* targetFilename)
{
    char *fname;
    unsigned char *pTempLine;
    long Top, Bot, i, Size;

    fname = filePathPrepend("ScreenShots/", FF_UserSettingsPath);
    if (!fileMakeDirectory(fname))
        return;

    if (!targetFilename)
    {
        _ssAppendScreenshotFilename(fname);
    } else {
        strcat(fname, targetFilename);
    }

    fileNameReplaceSlashesInPlace(fname);

#if SS_VERBOSE_LEVEL >= 1
    dbgMessagef("Saving %dx%d screenshot to '%s'.", MAIN_WindowWidth, MAIN_WindowHeight, fname);
#endif

    // Flip buffer contents vertically due to OpenGL's origin being in the lower-left corner
    Size = MAIN_WindowWidth*3;
    pTempLine = (unsigned char *)malloc(Size);

    for (i = 0; i < (MAIN_WindowHeight / 2); i++)
    {
        Top = i;
        Bot = (MAIN_WindowHeight - 1) - i;

        memcpy(pTempLine, buf + (Size * Top), Size);
        memcpy(buf + (Size * Top), buf + (Size * Bot), Size);
        memcpy(buf + (Size * Bot), pTempLine, Size);
    }
    free(pTempLine);

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(buf, MAIN_WindowWidth, MAIN_WindowHeight, 24, 3 * MAIN_WindowWidth, 0x0000FF, 0x00FF00, 0xFF0000, 0);
    
    IMG_SaveJPG(surface, fname, 100);
    
    SDL_FreeSurface(surface);
}


void ssTakeScreenshot(const char* filename)
{
    ubyte* screenshot_buffer =
#ifdef _WIN32
        (void *)VirtualAlloc(NULL, 3 * MAIN_WindowWidth * MAIN_WindowHeight,  // 3 = RGB
            MEM_COMMIT, PAGE_READWRITE);
#else
        mmap(NULL, 3 * MAIN_WindowWidth * MAIN_WindowHeight,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
#endif

    if (screenshot_buffer != NULL)
    {
        int result = 0;
        
        glReadPixels(0, 0, MAIN_WindowWidth, MAIN_WindowHeight,
            GL_RGB, GL_UNSIGNED_BYTE, screenshot_buffer);
            
        _ssSaveScreenshot(screenshot_buffer, filename);

#ifdef _WIN32
        result = VirtualFree(screenshot_buffer, 0, MEM_RELEASE);
        dbgAssertOrIgnore(result);
#else
        result = munmap(screenshot_buffer, 3*MAIN_WindowWidth*MAIN_WindowHeight);
        dbgAssertOrIgnore(result != -1);
#endif
    }
}
