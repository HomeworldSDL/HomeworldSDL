/*=============================================================================
    Name    : rinit.c
    Purpose : rGL / OpenGL enumeration initialization routines

    Created 1/5/1999 by khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/


/*#define WIN32_LEAN_AND_MEAN*/
#define STRICT

#include "rinit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "Debug.h"
#include "StringSupport.h"
#include "Types.h"
#include "devstats.h"

extern unsigned int mainSoftwareDirectDraw;
extern unsigned int mainOutputCRC;

#define RIN_MODESINDEVSTATLIST 10
unsigned int rinDevstatModeTable[RIN_MODESINDEVSTATLIST][3] =
{
    { 16,  640, DEVSTAT_NO_64048016 },
    { 16,  800, DEVSTAT_NO_80060016 },
    { 16, 1024, DEVSTAT_NO_102476816 },
    { 16, 1280, DEVSTAT_NO_1280102416 },
    { 16, 1600, DEVSTAT_NO_1600120016 },
    { 32,  640, DEVSTAT_NO_64048032 },
    { 32,  800, DEVSTAT_NO_80060032 },
    { 32, 1024, DEVSTAT_NO_102476832 },
    { 32, 1280, DEVSTAT_NO_1280102432 },
    { 32, 1600, DEVSTAT_NO_1600120032 },
};

extern unsigned int* devTable;
extern int devTableLength;

static rdevice* rDeviceList;
static int nDevices;

unsigned int gDevcaps  = 0xFFFFFFFF;
unsigned int gDevcaps2 = 0x00000000;

#if 0	/* Direct3D stuff...not used. */
char* gDescription = NULL;
char* gDriverName = NULL;

typedef HRESULT(WINAPI * DIRECTDRAWCREATE)(GUID*, LPDIRECTDRAW*, IUnknown*);
typedef HRESULT(WINAPI * DIRECTINPUTCREATE)(HINSTANCE, DWORD, LPDIRECTINPUT*, IUnknown*);
#endif

extern unsigned int sstHardwareExists(int*);
extern unsigned int glCapNT(void);

static void* rinMemAlloc(int size)
{
    unsigned char* block;

    block = (unsigned char*)malloc(size);
    memset(block, 0, size);

    return (void*)block;
}

static void rinMemFree(void* memblock)
{
    free(memblock);
}

extern unsigned int crc32Compute(void*, unsigned int);

/*-----------------------------------------------------------------------------
    Name        : rinDeviceCRC
    Description : generate a CRC of the detected devices for detection of
                  changing hardware (light assurance that previous video
                  mode will still be working correctly)
    Inputs      :
    Outputs     :
    Return      : crc32
----------------------------------------------------------------------------*/
unsigned int rinDeviceCRC(void)
{
    rdevice* cdev;
    rdevice* devlist;
    rdevice* pdevlist;
    unsigned int crc;

    if (nDevices == 0 || rDeviceList == NULL)
    {
        return 0;
    }

    devlist = (rdevice*)rinMemAlloc(nDevices * sizeof(rdevice));
    pdevlist = devlist;

    cdev = rDeviceList;
    do
    {
        memcpy(pdevlist, cdev, sizeof(rdevice));
        pdevlist->modes = NULL;
        pdevlist->next = NULL;
        pdevlist++;
        cdev = cdev->next;
    } while (cdev != NULL);

    crc = crc32Compute((void*)devlist, nDevices * sizeof(rdevice));

    rinMemFree(devlist);

    return crc;
}

