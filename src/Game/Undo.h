// =============================================================================
//  Undo.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 8/4/1997 by lmoloney
// =============================================================================

#ifndef ___UNDO_H
#define ___UNDO_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------

typedef bool (*undofunc)(sdword userID, ubyte *userData, sdword length);

typedef struct undoinfo
{
    undofunc function;        // function to call to undo
    sdword userID;            // arbitrary user ID
    ubyte *userData;          // allocated user data
    sdword length;            // length of allocated user data
} undoinfo;

void udStartup(void);
void udShutdown(void);

sdword udLatestThingPush(undofunc function, sdword userID, ubyte *userData, sdword length);
sdword udLatestThingUndo(void);

#endif
