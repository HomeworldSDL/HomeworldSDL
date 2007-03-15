// =============================================================================
//  AIVar.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
// =============================================================================

#ifndef ___AIVAR_H
#define ___AIVAR_H

#include "Types.h"

/*=============================================================================
    Switches
=============================================================================*/

#ifdef HW_Debug

#define AI_VERBOSE_LOGGING      1               //General error checking

#else

#define AI_VERBOSE_LOGGING      0               //General error checking

#endif


// INTERFACE -------------------------------------------------------------------

#define AIVAR_LABEL_MAX_LENGTH 47

typedef struct AIVar
{
    sdword value;
    char label[AIVAR_LABEL_MAX_LENGTH+1];
} AIVar;

void aivarStartup(void);
void aivarShutdown(void);
AIVar *aivarCreate(char *label);
AIVar *aivarFind(char *label);
AIVar *aivarFindAnyFSM(char *label);
char *aivarLabelGenerate(char *label);
char *aivarLabelGet(AIVar *var);
void aivarLabelSet(AIVar *var, char *label);
void aivarDestroy(AIVar *var);
void aivarDestroyAll(void);
void aivarValueSet(AIVar *var, sdword value);
sdword aivarValueGet(AIVar *var);
void aivarSave(void);
void aivarLoad(void);
sdword AIVarToNumber(AIVar *aivar);
AIVar *NumberToAIVar(sdword number);

extern AIVar *aivRenderMainScreen;

#endif
