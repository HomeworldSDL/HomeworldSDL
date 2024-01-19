// =============================================================================
//  Debug.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created June 1997 by Luke Moloney
// =============================================================================

#include "Debug.h"

#include "File.h"
#include "utility.h"

#ifdef __GNUC__
    #ifndef _STDARG_H
        #include <stdarg.h> 
    #endif
#endif

#if defined _MSC_VER
	#define  snprintf   _snprintf
	#define vsnprintf  _vsnprintf
#endif


#define DBG_ExitCode            0xfed5   // default exit code

/*=============================================================================
    Data:
=============================================================================*/
char dbgFatalErrorString[DBG_BufferLength];
sdword dbgAllowInterrupts = TRUE;

/*-----------------------------------------------------------------------------
    Name        : dbgStackDump
    Description : Dumps the stack to a file for debugging purposes.
    Inputs      : void
    Outputs     :
    Return      : filename of file written to, or NULL on error
----------------------------------------------------------------------------*/
#if DBG_STACK_CONTEXT
static char dbgStackFilename[PATH_MAX + 1];
udword dbgStackBase = 0;


char *dbgStackDump(void)
{
    udword nDwords, _ESP;
    char *blankPtr;
    struct tm *newtime;
    time_t aclock;
    FILE *fp;
    udword referenceAddress;

    if (dbgStackBase == 0)
    {
        return(NULL);
    }

    //find stack pointers
#if defined (__GNUC__) && defined (__i386__)
    __asm__ __volatile__ ( "movl %%esp, %0\n\t" : "=r" (_ESP) );
#endif

    _ESP = _ESP & (~3);                                     //round off to dword boundary
    dbgStackBase = dbgStackBase & (~3);
    nDwords = dbgStackBase - _ESP;

    //create a filename with the time in it
    time( &aclock );                                        //Get time in seconds
    newtime = localtime( &aclock );                         //Convert time to struct
    sprintf(dbgStackFilename, "stack-%s.dump", asctime(newtime));
    for (blankPtr = dbgStackFilename; *blankPtr; blankPtr++)
    {
        if (strchr(": \n\r", *blankPtr))
        {
            *blankPtr = '-';
        }
    }

    blankPtr = filePathPrepend(dbgStackFilename, FF_UserSettingsPath);
    if (!fileMakeDestinationDirectory(blankPtr))
        return NULL;

    fp = fopen(blankPtr, "wb");                             //open the dump file
    if (fp == NULL)
    {
        return(NULL);
    }
    fwrite(&_ESP, sizeof(udword), 1, fp);                   //write the stack reference
    referenceAddress = (udword)&dbgFatalf;
    fwrite(&referenceAddress, sizeof(udword), 1, fp);       //write the .text reference
    while (nDwords)
    {
        fwrite((udword *)_ESP, sizeof(udword), 1, fp);      //write the stack block
        nDwords--;
        _ESP++;
    }
    fclose(fp);                                             //close the file
    return(dbgStackFilename);
}
#endif //DBG_STACK_CONTEXT

/*-----------------------------------------------------------------------------
    Name        : dbgMessage
    Description : Post a generic message
    Inputs      : string - message to print
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void dbgMessage(char *string)
{
    /* Debug window disabled (using stdout instead, at least for now).
       dbw*() functions in other parts of code (main.c, utility.c, and
       Options.c) will need to be uncommented if the debug window is
       reenabled. */
    /*return(dbwPrint(0, string));*/
    printf("%s\n", string);
    fflush(NULL);
}

/*-----------------------------------------------------------------------------
 Name        : dbgMessagef
 Description : same as dbgMessage() but with interpolated string
 Inputs      : same as dbgMessage()
 Outputs     :
 Return      : void
 ----------------------------------------------------------------------------*/
void dbgMessagef(char *format, ...)
{
    char message[DBG_BufferLength];
    va_list argList;

    // interpolate variable arguments into formatted string
    va_start(argList, format);
    vsnprintf(message, DBG_BufferMax, format, argList);
    va_end(argList);

    dbgMessage(message);
}

