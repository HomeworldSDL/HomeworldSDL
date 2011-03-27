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

#ifdef HW_BUILD_FOR_DEBUGGING
    #define DBG_ASSERT                 1   // assertion checking
    #define DBG_FILE_LINE              1   // print file and line
    #define DBG_STACK_CONTEXT          1   // dump stack context at fatal error time
    #define DBG_FATAL_DIE_NOISILY      FALSE // FALSE==use int 3, TRUE==assert(0) for debugger to catch
#else
    #define DBG_ASSERT                 0
    #define DBG_FILE_LINE              0
    #define DBG_STACK_CONTEXT          0
    #define DBG_FATAL_DIE_NOISILY      TRUE
#endif


#if DBG_FILE_LINE
    #define DBG_Loc  __FILE__, __LINE__
#else
    #define DBG_Loc  NULL, 0
#endif


#if DBG_ASSERT
    #define dbgAssertOrIgnore(expr) if (!(expr)) { dbgFatalf(DBG_Loc, "Assertion of (%s) failed.", #expr); }
    #define dbgAssertAlwaysDo(expr) dbgAssertOrIgnore(expr)
#else
    #define dbgAssertOrIgnore(expr) ((void)0)
    #define dbgAssertAlwaysDo(expr) (expr)
#endif


// maximum length of a single string for format printing
#define DBG_BufferLength        2048
#define DBG_BufferMax           (DBG_BufferLength - 1)


#if DBG_STACK_CONTEXT
    extern udword dbgStackBase;
#endif

extern char   dbgFatalErrorString[DBG_BufferLength];
extern sdword dbgAllowInterrupts;

void dbgMessage  (char *string     );
void dbgMessagef (char *format, ...);

void dbgWarning  (char *file, sdword line, char *string      );
void dbgWarningf (char *file, sdword line, char *format, ... );

void dbgFatal    (char *file, sdword line, char *string      );
void dbgFatalf   (char *file, sdword line, char *format, ... );

void dbgNonFatal (char *file, sdword line, char *string      );
void dbgNonFatalf(char *file, sdword line, char *format, ... );

#endif