/*-----------------------------------------------------------------------------
    Name        : rinAddMode
    Description : add a display mode to a video driver
    Inputs      : dev - the device to add the mode to
                  width, height - dimensions
                  depth - bitdepth
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void rinAddMode(rdevice* dev, int width, int height, int depth)
{
    rmode* mode;
    rmode* cmode;

    mode = (rmode*)rinMemAlloc(sizeof(rmode));
    mode->width  = width;
    mode->height = height;
    mode->depth  = depth;

    if (dev->modes == NULL)
    {
        //no modes yet
        dev->modes = mode;
        mode->next = NULL;
    }
    else
    {
        //add to tail
        cmode = dev->modes;
        while (cmode->next != NULL)
        {
            cmode = cmode->next;
        }
        cmode->next = mode;
        mode->next = NULL;
    }
}

/*-----------------------------------------------------------------------------
    Name        : rinModeAccepted
    Description : decides whether a given mode is supported
    Inputs      : dev - the device in question
                  width, height, depth - display mode characteristics
    Outputs     :
    Return      : true or false
----------------------------------------------------------------------------*/
static bool rinModeAccepted(rdevice* dev, int width, int height, int depth)
{
    int index;

    //accept 16, 24, or 32 bit mode only
    if (depth != 16 && depth != 24 && depth != 32)
    {
        return FALSE;
    }

    //possibly eliminate 32bit modes, if devstats tells us to
    if (depth == 32)
    {
        if (dev->type == RIN_TYPE_DIRECT3D)
        {
            if (dev->devcaps & DEVSTAT_NO_32BIT)
            {
                return FALSE;
            }
        }
        else if (dev->type == RIN_TYPE_OPENGL)
        {
            if ((dev->devcaps & DEVSTAT_NO_32BIT_GL) ||
                (dev->devcaps & DEVSTAT_NO_32BIT))
            {
                return FALSE;
            }
        }
    }
	
    //check for specifically disabled display modes
    for (index = 0; index < RIN_MODESINDEVSTATLIST; index++)
    {
        if (rinDevstatModeTable[index][0] == depth &&
            rinDevstatModeTable[index][1] == width)
        {
            if (dev->devcaps & rinDevstatModeTable[index][2])
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : rinSortModes
    Description : sorts a device's display modes the way I like them
                  (one bitdepth at a time), and remove duplicates
    Inputs      : dev - the device whose modes are to be sorted
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void rinSortModes(rdevice* dev)
{
    rmode* cmode;
    rmode* freeMode;
    rdevice dummy;
    int depths[] = { 32, 24, 16, 0 };
    int depth;

    if (dev->modes == NULL)
    {
        return;
    }

    memset(&dummy, 0, sizeof(rdevice));

    for (depth = 0; depths[depth] != 0; depth++)
    {
        cmode = dev->modes;
        do
        {
            if (cmode->depth == depths[depth])
            {
                if (rinModeAccepted(dev, cmode->width, cmode->height, cmode->depth) &&
                    (cmode->next==NULL || cmode->next->width!=cmode->width ||
                     cmode->next->height!=cmode->height || cmode->next->depth!=cmode->depth))
                {
                    rinAddMode(&dummy, cmode->width, cmode->height, cmode->depth);
                }
            }
            cmode = cmode->next;
        } while (cmode != NULL);
    }

    //free modes on dev
    cmode = dev->modes;
    while (cmode != NULL)
    {
        freeMode = cmode;    //save to free
        cmode = cmode->next; //next
        rinMemFree(freeMode);   //free
    }

    //attach new modes
    dev->modes = dummy.modes;
}

/*-----------------------------------------------------------------------------
    Name        : rinCopyModes
    Description : copy one driver's display modes to another
    Inputs      : devOut - output device
                  devIn - input device
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void rinCopyModes(rdevice* devOut, rdevice* devIn)
{
    rmode* cmode;

    cmode = devIn->modes;
    if (cmode == NULL)
    {
        devOut->modes = NULL;
    }
    else
    {
        do
        {
            rinAddMode(devOut, cmode->width, cmode->height, cmode->depth);
            cmode = cmode->next;
        } while (cmode != NULL);
    }
}

/*-----------------------------------------------------------------------------
    Name        : rinCopyModesSelectively
    Description : copy certain display modes from one device to another
    Inputs      : devOut - output device
                  devIn - input device
                  depth - bitdepth of acceptable modes
                  maxWidth - maximum acceptable width
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void rinCopyModesSelectively(rdevice* devOut, rdevice* devIn, int depth, int maxWidth)
{
    rmode* cmode;

    cmode = devIn->modes;
    if (cmode == NULL)
    {
        devOut->modes = NULL;
    }
    else
    {
        do
        {
            if ((cmode->depth == depth) &&
                (cmode->width <= maxWidth))
            {
                rinAddMode(devOut, cmode->width, cmode->height, cmode->depth);
            }
            cmode = cmode->next;
        } while (cmode != NULL);
    }
}

/*-----------------------------------------------------------------------------
    Name        : rinAddDevice
    Description : add a device to the head of the device list
    Inputs      : dev - the device to add
    Outputs     : rDeviceList is modified
    Return      :
----------------------------------------------------------------------------*/
void rinAddDevice(rdevice* dev)
{
    rdevice* cdev;

    if (rDeviceList == NULL)
    {
        rDeviceList = dev;
        dev->next = NULL;
    }
    else
    {
        //add to head
        cdev = rDeviceList;
        dev->next = cdev;
        rDeviceList = dev;
    }
}

/*-----------------------------------------------------------------------------
    Name        : rinGetDeviceList
    Description : return pointer to device list
    Inputs      :
    Outputs     :
    Return      : rDeviceList
----------------------------------------------------------------------------*/
rdevice* rinGetDeviceList(void)
{
    return rDeviceList;
}

/*-----------------------------------------------------------------------------
    Name        : rinFreeDevices
    Description : clear the device list (free memory and such)
    Inputs      :
    Outputs     : rDeviceList and constituents are freed
    Return      :
----------------------------------------------------------------------------*/
int rinFreeDevices(void)
{
    rdevice* dev;
    rdevice* freeDev;
    rmode* mode;
    rmode* freeMode;

    //device loop
    dev = rDeviceList;
    while (dev != NULL)
    {
        //mode loop
        mode = dev->modes;
        while (mode != NULL)
        {
            freeMode = mode;    //save to free
            mode = mode->next;  //next
            rinMemFree(freeMode);  //free
        }

        freeDev = dev;          //save to free
        dev = dev->next;        //next
        rinMemFree(freeDev);       //free
    }

    rDeviceList = NULL;

    //success
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : rinMaxWidth
    Description : return max width from all modes of all devices
    Inputs      :
    Outputs     :
    Return      : max available width, or 9999 if no devices available
----------------------------------------------------------------------------*/
int rinMaxWidth(void)
{
    rdevice* cdev;
    rmode* cmode;
    int maxWidth;

    if (nDevices == 0)
    {
        return 9999;
    }

    cdev = rDeviceList;
    maxWidth = 0;
    while (cdev != NULL)
    {
        cmode = cdev->modes;
        while (cmode != NULL)
        {
            if (cmode->width > maxWidth)
            {
                maxWidth = cmode->width;
            }
            cmode = cmode->next;
        }
        cdev = cdev->next;
    }

    return (maxWidth == 0) ? 9999 : maxWidth;
}

/*-----------------------------------------------------------------------------
    Name        : rinEnumeratePrimary
    Description : enumerate available display modes on the primary display
    Inputs      : dev - the device whose modes are to be filled
    Outputs     :
    Return      : true or false (could or couldn't enumerate)
----------------------------------------------------------------------------*/
bool rinEnumeratePrimary(rdevice* dev)
{
	SDL_DisplayMode mode;
    Uint32 flags;
	int max_width;
	int i, j;
	const int display_index = 0;

	if (!dev)
		return FALSE;

	max_width = rinMaxWidth();

	/* Make sure SDL video is initialized. */
	flags = SDL_WasInit(SDL_INIT_EVERYTHING);
	if (!flags)
	{
		if (SDL_Init(SDL_INIT_VIDEO) == -1)
			return FALSE;
	}
	else if (!(flags & SDL_INIT_VIDEO))
	{
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
			return FALSE;
	}

	for(i=SDL_GetNumDisplayModes(display_index); i>0; i--){
		if(SDL_GetDisplayMode(display_index, i-1, &mode)){
			dbgMessagef("Error in SDL_GetDisplayMode(): %d %s",i-1,SDL_GetError());
			return FALSE;
		}
		if (mode.w >= 640 && mode.h >= 480)
		{
			rinAddMode(dev, mode.w, mode.h, SDL_BITSPERPIXEL(mode.format));
		}
	}
	
	return (dev->modes != 0);
}

/*-----------------------------------------------------------------------------
    Name        : rinEnumerateDevices
    Description : populate the device list by enumerating available renderers
    Inputs      :
    Outputs     : rDeviceList is filled
    Return      :
----------------------------------------------------------------------------*/
int rinEnumerateDevices(void)
{
    rdevice* dev;
    rdevice* gldev;
    rdevice  primaryDev;
    bool primaryVal;
    
#ifndef _MACOSX_FIX_GL
    int maxWidth;
#endif

#if 0	/* CRC log only used by Direct3D. */
    if (mainOutputCRC)
    {
        rinResetCRCLog();
    }
#endif

    rDeviceList = NULL;
    nDevices = 0;
    gDevcaps  = 0xFFFFFFFF;
    gDevcaps2 = 0x00000000;

    //add Direct3D devices
    /*nDevices += rinEnumerateDirect3D();*/

    //add OpenGL device
    dev = (rdevice*)rinMemAlloc(sizeof(rdevice));
    if (nDevices == 1)
    {
        dev->devcaps  = rDeviceList->devcaps;
        dev->devcaps2 = rDeviceList->devcaps2;
    }
    else if (nDevices == 2)
    {
        dev->devcaps  = rDeviceList->next->devcaps;
        dev->devcaps2 = rDeviceList->next->devcaps2;
    }
    else if (gDevcaps != 0xFFFFFFFF)
    {
        dev->devcaps  = gDevcaps;
        dev->devcaps2 = gDevcaps2;
    }
    else
    {
        dev->devcaps = gDevcaps = 0;
        dev->devcaps2 = gDevcaps2 = 0;
    }
    if (dev->devcaps & DEVSTAT_NO_DDRAWSW)
    {
        mainSoftwareDirectDraw = 0;
    }
    dev->type = RIN_TYPE_OPENGL;
    dev->data[0] = '\0';
    if (strCurLanguage == 1)
    {
        strncpy(dev->name, "OpenGL Standard", 63);
    }
    else if (strCurLanguage == 2)
    {
        strncpy(dev->name, "Standard-OpenGL", 63);
    }
    if (strCurLanguage == 3)
    {
        strncpy(dev->name, "OpenGL predeterminado", 63);
    }
    else if (strCurLanguage == 4)
    {
        strncpy(dev->name, "OpenGL di Default", 63);
    }
    else
    {
        strncpy(dev->name, "Default OpenGL", 63);
    }
    dev->modes = NULL;
    //try good old DirectDraw <= 3 enumeration
    memset(&primaryDev, 0, sizeof(rdevice));
    primaryVal = rinEnumeratePrimary(&primaryDev);
    if (primaryVal)
    {
        rinCopyModes(dev, &primaryDev);
    }
    else
    {
        //assume these are supported
        rinAddMode(dev,  640, 480, 16);
        rinAddMode(dev,  800, 600, 16);
        rinAddMode(dev, 1024, 768, 16);
    }
    rinSortModes(dev);
    gldev = dev;
    nDevices++;

    if (!(gDevcaps & DEVSTAT_NOGL_9X))
    {
        rinAddDevice(gldev);
    }

    //success
    return 1;
}