/*-----------------------------------------------------------------------------
    Name        : dbgWarning
    Description : Post a warning message
    Inputs      :
        file - points to file name string
        line - line number called from
        string - output string
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void dbgWarning(char *file, sdword line, char *string)
{
    snprintf(dbgFatalErrorString, DBG_BufferMax, "\n%s (%d): Warning - %s", file, line, string);

    dbgMessage(dbgFatalErrorString);
}

/*-----------------------------------------------------------------------------
 Name        : dbgWarningf
 Description : same as dbgWarning() but with interpolated string
 Inputs      : same as dbgWarning()
 Outputs     :
 Return      : void
 ----------------------------------------------------------------------------*/
void dbgWarningf(char *file, sdword line, char *format, ...)
{
    char message[DBG_BufferLength];
    va_list argList;

    // interpolate variable arguments into formatted string
    va_start(argList, format);
    vsnprintf(message, DBG_BufferMax, format, argList);
    va_end(argList);

    dbgWarning(file, line, message);
}

/*-----------------------------------------------------------------------------
    Name        : dbgFatal
    Description : Print a fatal error message and exit program.
    Inputs      :
        file - points to file name string
        line - line number called from
        string - output string
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void dbgFatal(char *file, sdword line, char *string)
{
    char *fileName = NULL;

#if DBG_STACK_CONTEXT
    fileName = dbgStackDump();
#endif

    if (fileName) {
        snprintf(dbgFatalErrorString, DBG_BufferMax, "\n%s (%d): Fatal error - %s (Dumped to '%s')", file, line, string, fileName);
    }
    else {
        snprintf(dbgFatalErrorString, DBG_BufferMax, "\n%s (%d): Fatal error - %s", file, line, string);
    }

    dbgMessage(dbgFatalErrorString);

    if (dbgAllowInterrupts)
    {
        char *null_ptr = NULL;
        *null_ptr = 1;  // deliberate out of bounds memory assignment
    }

    utyFatalErrorWaitLoop(DBG_ExitCode);                    //exit with a MessageBox
}

/*-----------------------------------------------------------------------------
 Name        : dbgFatalf
 Description : same as dbgFatal() but with interpolated string
 Inputs      : same as dbgFatal()
 Outputs     :
 Return      : void
 ----------------------------------------------------------------------------*/
void dbgFatalf(char *file, sdword line, char *format, ...)
{
    char message[DBG_BufferLength];
    va_list argList;

    // interpolate variable arguments into formatted string
    va_start(argList, format);
    vsnprintf(message, DBG_BufferMax, format, argList);
    va_end(argList);
    
    dbgFatal(file, line, message);
}

/*-----------------------------------------------------------------------------
    Name        : dbgNonFatal
    Description : Non-fatal error handling: pops up a dialog but does not
                    exit the game.
    Inputs      : same as dbgFatal
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void dbgNonFatal(char *file, sdword line, char *string)
{
    snprintf(dbgFatalErrorString, DBG_BufferMax, "\n%s (%d): Non-fatal error - %s", file, line, string);

    dbgMessage(dbgFatalErrorString);

    if (dbgAllowInterrupts)
    {
        char *null_ptr = NULL;
        *null_ptr = 1;  // deliberate out of bounds memory assignment
    }

    utyNonFatalErrorWaitLoop();
}

/*-----------------------------------------------------------------------------
    Name        : dbgNonFatalf
    Description : same as dbgNonFatal() but with interpolated string
    Inputs      : same as dbgNonFatal()
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void dbgNonFatalf(char *file, sdword line, char *format, ...)
{
    char message[DBG_BufferLength];
    va_list argList;

    // interpolate variable arguments into formatted string
    va_start(argList, format);
    vsnprintf(message, DBG_BufferMax, format, argList);
    va_end(argList);

    dbgNonFatal(file, line, message);
}
