// =============================================================================
//  Randy.c
//  - Generic random-number generators with multiple streams
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 11/11/1997 by lmoloney
// =============================================================================

#include "Randy.h"

#include "Debug.h"
#include "SaveGame.h"

#if RANDOM_DEBUG_CALL_SEQ
    #include "File.h"
    #include "Globals.h"
    #include "Memory.h"
    #include "Universe.h"
#endif

/*=============================================================================
    Data:
=============================================================================*/
ranstream ranStream[RANDOM_NUMBER_STREAMS];

#if RANDOM_DEBUG_CALL_SEQ
    #define RANDOM_LOG_FILE            "Randylog.txt"
    #define RANDOM_LOG_BUFFER_LENGTH   512
    #define RANDOM_LOG_STRING_LENGTH   24

    typedef struct
    {
        char   file[RANDOM_LOG_STRING_LENGTH];
        sdword line;
        udword univCounter;
    }
    ranlogentry;

    bool32         ranCallerDebug = FALSE;
    bool32         ranLogCleared  = FALSE;
    sdword       ranLogIndex    = 0;
    ranlogentry *ranLogBuffer   = NULL;
#endif

/*=============================================================================
    Functions:
=============================================================================*/
/*-----------------------------------------------------------------------------
    Name        : ranRandom
    Description : Get a random number from a random number sequence.
    Inputs      : ranIndex - index of random number sequence.
                  file, line - where it was called from
    Outputs     :
    Return      : New random number.
----------------------------------------------------------------------------*/
#if RANDOM_DEBUG_CALL_SEQ
udword ranRandomFn(sdword ranIndex, char *file, sdword line)
#else
udword ranRandomFn(sdword ranIndex)
#endif
{
    long int s;
    ranstream *stream;

#if RANDOM_DEBUG_CALL_SEQ
    char *fileNameFull;
    char *truncatedFile;
    FILE *fp;

    if ((ranCallerDebug || logEnable == LOG_ON || logEnable == LOG_VERBOSE) && ranIndex == RANDOM_GAME && file != NULL)
    {
        if (ranLogBuffer == NULL)
        {
            ranLogBuffer = memAlloc(sizeof(ranlogentry) * RANDOM_LOG_BUFFER_LENGTH, "ranLogBuffer", NonVolatile);
        }
        if (ranLogIndex >= RANDOM_LOG_BUFFER_LENGTH)
        {
            fileNameFull = filePathPrepend(RANDOM_LOG_FILE, FF_UserSettingsPath);

            if (fileMakeDestinationDirectory(fileNameFull))
            {
                fp = fopen(fileNameFull, ranLogCleared ? "at" : "wt");
                if (fp != NULL)
                {
                    ranLogCleared = TRUE;
                    for (ranLogIndex = 0; ranLogIndex < RANDOM_LOG_BUFFER_LENGTH; ranLogIndex++)
                    {
                        fprintf(fp, "%s(%d): %d\n", ranLogBuffer[ranLogIndex].file, ranLogBuffer[ranLogIndex].line, ranLogBuffer[ranLogIndex].univCounter);
                    }
                    fclose(fp);
                }
            }
            ranLogIndex = 0;
        }
        truncatedFile = file + strlen(file) - (RANDOM_LOG_STRING_LENGTH - 1);
        strcpy(ranLogBuffer[ranLogIndex].file, max(file, truncatedFile));
        ranLogBuffer[ranLogIndex].line = line;
        ranLogBuffer[ranLogIndex].univCounter = universe.univUpdateCounter;
        ranLogIndex++;
    }
#endif
    dbgAssertOrIgnore(ranIndex < RANDOM_NUMBER_STREAMS);
    stream = &ranStream[ranIndex];
    if(stream->y>stream->x+stream->c){s=stream->y-(stream->x+stream->c);stream->c=0;}
    else {s=(stream->x+stream->c)-stream->y-18;stream->c=1;}
    stream->x=stream->y; stream->y=stream->z;
    return ((stream->z=s) + (stream->n=69069*stream->n+1013904243));
}

/*-----------------------------------------------------------------------------
    Name        : ranRandomFnSimple
    Description : A special version of the random number generator with no file
                    or line parmeters
    Inputs      :ranIndex - index of random number sequence.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
#if RANDOM_DEBUG_CALL_SEQ
udword ranRandomFnSimple(sdword ranIndex)
{
    return(ranRandomFn(ranIndex, NULL, 0));
}
#endif
/*-----------------------------------------------------------------------------
    Name        : ranNumberGet
    Description : Get position in random-number sequence.
    Inputs      : ranIndex - index of random number sequence.
    Outputs     :
    Return      : Current random stream position for this stream.
----------------------------------------------------------------------------*/
udword ranNumberGet(sdword ranIndex)
{
    dbgAssertOrIgnore(ranIndex < RANDOM_NUMBER_STREAMS);
    return(ranStream[ranIndex].n);
}

