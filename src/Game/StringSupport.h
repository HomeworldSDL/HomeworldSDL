// =============================================================================
//  StringSupport.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 5/7/1998 by ddunlop
// =============================================================================

#ifndef ___STRING_SUPPORT_H
#define ___STRING_SUPPORT_H

// try not to include too many files in here since StringSupport.h may be included
// by jpeg files, etc.

#include "StringsOnly.h"
#include "Types.h"

// INTERFACE -------------------------------------------------------------------

typedef enum
{
    languageEnglish,
    languageFrench,
    languageGerman,
    languageSpanish,
    languageItalian
} strLanguageType;

extern udword strCurLanguage;
extern udword strCurKeyboardLanguage;

bool8 strLoadLanguage(strLanguageType language);
bool8 strFreeLanguage(void);
void  strSetStringCB(char *directory, char *field, void *dataToFillIn);
strGamesMessages strNameToEnum(char *string);
void strSetCurKeyboard(void);

#endif
