/*=============================================================================
    Name    : screenshot.h
    Purpose : output screen grabs to disk

    Created 7/15/1998 by khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___SCREENSHOT_H
#define ___SCREENSHOT_H

#define SS_SCREENSHOTS         1
#ifndef HW_BUILD_FOR_DISTRIBUTION

#define SS_ERROR_CHECKING      1               //basic error checking
#define SS_VERBOSE_LEVEL       1               //control verbose printing

#else

#define SS_ERROR_CHECKING      0               //no error ckecking in retail
#define SS_VERBOSE_LEVEL       1               //SDL prints screenshot path; was 0 for retail

#endif
#include "Types.h"

void ssSaveScreenshot(ubyte* buf);

#endif