/*-----------------------------------------------------------------------------
    Name        : ranNumberSet
    Description : Set position in random-number sequence.
    Inputs      : ranIndex - index of random number sequence.
                  nn - random number to start at.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ranNumberSet(sdword ranIndex, udword nn)
{
    dbgAssertOrIgnore(ranIndex < RANDOM_NUMBER_STREAMS);
    ranStream[ranIndex].n=nn;
}

/*-----------------------------------------------------------------------------
    Name        : ranParametersSet
    Description : Set the parameters used to define a random-number sequence.
                    These will have defaults.
    Inputs      : ranIndex - index of random number sequence.
                  xx, yy, zz, cc, nn - parameters for the random stream
    Outputs     : stores parameters for later usage
    Return      : void
----------------------------------------------------------------------------*/
void ranParametersSet(sdword ranIndex, udword xx,udword yy,udword zz,udword cc,udword nn)
{
    dbgAssertOrIgnore(ranIndex < RANDOM_NUMBER_STREAMS);
    ranStream[ranIndex].x=xx; ranStream[ranIndex].y=yy; ranStream[ranIndex].z=zz; ranStream[ranIndex].c=cc; ranStream[ranIndex].n=nn;
}

/*-----------------------------------------------------------------------------
    Name        : ranParametersGet
    Description : Get the parameters used to define a random-number sequence.
    Inputs      : ranIndex - index of random number sequence.
    Outputs     : xx, yy, zz, cc, nn - parameters for the random stream
    Return      : void
----------------------------------------------------------------------------*/
void ranParametersGet(sdword ranIndex, udword *xx,udword *yy,udword *zz,udword *cc,udword *nn)
{
    dbgAssertOrIgnore(ranIndex < RANDOM_NUMBER_STREAMS);
    *xx = ranStream[ranIndex].x; *yy = ranStream[ranIndex].y; *zz = ranStream[ranIndex].z; *cc = ranStream[ranIndex].c; *nn = ranStream[ranIndex].n;
}

/*-----------------------------------------------------------------------------
    Name        : ranParametersReset
    Description : Reset a stream's parameters to default values.
    Inputs      : ranIndex - index of random number sequence.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ranParametersReset(sdword ranIndex)
{
    dbgAssertOrIgnore(ranIndex < RANDOM_NUMBER_STREAMS);
    ranStream[ranIndex].x=521288629;
    ranStream[ranIndex].y=362436069;
    ranStream[ranIndex].z=1613801;
    ranStream[ranIndex].c=1;
    ranStream[ranIndex].n=1131199209;
}

/*-----------------------------------------------------------------------------
    Name        : ranRandomize
    Description : randomizes the number stream based on system time
    Inputs      : ranIndex - index of random number sequence.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ranRandomize(sdword ranIndex)
{
    Uint32 curr_ticks;
    udword iter;
    udword i;

    curr_ticks = SDL_GetTicks();

    iter = curr_ticks & 255;

    for (i=0;i<iter;i++)
    {
        (void)ranRandom(ranIndex);      // call a "random" amount of times based on current time
    }
}

/*-----------------------------------------------------------------------------
    Name        : ranStartup
    Description : Startup random module
    Inputs      :
    Outputs     : inits all the random streams to default values
    Return      :
----------------------------------------------------------------------------*/
void ranStartup(void)
{
    sdword ranIndex;

    for (ranIndex = 0; ranIndex < RANDOM_NUMBER_STREAMS; ranIndex++)
    {
        ranParametersReset(ranIndex);
    }
}

/*-----------------------------------------------------------------------------
    Name        : ranShutdown
    Description : Shuts down the random module, currently does nothing
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void ranShutdown(void)
{
#if RANDOM_DEBUG_CALL_SEQ
    char *fileNameFull;
    FILE *fp;
    sdword index;

    if (ranLogBuffer != NULL)
    {
        fileNameFull = filePathPrepend(RANDOM_LOG_FILE, FF_UserSettingsPath);

        if (fileMakeDestinationDirectory(fileNameFull))
        {
            fp = fopen(fileNameFull, ranLogCleared ? "at" : "wt");
            if (fp != NULL)
            {
                ranLogCleared = TRUE;
                for (index = 0; index < ranLogIndex; index++)
                {
                    fprintf(fp, "%s(%d): %d\n", ranLogBuffer[index].file, ranLogBuffer[index].line, ranLogBuffer[index].univCounter);
                }
                fclose(fp);
            }
        }
        ranLogIndex = 0;
        memFree(ranLogBuffer);
    }

#endif
    ;
}

/*=============================================================================
    Save Game stuff
=============================================================================*/

void ranSave(void)
{
    sdword ranIndex;

    for (ranIndex = 0; ranIndex < RANDOM_NUMBER_STREAMS; ranIndex++)
    {
        SaveStructureOfSize(&ranStream[ranIndex],sizeof(ranstream));
    }
}

void ranLoad(void)
{
    sdword ranIndex;

    for (ranIndex = 0; ranIndex < RANDOM_NUMBER_STREAMS; ranIndex++)
    {
        LoadStructureOfSizeToAddress(&ranStream[ranIndex],sizeof(ranstream));
    }
}

