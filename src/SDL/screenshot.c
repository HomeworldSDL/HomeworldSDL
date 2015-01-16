// =============================================================================
//  screenshot.c
//  - take screenshots via OpenGL
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/15/1998 by khent
// =============================================================================

#include "screenshot.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <sys/mman.h>
#endif

#include "Debug.h"
#include "glinc.h"
#include "interfce.h"
#include "main.h"

#ifdef _WIN32
	#define sleep(x) _sleep((x) * 1000)
#else
    #include <unistd.h>
#endif

static void _ssAppendScreenshotFilename(char* savePath);
static void _ssSaveScreenshot(ubyte* buf);


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


static void _ssSaveScreenshot(ubyte* buf)
{
    char *fname;
    FILE* out;
    unsigned char *pTempLine;
    long Top, Bot, i, Size;

    JPEGDATA jp;

    fname = filePathPrepend("ScreenShots/", FF_UserSettingsPath);
    if (!fileMakeDirectory(fname))
        return;

    _ssAppendScreenshotFilename(fname);

#if SS_VERBOSE_LEVEL >= 1
    dbgMessagef("Saving %dx%d screenshot to '%s'.", MAIN_WindowWidth, MAIN_WindowHeight, fname);
#endif

    out = fopen(fname, "wb");
    if (out == NULL)
    {
        return;
    }

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

    // Fill out the JPG lib info structure
    memset(&jp, 0, sizeof(jp));

    jp.ptr = buf;
    jp.width = MAIN_WindowWidth;
    jp.height = MAIN_WindowHeight;
    jp.output_file = out;
    jp.aritcoding = 0;
    jp.quality = 97;

    JpegWrite(&jp);

    fclose(out);
}


void ssTakeScreenshot(void)
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
            
        _ssSaveScreenshot(screenshot_buffer);

#ifdef _WIN32
        result = VirtualFree(screenshot_buffer, 0, MEM_RELEASE);
        dbgAssertOrIgnore(result);
#else
        result = munmap(screenshot_buffer, 3*MAIN_WindowWidth*MAIN_WindowHeight);
        dbgAssertOrIgnore(result != -1);
#endif
    }
}
