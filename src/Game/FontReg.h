/*=============================================================================
    Name    : Fontreg.h
    Purpose : Definitions for font registry.

    Created 8/6/1997 by lmoloney
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___FONTREG_H
#define ___FONTREG_H

#include "font.h"

/*=============================================================================
    Switches:
=============================================================================*/
#ifdef HW_BUILD_FOR_DEBUGGING

#define FR_ERROR_CHECKING     1               //general error checking
#define FR_VERBOSE_LEVEL      0               //print extra info

#else

#define FR_ERROR_CHECKING     0               //general error checking
#define FR_VERBOSE_LEVEL      0               //print extra info

#endif

/*=============================================================================
    Definitions:
=============================================================================*/
//font registry parameters
#define FR_NumberFonts          32+1          // hack to prevent error on NULL
#ifdef _WIN32
#define FR_PrependPath          "fonts\\"
#define FR_English              "English\\"
#define FR_German               "German\\"
#define FR_French               "French\\"
#define FR_Spanish              "Spanish\\"
#define FR_Italian              "Italian\\"
#else
#define FR_PrependPath          "fonts/"
#define FR_English              "English/"
#define FR_German               "German/"
#define FR_French               "French/"
#define FR_Spanish              "Spanish/"
#define FR_Italian              "Italian/"
#endif

/*=============================================================================
    Type definitions:
=============================================================================*/
//font registry entry
typedef struct
{
    char *name;
    fonthandle handle;
    fontheader *fontdat;
    sdword nUsageCount;
}
fontregistry;

/*=============================================================================
    Extern's:
=============================================================================*/
extern fontregistry frFontRegistry[FR_NumberFonts];


/*=============================================================================
    Macros:
=============================================================================*/
#define frNamesCompare(n, m)  (!strcmp((n), (m)))

/*=============================================================================
    Functions:
=============================================================================*/
//startup/shutdown
void frStartup(void);
void frReset(void);
void frShutdown(void);

void frReloadGL(void);
void frReloadFonts(void);

//register a font
fonthandle frFontRegister(char *fileName);
void frFontUnregister(fonthandle font);

#endif
