// =============================================================================
//  Debug.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created June 1997 by Luke Moloney
// =============================================================================

#ifndef ___DEBUG_H
#define ___DEBUG_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------


// make sure the HW_Level environment variable is set correctly
#if !(defined(HW_Debug) || defined(HW_BUILD_FOR_DISTRIBUTION))
    #error HW_Level must be one of HW_Debug or HW_BUILD_FOR_DISTRIBUTION
#endif


#ifndef HW_BUILD_FOR_DISTRIBUTION
    #define DBG_ASSERT              1           // assertion checking
    #define DBG_FILE_LINE           1           // print file and line
    #define DBG_STACK_CONTEXT       1           // dump stack context at fatal error time
#else
    #define DBG_ASSERT              0
    #define DBG_FILE_LINE           0           // don't print file and line
    #define DBG_STACK_CONTEXT       0           // dump stack context at fatal error time
#endif


#if DBG_FILE_LINE
    #define DBG_Loc         __FILE__, __LINE__
#else
    #define DBG_Loc         NULL, 0
#endif


#if DBG_ASSERT
    #define dbgAssertOrIgnore(expr) if (!(expr)) { dbgFatalf(DBG_Loc, "Assertion of (%s) failed.", #expr); }
    #define dbgAssertAlwaysDo(expr) dbgAssertOrIgnore(expr)
#else
    #define dbgAssertOrIgnore(expr) ((void)0)
    #define dbgAssertAlwaysDo(expr) (expr)
#endif


#if DBG_STACK_CONTEXT
    extern udword dbgStackBase;
    char *dbgStackDump(void);
#endif


// maximum length of a single string for format printing
#define DBG_BufferLength        2048
#define DBG_BufferMax           (DBG_BufferLength - 1)

// message box style
#define DBG_MBCaption           "Fatal Error"
#define DBG_MBFlags             MB_ICONSTOP | MB_OK

// default exit code
#define DBG_ExitCode            0xfed5

extern char dbgFatalErrorString[DBG_BufferLength];
extern sdword dbgInt3Enabled;

sdword dbgMessage(char *string);
sdword dbgMessagef(char *format, ...);
sdword dbgWarning(char *file, sdword line, char *format);
sdword dbgWarningf(char *file, sdword line, char *format, ...);
sdword dbgFatal(char *file, sdword line, char *format);
sdword dbgFatalf(char *file, sdword line, char *format, ...);
sdword dbgNonFatal(char *file, sdword line, char *error);
sdword dbgNonFatalf(char *file, sdword line, char *format, ...);


#endif
