// =============================================================================
//  screenshot.h
//  - output screen grabs to disk
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/15/1998 by khent
// =============================================================================

#ifndef ___SCREENSHOT_H
#define ___SCREENSHOT_H

#include "Key.h"
#include "Types.h"

#ifdef HW_BUILD_FOR_DEBUGGING
    #define SS_VERBOSE_LEVEL    1     // control verbose printing
#else
    #define SS_VERBOSE_LEVEL    1     // SDL prints screenshot path; was 0 for retail
#endif


#define SS_SCREENSHOT_KEY   SCROLLKEY

#ifdef _MACOSX
    // MAC OS X captures high F-keys for system functions like monitor
    // brightness and Expose etc so we also check for: 

    // F13 on extended keyboards
    #define SS_SCREENSHOT_KEY_2  PRINTKEY
    
    // key next to the 1 on the top numeric keys under esc for laptops
    // which remaps all the function keys to system use
    //#define SS_SCREENSHOT_KEY_3  PLUSMINUS
#endif


void ssTakeScreenshot(void);

#endif
