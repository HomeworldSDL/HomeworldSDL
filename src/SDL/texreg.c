/*=============================================================================
    Name    : Texreg.c
    Purpose : Maintain a texture registry

    Created 9/26/1997 by lmoloney
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "glinc.h"
#include <stdio.h>
#include <stdlib.h>
#include "Debug.h"
#include "Memory.h"
#include "Twiddle.h"
#include "prim2d.h"
#include "File.h"
#include "Key.h"
#include "texreg.h"
#include "StatScript.h"
#include "StringSupport.h"
#include "ColPick.h"
#include "render.h"

#include "main.h"
#include "utility.h"
#include "HorseRace.h"

#include "Universe.h"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

/*=============================================================================
    Data:
=============================================================================*/
//configuration information:
static bool32 trNoPalInitialized = FALSE;

sdword trTextureChanges = 0;
sdword trAvoidedChanges = 0;

//actual registry:
texreg *trTextureRegistry = NULL;
crc32 *trNameCRCs = NULL;                              //separate list to reduce cache misses during searches.
sdword trLowestFree = 0;                        //indices of extremes free texture indices
sdword trHighestAllocated = -1;

//default coloration palette
trcolorinfo trDefaultColorInfo = {colWhite, colWhite};
trcolorinfo trUnusedColorInfo = {TR_UnusedColorInfo, TR_UnusedColorInfo};
trmemmap *trSizeSortList = NULL;
sdword trSizeSortLength;
trmeshsort *trMeshSortList;
sdword trMeshSortLength;
//sdword trMeshSortSubLength;
sdword trMeshSortIndex;

//info on RAM pools for texture fitting
//!!! for now, we just say there is one pool of 4MB
trrampool trRamPoolList[TR_NumberRamPools] =
{
    {
        NULL,                                   //no real base
        1024 * 1024 * 10,                       //n MB of texture RAM
        0                                       //none used yet
    }
};
sdword trNumberRamPools = 1;                    //only one pool for now
sdword trRamPoolGranularity = 16;               //granularity - this is just a guess

#if TR_DEBUG_TEXTURES
color trTestPalette0[TR_PaletteLength];         //black-to-white palette
bool32 trSpecialTextures = FALSE;
sdword trSpecialTextureMode = TSM_None;
char *trSpecialTextureName[TSM_NumberModes] =
{"Normal", "Uncolored", "Base color buffer", "Stripe color buffer"};
#endif

sdword texLinearFiltering = TRUE;               //enable bi-linear filtering
sdword texLinearMag = TRUE;

//multiplier to the team color effect multipliers
real32 trBaseColorScalar = 0.0f;
real32 trStripeColorScalar = 0.0f;

#if TR_PRINT_TEXTURE_NAMES
bool32 trPrintTextureNames = FALSE;
#endif

trhandle trCurrentHandle = TR_Invalid;

sdword trNoPalHighestAllocated;
sdword trNoPalBytesAllocated;

static sdword trNpNumHandles = TR_NP_NumHandles;
nopalreg* trNoPalRegistry = NULL;

#define ADJ_NPQUEUE(x) ((x) % (trNoPalQueueSize - 1))

sdword  trNoPalQueueSize;
sdword  trNoPalQueueHead, trNoPalQueueTail;
udword* trNoPalQueue;

sdword trNoPalMaxBytes = 20 * 1024 * 1024;

/*=============================================================================
    Functions:
=============================================================================*/

void trNoPalReadjustWithoutPending(void);

/*-----------------------------------------------------------------------------
    Name        : trReload
    Description : fixup .DLL hooks that may have changed
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trReload(void)
{
}

/*-----------------------------------------------------------------------------
    Name        : trStartup
    Description : startup the texture registry
    Inputs      : void
    Outputs     : allocates the texture registry
    Return      :
----------------------------------------------------------------------------*/
void trStartup(void)
{
#if TR_DEBUG_TEXTURES
    sdword index;
    for (index = 0; index < TR_PaletteLength; index++)
    {
        trTestPalette0[index] = colRGB(index, index, index);
    }
#endif

#if TR_VERBOSE_LEVEL >= 1
    dbgMessagef("trStartup: creating a registry of %d entries", TR_RegistrySize);
#endif  //TR_VERBOSE_LEVEL
    dbgAssertOrIgnore(trTextureRegistry == NULL);
    trTextureRegistry = memAlloc(TR_RegistrySize * sizeof(texreg),
                                 "Texture registry", NonVolatile);//allocate texture registry
    trNameCRCs = memAlloc(TR_RegistrySize * sizeof(crc32), "texRegCRC's", NonVolatile);

    trNoPalStartup();                                       //must come before trReset
    trReset();                                              //reset the newly-allocated texture registry

    //(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

/*-----------------------------------------------------------------------------
    Name        : trReset
    Description : Reset the texture registry
    Inputs      :
    Outputs     : Deletes any textures which may be floating about.
    Return      :
----------------------------------------------------------------------------*/
static sdword bNewList = TRUE;
void trReset(void)
{
    sdword index;
    texreg *reg;

    trNoPalReset();

#if TR_VERBOSE_LEVEL >= 1
    dbgMessagef("trReset: resetting texture registry");
#endif  //TR_VERBOSE_LEVEL
    for (index = 0; index < TR_RegistrySize; index++)
    {
        if ((!bNewList) && (trAllocated(index)))
        {
            trTextureDelete(index);                         //delete the texture if it exists
        }
        //since we really only need to set the internal handle as invalid
        //to dictate that the registry entry is blank, we could leave the
        //other members in whatever state they were previously.
        //Hence, we only do this in debug mode.
        reg = trStructure(index);
        reg->flags = 0;
#if TR_ERROR_CHECKING
        reg->meshReference = NULL;
        reg->nUsageCount = 0;
        reg->diskWidth = reg->diskHeight = 0;
        reg->scaledWidth = reg->scaledHeight = 0;
        reg->fileName = NULL;
        reg->nPalettes = 0;
        reg->palettes = NULL;
#endif
        reg->nSharedTo = 0;
        reg->sharedFrom = TR_NotShared;
        trSetFree(index);
        trClearPending(index);
    }
    trLowestFree = 0;
    trHighestAllocated = -1;
    bNewList = FALSE;
    trCurrentHandle = TR_Invalid;
    memClearDword(trNameCRCs, 0, TR_RegistrySize);           //clear all CRC's to 0
}

/*-----------------------------------------------------------------------------
    Name        : trShutdown
    Description : Shut down the texture registry.
    Inputs      :
    Outputs     : Deletes any textures which may be floating about.
    Return      :
----------------------------------------------------------------------------*/
void trShutdown(void)
{
#if TR_VERBOSE_LEVEL >= 1
    dbgMessagef("trShutdown: shutting down texture registry");
#endif  //TR_VERBOSE_LEVEL
    trReset();                                              //this'll kill all the remaining textures
    memFree(trTextureRegistry);                             //free the texture registry itself
    trTextureRegistry = NULL;
    memFree(trNameCRCs);
    trNameCRCs = NULL;

    bNewList = TRUE;

    trNoPalShutdown();
}

/*-----------------------------------------------------------------------------
    Name        : trInternalTexturesDelete
    Description : Delete the internal textures of a texreg, including the
                    handle(s) and palette(s) without deleting anything else in it.
    Inputs      : handle - handle of the textures to delete
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trInternalTexturesDelete(trhandle handle)
{
    texreg *reg = trStructure(handle);
    trcolorinfo *oldColorInfos, *newColorInfos;
    sdword index;

    udword *handles;

    dbgAssertOrIgnore(!trPending(trIndex(handle)));                 //make sure it has internal textures
    //sharing handled
    if (reg->sharedFrom != TR_NotShared)
    {
        oldColorInfos = (trcolorinfo *)reg->palettes;
    }
    else if (bitTest(reg->flags, TRF_Paletted))
    {                                                       //if it's a paletted texture
        oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes);
    }
    else
    {                                                       //else's it's an RGB texture
        oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes);
    }
    newColorInfos = memAlloc(TR_NumPaletteSize, "dupe trcolorinfo list", NonVolatile);
    memcpy(newColorInfos, oldColorInfos, sizeof(trcolorinfo) * reg->nPalettes);
    if (bitTest(reg->flags, TRF_Paletted))
    {
        if (reg->handle != TR_InvalidInternalHandle)
        {
            trNoPalTextureDelete(reg->handle);
        }
    }
    else if (reg->nPalettes == 1)
    {
        if (reg->handle != TR_InvalidInternalHandle)
        {                                                   //if this texture exists
            glDeleteTextures(1, (GLuint*)&reg->handle);              //delete GL-internal texture
        }
    }
    else
    {
        handles = (udword *)reg->palettes;
        for (index = 0; index < reg->nPalettes; index++)
        {
            if (handles[index] != TR_InvalidInternalHandle)
            {
                glDeleteTextures(1, (GLuint*)&handles[index]);
                handles[index] = TR_InvalidInternalHandle;
            }
        }
    }
    reg->handle = TR_InvalidInternalHandle;                 //no handle left here
    //don't even bother killing the special textures

    //free the palette or list of texture handles
    //sharing handled
    if (reg->sharedFrom != TR_NotShared)
    {                                                       //if it's shared
        memFree(reg->palettes);
    }
    else if (bitTest(reg->flags, TRF_Paletted))
    {   //if it's a paletted texture, it shares it's palette with other textures.
        //We have a usage count, the alpha (high bit) of the first entry is our
        //usage count.
        if ((((color *)reg->palettes)[0] & 0xff000000) != 0)
        {                                                   //if the palette has a usage count
            dbgAssertOrIgnore((((color *)reg->palettes)[0] & 0xff000000) != 0xff000000);
            ((color *)reg->palettes)[0] -= 0x01000000;      //decrement the usage count
            if (((((color *)reg->palettes)[0]) & 0xff000000) == 0)
            {                                               //if usage count reaches 0
                memFree(reg->palettes);                     //free the palette
            }
        }
    }
    else
    {                                                       //else RGB: free list of texture handles
        dbgAssertOrIgnore(reg->palettes != NULL);
        memFree(reg->palettes);
    }
    reg->palettes = (ubyte *)newColorInfos;                 //save list of color infos
}

/*-----------------------------------------------------------------------------
    Name        : trAllSharedFromDelete
    Description : 'Unshare' any texture shared from the specified index.
    Inputs      : iSharedFrom - index to remove references to
                  bRemoveInternalTextures - if TRUE, the texture will have it's
                    innards freed properly.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trAllSharedFromDelete(sdword iSharedFrom, bool32 bRemoveInternalTextures)
{
    sdword index;

    for (index = 0; index <= trHighestAllocated; index++)
    {
        if (index == iSharedFrom || (!trAllocated(index)))
        {
            continue;
        }
        if (trTextureRegistry[index].sharedFrom == iSharedFrom)
        {                                                   //if this is one of the textures sharing the one we're removing references from
            //trTextureDelete(index);
            if (bRemoveInternalTextures && (!trPending(index)))
            {
                trInternalTexturesDelete(index);
            }
            trSetPending(index);                            //make this texture pending
            trTextureRegistry[index].sharedFrom = TR_NotShared;//set it to not shared
            trTextureRegistry[iSharedFrom].nSharedTo--;
            if (trTextureRegistry[iSharedFrom].nSharedTo == 0)
            {
                break;
            }
        }
    }
    dbgAssertOrIgnore(trTextureRegistry[iSharedFrom].nSharedTo == 0);
}

/*-----------------------------------------------------------------------------
    Name        : trTextureDelete
    Description : Deletes a given texture.
    Inputs      : handle - handle of the texture to delete
    Outputs     : Deletes ALL instances of the texture.  This is NOT the
                    same as trTextureUnregister.
    Return      : Old usage count
----------------------------------------------------------------------------*/
sdword trTextureDelete(trhandle handle)
{
    texreg *reg = trStructure(handle);
    sdword oldUsageCount;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return(0);
    }
#endif
#if TR_VERBOSE_LEVEL >= 2
    dbgMessagef("trTextureDelete: deleting texture handle 0x%x, internalHandle 0x%x", handle, reg->handle);
#endif  //TR_VERBOSE_LEVEL
    dbgAssertOrIgnore(trAllocated(trIndex(handle)));
    oldUsageCount = reg->nUsageCount;                       //remember usage count
    //kill the GL textures
    if (!trPending(trIndex(handle)))
    {                                                       //if this texture was loaded proper
        trInternalTexturesDelete(handle);
    }
    //sharing handled
    if (reg->sharedFrom != TR_NotShared)
    {                                                       //if this one is shared from another texture
        dbgAssertOrIgnore(reg->nSharedTo == 0);
        dbgAssertOrIgnore(trAllocated(reg->sharedFrom));
        dbgAssertOrIgnore(trTextureRegistry[reg->sharedFrom].nSharedTo > 0);
        trTextureRegistry[reg->sharedFrom].nSharedTo--;
    }
    if (reg->nSharedTo > 0)
    {                                                       //if this texture is shared to otherTextures
        trAllSharedFromDelete(trIndex(handle), FALSE);
    }

    //delete the name string
    memFree(reg->fileName);                                 //free the name of the texture
    trNameCRCs[trIndex(handle)] = 0;                        //clear this CRC to blank
    reg->flags = 0;
#if TR_ERROR_CHECKING
    reg->meshReference = NULL;
    reg->nUsageCount = 0;
    reg->diskWidth = reg->diskHeight = 0;
    reg->scaledWidth = reg->scaledHeight = 0;
    reg->fileName = NULL;
    reg->nPalettes = 0;
    reg->palettes = NULL;
#endif
    reg->nSharedTo = 0;
    reg->sharedFrom = TR_NotShared;
    trSetFree(trIndex(handle));
    trLowestFree = min(trLowestFree, (sdword)trIndex(handle));//maintain index of lowest free index
    if ((sdword)trIndex(handle) == trHighestAllocated)
    {                                                       //if deleting highest allocated
        for (; trHighestAllocated > 0; trHighestAllocated--)
        {                                                   //search for highest allocated
            if (trAllocated(trHighestAllocated))
            {                                               //if this guy allocated
                break;
            }
        }
    }
    return(oldUsageCount);
}

/*-----------------------------------------------------------------------------
    Name        : trColorInfosPointer
    Description : Get the list of colorinfo structures for a given texture.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
trcolorinfo *trColorInfosPointer(sdword textureIndex)
{
    texreg *reg = trStructure(textureIndex);

    if (trPending(textureIndex) || (reg->sharedFrom != TR_NotShared))
    {                                                   //if pending or shared
        return((trcolorinfo *)reg->palettes);
    }

    if (bitTest(reg->flags, TRF_Paletted))
    {                                                   //if it's a paletted texture
        return((trcolorinfo *)((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes));
    }
    else
    {                                                   //else's it's an RGB texture
        return((trcolorinfo *)((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes));
    }
}

/*-----------------------------------------------------------------------------
    Name        : trColorsEqual
    Description : See if there is a version of this texture with already
                    matching palette.
    Inputs      : info - color info requested for this texture
                  textureIndex - index of texture to check for matches
    Outputs     : Examines all palettes associated with this texture for a match with info.
    Return      : index of matching palette or -1 if no match
----------------------------------------------------------------------------*/
sdword trColorsEqual(trcolorinfo *info, sdword textureIndex)
{
    sdword index;
    trcolorinfo *textureColor;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return(-1);
    }
#endif
    dbgAssertOrIgnore(info != NULL);
    dbgAssertOrIgnore(trTextureRegistry[textureIndex].palettes != NULL);

    if (info == &trDefaultColorInfo)
    {
        return(0);                                          //return 0 for default palettes
    }
    textureColor = trColorInfosPointer(textureIndex);
    //textureColor = (trcolorinfo *)trTextureRegistry[textureIndex].palettes;//!!! <- does this work for non-pending textures
    for (index = 0; index < trTextureRegistry[textureIndex].nPalettes; index++, textureColor++)
    {                                                       //for all palettes in this texture
        if (trUnusedInfo(textureColor))
        {                                                   //if this slot not in use
            continue;                                       //skip it
        }
        if (info->base == textureColor->base &&             //if base/detail match
            info->detail == textureColor->detail)
        {
            return(index);                                  //this is the palette we want
        }
    }
    return(-1);                                             //no matches found
}

/*-----------------------------------------------------------------------------
    Name        : trAllColorsEqual
    Description : See if the color allocations to a pair of texreg structures
                    are the same.
    Inputs      : index0, 1 - indeces to compare.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool32 trAllColorsEqual(sdword index0, sdword index1)
{
    sdword index;
    trcolorinfo *textureColor0;
    trcolorinfo *textureColor1;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return(FALSE);
    }
#endif
    //dbgAssertOrIgnore(trPending(index0));
    //dbgAssertOrIgnore(trPending(index1));
    dbgAssertOrIgnore(index0 != index1);
    dbgAssertOrIgnore(index0 <= trHighestAllocated);
    dbgAssertOrIgnore(index1 <= trHighestAllocated);
    if (trTextureRegistry[index0].nPalettes != trTextureRegistry[index1].nPalettes)
    {
        return(FALSE);
    }
    textureColor0 = trColorInfosPointer(index0);//(trcolorinfo *)trTextureRegistry[index0].palettes;
    textureColor1 = trColorInfosPointer(index1);//(trcolorinfo *)trTextureRegistry[index1].palettes;
    for (index = 0; index < trTextureRegistry[index0].nPalettes; index++, textureColor0++, textureColor1++)
    {                                                       //for all palettes in this texture
        if (textureColor0->base != textureColor1->base ||   //if base/detail differ
            textureColor0->detail != textureColor1->detail)
        {
            return(FALSE);                                  //we must discard the whole batch
        }
    }
    return(TRUE);
}

/*-----------------------------------------------------------------------------
    Name        : trColorsIndexAlloc
    Description : Allocate a new colorinfo for a pending texture load
    Inputs      : info - what color we need
                  textureIndex - what texture to allocate it for
    Outputs     : puts the new info into the color info list in a free spot if
                    possible, or allocates a new slot.
    Return      :
    Note        : Should already have called trColorsEqual to verify this
                    color scheme not already there.
----------------------------------------------------------------------------*/
sdword trColorsIndexAlloc(trcolorinfo *info, sdword textureIndex)
{
    sdword index;
    texreg *reg = &trTextureRegistry[textureIndex];
    trcolorinfo *textureColor;

    textureColor = trColorInfosPointer(textureIndex);
    for (index = 0; index < reg->nPalettes; index++, textureColor++)
    {
        if (trUnusedInfo(textureColor))
        {                                                   //if this slot free
            *textureColor = *info;                          //put the color into the last slot
            return(index);                                  //use it
        }
    }
    //no slot found, allocate a new one
    dbgAssertOrIgnore(reg->nPalettes < TR_NumPalettesPerTexture);   //make sure there is room
    *textureColor = *info;                                  //put the color into the last slot
    reg->nPalettes++;                                       //one more palette now
    return(reg->nPalettes - 1);
}

/*-----------------------------------------------------------------------------
    Name        : trFindTextureIndexByName
    Description : Find the handle for a given texture name, if it has been registered.
    Inputs      : fileName - name of texture to find.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
sdword trFindTextureIndexByName(char *fileName)
{
    sdword index;
    sdword length;
    crc32 nameCRC, *regCRC;
    texreg *reg;

    //strupr(fileName);                                       //!!! is this going to screw anything up?
    /* Yes, yes it is... */
    length = strlen(fileName);
    dbgAssertOrIgnore(length > 0);

    nameCRC = crc32Compute((ubyte*)fileName, length);               //compute a name for the CRC

    reg = &trTextureRegistry[trHighestAllocated];
    regCRC = &trNameCRCs[trHighestAllocated];
    for (index = trHighestAllocated; index >= 0 ; index--, reg--, regCRC--)
    {                                                       //for all registry entries
        if (*regCRC == nameCRC)
        {                                                   //if names have matching CRC's
            if (!trAllocated(index))
            {                                               //if not allocated
                continue;                                   //skip it
            }
            if (!strcmp(reg->fileName, fileName))
            {                                               //and they match names
                return(index);
            }
        }
    }
    return(TR_NotShared);                                   //no index found; error
}

/*-----------------------------------------------------------------------------
    Name        : trTextureRegister
    Description : Register a texture for later loading
    Inputs      : fileName - path and name of texture to load
                  info - color information for colorizing texture or NULL if
                    texture is not to be colored.
                  meshReference - pointer to the mesh requesting texture.
                    Only used for colorization.
    Outputs     : Allocates and fills in an entry in the texture registry.
    Return      : Handle to registered texture.
----------------------------------------------------------------------------*/
trhandle trTextureRegister(char *fileName, trcolorinfo *info, void *meshReference)
{
    sdword index, paletteIndex, length;
    crc32 nameCRC, *regCRC;
    texreg *reg;

    //strupr(fileName);                                       //!!! is this going to screw anything up?
    /* Yup... */
    length = strlen(fileName);
    dbgAssertOrIgnore(length > 0);

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return(0);
    }
#endif
#if TR_VERBOSE_LEVEL >= 2
    dbgMessagef("trTextureRegister: registering '%s', info 0x%x", fileName, info);
#endif  //TR_VERBOSE_LEVEL
    if (info == NULL)
    {
        info = &trDefaultColorInfo;                         //default colorInfo on NULL
    }

    nameCRC = crc32Compute((ubyte*)fileName, length);               //compute a name for the CRC
    reg = &trTextureRegistry[trHighestAllocated];
    regCRC = &trNameCRCs[trHighestAllocated];

    for (index = trHighestAllocated; index >= 0 ; index--, reg--, regCRC--)
    {                                                       //for all registry entries
        if (*regCRC == nameCRC)
        {                                                   //if names have matching CRC's
            if (!trAllocated(index))
            {                                               //if not allocated
                continue;                                   //skip it
            }
            if (!strcmp(reg->fileName, fileName))
            {                                               //and they match names
                if ((paletteIndex = trColorsEqual(info, index)) >= 0)
                {                                           //and they're the same color
                    //... use this texture
                    dbgAssertOrIgnore(reg->nUsageCount < SDWORD_Max);
                    reg->nUsageCount++; //update usage count
#if TR_VERBOSE_LEVEL >= 2
                    dbgMessagef("trTextureRegister: texture handle 0x%x nUsageCount incremented to %d", index, reg->nUsageCount);
#endif  //TR_VERBOSE_LEVEL
                    return(trHandleMake(index, paletteIndex));//and use this texture
                }
                else
                {                                           //colors do not match
                    //!!! create a new paletted copy
                    //... if it is a pending texture
                    dbgAssertOrIgnore(reg->nUsageCount < SDWORD_Max);
                    reg->nUsageCount++; //update usage count
#if TR_VERBOSE_LEVEL >= 2
                    dbgMessagef("trTextureRegister: texture handle 0x%x nUsageCount incremented to %d", index, reg->nUsageCount);
#endif  //TR_VERBOSE_LEVEL
                    /*
                    ((trcolorinfo *)reg->palettes)
                        [reg->nPalettes] = *info;//retain reference to the color info
                    reg->nPalettes++;//update palette index
                    dbgAssertOrIgnore(reg->nPalettes <= TR_NumPalettesPerTexture);
                    */
                    paletteIndex = trColorsIndexAlloc(info, index);
                    return(trHandleMake(index, paletteIndex));
                }
            }
        }
    }

    //if we get here, no matching texture was found; allocate a
    //registry structure and load in texture
    for (index = trLowestFree; index < TR_RegistrySize; index++)
    {                                                       //search for a free structure
        if (!trAllocated(index))
        {                                                   //if free structure
#if TR_VERBOSE_LEVEL >= 2
            dbgMessagef("trTextureRegister: allocated index 0x%x for texture '%s'.", index, fileName);
#endif  //TR_VERBOSE_LEVEL
            // Notice how we just say that all textures are paletted?
            //  Well, we may want to put in a check for some other condition
            //  which forces the texture to be RGB, say for inclusion of an
            //  alpha channel or something.
            /*
            if (!loadAlpha)
            {
                bitSet(trTextureRegistry[index].flags, TRF_Paletted);
            }
            else
            {
                bitSet(trTextureRegistry[index].flags, TRF_Alpha);
            }
            */

            trTextureRegistry[index].meshReference = meshReference;//texture's parent mesh
            trTextureRegistry[index].nUsageCount = 1;       //one usage of this texture
            trNameCRCs[index] = nameCRC;                    //save the name CRC
            trTextureRegistry[index].fileName = memAlloc(length + 1, "NameTex", NonVolatile);
            strcpy(trTextureRegistry[index].fileName, fileName);
            trTextureRegistry[index].nPalettes = 1;         //one palette to start
            trTextureRegistry[index].currentPalette = -1;   //no palette yet registered
            trTextureRegistry[index].paletteCRC = TR_BadCRC;
            trTextureRegistry[index].palettes =             //allocate the temp palette list
                memAlloc(TR_NumPaletteSize, "Temp trcolorinfo list", 0);
            memClearDword(trTextureRegistry[index].palettes, TR_UnusedColorInfo, TR_NumPaletteSize / sizeof(udword));
            trTextureRegistry[index].baseScalar = (uword)colRealToUdword(trBaseColorScalar);
            trTextureRegistry[index].stripeScalar = (uword)colRealToUdword(trStripeColorScalar);
            trTextureRegistry[index].handle = TR_InvalidInternalHandle;
            dbgAssertOrIgnore(info != NULL);
            *((trcolorinfo *)trTextureRegistry[index].palettes) = *info;
            trSetPending(index);                            //texture is pending
            trSetAllocated(index);                          //and in use
            trHighestAllocated = max(trHighestAllocated, index);//update highest allocated
            if (index == trLowestFree)
            {                                               //if allocating lowest free
                for (; trLowestFree < TR_RegistrySize; trLowestFree++)
                {                                           //search for lowest free structure
                    if (!trAllocated(trLowestFree))
                    {                                       //if structure free
                        break;                              //done
                    }
                }
            }
            return(trHandleMake(index, 0));
        }
    }

#if TR_ERROR_CHECKING
    dbgFatalf(DBG_Loc, "\ntrTextureRegister: unable to allocate texture '%s' from list of %d entries", fileName, TR_RegistrySize);
#endif //TR_ERROR_CHECKING
    //should never get here
    return(0);
}

/*-----------------------------------------------------------------------------
    Name        : trRegisterAddition
    Description : Register an 'addition' to a texture.  This means add a color
                    that is not already there.
    Inputs      : handle - handle of texture already registered
                  info - color to register
    Outputs     :
    Return      : new handle
----------------------------------------------------------------------------*/
trhandle trRegisterAddition(trhandle handle, trcolorinfo *info)
{
    texreg *reg;
    sdword colorIndex, index = trIndex(handle);
    trhandle newHandle;
    trcolorinfo *oldColorInfos, *newColorInfos;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return(0);
    }
#endif

    dbgAssertOrIgnore(trAllocated(index));

    reg = trStructure(handle);

    colorIndex = trColorsEqual(info, index);

    //sharing handled
    if (trPending(index) || reg->sharedFrom != TR_NotShared)
    {                                                       //if texture not loaded yet
        if (colorIndex >= 0)
        {                                                   //if these guys are the same color
            newHandle = trHandleMake(index, colorIndex);    //use existing handle
        }
        else
        {                                                   //else this color not registered yet
            colorIndex = trColorsIndexAlloc(info, index);
            newHandle = trHandleMake(index, colorIndex);
        }
    }
    else
    {   //... kill the current textures to load from scratch ...

        //get list of old colors
        /*
        if (bitTest(reg->flags, TRF_Paletted))
        {                                                   //if it's a paletted texture
            oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes);
        }
        else
        {                                                   //else's it's an RGB texture
            oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes);
        }
        */
        oldColorInfos = trColorInfosPointer(index);
        if (colorIndex >= 0 &&
            oldColorInfos[colorIndex].base == info->base &&
            oldColorInfos[colorIndex].detail == info->detail)
        {                                                   //if the colors match
            newHandle = trHandleMake(index, colorIndex);
        }
        else
        {                                                   //if either color has changed
    #if TR_VERBOSE_LEVEL >= 2
            dbgMessagef("trRegisterAddition: updating texture handle 0x%x", handle);
    #endif  //TR_VERBOSE_LEVEL
            //make duplicate copy of the colorinfo structures
            newColorInfos = memAlloc(TR_NumPaletteSize, "dupe trcolorinfo list", NonVolatile);
            memcpy(newColorInfos, oldColorInfos, sizeof(trcolorinfo) * reg->nPalettes);

            //kill the GL textures and palettes
            trInternalTexturesDelete(handle);

            reg->palettes = (ubyte *)newColorInfos;         //save list of color infos
            trSetPending(index);                            //make this texture pending, to be re-loaded
            colorIndex = trColorsIndexAlloc(info, index);
            newHandle = trHandleMake(index, colorIndex);
        }
    }
    dbgAssertOrIgnore(reg->nUsageCount < SDWORD_Max);
    reg->nUsageCount++;                                     //update usage count
#if TR_VERBOSE_LEVEL >= 2
    dbgMessagef("trTextureRegister: texture handle 0x%x nUsageCount incremented to %d", index, reg->nUsageCount);
#endif  //TR_VERBOSE_LEVEL
    return(newHandle);
}

/*-----------------------------------------------------------------------------
    Name        : trRegisterRemoval
    Description : Remove a particular color from a texture.
    Inputs      : handle - handle we no longer need
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trRegisterRemoval(trhandle handle)
{
    texreg *reg = trStructure(handle);
    sdword index = trIndex(handle);
    sdword colorIndex = trPaletteIndex(handle);//, infoIndex;
    trcolorinfo *oldColorInfos, *newColorInfos;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return;
    }
#endif
    dbgAssertOrIgnore(trAllocated(index));

    if (reg->nUsageCount == 0)
    {                                                       //if they've all been deleted already
        return;                                             //nothing to do
    }
    //sharing handled
    if (trPending(index) || reg->sharedFrom != TR_NotShared)
    {                                                       //if the texture has not yet been loaded
        ((trcolorinfo *)reg->palettes)[colorIndex] = trUnusedColorInfo;//make this default colors (no longer needed)
    }
    else
    {                                                       //else texture is already loaded
        //get list of old colors
        /*
        if (bitTest(reg->flags, TRF_Paletted))
        {                                                   //if it's a paletted texture
            oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes);
        }
        else
        {                                                   //else's it's an RGB texture
            oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes);
        }
        */
        oldColorInfos = trColorInfosPointer(index);
        //make duplicate copy of the colorinfo structures
        newColorInfos = memAlloc(TR_NumPaletteSize, "dupe trcolorinfo list", NonVolatile);
        memcpy(newColorInfos, oldColorInfos, sizeof(trcolorinfo) * reg->nPalettes);
        newColorInfos[colorIndex] = trUnusedColorInfo;      //clear this color info

        //kill the GL textures and palettes
        trInternalTexturesDelete(handle);

        //now set the new colorinfo structures into the texreg
        reg->palettes = (ubyte *)newColorInfos;
        trSetPending(index);
    }
    reg->nUsageCount--;                                     //one less reference to this texture
}

/*-----------------------------------------------------------------------------
    Name        : trTextureColorsUpdate
    Description : Update the color of a given texture, flagging it for re-
                    loading if the colors have changed.
    Inputs      : handle - handle of already loaded texture
                  info - new colors
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trTextureColorsUpdate(trhandle handle, trcolorinfo *info)
{
    texreg *reg;
    trcolorinfo *oldColorInfos, *newColorInfos;
    sdword colorIndex;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return;
    }
#endif
    reg = trStructure(handle);
    colorIndex = trPaletteIndex(handle);

    //first, let's see if this texture is already loaded.  That's the
    //only time it makes sense.
    //sharing handled
    if (trPending(trIndex(handle)) || reg->sharedFrom != TR_NotShared)
    {                                                       //if texture not yet loaded
        newColorInfos = (trcolorinfo *)reg->palettes;       //get list of colors already allocated
        newColorInfos[colorIndex] = *info;                  //set new color
    }
    else
    {
        //get list of old colors
        /*
        if (bitTest(reg->flags, TRF_Paletted))
        {                                                   //if it's a paletted texture
            oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes);
        }
        else
        {                                                   //else's it's an RGB texture
            oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes);
        }
        */
        oldColorInfos = trColorInfosPointer(trIndex(handle));
        if (oldColorInfos[colorIndex].base != info->base ||
            oldColorInfos[colorIndex].detail != info->detail)
        {                                                   //if either color has changed
    #if TR_VERBOSE_LEVEL >= 2
            dbgMessagef("trTextureColorsUpdate: updating texture handle 0x%x", handle);
    #endif  //TR_VERBOSE_LEVEL
            //make duplicate copy of the colorinfo structures
            newColorInfos = memAlloc(TR_NumPaletteSize, "dupe trcolorinfo list", NonVolatile);
            memcpy(newColorInfos, oldColorInfos, sizeof(trcolorinfo) * reg->nPalettes);

            //kill the GL textures and palettes
            trInternalTexturesDelete(handle);

            newColorInfos[colorIndex] = *info;              //set new color
            reg->palettes = (ubyte *)newColorInfos;         //save list of color infos
            trSetPending(trIndex(handle));                  //make this texture pending, to be re-loaded
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : trTextureUnregister
    Description : 'Unregisters' a texture. i.e. decrements it's usage count.
    Inputs      : handle - handle of texture whose usage count is to be decremented.
    Outputs     : Decrements usage count.  Does not delete texture.
    Return      : New usage count for texture.
    Note        : All texture no longer in use will be freed up by either
                    trRegistryRefresh or trTextureDeleteAllUnregistered.
----------------------------------------------------------------------------*/
sdword trTextureUnregister(trhandle handle)
{
    sdword index = trIndex(handle);
    sdword paletteIndex = trPaletteIndex(handle);

#ifdef _X86_64
// Legacy error catching due to not everything working straight away.
if (handle >= TR_RegistrySize){
    dbgMessagef("Invalid handle %lx in trTextureUnregister",handle);
    return 0;
}
#endif

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        return(0);
    }
#endif
    //sharing handled - nothing special here...
    dbgAssertOrIgnore(trAllocated(index));
    trTextureRegistry[index].nUsageCount--;
    if (trPending(index))
    {                                                       //if texture not yet loaded
        ((trcolorinfo *)trTextureRegistry[index].palettes)[paletteIndex] = trUnusedColorInfo;//this palette not needed anymore
    }
#if TR_VERBOSE_LEVEL >= 2
    dbgMessagef("trTextureUnregister: handle 0x%x nUsageCount decremented to %d", handle, trTextureRegistry[index].nUsageCount);
#endif  //TR_VERBOSE_LEVEL
    return(trTextureRegistry[index].nUsageCount);
}

/*-----------------------------------------------------------------------------
    Name        : trTextureDeleteAllUnregistered
    Description : Delete all texture unregistered to <= 0 usage count
    Inputs      : void
    Outputs     : Deletes all texture no longer needed
    Return      : void
----------------------------------------------------------------------------*/
void trTextureDeleteAllUnregistered(void)
{
    sdword index;

#if TR_VERBOSE_LEVEL >= 1
    dbgMessagef("trTextureDeleteAllUnregistered");
#endif  //TR_VERBOSE_LEVEL
    for (index = 0; index <= trHighestAllocated; index++)
    {                                                       //for all textures in allocated range
        if (trAllocated(index))
        {                                                   //if texture allocated
            if (trTextureRegistry[index].nUsageCount <= 0)
            {                                               //if texture no longer in use
                trTextureDelete(index);
            }
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : trMeshSortAdd
    Description : Add a reference to this texture in the mesh-sorted list.
    Inputs      : regIndex - index into texture registry of texture
                : teamFlags - falgs for team effect from the image measurment.
    Outputs     : Adds to mesh-sorted list.
    Return      : void
----------------------------------------------------------------------------*/
void trMeshSortAdd(sdword regIndex, udword teamFlags)
{
    sdword index, j;
    void *meshReference = trStructure(regIndex)->meshReference;
    trcolorinfo *palettes, *otherPalettes;
    crc32 paletteCRC;
    sdword paletteSize, otherPaletteSize;

    palettes = trColorInfosPointer(regIndex);
    paletteSize = sizeof(trcolorinfo) * trStructure(regIndex)->nPalettes;
    dbgAssertOrIgnore(paletteSize > 0);
    paletteCRC = crc32Compute((ubyte *)palettes, paletteSize);

    for (index = 0; index < trMeshSortIndex; index++)
    {                                                       //search through list
        if (trMeshSortList[index].meshReference == meshReference && trMeshSortList[index].paletteCRC == paletteCRC)
        {                                                   //if matching mesh reference and palette CRC
            otherPaletteSize = sizeof(trcolorinfo) * trStructure(trMeshSortList[index].textureList[0])->nPalettes;
            dbgAssertOrIgnore(otherPaletteSize > 0);
            if (paletteSize == otherPaletteSize)
            {                                               //if they have the same numper of palettes
                otherPalettes = trColorInfosPointer(trMeshSortList[index].textureList[0]);
                if (!memcmp((void *)palettes, (void *)otherPalettes, paletteSize))
                {                                           //and the actual palettes are the same (just making sure they're the same color)
#if TR_ERROR_CHECKING
                    for (j = 0; j < trMeshSortList[index].nTextures; j++)
                    {                                       //verify this texture not already referenced
                        dbgAssertOrIgnore(trMeshSortList[index].textureList[j] != regIndex);
                    }
#endif
                    if (trMeshSortList[index].nTextures >= trMeshSortList[index].nAllocated)
                    {                                       //grow this texture list
                        trMeshSortList[index].nAllocated += TR_SortListAllocationStep;
                        trMeshSortList[index].textureList = memRealloc(trMeshSortList[index].textureList,
                                trMeshSortList[index].nAllocated * sizeof(sdword),
                                "TextureMeshSortSubList", 0);
                    }
                    trMeshSortList[index].textureList[trMeshSortList[index].nTextures] = regIndex;
                    trMeshSortList[index].nTextures++;      //store new texture reference
                    trMeshSortList[index].teamFlags |= teamFlags & (TRF_TeamColor0 | TRF_TeamColor1);
                    return;
                }
            }
        }
    }
    //if it gets here, no matching reference was found, we need to create a new one
    //grow the texture sort list if needed
    if (index >= trMeshSortLength)
    {                                                       //if the sort list too short
        dbgAssertOrIgnore(index == trMeshSortLength);               //should never be over
        trMeshSortList = memRealloc(trMeshSortList, (trMeshSortLength + TR_MeshSortGrowBy) * sizeof(trmeshsort), "Texture Mesh Sort List", 0);
        for (j = index; j < index + TR_MeshSortGrowBy; j++)
        {
            trMeshSortList[j].meshReference = NULL;         //no mesh reference yet
            trMeshSortList[j].nAllocated = TR_SortListAllocationStep;//starting amount of space for textures
            trMeshSortList[j].nTextures = 0;                //no textures in list yet
            trMeshSortList[j].teamFlags = 0;                //default is no team colors
            trMeshSortList[j].textureList =                 //allocate our local list
                memAlloc(TR_SortListAllocationStep * sizeof(sdword),
                         "TextureMeshSortSubList", 0);
        }
        trMeshSortLength += TR_MeshSortGrowBy;
    }
    trMeshSortList[index].meshReference = meshReference;    //use new mesh-sort structure
    trMeshSortList[index].paletteCRC = paletteCRC;
    trMeshSortList[index].textureList[0] = regIndex;
    trMeshSortList[index].nTextures = 1;
    trMeshSortList[index].teamFlags |= teamFlags & (TRF_TeamColor0 | TRF_TeamColor1);
    trMeshSortIndex++;
}

/*=============================================================================
    Following are a set of texture for directly creating and manipulating
        textures of different types.  These functions entirely circumvent the
        texture registry.
=============================================================================*/

/*-----------------------------------------------------------------------------
    Name        : trCreateUnpalettedTexture
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trCreateUnpalettedTexture(ubyte* palette, ubyte* data, sdword width, sdword height)
{
    ubyte* sp;
    ubyte* dp;
    sdword i, index;
    ubyte* rgba;

    rgba = memAlloc(4 * width * height, "temp rgba", Pyrophoric);

    dp = rgba;
    sp = data;
    for (i = 0; i < width*height; i++, dp += 4, sp++)
    {
        index = (*sp) << 2;
        dp[0] = palette[index + 0];
        dp[1] = palette[index + 1];
        dp[2] = palette[index + 2];
        dp[3] = palette[index + 3];
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    
    memFree(rgba);
}

/*-----------------------------------------------------------------------------
    Name        : trPalettedTextureCreate
    Description : Create a new texture object handle from some indexed data and
                    a palette.
    Inputs      : data - indexed-mode image to make texture of
                  palette - (one of the) palettes to go with image
                  width/height - dimensions of image
    Outputs     : Creats a texture object and downloads it
    Return      : new texture handle
----------------------------------------------------------------------------*/
udword trPalettedTextureCreate(ubyte *data, color *palette, sdword width, sdword height)
{
    udword newHandle;
#if TR_ERROR_CHECKING
    udword internalWidth;
#endif
    ubyte* tempData;
#if TR_ASPECT_CHECKING
    sdword oldWidth, oldHeight;
#endif

    tempData = NULL;

    glGenTextures(1, (GLuint*)&newHandle);                           //create a texture name
    primErrorMessagePrint();
    trClearCurrent();
    glBindTexture(GL_TEXTURE_2D, newHandle);                //bind texture for modification
    primErrorMessagePrint();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    primErrorMessagePrint();
    //first see if this texture can fit in RAM.  It should because we've already packed the textures
#if TR_ASPECT_CHECKING
    if ((width / height) > 8 || (height / width) > 8)
    {
        dbgWarningf(DBG_Loc, "\ntrPalettedTextureCreate: aspect overflow in texture of size %d x %d", width, height);

        oldWidth  = width;
        oldHeight = height;
        while ((width / height) > 8)
        {
            width >>= 1;
        }
        while ((height / width) > 8)
        {
            height >>= 1;
        }
        tempData = trImageScaleIndexed(data, oldWidth, oldHeight, width, height, FALSE);
        data = tempData;
    }
#endif //TR_ASPECT_CHECKING

    trCreateUnpalettedTexture((ubyte*)palette, data, width, height);

    if (texLinearFiltering)
    {                                                       //set min/mag filters to point samplingor linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    if (tempData != NULL)
    {
        memFree(tempData);
    }
    primErrorMessagePrint();

    return(newHandle);                                      //return new handle
}

/*-----------------------------------------------------------------------------
    Name        : trRGBTextureCreate
    Description : Creates a new RGB texture object based upon the specified RGB image.
    Inputs      : data - RGBA data to create texture from
                  width/height - size of image
    Outputs     : ..
    Return      : new texture handle
    Note        : textures created in this manner should only be deleted using
                    the following function and made current by the function
                    after that.
----------------------------------------------------------------------------*/
udword trRGBTextureCreate(color *data, sdword width, sdword height, bool32 useAlpha)
{
    udword newHandle, destType;
    color* tempData;
#if TR_ASPECT_CHECKING
    sdword oldWidth, oldHeight;
#endif

    tempData = NULL;

    destType = TR_RGBAType;

    glGenTextures(1, (GLuint*)&newHandle);                           //create a texture name
    primErrorMessagePrint();
    trClearCurrent();
    glBindTexture(GL_TEXTURE_2D, newHandle);                //bind texture for modification
    primErrorMessagePrint();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //first see if this texture can fit in RAM.  It should because we've already packed the textures
#if TR_ASPECT_CHECKING
    if (width / height > 8 || height / width > 8)
    {
        dbgWarningf(DBG_Loc, "\ntrRGBTextureCreate: aspect overflow in texture of size %d x %d", width, height);

        oldWidth  = width;
        oldHeight = height;
        while ((width / height) > 8)
        {
            width >>= 1;
        }
        while ((height / width) > 8)
        {
            height >>= 1;
        }
        tempData = trImageScale(data, oldWidth, oldHeight, width, height, FALSE);
        data = tempData;
    }
#endif //TR_ASPECT_CHECKING

    glTexImage2D(GL_TEXTURE_2D, 0, destType, width,       //create the GL texture object
                     height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (texLinearFiltering)
    {                                                       //set min/mag filters to point samplingor linear
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    if (tempData != NULL)
    {
        memFree(tempData);
    }
    primErrorMessagePrint();
    return(newHandle);                                      //return new handle
}

/*-----------------------------------------------------------------------------
    Name        : trRGBTextureUpdate
    Description : Update a previously created texture.
    Inputs      : handle - handle of previously created texture
                  data,width,height - same as for creation
    Outputs     : copies data to the texture and makes texture current
    Return      : void
----------------------------------------------------------------------------*/
void trRGBTextureUpdate(udword handle, color *data, sdword width, sdword height)
{
    trClearCurrent();
    glBindTexture(GL_TEXTURE_2D, handle);                   //bind texture for modification
    primErrorMessagePrint();

    glTexImage2D(GL_TEXTURE_2D, 0, TR_RGBType, width,       //create the GL texture object
                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
/*-----------------------------------------------------------------------------
    Name        : trRGBTextureDelete
    Description : Delete a texture which was created using trRGBTextureDelete
    Inputs      : handle - handle of texture returned from trRGBTextureCreate
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void trRGBTextureDelete(udword handle)
{
    glDeleteTextures(1, (GLuint*)&handle);
    primErrorMessagePrint();
}

/*-----------------------------------------------------------------------------
    Name        : trRGBTextureMakeCurrent
    Description : Make an RGB texture current
    Inputs      : handle - handle of texture to make current
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void trRGBTextureMakeCurrent(udword handle)
{
    trClearCurrent();
    glBindTexture(GL_TEXTURE_2D, handle);
    primErrorMessagePrint();
}

/*-----------------------------------------------------------------------------
    Name        : trPalettedTextureMakeCurrent
    Description : Make an RGB texture current
    Inputs      : handle - handle of texture to make current
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void trPalettedTextureMakeCurrent(udword handle, color *palette)
{
    trClearCurrent();
    glBindTexture(GL_TEXTURE_2D, handle);
    primErrorMessagePrint();
}

/*-----------------------------------------------------------------------------
    Name        : trScaleDown
    Description : Scale a single scanline of an image down by varying amounts.
    Inputs      : dest - destination scanline
                  source - source scanline
                  width - width of destination buffer
                  scalefactor - amount we're scaling down by
    Outputs     : copies source pixels to dest
    Return      : void
----------------------------------------------------------------------------*/
void trScaleDown(color *dest, color *source, sdword width, sdword scaleFactor)
{
    while (width > 0)
    {
        *dest = *source;
        width--;
        dest++;
        source += scaleFactor;
    }
}

/*-----------------------------------------------------------------------------
    Name        : trScaleArbitrary
    Description : Scale a single scanline of an image .
    Inputs      : dest - destination scanline
                  source - source scanline
                  width - width of destination buffer
                  scalefactor - amount we're scaling by (64k = 1.0)
    Outputs     : copies source pixels to dest
    Return      : void
----------------------------------------------------------------------------*/
void trScaleArbitrary(color *dest, color *source, sdword width, sdword scaleFactor)
{
    udword accumulator = 0;

    while (width > 0)
    {
        *dest = source[accumulator >> 16];
        dest++;
        width--;
        accumulator += scaleFactor;
    }
}

/*-----------------------------------------------------------------------------
    Name        : trImageScale
    Description : Scales an image down to a new size.
    Inputs      : data - RGBA buffer to be scaled down
                  width/height - size of image
                  newWidth/newHeight - size to scale to must be <= width/height,
                        even exponent of 2, >= 4
                  bFree - if true, the source image will be freed after scaling
                    if false, image will not be freed at all;  Memory will always
                    be allocated.
    Outputs     : Allocates new buffer and frees old one.
    Return      : Newly allocated and scaled buffer.
----------------------------------------------------------------------------*/
color *trImageScale(color *data, sdword width, sdword height, sdword newWidth, sdword newHeight, bool32 bFree)
{
    color *newBuffer;
    sdword index, scaleX, scaleY;
    udword accumulator, srcY;

    dbgAssertOrIgnore(newWidth <= width && newHeight <= height);    //verify the preconditions
    dbgAssertOrIgnore(newWidth >= TR_MinimumSizeX && newHeight >= TR_MinimumSizeY);
    if (newWidth == width && newHeight == height)
    {                                                       //if new size same as old size
        if (bFree)
        {
            return(data);                                       //done
        }
        else
        {
            newBuffer = memAlloc(sizeof(color) * newWidth * newHeight,
                                 "trImageScaled buffer", 0);
            memcpy(newBuffer, data, sizeof(color) * newWidth * newHeight);
            return(newBuffer);
        }
    }
    newBuffer = memAlloc(sizeof(color) * newWidth * newHeight,
                         "trImageScaled buffer", 0);

    if (bitNumberSet(width, 16) != 1 || bitNumberSet(height, 16) != 1)
    {                                                       //arbitrary scale
        scaleX = (width << 16) / newWidth;
        scaleY = (height << 16) / newHeight;
        accumulator = 0;

        for (index = 0; index < newHeight; index++)
        {
            srcY = accumulator >> 16;
            trScaleArbitrary(newBuffer + newWidth * index, data + width * srcY, newWidth, scaleX);
            accumulator += scaleY;
        }
    }
    else
    {
        scaleX = width / newWidth;
        scaleY = height / newHeight;

        for (index = 0; index < newHeight; index++)
        {                                                       //for each new scanline
            trScaleDown(newBuffer + newWidth * index,           //scale the scanline down
                     data + width * index * scaleY, newWidth, scaleX);
        }
    }
    if (bFree)
    {
        memFree(data);                                          //free the old color buffer
    }
    return(newBuffer);                                      //and allocate the new one
}

#ifdef _X86_64

/*-----------------------------------------------------------------------------
    Name        : tr64LifAdjustLoad
    Description : Load in a .lif file and fix up it's pointers
    Inputs      : fileName - name of file to load
                  flags - load volatile?
    Outputs     :
    Return      : pointer to the header of newly allocated file.
    Note        : the header of this image can be memfreed as it's all 1 big block
----------------------------------------------------------------------------*/

lifheader* tr64LifAdjustLoad(lifheader_disk *oldHeader, sdword oldLength)
{

    sdword newLength;
    sdword lenDiff;
    lifheader *newHeader = NULL;


    lenDiff =  sizeof(lifheader) - sizeof(lifheader_disk);
    newLength = oldLength + lenDiff;

    newHeader =  (lifheader *)memAlloc (newLength,"",0);


    strcpy(newHeader->ident,oldHeader->ident);
    newHeader->version = oldHeader->version;
    newHeader->flags = oldHeader->flags;
    newHeader->width = oldHeader->width;
    newHeader->height = oldHeader->height;
    newHeader->paletteCRC = oldHeader->paletteCRC;
    newHeader->imageCRC = oldHeader->imageCRC;
    newHeader->data = (ubyte *)((Uint64)oldHeader->data + lenDiff);
    newHeader->palette = (color*)((Uint64)oldHeader->palette + lenDiff);
    newHeader->teamEffect0 = (ubyte *)((Uint64)oldHeader->teamEffect0 + lenDiff);
    newHeader->teamEffect1 = (ubyte *)((Uint64)oldHeader->teamEffect1 + lenDiff);

    memcpy ((void*)newHeader+sizeof(lifheader), (void*)oldHeader + sizeof(lifheader_disk), oldLength - sizeof(lifheader_disk));

//    memFree(oldHeader); // Probably should free this here as it'll not be free'ed elsewhere
    return newHeader;


}

#endif


/*-----------------------------------------------------------------------------
    Name        : trLIFFileLoad
    Description : Load in a .lif file and fix up it's pointers
    Inputs      : fileName - name of file to load
                  flags - load volatile?
    Outputs     :
    Return      : pointer to the header of newly allocated file.
    Note        : the header of this image can be memfreed as it's all 1 big block
----------------------------------------------------------------------------*/
lifheader *trLIFFileLoad(char *fileName, udword flags)
{
    lifheader *newHeader;

#ifdef _X86_64
    sdword loadLength= 0;
    lifheader *oldHeader;
    loadLength = fileLoadAlloc(fileName, (void**)&oldHeader, flags);             //load in the .LiF file
    newHeader = tr64LifAdjustLoad((lifheader_disk *)oldHeader, loadLength);
#else
    fileLoadAlloc(fileName, (void**)&newHeader, flags);             //load in the .LiF file
#endif

#if FIX_ENDIAN
    newHeader->version     = FIX_ENDIAN_INT_32( newHeader->version );
    newHeader->flags       = FIX_ENDIAN_INT_32( newHeader->flags );
    newHeader->width       = FIX_ENDIAN_INT_32( newHeader->width );
    newHeader->height      = FIX_ENDIAN_INT_32( newHeader->height );
    newHeader->paletteCRC  = FIX_ENDIAN_INT_32( newHeader->paletteCRC );
    newHeader->imageCRC    = FIX_ENDIAN_INT_32( newHeader->imageCRC );
    newHeader->data        = ( ubyte *)FIX_ENDIAN_INT_32( ( udword )newHeader->data );
    newHeader->palette     = ( color *)FIX_ENDIAN_INT_32( ( udword )newHeader->palette );
    newHeader->teamEffect0 = ( ubyte *)FIX_ENDIAN_INT_32( ( udword )newHeader->teamEffect0 );
    newHeader->teamEffect1 = ( ubyte *)FIX_ENDIAN_INT_32( ( udword )newHeader->teamEffect1 );
#endif

#if TR_ERROR_CHECKING
    if (strcmp(newHeader->ident, LIF_FileIdentifier))
    {                                                       //verify proper file
        dbgFatalf(DBG_Loc, "Incorrect file identifier in '%s'.", fileName);
    }
    if (newHeader->version != LIF_FileVersion)
    {
        dbgFatalf(DBG_Loc, "File '%s' is version 0x%x instead of 0x%x", fileName, newHeader->version, LIF_FileVersion);
    }
    /*
    if (newHeader->width > TR_TextureWidthMax || newHeader->height > TR_TextureHeightMax)
    {
        dbgFatalf(DBG_Loc, "Texture '%s' too big ( %dx%d > %dx%d ).", fileName, newHeader->width, newHeader->height, TR_TextureWidthMax, TR_TextureHeightMax);
    }
    */
#endif
    //fixup pointers in the header
    newHeader->data = (memsize)newHeader + newHeader->data;
    newHeader->palette = (color *)((memsize)newHeader + (ubyte *)newHeader->palette);
    newHeader->teamEffect0 = (memsize)newHeader + (ubyte *)newHeader->teamEffect0;
    newHeader->teamEffect1 = (memsize)newHeader + (ubyte *)newHeader->teamEffect1;

    return(newHeader);
}

/*-----------------------------------------------------------------------------
    Name        : trScaleDownIndexed
    Description : Scale a single scanline of an image down by varying amounts.
    Inputs      : dest - destination scanline
                  source - source scanline
                  width - width of destination buffer
                  scalefactor - amount we're scaling down by
    Outputs     : copies source pixels to dest
    Return      : void
----------------------------------------------------------------------------*/
void trScaleDownIndexed(ubyte *dest, ubyte *source, sdword width, sdword scaleFactor)
{
    while (width > 0)
    {
        *dest = *source;
        width--;
        dest++;
        source += scaleFactor;
    }
}

/*-----------------------------------------------------------------------------
    Name        : trImageScaleIndexed
    Description : Scales an image down to a new size.
    Inputs      : data - RGBA buffer to be scaled down
                  width/height - size of image
                  newWidth/newHeight - size to scale to must be <= width/height,
                        even exponent of 2, >= 4
                  bFree - if TRUE, the passed data will be freed if scaling performed.
                    If false, memory will be allocated no matter if the size is the same.
    Outputs     : Allocates new buffer and frees old one.
    Return      : Newly allocated and scaled buffer.
----------------------------------------------------------------------------*/
ubyte *trImageScaleIndexed(ubyte *data, sdword width, sdword height, sdword newWidth, sdword newHeight, bool32 bFree)
{
    ubyte *newBuffer;
    sdword index, scaleX, scaleY;

    dbgAssertOrIgnore(newWidth <= width && newHeight <= height);    //verify the preconditions
    dbgAssertOrIgnore(newWidth >= TR_MinimumSizeX && newHeight >= TR_MinimumSizeY);
    dbgAssertOrIgnore(bitNumberSet(width, 16) == 1 && bitNumberSet(height, 16) == 1);
    if (newWidth == width && newHeight == height)
    {                                                       //if new size same as old size
        if (bFree)
        {
            return(data);
        }
        else
        {
            newBuffer = memAlloc(newWidth * newHeight,
                                 "trImageScaleIndexed buffer", Pyrophoric);
            memcpy(newBuffer, data, width * height);
            return(newBuffer);
        }
    }

    newBuffer = memAlloc(newWidth * newHeight,
                         "trImageScaleIndexed buffer", Pyrophoric);
    scaleX = width / newWidth;
    scaleY = height / newHeight;

    for (index = 0; index < newHeight; index++)
    {                                                       //for each new scanline
        trScaleDownIndexed(newBuffer + newWidth * index,           //scale the scanline down
                 data + width * index * scaleY, newWidth, scaleX);
    }
    if (bFree)
    {
        memFree(data);                                          //free the old color buffer
    }
    return(newBuffer);                                      //and allocate the new one
}

/*-----------------------------------------------------------------------------
    Name        : trPreLoadedTextureScale
    Description : Scale an already-loaded image to a new size.
    Inputs      : handle - index into texture registry
                  newWidth, newHeight - new size to make it.
    Outputs     : Creates a new, scaled down texture
    Return      : void
    Note        : If it's an RGBA texture, all colorized copies of the texture
                    will be scaled.
                  If the image has to be scaled up, it will be just flagged for
                    later re-loading (made pending).
----------------------------------------------------------------------------*/
void trPreLoadedTextureScale(sdword handle, sdword newWidth, sdword newHeight)
{
    texreg *reg = &trTextureRegistry[handle];
    ubyte *newIndexed, *oldIndexed;
    trcolorinfo *oldColorInfos, *newColorInfos;
    udword *handles;
    bool32 bUseAlpha;
    sdword index;

    //sharing handled
    if (reg->sharedFrom != TR_NotShared)
    {                                                       //do nothing to shared textures; they're not included in VRAM sizing anyhow
        return;
    }
    //actually do we really?  Maybe we don't...
    dbgAssertOrIgnore(newWidth != reg->scaledWidth || newHeight != reg->scaledHeight);
    dbgAssertOrIgnore(!trPending(handle));

    //... flag the texture for re-loading (make pending).
    if (bitTest(reg->flags, TRF_Paletted))
    {                                                   //if it's a paletted texture
        oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes);
    }
    else
    {                                                   //else's it's an RGB texture
        oldColorInfos = (trcolorinfo *)((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes);
    }
    //make duplicate copy of the colorinfo structures
    newColorInfos = memAlloc(TR_NumPaletteSize, "dupe trcolorinfo list", NonVolatile);
    memcpy(newColorInfos, oldColorInfos, sizeof(trcolorinfo) * reg->nPalettes);

    trInternalTexturesDelete(handle);                   //kill the GL textures and palettes
    reg->palettes = (ubyte *)newColorInfos;             //save new list of color infos
    trSetPending(trIndex(handle));                      //make this texture pending, to be re-loaded

    reg->scaledWidth = newWidth;
    reg->scaledHeight = newHeight;
}

/*-----------------------------------------------------------------------------
    Name        : trMeshSortListSort
    Description : Sorts a mesh-sorted list to put the paletted textures at the start
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
int meshSortSortCompare(void const* p0, void const* p1)
{
    return((sdword)bitTest(trTextureRegistry[*((sdword*)p1)].flags, TRF_Paletted) -
           (sdword)bitTest(trTextureRegistry[*((sdword*)p0)].flags, TRF_Paletted));
}
void trMeshSortListSort(trmeshsort* sortList)
{
    qsort(sortList->textureList, sortList->nTextures, sizeof(sdword), meshSortSortCompare);
}

/*-----------------------------------------------------------------------------
    Name        : trBufferColorRGB
    Description : Colorize an image (or palette) based upon team effect buffers
    Inputs      : dest - where to put the colorized image
                  source - where to read the uncolorized version
                  teamEffect0,1 - team color effect buffers
                  teamColor0,1 - team colors to colorize to
                  size - size fo buffer, in pixels
                  flags - texreg flags telling us how to colorize the image
                  effectScalar0, effectScalar1 - scalar of
                    the team effect values to be read.
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void trBufferColorRGB(color *dest, color *source,
                      ubyte *teamEffect0, ubyte *teamEffect1,
                      color teamColor0, color teamColor1,
                      sdword size, udword flags,
                      real32 effectScalar0, real32 effectScalar1)
{
    real32 teamRed0 = colUbyteToReal(colRed(teamColor0));
    real32 teamRed1 = colUbyteToReal(colRed(teamColor1));
    real32 teamGreen0 = colUbyteToReal(colGreen(teamColor0));
    real32 teamGreen1 = colUbyteToReal(colGreen(teamColor1));
    real32 teamBlue0 = colUbyteToReal(colBlue(teamColor0));
    real32 teamBlue1 = colUbyteToReal(colBlue(teamColor1));
    real32 redSource, greenSource, blueSource, redDest, greenDest, blueDest, effectReal0, effectReal1;

    while (size > 0)
    {
        //read the info from buffers into floating point
        redSource = colUbyteToReal(colRed(*source));
        greenSource = colUbyteToReal(colGreen(*source));
        blueSource = colUbyteToReal(colBlue(*source));
        if (bitTest(flags, TRF_TeamColor0))
        {
            effectReal0 = min(1.0f, colUbyteToReal(*teamEffect0) * effectScalar0);
        }
        else
        {
            effectReal0 = 0.0f;
        }
        if (bitTest(flags, TRF_TeamColor1))
        {
            effectReal1 = min(1.0f, colUbyteToReal(*teamEffect1) * effectScalar1);
        }
        else
        {
            effectReal1 = 0.0f;
        }
        //blend
        redDest = redSource * (1.0f - effectReal0) + teamRed0 * effectReal0;
        redDest = redDest * (1.0f - effectReal1) + teamRed1 * effectReal1;
        greenDest = greenSource * (1.0f - effectReal0) + teamGreen0 * effectReal0;
        greenDest = greenDest * (1.0f - effectReal1) + teamGreen1 * effectReal1;
        blueDest = blueSource * (1.0f - effectReal0) + teamBlue0 * effectReal0;
        blueDest = blueDest * (1.0f - effectReal1) + teamBlue1 * effectReal1;
        //write back
        *dest = colRGBA(colRealToUbyte(redDest), colRealToUbyte(greenDest), colRealToUbyte(blueDest), colAlpha(*source));
        //update pointers
        size--;
        dest++;
        source++;
        teamEffect0++;
        teamEffect1++;
    }
}

/*-----------------------------------------------------------------------------
    Name        : trMeshSortListLoad
    Description : Load a list of texture associated with a particular mesh.
    Inputs      : sortList - which mesh to load
    Outputs     : Loads all the pending textures
    Return      : void
    Note        : This function works for indexed and RGB mode textures.
                    See notice in comment block of trMakeCurrent for more detail.
----------------------------------------------------------------------------*/
void trMeshSortListLoad(trmeshsort *sortList)
{
    sdword index, count;
    sdword paletteIndex;
    texreg *reg, *otherReg;
    trcolorinfo *colorInfo;
    char fullName[PATH_MAX];
    color *colorData;
    lifheader *lifFile;
    ubyte *scaledData;
    color *registeredPalette;
    real32 scalar0[MAX_MULTIPLAYER_PLAYERS], scalar1[MAX_MULTIPLAYER_PLAYERS];
    real32 baseScalar, stripeScalar;
    ShipRace race;

#if TR_ERROR_CHECKING
    sdword firstIndex = -1;
    sdword j;
    //make sure all pending textures have the same palette requirements.
    for (index = 0; index < sortList->nTextures; index++)
    {
        if (trPending(sortList->textureList[index]))
        {                                                   //if texture needs loading
            if (firstIndex == -1)
            {                                               //if this is the first pending texture
                firstIndex = sortList->textureList[index];  //just remember the first one
            }
            else
            {                                               //else not first pending texture
                dbgAssertOrIgnore(trTextureRegistry[sortList->textureList[index]].nPalettes ==
                          trTextureRegistry[firstIndex].nPalettes);   //make sure same # palettes
                for (j = 0; j < trTextureRegistry[firstIndex].nPalettes; j++)
                {                                           //make sure identical palette info
                    dbgAssertOrIgnore(((trcolorinfo *)trTextureRegistry[sortList->textureList[index]].palettes)[j].base ==
                              ((trcolorinfo *)trTextureRegistry[firstIndex].palettes)[j].base);
                    dbgAssertOrIgnore(((trcolorinfo *)trTextureRegistry[sortList->textureList[index]].palettes)[j].detail ==
                              ((trcolorinfo *)trTextureRegistry[firstIndex].palettes)[j].detail);
                }
            }
        }
    }
    firstIndex = -1;
#endif //TR_ERROR_CHECKING

    //load in prequantized images
    for (index = 0; index < sortList->nTextures; index++)
    {
        if (trPending(sortList->textureList[index]))
        {                                                   //if texture needs loading
            reg = &trTextureRegistry[sortList->textureList[index]];

            if (reg->sharedFrom != TR_NotShared)
            {                                               //if this texture just refers to another texture
                // notice that we keep the allocated copy of the colorInfo structures (reg->palettes)
                // this is so we can ensure that the colorinfos of this texture and the texture to
                // which it refers are consistant.  This is OK, but we must keep track of this case
                // and treat it specially later on when we try to access the color info structures
                trClearPending(sortList->textureList[index]);
                continue;
            }
            //load in the image from pre-quantized .LiF file
            if (bitTest(reg->flags, TRF_SharedFileName))
            {
                strcpy(fullName, (char *)memchr(reg->fileName, 0, SWORD_Max) + 1);
            }
            else
            {
                strcpy(fullName, reg->fileName);
            }
            strcat(fullName, ".LiF");                       //create full filename
            lifFile = trLIFFileLoad(fullName, 0);           //load in the file

            bitClear(reg->flags, TRF_TeamColor0);
            bitClear(reg->flags, TRF_TeamColor1);
            bitClear(reg->flags, TRF_Paletted);
            bitClear(reg->flags, TRF_Alpha);
            if (bitTest(lifFile->flags, TRF_TeamColor0))
            {
                bitSet(reg->flags, TRF_TeamColor0);
            }
            if (bitTest(lifFile->flags, TRF_TeamColor1))
            {
                bitSet(reg->flags, TRF_TeamColor1);
            }
            if (bitTest(lifFile->flags, TRF_Paletted))
            {
                bitSet(reg->flags, TRF_Paletted);
            }
            if (bitTest(lifFile->flags, TRF_Alpha))
            {
                bitSet(reg->flags, TRF_Alpha);
            }

#if TR_ERROR_CHECKING
            if (lifFile->width != reg->diskWidth || lifFile->height != reg->diskHeight)
            {
                dbgFatalf(DBG_Loc, "File '%s' is size (%dx%d) instead of (%dx%d) as noted in textures.ll.",
                    fullName, lifFile->width, lifFile->height, reg->diskWidth, reg->diskHeight);
            }
#endif
            colorInfo = (trcolorinfo *)reg->palettes;
            //find out what race we're dealing with and get it's hue-specific scalars for all teams
//            reg = &trTextureRegistry[sortList->textureList[0]];
            baseScalar = colUdwordToReal(reg->baseScalar);
            stripeScalar = colUdwordToReal(reg->stripeScalar);
#ifdef _WIN32
            if (strchr(reg->fileName, '\\') && (baseScalar != 0.0f || stripeScalar != 0.0f))
#else
            if (strpbrk(reg->fileName, "\\/") && (baseScalar != 0.0f || stripeScalar != 0.0f))
#endif
            {
#ifdef _WIN32
                count = strchr(reg->fileName, '\\') - (reg->fileName);
#else
                count = strpbrk(reg->fileName, "\\/") - (reg->fileName);
#endif
                dbgAssertOrIgnore(count > 0);
                memStrncpy(fullName, reg->fileName, count + 1);
                //fullName[count] = 0;
                race = StrToShipRace(fullName);
                dbgAssertOrIgnore ((race < 10) || (race == 0xffffffff));
                if (race != 0xffffffff)
                {
                    for (count = 0; count < reg->nPalettes; count++)
                    {
                        cpTeamEffectScalars(&scalar0[count], &scalar1[count], colorInfo[count].base, colorInfo[count].detail, race);
                        scalar0[count] = (scalar0[count] - 1.0f) * baseScalar + 1.0f;
                        scalar1[count] = (scalar1[count] - 1.0f) * stripeScalar + 1.0f;
                    }
                }
            }
            else
            {
                for (count = 0; count < reg->nPalettes; count++)
                {
                    scalar0[count] = scalar1[count] = 1.0f;
                }
            }
            if (bitTest(reg->flags, TRF_Paletted))
            {                                               //load in paletted style
                //duplicate and blend the palettes for this image
#if TR_DEBUG_TEXTURES
                if (trSpecialTextures)
                {
                    ubyte *tempBuffer;
                    sdword index;
                    //create uncolored palette
                    reg->uncolored = trPalettedTextureCreate(lifFile->data, lifFile->palette, reg->diskWidth, reg->diskHeight);
                    //duplicate the uncolored palette
                    reg->uncoloredPalette = memAlloc(TR_PaletteSize, "UncoloredTestPalette", 0);
                    memcpy(reg->uncoloredPalette, lifFile->palette, TR_PaletteSize);
                    //create textures of the team effect buffers
                    tempBuffer = memAlloc(reg->diskWidth * reg->diskHeight, "FHGEEABuffer", 0);
                    if (bitTest(sortList->teamFlags, TRF_TeamColor0))
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = lifFile->teamEffect0[lifFile->data[index]];
                        }
                    }
                    else
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = 0;
                        }
                    }
                    reg->col0Handle = trPalettedTextureCreate(tempBuffer, trTestPalette0, reg->diskWidth, reg->diskHeight);
                    if (bitTest(sortList->teamFlags, TRF_TeamColor1))
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = lifFile->teamEffect1[lifFile->data[index]];
                        }
                    }
                    else
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = 0;
                        }
                    }
                    reg->col1Handle = trPalettedTextureCreate(tempBuffer, trTestPalette0, reg->diskWidth, reg->diskHeight);
                    memFree(tempBuffer);
                    //set the test texture flag
                }
#endif
                //see if the palette is already loaded by a previous texture in this list
                for (paletteIndex = 0; paletteIndex < index; paletteIndex++)
                {
                    otherReg = &trTextureRegistry[sortList->textureList[paletteIndex]];
                    if (bitTest(otherReg->flags, TRF_Paletted) && otherReg->sharedFrom == TR_NotShared)
                    {                                       //if other texture is paletted and not a shared texture
                        dbgAssertOrIgnore(otherReg->paletteCRC != TR_BadCRC);
                        if (otherReg->paletteCRC == lifFile->paletteCRC)
                        {                                   //if this palette matches other palette
                            //update palette register count
                            registeredPalette = (color *)otherReg->palettes;
#if TR_ERROR_CHECKING
                            if ((registeredPalette[0] & 0xff000000) >= 0xfe000000)
                            {
                                dbgFatalf(DBG_Loc, "Texture '%s', index %d has a usage count of %d.", otherReg->fileName, sortList->textureList[paletteIndex], registeredPalette[0] >> 24);
                            }
#endif
                            registeredPalette[0] += 0x01000000;
                            reg->palettes = (ubyte *)registeredPalette;
                            goto alreadyFoundPalette;       //skip the allocation bit
                        }
                    }
                }
                //else no matching palette was found, we must allocate and colorize some
                reg->palettes = memAlloc(TR_PaletteSize * reg->nPalettes +
                                         sizeof(trcolorinfo) * reg->nPalettes,
                                         "TexturePalettePool", NonVolatile);
                //keep track of what the colors were, for later reference
                memcpy((ubyte *)reg->palettes + TR_PaletteSize * reg->nPalettes,
                       colorInfo, sizeof(trcolorinfo) * reg->nPalettes);
                for (count = 0; count < reg->nPalettes; count++)
                {
                    //duplicate the palettes
                    if (!trUnusedInfo(&colorInfo[count]))
                    {                                       //if this color scheme in use
                        if (sortList->teamFlags & (TRF_TeamColor0 | TRF_TeamColor1))
                        {                                   //if there is team color
                            trBufferColorRGB((color *)(reg->palettes + count * TR_PaletteSize), lifFile->palette,
                                lifFile->teamEffect0, lifFile->teamEffect1, colorInfo[count].base, colorInfo[count].detail,
                                256, sortList->teamFlags, scalar0[count], scalar1[count]);
                        }
                        else
                        {                                   //no team color, don't colorize it
                            memcpy(reg->palettes + count * TR_PaletteSize, lifFile->palette, TR_PaletteSize);
                        }
                    }
                    else
                    {                                       //color not in use, clear this palette to rave-borg green so we can spot it
                        memClearDword((color *)(reg->palettes + count * TR_PaletteSize), TR_UnusedColorPalette, 256);
                    }
                }
                ((color *)reg->palettes)[0] &= 0x01ffffff;  //set new palette usage count to 1
alreadyFoundPalette:;
                //scale the image down as needed
                scaledData = trImageScaleIndexed(lifFile->data, reg->diskWidth, reg->diskHeight, reg->scaledWidth, reg->scaledHeight, FALSE);

                //create the GL texture object now
                reg->handle = trNoPalTextureCreate(scaledData, (ubyte*)reg->palettes, reg->scaledWidth, reg->scaledHeight, sortList->textureList[index]);

                //save the palette CRC for palette sharing
                reg->paletteCRC = lifFile->paletteCRC;

                //free the image as loaded
                memFree(scaledData);                        //free scaled copy, no longer needed
            }
            else
            {                                               //load it in RGB style
                ubyte *scaledTeam0, *scaledTeam1;           //team color effect buffers scaled down
#if TR_DEBUG_TEXTURES
                sdword index;
                if (trSpecialTextures)
                {
                    color *tempBuffer;
                    //create uncolored palette
                    reg->uncolored = trRGBTextureCreate((color *)lifFile->data, reg->diskWidth, reg->diskHeight, FALSE);
                    //create textures of the team effect buffers
                    tempBuffer = memAlloc(sizeof(color) * reg->diskWidth * reg->diskHeight, "TempDKJFBuffer", 0);
                    if (bitTest(sortList->teamFlags, TRF_TeamColor0))
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = colRGB(lifFile->teamEffect0[index], lifFile->teamEffect0[index], lifFile->teamEffect0[index]);
                        }
                    }
                    else
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = colRGB(0, 0, 0);
                        }
                    }
                    reg->col0Handle = trRGBTextureCreate(tempBuffer, reg->diskWidth, reg->diskHeight, FALSE);
                    if (bitTest(sortList->teamFlags, TRF_TeamColor1))
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = colRGB(lifFile->teamEffect1[index], lifFile->teamEffect1[index], lifFile->teamEffect1[index]);
                        }
                    }
                    else
                    {
                        for (index = 0; index < reg->diskWidth * reg->diskHeight; index++)
                        {
                            tempBuffer[index] = colRGB(0, 0, 0);
                        }
                    }
                    reg->col1Handle = trRGBTextureCreate(tempBuffer, reg->diskWidth, reg->diskHeight, FALSE);
                    memFree(tempBuffer);
                    //set the test texture flag
                }
#endif
                //scale the texture down
                colorData = trImageScale((color *)lifFile->data, reg->diskWidth, reg->diskHeight, reg->scaledWidth, reg->scaledHeight, FALSE);
                scaledTeam0 = trImageScaleIndexed(lifFile->teamEffect0, reg->diskWidth, reg->diskHeight, reg->scaledWidth, reg->scaledHeight, FALSE);
                scaledTeam1 = trImageScaleIndexed(lifFile->teamEffect1, reg->diskWidth, reg->diskHeight, reg->scaledWidth, reg->scaledHeight, FALSE);
                //duplicate and blend the texture for the different teams
                if (colorInfo != NULL)
                {
                    //allocate the list of handles
                    reg->palettes = memAlloc(sizeof(udword) * reg->nPalettes +
                                sizeof(trcolorinfo) * reg->nPalettes,
                                "TextureRGBHandleList", NonVolatile);
                    //keep track of what the colors were, for later reference
                    memcpy((ubyte *)reg->palettes + sizeof(udword) * reg->nPalettes,
                           colorInfo, sizeof(trcolorinfo) * reg->nPalettes);
                    for (count = 0; count < reg->nPalettes; count++)
                    {
                        //blend the buffer by team color
                        if (!trUnusedInfo(&colorInfo[count]))
                        {                                   //if this team color in use
                            if (reg->flags & (TRF_TeamColor0 | TRF_TeamColor1))
                            {                               //and there is team color
                                trBufferColorRGB((color *)lifFile->data, colorData,
                                    scaledTeam0, scaledTeam1, colorInfo[count].base, colorInfo[count].detail,
                                    reg->scaledWidth * reg->scaledHeight, reg->flags, scalar0[count], scalar1[count]);
                                reg->handle =
                                    trRGBTextureCreate((color *)lifFile->data, reg->scaledWidth, reg->scaledHeight, bitTest(reg->flags, TRF_Alpha));
                            }
                            else
                            {                               //else no team color
                                reg->handle =
                                    trRGBTextureCreate(colorData, reg->scaledWidth, reg->scaledHeight, bitTest(reg->flags, TRF_Alpha));
                            }
                            //create the texture
                            if (reg->nPalettes > 1)
                            {
                                ((udword *)reg->palettes)[count] = reg->handle;
                                reg->handle = TR_InvalidInternalHandle;
                            }
                        }
                        else
                        {
                            ((udword *)reg->palettes)[count] = TR_InvalidInternalHandle;
                        }
                    }
                }
                else
                {
                    reg->palettes = NULL;
                    reg->handle =                           //create the texture
                        trRGBTextureCreate(colorData, reg->scaledWidth, reg->scaledHeight, bitTest(reg->flags, TRF_Alpha));
                }
                //free the no longer needed texture
                memFree(scaledTeam0);
                memFree(scaledTeam1);
                memFree(colorData);
            }
            trClearPending(sortList->textureList[index]);
//            bitClear(reg->flags, TRF_Pending);              //texture no longer pending
            memFree(colorInfo);
            memFree(lifFile);                               //get rid of the image which is no longer needed
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : trSizeSortCompare
    Description : Qsort callback for sorting the size sort list
    Inputs      : e1, e2 - cast to trmemmap pointers
    Outputs     :
    Return      : area2 - area1
----------------------------------------------------------------------------*/
int trSizeSortCompare(const void *e1, const void *e2)
{
    return(((trmemmap *)e2)->width * ((trmemmap *)e2)->height -
           ((trmemmap *)e1)->width * ((trmemmap *)e1)->height);
}

/*-----------------------------------------------------------------------------
    Name        : trCramScaleTableCompute
    Description : Build a table for all possible texture sizes, what
                  the closest size, integer exponents of 2, would be when
                  multiplied by the scaling factor.
    Inputs      : dest - where to buld the table
                  scaleFactor - how we are scaling the thing.
    Outputs     : computes an optimum scaling table in dest; a set of DWORDS where:
                    high word = width, low word = height
    Return      : void
----------------------------------------------------------------------------*/
void trCramScaleTableCompute(sdword dest[TR_TexSizesX][TR_TexSizesY], sdword scaleFactor)
{
    sdword x, y, index, closestSizeDiff, closestIndex = 0;
    sdword closestWidth, closestHeight, widthToMatch, heightToMatch, width, height;
    sdword xDiff, yDiff;

    dbgAssertOrIgnore(scaleFactor <= 65536);                        //make sure we're not scaling up
    for (x = 0; x < TR_TexSizesX; x++)
    {                                                       //for all widths/heights
        for (y = 0; y < TR_TexSizesY; y++)
        {
            closestSizeDiff = SDWORD_Max;                   //ensure we'll find a better fit
            closestWidth = SDWORD_Max;
            closestHeight = SDWORD_Max;
            widthToMatch = (TR_MinimumSizeX << x) * scaleFactor / 65536;
            heightToMatch = (TR_MinimumSizeY << y) * scaleFactor / 65536;
            for (index = max(x, y); index >= 0; index--)    //for all x/y sizes smaller than this x/y size,
            {
                width = (TR_MinimumSizeX << max(x - index, 0));
                height = (TR_MinimumSizeX << max(y - index, 0));
                if (abs(width - widthToMatch) + abs(height - heightToMatch) <= closestSizeDiff &&
                    (x >= max(x - index, 0)) && (y >= max(y - index, 0)))
                {
                    closestSizeDiff = abs(width - widthToMatch) + abs(height - heightToMatch);
                    closestWidth = width;
                    closestHeight = height;
                    closestIndex = index;
                    if (closestSizeDiff == 0)
                    {                                       //if size matches exactly
                        break;                              //don't check any other sizes
                    }
                }
                if (widthToMatch > heightToMatch)
                {                                           //w/2 x h
                    width = (TR_MinimumSizeX << max(x - index - 1, 0));
                    height = (TR_MinimumSizeX << max(y - index, 0));
                }
                else
                {                                           //w x h/2
                    width = (TR_MinimumSizeX << max(x - index, 0));
                    height = (TR_MinimumSizeX << max(y - index - 1, 0));
                }
                if (abs(width - widthToMatch) + abs(height - heightToMatch) <= closestSizeDiff &&
                    (x >= max(x - index - 1, 0)) && (y >= max(y - index, 0)))
                {                                           //if this is as good as any other fit
                    closestSizeDiff = abs(width - widthToMatch) + abs(height - heightToMatch);
                    closestWidth = width;
                    closestHeight = height;
                    closestIndex = index | 0x40000000;      //negative or positive tells which one we tried
                    if (closestSizeDiff == 0)
                    {                                       //if size matches exactly
                        break;                              //don't check any other sizes
                    }
                }
            }
            if (!(closestIndex & 0x40000000))
            {                                               //if closest was x * y
                //high word - width, low word - height
                dest[x + TR_SSBMinX][y + TR_SSBMinY] = ((TR_MinimumSizeX << (max(x - closestIndex, 0))) << 16) |
                    (TR_MinimumSizeY << (max(y - closestIndex, 0)));
                dbgAssertOrIgnore((TR_MinimumSizeX << (max(x - closestIndex, 0))) <=
                          (TR_MinimumSizeX << x));          //make sure smaller than dimensions to match
                dbgAssertOrIgnore((TR_MinimumSizeY << (max(y - closestIndex, 0))) <=
                          (TR_MinimumSizeY << y));
            }
            else
            {                                               //closest was x/2 * y
                if (x > y)
                {                                           //if texture larger in x
                    xDiff = 1;                              //scale x down
                    yDiff = 0;
                }
                else
                {                                           //texture larger in y
                    xDiff = 0;
                    yDiff = 1;                              //scale y down instead of x
                }
                closestIndex &= 0x0fffffff;                 //remove the flag bit
                dest[x + TR_SSBMinX][y + TR_SSBMinY] = ((TR_MinimumSizeX << (max(x - closestIndex - xDiff, 0))) << 16) |
                    (TR_MinimumSizeY << (max(y - closestIndex - yDiff, 0)));
                dbgAssertOrIgnore((TR_MinimumSizeX << (max(x - closestIndex - xDiff, 0))) <=
                          (TR_MinimumSizeX << x));          //make sure smaller than dimensions to match
                dbgAssertOrIgnore((TR_MinimumSizeY << (max(y - closestIndex - yDiff, 0))) <=
                          (TR_MinimumSizeY << y));
            }
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : trCramAttempt
    Description : Attempt to fit all textures into the current RAM pools
                    given a certain scaling factor.
    Inputs      : scaleFactor - amount to resize the textures, 65536 being to
                    not scale at all.
    Outputs     :
    Return      : Total amount of RAM needed for this scaling, accounting for
                    granularity.
----------------------------------------------------------------------------*/
sdword trCramAttempt(sdword scaleFactor)
{
    sdword x, y, index, used, size;
#if TR_ERROR_CHECKING
    sdword width, height;
#endif
    sdword newSize[TR_TexSizesX][TR_TexSizesY];
    texreg *reg;

#if TR_VERBOSE_LEVEL >= 2
    dbgMessagef("trCramAttempt: fitting textures, scale factor %d", scaleFactor);
#endif  //TR_VERBOSE_LEVEL
    for (index = used = 0; index < trNumberRamPools; index++)
    {
        trRamPoolList[index].used = 0;                      //all pools empty
    }
    trCramScaleTableCompute(newSize, scaleFactor);          //compute table of sizes

    //now we have our crazy table, we can go on to see if we can fit these guys
    for (index = 0; index < trSizeSortLength; index++)
    {
        if (!trAllocated(index))
        {                                                   //don't count unallocated textures
            continue;
        }
        reg = &trTextureRegistry[trSizeSortList[index].index];//get pointer to registry structure
        //sharing handled
        if (reg->sharedFrom != TR_NotShared)
        {                                                   //don't consider shared textures in VRAM size computations
            continue;
        }
        dbgAssertOrIgnore(bitNumberSet((udword)reg->diskWidth, 16) == 1);//make sure integer exponent of 2 in size
        dbgAssertOrIgnore(bitNumberSet((udword)reg->diskHeight, 16) == 1);
        size = newSize[bitLowBitPosition((udword)reg->diskWidth) - TR_SSBMinX]
            [bitLowBitPosition((udword)reg->diskHeight) - TR_SSBMinY];//get image size from table
#if TR_ERROR_CHECKING
        width = (size & 0xffff0000) >> 16;
        height = size & 0x0000ffff;
        dbgAssertOrIgnore(reg->diskWidth >= width);
        dbgAssertOrIgnore(reg->diskHeight >= height);
#endif
        size = ((size & 0xffff0000) >> 16) * (size & 0x0000ffff);//convert to just size
        size = (size + (trRamPoolGranularity - 1)) &
            (~(trRamPoolGranularity - 1));                  //round up for granularity
        //now try to fit a texture of this new size into some RAM pool
        //!!! might want to alternate the starting pool so the
        //!!! big ones get evenly distributed
        if (bitTest(reg->flags, TRF_Paletted))
        {                                                   //if paletted texture
            for (x = 0; x < trNumberRamPools; x++)
            {
                if (trRamPoolList[x].used + size <= trRamPoolList[x].length)
                {                                           //if it'll fit in this one
                    trRamPoolList[x].used += size;          //update this pools used amount
                    used += size;                           //update total used amount
                    break;                                  //stop running through the pools
                }
                else
                {
                    //unable to fit texture in any slot, keep calculating total size anyhow
                    used += size;
                }
            }
        }
        else
        {                                                   //else an RGB texture
            for (y = 0; y < reg->nPalettes; y++)
            {                                               //do a multiple of times because RGB textures need to be duplicated
                for (x = 0; x < trNumberRamPools; x++)
                {
                    if (trRamPoolList[x].used + size <= trRamPoolList[x].length)
                    {                                       //if it'll fit in this one
                        trRamPoolList[x].used += size * 2;      //update this pools used amount
                        used += size * 2;                       //update total used amount
                        goto itFitFine;                     //stop running through the pools
                    }
                }
                //unable to fit texture in any slot, keep calculating total size anyhow
                used += size;
itFitFine:;
            }
        }
    }
    return(used);                                           //now we've tried to fit all the textures
}

/*-----------------------------------------------------------------------------
    Name        : trCramRAMScaleCompute
    Description : Compute a scaling factor for fitting all registered textures
                    into VRAM.
    Inputs      : void
    Outputs     :
    Return      : Newly computed scaling factor
----------------------------------------------------------------------------*/
sdword trCramRAMScaleCompute(void)
{
    sdword index, scaleFactor, bestScaleFactor, scaleOld;
    sdword totalUsed, totalAvailable, margin, bestMargin;
    sdword iterations = 0;

    for (index = totalAvailable = 0; index < trNumberRamPools; index++)
    {
        totalAvailable += trRamPoolList[index].length;      //compute length of all pools
    }

    margin = -1;
    bestScaleFactor = scaleFactor = 65536;                  //fixed-point multiplier for scaling.  This means 1
    bestMargin = SDWORD_Max;
    while (margin < 0 || margin > totalAvailable * TR_AvailabilityMargin)
    {
        totalUsed = trCramAttempt(scaleFactor);
        margin = totalAvailable - totalUsed;
        // detect if it fit no problem on first try
        if (scaleFactor >= 65536 && margin >= 0)
        {                                                   //if it fit at full size
            bestScaleFactor = scaleFactor;                  //we're done
            break;
        }
        // record best scale factor
        if (margin >= 0 && margin < bestMargin)
        {                                                   //if best margin so far
            bestMargin = margin;                            //remember this scale factor
            bestScaleFactor = scaleFactor;
        }
        scaleOld = scaleFactor;
        scaleFactor += (sdword)((sqword)margin * (sqword)65536 /
                                (sqword)totalAvailable);
        if (scaleFactor <= 0)
        {
            scaleFactor = scaleOld / 2;
        }
        if (margin > 65536)
        {                                                   //if it 'over-fit'
            break;
        }
        if (iterations > TR_MaxCramIterations && margin >= 0)//only do this so many times
        {
            break;
        }
        iterations++;
    }
#if TR_VERBOSE_LEVEL >= 1
    dbgMessagef("trCramRAMScaleCompute: fit textures, scale factor %d, in %d iterations", min(65536, bestScaleFactor), iterations);
#endif  //TR_VERBOSE_LEVEL
    //now we have a scale factor which will enable us to fit all textures
    //into a given amount of texture RAM, provided we follow the same fitting
    //algorithm as trCramAttempt.  The final step here is to run through all the
    //textures and set their scaled sizes, if they are pending, or actually
    //scale them down as needed, if they are already loaded.
    return(min(65536, bestScaleFactor));                    //make sure we're not scaling anything UP
}

/*-----------------------------------------------------------------------------
    Name        : trCramIntoRAM
    Description : Adjusts sizes of all registered textures to fit them into a
                    specified amount of texture RAM.  Available RAM pools are
                    given by trRamPoolList[].
    Inputs      : texture scaling factor, provided by trCramRAMScaleCompute
                    or supplied independently.
    Outputs     : Any textures previously loaded will be copied to system RAM,
                    scaled down and copied back to texture RAM if they need
                    scaling down.
    Return      : void
    Note        : This function is almost identical to trCramAttempt except
        that it actually fits the textures, rather than just seeing if it fits.
----------------------------------------------------------------------------*/
void trCramIntoRAM(sdword scaleFactor)
{
    sdword x, y, index, used, size, width, height;
    sdword newSize[TR_TexSizesX][TR_TexSizesY];
    texreg *reg;

#if TR_VERBOSE_LEVEL >= 2
    dbgMessagef("trCramIntoRAM: fitting textures, scale factor %d", scaleFactor);
#endif  //TR_VERBOSE_LEVEL
    for (index = used = 0; index < trNumberRamPools; index++)
    {
        trRamPoolList[index].used = 0;                      //all pools empty
    }
    trCramScaleTableCompute(newSize, scaleFactor);          //compute table of sizes

    //now we have our crazy table, we can go on and fit these guys
    for (index = 0; index < trSizeSortLength; index++)
    {
        reg = &trTextureRegistry[trSizeSortList[index].index];//get pointer to registry structure
        dbgAssertOrIgnore(bitNumberSet((udword)reg->diskWidth, 16) == 1);//make sure integer exponent of 2 in size
        dbgAssertOrIgnore(bitNumberSet((udword)reg->diskHeight, 16) == 1);
        size = newSize[bitLowBitPosition((udword)reg->diskWidth) - TR_SSBMinX]
            [bitLowBitPosition((udword)reg->diskHeight) - TR_SSBMinY];//get image size from table
        width = (size & 0xffff0000) >> 16;
        height = size & 0x0000ffff;
        size = width * height;                              //convert to just size
        size = (size + (trRamPoolGranularity - 1)) &
            (~(trRamPoolGranularity - 1));                  //round up for granularity
        if (trPending(trSizeSortList[index].index))
        {                                                   //if image hasn't been loaded yet
            reg->scaledWidth = (sword)width;                //just set the size of the texture
            reg->scaledHeight = (sword)height;
            dbgAssertOrIgnore(reg->diskWidth >= reg->scaledWidth);
            dbgAssertOrIgnore(reg->diskHeight >= reg->scaledHeight);
        }
        else
        {                                                   //else image already loaded and in use
            if (width != reg->scaledWidth || height != reg->scaledHeight)
            {                                               //if it needs to be scaled down
                trPreLoadedTextureScale(trSizeSortList[index].index,
                                        width, height);     //scale it back
            }
        }
        //now try to fit a texture of this new size into some RAM pool
        if (bitTest(reg->flags, TRF_Paletted))
        {                                                   //if paletted texture
            for (x = 0; x < trNumberRamPools; x++)
            {
                if (trRamPoolList[x].used + size <= trRamPoolList[x].length)
                {                                           //if it'll fit in this one
                    trRamPoolList[x].used += size;          //update this pools used amount
                    used += size;                           //update total used amount
                    break;                                  //stop running through the pools
                }
                else
                {
                    //unable to fit texture in any slot, keep calculating total size anyhow
                    used += size;
                }
            }
        }
        else
        {                                                   //else an RGB texture
            for (y = 0; y < reg->nPalettes; y++)
            {                                               //do a multiple of times because RGB textures need to be duplicated
                for (x = 0; x < trNumberRamPools; x++)
                {
                    if (trRamPoolList[x].used + size <= trRamPoolList[x].length)
                    {                                       //if it'll fit in this one
                        trRamPoolList[x].used += size;      //update this pools used amount
                        used += size;                       //update total used amount
                        goto itFitFine;                     //stop running through the pools
                    }
                }
                //unable to fit texture in any slot, keep calculating total size anyhow
#if TR_ERROR_CHECKING
                dbgFatalf(DBG_Loc, "\ntrCramIntoRAM: Ran out of texture RAM!");
#endif
itFitFine:;
            }
        }
    }
}

#ifdef _X86_64
void trListFileLoadAndConvertTo64Bit(char *fileName, void** loadAddress)
{
    int i;

    int loopvar;
    int loopcount;

    llfileheader *oldHeader = NULL;
    llfileheader *newHeader = NULL;

    llelement_disk *oldElement = NULL;
    llelement *newElement = NULL;

    memsize offset=0;
    memsize loopsize=0;

    void * oldptr = NULL;
    void * newptr = NULL;
    void * oldbase = NULL;
    void * newbase = NULL;
    void * tmpptr = NULL;

    int oldLength;
    int newLength;

    udword *oldShare = NULL;
    udword *newShare = NULL;

    oldLength = fileLoadAlloc(fileName, (void **)&oldbase, NonVolatile);

    oldHeader = oldptr = oldbase;

    //printf("%s size is %d\n", fileName, oldLength);

    newLength = 2*oldLength;

    newbase = memAlloc(2*oldLength, "texreg 64bit conversion", 0);
    newHeader = newbase;
    newptr =  (void*) newbase;

    memset (newbase, 0 ,2*oldLength);

    offset=0;

    strncpy(newHeader->ident, oldHeader->ident,8);
    newHeader->version = oldHeader->version;
    newHeader->nElements = oldHeader->nElements;
    newHeader->stringLength = oldHeader->stringLength;
    newHeader->sharingLength = oldHeader->sharingLength;
    newHeader->totalLength = oldHeader->totalLength + oldHeader->sharingLength;

    offset +=sizeof(llfileheader);
    newptr +=sizeof(llfileheader);

    tmpptr=oldbase + 28 + ( newHeader->nElements * sizeof(llelement_disk));

    oldElement= oldbase+ /*sizeof(llfileheader)*/ 28/*old llfileheader*/; 
    newElement= newbase+ sizeof(llfileheader); 

    for (i=0;i<newHeader->nElements;i++){
        newElement->textureName = (char*)((memsize)oldElement->textureName); 
        newElement->width = oldElement->width; 
        newElement->height = oldElement->height; 
        newElement->flags = oldElement->flags; 
        newElement->imageCRC = oldElement->imageCRC; 
        newElement->nShared = oldElement->nShared; 
        if (oldElement->sharedTo != -1){    
    //        printf("%x: pointer to %x\n",i,oldElement->sharedTo);
            newElement->sharedTo = (sdword*)((memsize)oldElement->sharedTo); 
        }
        else {
            newElement->sharedTo = (sdword *)-1;
        }
        if (oldElement->sharedFrom != -1){    
     //       printf("%x:pointer to parent %x\n",i,oldElement->sharedFrom);
            newElement->sharedFrom = oldElement->sharedFrom; 
        }
        else{
            newElement->sharedFrom = -1;
        }

        //printf("%5d: %s: shared:%2d sharedTo: %5d sharedFrom:%5d\n",i, (memsize)tmpptr + (memsize)newElement->textureName, newElement->nShared,  newElement->sharedTo, newElement->sharedFrom); 

        offset += sizeof(llelement);
        newptr += sizeof(llelement);
        oldElement++;
        newElement++;

    }

    oldptr = (void*)oldElement;
    memcpy(newptr,oldptr,newHeader->stringLength);
    offset+=newHeader->stringLength;
    newptr+=newHeader->stringLength;

    loopcount= oldHeader->sharingLength / 4;

    
    oldShare=(udword*)((memsize)oldptr +newHeader->stringLength) ;
    newShare=(sdword*)newptr;

    for(loopvar = 0; loopvar<loopcount; loopvar++){
        newShare[loopvar] = oldShare[loopvar] ;
    }

    offset+=newHeader->sharingLength;
    newptr+=newHeader->sharingLength;

    newHeader->totalLength = offset - sizeof(llfileheader);


    memFree(oldbase);
    
    *loadAddress = newbase;
}
#endif



/*-----------------------------------------------------------------------------
    Name        : trListFileLoad
    Description : Loads in the named liflist file.
    Inputs      : name - name of list file to load
                  number - parameter to recieve the number of elements
    Outputs     :
    Return      : pointer to the .lif listing
    Note        : the element list can be freed by memFree
----------------------------------------------------------------------------*/
llelement *trListFileLoad(char *name, sdword *number)
{
    llfileheader header;
    sdword index;
    llelement *list;
    char *stringBlock, *sharingBlock;
    ubyte* loadAddress;

#ifdef _X86_64
    trListFileLoadAndConvertTo64Bit(name, (void **)&loadAddress);
#else
    fileLoadAlloc(name, (void **)&loadAddress, NonVolatile);
#endif

    memcpy(&header, loadAddress, sizeof(llfileheader));

#if FIX_ENDIAN
    header.version       = FIX_ENDIAN_INT_32( header.version );
    header.nElements     = FIX_ENDIAN_INT_32( header.nElements );
    header.stringLength  = FIX_ENDIAN_INT_32( header.stringLength );
    header.sharingLength = FIX_ENDIAN_INT_32( header.sharingLength );
    header.totalLength   = FIX_ENDIAN_INT_32( header.totalLength );
#endif

#if TR_ERROR_CHECKING
    if (strcmp(header.ident, LL_FileIdentifier) != 0)
    {
        dbgFatalf(DBG_Loc, "Incorrect identifier string: '%s', not '%s' as it should be.", header.ident, LL_FileIdentifier);
    }
    if (header.version != LL_FileVersion)
    {
        dbgFatalf(DBG_Loc, "Incorrect version 0x%x, expected 0x%x.", header.version, LL_FileVersion);
    }
    if (header.nElements < 0 || header.nElements >= LL_MaxElements)
    {
        dbgFatalf(DBG_Loc, "Unexpected number of elements: %d", header.nElements);
    }
    dbgAssertOrIgnore((udword)header.totalLength > header.nElements * sizeof(llelement));
#endif
    //allocate the list and string block
    list = memAlloc(header.totalLength, "LiFList", 0);
    stringBlock = (char *)list + header.nElements * sizeof(llelement);
    //read in the existing file to the element list/string table
    memcpy(list, loadAddress + sizeof(llfileheader), header.totalLength);

    //fix up the names of the texture files
    for (index = 0; index < header.nElements; index++)
    {
#if FIX_ENDIAN
        list[index].textureName = ( char *)FIX_ENDIAN_INT_32( ( udword )list[index].textureName );
        list[index].width       = FIX_ENDIAN_INT_32( list[index].width );
        list[index].height      = FIX_ENDIAN_INT_32( list[index].height );
        list[index].flags       = FIX_ENDIAN_INT_32( list[index].flags );
        list[index].imageCRC    = FIX_ENDIAN_INT_32( list[index].imageCRC );
        list[index].nShared     = FIX_ENDIAN_INT_32( list[index].nShared );
        list[index].sharedTo    = ( sdword *)FIX_ENDIAN_INT_32( ( udword )list[index].sharedTo );
        list[index].sharedFrom  = FIX_ENDIAN_INT_32( list[index].sharedFrom );
#endif

        list[index].textureName += (memsize)stringBlock;
    }
    //fix up any sharing pointers there may be
    if (header.sharingLength != 0)
    {
        sharingBlock = (char *)stringBlock + header.stringLength;
        for (index = 0; index < header.nElements; index++)
        {
            if (list[index].nShared != 0)
            {
//                (ubyte *)list[index].sharedTo += (udword)sharingBlock;
                list[index].sharedTo = (sdword *)((memsize)sharingBlock + (ubyte *)list[index].sharedTo);

#if FIX_ENDIAN
                // anonymous block so I can declare i with limited scope and not have
                // a plain C compiler complain
                {
                    int  i = 0;
                    for( i = 0; i < list[index].nShared; i++ )
                    {
                        list[index].sharedTo[i] = FIX_ENDIAN_INT_32( list[index].sharedTo[i] );
                    }
                }
#endif
            }
        }
    }
    memFree(loadAddress);
    //return some info
    *number = header.nElements;
    return(list);
}

/*-----------------------------------------------------------------------------
    Name        : trImageMeasureFromListing
    Description : Get the dimensions of a named texture from the specified listing file
    Inputs      : name - name to look for
                  list - list of textures to use
                  length - length of the list
    Outputs     : width, height, flags - where to store the dimensions and flags, if found
                  sharedFrom - name of texture we're shared from, or NULL
    Return      : TRUE if found, FALSE otherwise
----------------------------------------------------------------------------*/
bool32 trImageMeasureFromListing(char *name, llelement *list, sdword listLength, sdword *width, sdword *height, udword *flags, char **sharedFrom)
{
    sdword base = 0, index, length = listLength, result;
    char ch, name_cpy[PATH_MAX];
    unsigned int i;

    /* I've successfully gone and crapped all over the file names by mixing in
       forward slashes with backward slashes... */
    for (i = 0; (ch = name[i]); i++)
        name_cpy[i] = (ch == '/' ? '\\' : ch);
    name_cpy[i] = '\0';

    while (length >= 1)
    {
        index = length / 2 + base;                          //get centre of section
        dbgAssertOrIgnore(index >= 0 && index < listLength);
        result = strcasecmp(name_cpy, list[index].textureName);
        if (result == 0)
        {                                                   //if exact match
            *width = list[index].width;
            *height = list[index].height;
            *flags = list[index].flags;
            if (list[index].sharedFrom < 0)
            {
                *sharedFrom = NULL;
            }
            else
            {
                *sharedFrom = list[list[index].sharedFrom].textureName;
            }
            return(TRUE);                                   //done searching
        }
        if (result < 0)
        {                                                   //if low half of section
            length = index - base;
        }
        else
        {
            if (length <= 1)
            {
                return(FALSE);                              //fails if it gets here
            }
            length = length + base - index;
            base = index;
        }
    }
    return(FALSE);
}

/*-----------------------------------------------------------------------------
    Name        : trSharedFilenameCreate
    Description : Make the filename of the indexed texture sharing-compatable.
    Inputs      : trIndex - index of texreg structure
                  lifListing - .ll file
                  listingLength - length of .ll file
    Outputs     : the indexed texreg will have the filename of the share parent
                    added to the end of the regular filename.
    Return      : void
----------------------------------------------------------------------------*/
void trSharedFilenameCreate(sdword trIndex, llelement *lifListing, sdword listingLength)
{
    sdword width, height;
    udword listFlags;
    char *pSharedFrom, *newFilename;
    bool32 bResult;
    texreg *reg = trStructure(trIndex);

    bResult = trImageMeasureFromListing(reg->fileName, lifListing, listingLength, &width, &height, &listFlags, &pSharedFrom);
    dbgAssertOrIgnore(bResult);                                     //get the share parent name from the .ll file
    dbgAssertOrIgnore(pSharedFrom != NULL);
    newFilename = memAlloc(strlen(reg->fileName) + strlen(pSharedFrom) + 2, "SharedFileName", NonVolatile);
    strcpy(newFilename, reg->fileName);                     //create the concatenated names
    strcpy((char *)memchr(newFilename, 0, SWORD_Max) + 1, pSharedFrom);
    memFree(reg->fileName);                                 //kill the old name string
    reg->fileName = newFilename;                            //remember new file name
    bitSet(reg->flags, TRF_SharedFileName);
}

/*-----------------------------------------------------------------------------
    Name        : trLiFMeasure
    Description : Measure a .LiF file by loading it's header.
    Inputs      : fileName - name of .LiF file
    Outputs     : width, height, flags - out parameters for the file attributes
    Return      : TRUE if the file measured OK, FALSE otherwise.
----------------------------------------------------------------------------*/
bool32 trLiFMeasure(char *fileName, sdword *width, sdword *height, udword *flags)
{
    filehandle handle;
    lifheader header;

    if (!fileExists(fileName, 0))
    {
        return(FALSE);
    }
    handle = fileOpen(fileName, 0);
    fileBlockRead(handle, &header, sizeof(lifheader));
    fileClose(handle);

#if FIX_ENDIAN
    header.version     = FIX_ENDIAN_INT_32( header.version );
    header.flags       = FIX_ENDIAN_INT_32( header.flags );
    header.width       = FIX_ENDIAN_INT_32( header.width );
    header.height      = FIX_ENDIAN_INT_32( header.height );
    header.paletteCRC  = FIX_ENDIAN_INT_32( header.paletteCRC );
    header.imageCRC    = FIX_ENDIAN_INT_32( header.imageCRC );
    header.data        = ( ubyte *)FIX_ENDIAN_INT_32( ( udword )header.data );
    header.palette     = ( color *)FIX_ENDIAN_INT_32( ( udword )header.palette );
    header.teamEffect0 = ( ubyte *)FIX_ENDIAN_INT_32( ( udword )header.teamEffect0 );
    header.teamEffect1 = ( ubyte *)FIX_ENDIAN_INT_32( ( udword )header.teamEffect1 );
#endif

    if (strcmp(header.ident, LIF_FileIdentifier))
    {                                                       //verify proper file
        return(FALSE);
    }
    if (header.version != LIF_FileVersion)
    {
        return(FALSE);
    }
    *width = header.width;
    *height = header.height;
    *flags = header.flags;
    return(TRUE);
}

/*-----------------------------------------------------------------------------
    Name        : trRegistryRefresh
    Description : Refresh the texture registry, deleting old textures and
                    loading in new ones.
    Inputs      : void
    Outputs     : See description.
    Return      : void
    Note        : Nothing is currently done to ensure textures are memory
                    mapped the same size as they were in previous levels.
----------------------------------------------------------------------------*/
void trRegistryRefresh(void)
{
    sdword index, width, height, scaleFactor, texes;
    char fullName[PATH_MAX];
    char *pSharedFrom;
    texreg *reg;
    llelement *lifListing;
    sdword listingLength;
    udword listFlags;

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
        return;
#endif

#if TR_VERBOSE_LEVEL >= 1
    dbgMessagef("trRegistryRefresh: Refreshing up to %d textures", trHighestAllocated);
#endif  //TR_VERBOSE_LEVEL
    trTextureDeleteAllUnregistered();                       //start by getting rid of dead weight

    //allocate the texture size sort list
    dbgAssertOrIgnore(trSizeSortList == NULL);
    trSizeSortList = memAlloc((trHighestAllocated + 1) * sizeof(trmemmap), "Texture Size Sort List", 0);
    trSizeSortLength = 0;

    //allocate the mesh reference sort list
    trMeshSortLength = 0;
    trMeshSortIndex = 0;
    trMeshSortList = NULL;

    //load in the .ll file for sharing and sizing purposes
    lifListing = trListFileLoad(TR_LifListing, &listingLength);

    //sharing handled
    //this pass will ensure that all shared textures are consitent in color with the textures from which they are shared.
    //texture that are inconsitent will be made pending.
    for (index = 0; index <= trHighestAllocated; index++)
    {                                                       //for all loaded textures
        if (trAllocated(index))
        {                                                   //texture slot not free
            reg = &trTextureRegistry[index];                //get pointer to current texture reg struct
            if (reg->sharedFrom != TR_NotShared)
            {                                               //if this texture is shared from another texture
                if (!trAllColorsEqual(index, reg->sharedFrom))
                {                                           //if the shared texture is different colors from the texture it shares
                    //set this texture as pending
                    dbgAssertOrIgnore(trAllocated(reg->sharedFrom));
                    dbgAssertOrIgnore(trTextureRegistry[reg->sharedFrom].nSharedTo > 0);
                    trTextureRegistry[reg->sharedFrom].nSharedTo--;
                    reg->sharedFrom = TR_NotShared;         //no longer shared
                    trSetPending(index);                    //make it get loaded normally
                    trSharedFilenameCreate(index, lifListing, listingLength);//make the shared filename
                }
            }
        }
    }
    HorseRaceBeginBar(TEXTURE1_BAR);
    //now run through list and get image size of all pending images
    for (index = 0; index <= trHighestAllocated; index++)
    {                                                       //for all loaded textures
        if (trAllocated(index))
        {                                                   //texture slot not free
            reg = &trTextureRegistry[index];                //get pointer to current texture reg struct
            if (trPending(index))
            {                                               //if pending a load
                //... get width/height/flags trying several methods
                if (!trImageMeasureFromListing(reg->fileName, lifListing, listingLength, &width, &height, &listFlags, &pSharedFrom))
                {
#if TR_VERBOSE_LEVEL >= 1
                    dbgMessagef("Image '%s' not found in listing", reg->fileName);
#endif
                    strcpy(fullName, reg->fileName);
                    strcat(fullName, ".LiF");
                    if (trLiFMeasure(fullName, &width, &height, &listFlags))
                    {
#if TR_VERBOSE_LEVEL >= 1
                        dbgMessagef("...regular .LiF", reg->fileName);
#endif
                        reg->diskWidth = (sword)width;
                        reg->diskHeight = (sword)height;
#if TR_ERROR_CHECKING
                        if ((reg->flags & TRM_CompareFlags) != (listFlags & TRM_CompareFlags))
                        {
                            dbgWarningf(DBG_Loc, "Texture '%s' flags from listing inconsistent with expected.", reg->fileName);
                        }
#endif
                        reg->flags |= (uword)listFlags & TRM_ListFlag;//plug these flags straight in
                        if (bitTest(reg->flags, TRF_Alpha))
                        {                                   //if image says it's alpha
                            bitClear(reg->flags, TRF_Paletted); //it can't be paletted
                        }
                        else
                        {
                            bitSet(reg->flags, TRF_Paletted);
                        }
                    }
                    else
                    {
                        dbgMessagef("Cannot open '%s'. index=%d", fullName,index);
                        dbgFatalf(DBG_Loc, "Cannot open '%s'. index=%d", fullName,index);
                    }
                }
                else
                {                                           //else the list search measurment worked
                    reg->diskWidth = (sword)width;
                    reg->diskHeight = (sword)height;
                    reg->flags |= (ubyte)(listFlags & TRM_ListFlag);//plug these flags straight in
                    if (pSharedFrom == NULL)
                    {                                       //this texture not shared
                        reg->sharedFrom = TR_NotShared;
                    }
                    else
                    {                                       //this texture is shared
                        reg->sharedFrom = trFindTextureIndexByName(pSharedFrom);//find what texture it's shared to
                        if (reg->sharedFrom == TR_NotShared)
                        {                                   //the share parent is not loaded
                            trSharedFilenameCreate(index, lifListing, listingLength);//make the shared filename
#if TR_VERBOSE_LEVEL >= 2
                            dbgMessagef("Texture '%s' invalid for sharedFrom of '%s'.", pSharedFrom, reg->fileName);
#endif
                        }
                        else if (!trAllColorsEqual(index, reg->sharedFrom))
                        {                                   //if these textures do not share common texture colors
#if TR_VERBOSE_LEVEL >= 2
                            if (reg->sharedFrom == TR_NotShared)
                            {
                                dbgMessagef("Texture '%s' has different colors than '%s'", pSharedFrom, reg->fileName);
                            }
#endif
                            reg->sharedFrom = TR_NotShared; //can't allow this case
                            trSharedFilenameCreate(index, lifListing, listingLength);//make the shared filename
                        }
                        if (reg->sharedFrom != TR_NotShared)
                        {                                   //was it shared properly?
                            dbgAssertOrIgnore(trTextureRegistry[reg->sharedFrom].sharedFrom == TR_NotShared);//share parent must not be shared
                            dbgAssertOrIgnore(trTextureRegistry[reg->sharedFrom].nSharedTo < UBYTE_Max - 1);
                            trTextureRegistry[reg->sharedFrom].nSharedTo++;
                            //sharing handled - load a shared texture normally
                        }
                    }
#if TR_ERROR_CHECKING
                    if ((reg->flags & TRM_CompareFlags) != (listFlags & TRM_CompareFlags))
                    {
                        dbgWarningf(DBG_Loc, "Texture '%s' flags from header inconsistent with expected.", reg->fileName);
                    }
#endif
                    if (bitTest(reg->flags, TRF_Alpha))
                    {                                       //if image says it's alpha
                        bitClear(reg->flags, TRF_Paletted); //it can't be paletted
                    }
                    else
                    {
                        bitSet(reg->flags, TRF_Paletted);   //else it has to be paletted
                    }
                }
            }
            //only pending and pre-loaded textures get here
            dbgAssertOrIgnore(trAllocated(index));
            dbgAssertOrIgnore(trSizeSortLength <= trHighestAllocated);
            trSizeSortList[trSizeSortLength].index = index; //build unsorted list
            trSizeSortList[trSizeSortLength].width = reg->diskWidth;
            trSizeSortList[trSizeSortLength].height = reg->diskHeight;
            trSizeSortLength++;
            trMeshSortAdd(index, listFlags);                //add entry to mesh-sort list
        }

        if ((index & 31) == 0)
        {
            HorseRaceNext(((real32)index)/((real32)trHighestAllocated));
        }

        if (hrAbortLoadingGame)
        {
            break;
        }
    }
    memFree(lifListing);                                    //done with the listing, free it

    if (hrAbortLoadingGame)
    {
        goto abortloading;
    }

    //now that we have the lists, we can sort them
    qsort(trSizeSortList, trSizeSortLength, sizeof(trmemmap), trSizeSortCompare);

    //!!! important note:  the texture cramming operation uses an order inherently
    //  different from the one used in actual texture creation (size-sorted as
    //  opposed to mesh-sorted).  As such, it is possible that texture allocation
    //  could fail after all the textures have been supposedly crammed.  If this
    //  happens, this list could be arranged to cram textures in the same order
    //  as the mesh-sort lists.

    //fit all textures into allotted RAM
    scaleFactor = trCramRAMScaleCompute();
    trCramIntoRAM(scaleFactor);

    //now run through the mesh-sorted lists and load them in that order
    for (index = texes = 0; index < trMeshSortLength; index++)
    {
        if (trMeshSortList[index].nTextures)
            texes++;
    }
    HorseRaceBeginBar(TEXTURE2_BAR);  //texture barnumber = second parm
    for (index = 0; index < trMeshSortLength; index++)
    {                                                       //for each mesh-sort list
        HorseRaceNext(((real32)index)/((real32)texes));
        if (hrAbortLoadingGame)
        {
            break;
        }
        trMeshSortListSort(&trMeshSortList[index]);         //make sure alpha mapped textures are toward the end
        trMeshSortListLoad(&trMeshSortList[index]);         //load all textures for this list
    }
abortloading:
    //Horse Race temporary fix.  This needs to go after
    //the last bar is done loading.  HorseRaceNext will return true
    //after everyone has reached 100%

    //fix later!!!  This will hang if a player is dropped!
    //while(!HorseRaceNext(1.0f));

    //free the mesh reference sort lists
    for (index = trMeshSortLength - 1; index >= 0; index--)
    {                                                       //free all the sub-lists in reverse order of allocation
        memFree(trMeshSortList[index].textureList);
    }
    if (trMeshSortList != NULL)
    {
        memFree(trMeshSortList);                            //free the master list
    }

    //free the size-sorted list
    memFree(trSizeSortList);                                //free the size sort list
    trSizeSortList = NULL;                                  //and prevent any future references
    trSizeSortLength = 0;
    trCurrentHandle = TR_Invalid;
#if MEM_ANALYSIS
    memAnalysisCreate();
#endif

#if TR_TEXTURE_USAGE
    trTextureUsageList("texturesUsage.list");
#endif
}

/*-----------------------------------------------------------------------------
    Name        : trClearCurrent
    Description : clear the current texture (assign it to an invalid handle)
    Inputs      :
    Outputs     : trCurrentHandle is set to TR_InvalidHandle
    Return      :
----------------------------------------------------------------------------*/
void trClearCurrent()
{
    trCurrentHandle = TR_InvalidHandle;
}

/*-----------------------------------------------------------------------------
    Name        : trMakeCurrent
    Description : Make a given texture current
    Inputs      : handle - handle of texture/palette to use
    Outputs     : Makes the specified texture current, plus downloads palettes
                    as needed.
    Return      : void
    Note        : When in paletted mode, the integer part of reg->handle is
                    the GL handle of the indexed palette and palette is a set
                    of consecutive paletted for the differing teams.
                    When in non-paletted mode, or it's a non-paletted texture,
                    reg->handle is undefined and reg->palettes is a list of GL
                    handles which define the various colorized versions of the
                    texture.
----------------------------------------------------------------------------*/
void trMakeCurrent(trhandle handle)
{
    ubyte *newPalette;
    texreg *reg;

    //
    //GE01  Seem to be sent spurious texture handles due to the multiplayer options.
    // Print them then ignore them. :)  probably should wrap this further in build_for_debug tests.
    //
    if (handle != TR_Invalid && trIndex(handle) >= TR_RegistrySize){
#if TR_ERROR_CHECKING
        dbgMessagef("%s: sent invalid trhandle: 0x%lx", __func__, handle);
#endif
        return;
    }

#if TR_NIL_TEXTURE
    if (GLOBAL_NO_TEXTURES)
    {
        rndTextureEnable(FALSE);
        return;
    }
#endif
    if (handle == TR_Invalid)
    {
        rndTextureEnable(FALSE);
        return;
    }
    if (trCurrentHandle == handle)
    {                                                       //if this texture already enabled
        trAvoidedChanges++;
        return;                                             //don't do anything
    }
    trTextureChanges++;
    trCurrentHandle = handle;                               //save the current handle
    reg = trStructure(handle);                              //get pointer to registry entry
    if (reg->sharedFrom != TR_NotShared)
    {                                                       //is this a shared texture
#if TR_VISUALIZE_SHARING
        if (keyIsHit(SKEY) && keyIsHit(TKEY))
        {                                                   //texture sharing debuggery
            rndTextureEnable(FALSE);
            return;
        }
        else
#endif
        {
            reg = &trTextureRegistry[reg->sharedFrom];      //get texture it's shared from
        }
    }

#if TR_PRINT_TEXTURE_NAMES
    if (trPrintTextureNames)
    {
        dbgMessagef("trMakeCurrent: %s", reg->fileName);
    }
#endif
#if 0
    if (trSpecialTextures)
    {
        if (keyIsStuck(LBRACK))
        {
            keyClearSticky(LBRACK);
            trSpecialTextureMode = max(TSM_None, trSpecialTextureMode - 1);
            dbgMessagef("%s texture mode.", trSpecialTextureName[trSpecialTextureMode]);
        }
        if (keyIsStuck(RBRACK))
        {
            keyClearSticky(RBRACK);
            trSpecialTextureMode = min(TSM_NumberModes - 1, trSpecialTextureMode + 1);
            dbgMessagef("%s texture mode.", trSpecialTextureName[trSpecialTextureMode]);
        }
        if (trSpecialTextures && trSpecialTextureMode != TSM_None)
        {
            dbgAssertOrIgnore(trSpecialTextureMode < TSM_NumberModes);
            dbgAssertOrIgnore(trSpecialTextureMode > TSM_None);
            switch (trSpecialTextureMode)
            {
                case TSM_Uncolored:
                    glBindTexture(GL_TEXTURE_2D, reg->uncolored);
                    if (bitTest(reg->flags, TRF_Paletted))
                    {
                        trColorTable(reg->uncoloredPalette);
                    }
                    break;
                case TSM_TeamColor0:
                    glBindTexture(GL_TEXTURE_2D, reg->col0Handle);
                    if (bitTest(reg->flags, TRF_Paletted))
                    {
                        trColorTable(trTestPalette0);
                    }
                    break;
                case TSM_TeamColor1:
                    glBindTexture(GL_TEXTURE_2D, reg->col1Handle);
                    if (bitTest(reg->flags, TRF_Paletted))
                    {
                        trColorTable(trTestPalette0);
                    }
                    break;
            }
            primErrorMessagePrint();
            return;
        }
    }
#endif //TR_DEBUG_TEXTURES

    if (bitTest(reg->flags, TRF_Paletted))
    {                                                       //if we should use palettes
        if ((udword)reg->currentPalette != trPaletteIndex(handle))
        {
            reg->currentPalette = (sword)trPaletteIndex(handle);
            dbgAssertOrIgnore((sdword)trPaletteIndex(handle) < reg->nPalettes);
            newPalette = &reg->palettes[trPaletteIndex(handle) * TR_PaletteSize];
        }
        else
        {
            dbgAssertOrIgnore(trPaletteIndex(handle) < (udword)reg->nPalettes);
            newPalette = &reg->palettes[trPaletteIndex(handle) * TR_PaletteSize];
        }
        if (bitTest(reg->flags, TRF_NoPalPending))
        {
            trNoPalTextureRecreate(newPalette, reg->handle);
        }
        trNoPalMakeCurrent(newPalette, reg->handle);
        primErrorMessagePrint();
        glDisable(GL_ALPHA_TEST);                           //ditto
    }
    else
    {                                                       //else it's an non-paletted texture
        if (bitTest(reg->flags, TRF_Alpha))
        {                                                   //and has alpha
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
        }
        else
        {                                                   //else turning alpha off
            glDisable(GL_ALPHA_TEST);
        }
        //bind the proper version of this texture
        if (reg->nPalettes > 1)
        {
            glBindTexture(GL_TEXTURE_2D,                //set new texture
                          ((udword *)reg->palettes)[trPaletteIndex(handle)]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, reg->handle);  //set new texture
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : trFilterEnable
    Description : Enables/disables texture filtering
    Inputs      : bEnable - TRUE to enable filtering
    Outputs     : Makes each texture current and changes it's filtering mode
    Return      :
----------------------------------------------------------------------------*/
void trFilterEnable(sdword bEnable)
{
    sdword index, j;
    texreg *reg;

    for (index = 0; index < TR_RegistrySize; index++)
    {
        if (trAllocated(index) && (!trPending(index)) && (trTextureRegistry[index].sharedFrom == TR_NotShared))
        {                                                   //if we can change this guy
            reg = &trTextureRegistry[index];

            if (bitTest(reg->flags, TRF_Paletted) || reg->nPalettes == 1)
            {                                               //if paletted image
                if (bitTest(reg->flags, TRF_Paletted))
                {
                    trNoPalFilter(bEnable, reg->handle);
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, reg->handle);  //set texture for modification
                    if (bEnable)
                    {                                           //set min/mag filters to linear
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    }
                    else
                    {                                           //or point sampling
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    }
                }
            }
            else
            {
                if (reg->palettes != NULL)
                {
                    for (j = 0; j < reg->nPalettes; j++)
                    {                                       //for each sub-texture
                        glBindTexture(GL_TEXTURE_2D, ((udword *)reg->palettes)[j]);
                        if (bEnable)
                        {                                   //set min/mag filters to linear
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        }
                        else
                        {                                   //or point sampling
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        }
                    }
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, reg->handle);
                    if (bEnable)
                    {                                       //set min/mag filters to linear
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    }
                    else
                    {                                       //or point sampling
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    }
                }
            }
        }
    }
    texLinearFiltering = bEnable;                           //set new filter mode flag

    trClearCurrent();
}

/*-----------------------------------------------------------------------------
    Name        : trStructureGet
    Description : Get a texreg structure from a handle
    Inputs      : handle - valid texture handle
    Outputs     :
    Return      : structure
----------------------------------------------------------------------------*/
texreg *trStructureGet(trhandle handle)
{
    texreg *reg = trStructure(handle);

    //sharing handled
    if (reg->sharedFrom != TR_NotShared)
    {
        reg = trStructure(reg->sharedFrom);
    }
    return(reg);
}

/*-----------------------------------------------------------------------------
    Name        : trSetAllPending
    Description : ...
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trSetAllPending(bool32 freeNoPal)
{
    sdword index;

    for (index = 0; index <= trHighestAllocated; index++)
    {                                                       //for all textures in allocated range
        if (trAllocated(index))
        {                                                   //if texture allocated
            if (trTextureRegistry[index].nSharedTo > 0)
            {                                               //if this texture is shared to otherTextures
                trAllSharedFromDelete(index, TRUE);
            }
            if (!trPending(index))
            {
                trInternalTexturesDelete(index);
                trSetPending(index);
                if (!freeNoPal)
                {
                    //clear nopal pending flag.
                    //this texture isn't pending, it doesn't exist
                    bitClear(trTextureRegistry[index].flags, TRF_NoPalPending);
                }
                if (trTextureRegistry[index].sharedFrom != TR_NotShared)
                {                                               //if shared from another
                    dbgAssertOrIgnore(trTextureRegistry[index].nSharedTo == 0);
                    dbgAssertOrIgnore(trAllocated(trTextureRegistry[index].sharedFrom));
                    dbgAssertOrIgnore(trTextureRegistry[trTextureRegistry[index].sharedFrom].nSharedTo > 0);
                    trTextureRegistry[trTextureRegistry[index].sharedFrom].nSharedTo--;
                    trTextureRegistry[index].sharedFrom = TR_NotShared;//remove this sharing link
                }
            }
        }
    }

    if (freeNoPal)
    {
        trNoPalReadjust();
    }
    else
    {
        trNoPalReadjustWithoutPending();
    }
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalSingleRepDelete
    Description : delete a single un-paletted rep
    Inputs      : handle - the np handle
                  index - index within the np struct
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalSingleRepDelete(udword handle, sdword index)
{
    nopalreg* reg;

    dbgAssertOrIgnore(trNoPalAllocated(handle));

    reg = trNoPalStructure(handle);
    glDeleteTextures(1, (GLuint*)&reg->glhandle[index]);
    trNoPalBytesAllocated -= 4 * reg->width * reg->height;
    reg->glhandle[index] = 0;
    reg->crc[index] = 0;
    reg->timeStamp[index] = 0.0f;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalTextureDelete
    Description : wipe out a np struct - reps and data alike
    Inputs      : handle - the np handle
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalTextureDelete(udword handle)
{
    nopalreg* reg;
    sdword index;

    if (!trNoPalAllocated(handle))
    {
        return;
    }

    reg = trNoPalStructure(handle);
    for (index = 0; index < trNpNumHandles; index++)
    {
        if (reg->glhandle[index] != 0)
        {
            trNoPalSingleRepDelete(handle, index);
        }
    }

    if (reg->data != NULL)
    {
        memFree(reg->data);
        reg->data = NULL;
        trNoPalBytesAllocated -= reg->width * reg->height;
    }

    reg->texreghandle = TR_InvalidInternalHandle;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalTextureDeleteFromTexreg
    Description : flag the parent texreg w/ TRF_NoPalPending and delete all
                  un-paletted reps
    Inputs      : handle - the np handle
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalTextureDeleteFromTexreg(udword handle)
{
    nopalreg* reg;
    sdword index;

    dbgAssertOrIgnore(trNoPalAllocated(handle));

    reg = trNoPalStructure(handle);

    dbgAssertOrIgnore(reg->texreghandle != TR_InvalidInternalHandle);

    for (index = 0; index < trNpNumHandles; index++)
    {
        if (reg->glhandle[index] != 0)
        {
            trNoPalSingleRepDelete(handle, index);
            //only set pending if there's something to pend
            bitSet(trStructure(reg->texreghandle)->flags, TRF_NoPalPending);
        }
    }
}

void trNoPalTextureDeleteFromTexregWithoutPending(udword handle)
{
    nopalreg* reg;
    sdword index;

    dbgAssertOrIgnore(trNoPalAllocated(handle));

    reg = trNoPalStructure(handle);

    dbgAssertOrIgnore(reg->texreghandle != TR_InvalidInternalHandle);

    for (index = 0; index < trNpNumHandles; index++)
    {
        if (reg->glhandle[index] != 0)
        {
            trNoPalSingleRepDelete(handle, index);
        }
    }
}

#define NP_MEMFLAGS NonVolatile|ExtendedPool

/*-----------------------------------------------------------------------------
    Name        : trNoPalQueueStartup
    Description : startup the lra queue of np handles
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalQueueStartup(void)
{
    trNoPalQueueHead = 0;
    trNoPalQueueTail = 0;
    trNoPalQueueSize = TR_RegistrySize;
    trNoPalQueue = (udword*)memAlloc(trNoPalQueueSize * sizeof(udword), "nopal queue", NP_MEMFLAGS);
    memset(trNoPalQueue, 0, trNoPalQueueSize * sizeof(udword));
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalQueueReset
    Description : reset the lra queue of np handles
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalQueueReset(void)
{
    trNoPalQueueHead = 0;
    trNoPalQueueTail = 0;
    memset(trNoPalQueue, 0, trNoPalQueueSize * sizeof(udword));
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalQueueShutdown
    Description : shutdown the lra queue of np handles
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalQueueShutdown(void)
{
    memFree(trNoPalQueue);
    trNoPalQueueTail = 0;
    trNoPalQueueHead = 0;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalQueueFreeNext
    Description : free next item in the queue
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalQueueFreeNext(void)
{
    if (trNoPalQueueHead == trNoPalQueueTail)
    {
        //0-length queue
        return;
    }

    //free tail item
    if (trNoPalAllocated(trNoPalQueue[trNoPalQueueTail]))
    {
        trNoPalTextureDeleteFromTexreg(trNoPalQueue[trNoPalQueueTail]);
    }

    //move tail up
    trNoPalQueueTail = ADJ_NPQUEUE(trNoPalQueueTail+1);
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalQueueAdd
    Description : add an np texture to the queue
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalQueueAdd(udword handle)
{
    sdword head, targetBytes, prevBytes;

    //full-circle ?
    if (ADJ_NPQUEUE(trNoPalQueueHead+1) == trNoPalQueueTail)
    {
        trNoPalQueueFreeNext();
    }

    head = trNoPalQueueHead;
    trNoPalQueueHead = ADJ_NPQUEUE(trNoPalQueueHead+1);

    //need to make some room ?
    if (trNoPalBytesAllocated > trNoPalMaxBytes)
    {
        prevBytes = trNoPalBytesAllocated;
        targetBytes = trNoPalBytesAllocated * 2 / 3;

        //ASSERT: this will never be an infinite loop
        while (trNoPalBytesAllocated > targetBytes)
        {
            trNoPalQueueFreeNext();
        }

#ifdef HW_BUILD_FOR_DEBUGGING
        dbgMessagef("** nopal freed %dMB of textures **",
                    (prevBytes - trNoPalBytesAllocated) >> 20);
#endif
    }

    //add to head
    trNoPalQueue[head] = handle;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalReadjust
    Description : free all np textures, clear the lra queue
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalReadjust(void)
{
    sdword index;

    dbgAssertOrIgnore(trNoPalInitialized);

#ifdef HW_BUILD_FOR_DEBUGGING
    dbgMessagef("** nopal freed %dMB of textures **", trNoPalBytesAllocated >> 20);
#endif

    for (index = 0; index < TR_RegistrySize; index++)
    {
        if (trNoPalAllocated(index))
        {
            trNoPalTextureDeleteFromTexreg(index);
        }
    }

    trNoPalQueueReset();
}

void trNoPalReadjustWithoutPending(void)
{
    sdword index;

    dbgAssertOrIgnore(trNoPalInitialized);

    dbgMessagef("** nopalw/o freed %dMB of textures **", trNoPalBytesAllocated >> 20);

    for (index = 0; index < TR_RegistrySize; index++)
    {
        if (trNoPalAllocated(index))
        {
            trNoPalTextureDeleteFromTexregWithoutPending(index);
        }
    }

    trNoPalQueueReset();
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalGetHandle
    Description : obtain a fresh np handle
    Inputs      :
    Outputs     :
    Return      : an np handle
----------------------------------------------------------------------------*/
udword trNoPalGetHandle(void)
{
    udword handle;
    sdword index;

    //look for a handle in a fragmented pool.
    //this is less than efficient
    for (index = 0; index < trNoPalHighestAllocated; index++)
    {
        if (!trNoPalAllocated(index))
        {
            return index;
        }
    }

    handle = trNoPalHighestAllocated;

    trNoPalHighestAllocated++;
    dbgAssertOrIgnore(trNoPalHighestAllocated < TR_RegistrySize);

    return handle;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalTexImage
    Description : create an un-paletted version of a color index texture
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalTexImage(ubyte* data, ubyte* palette, sdword width, sdword height)
{
    ubyte* sp;
    ubyte* dp;
    sdword i, index;
    ubyte* rgba;
    uword* sdp;
    sdword r, g, b;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    rgba = memAlloc(4 * width * height, "temp rgba", Pyrophoric);

    dp = rgba;
    sp = data;
    for (i = 0; i < width*height; i++, dp += 4, sp++)
    {
        index = (*sp) << 2;
        dp[0] = palette[index + 0];
        dp[1] = palette[index + 1];
        dp[2] = palette[index + 2];
        dp[3] = palette[index + 3];
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

    if (texLinearFiltering)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    memFree(rgba);

    trNoPalBytesAllocated += 4 * width * height;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalTextureCreate
    Description :
    Inputs      : data - 8bit image
                  palette - initial palette for the image
                  width, height - image dimensions
                  texreghandle - parent texreg struct handle
    Outputs     :
    Return      : an np handle
----------------------------------------------------------------------------*/
udword trNoPalTextureCreate(ubyte* data, ubyte* palette, sdword width, sdword height, trhandle texreghandle)
{
    nopalreg* reg;
    udword handle;

    handle = trNoPalGetHandle();

    reg = trNoPalStructure(handle);

    dbgAssertOrIgnore(reg->data == NULL);

    //not pending
//    dbgAssertOrIgnore(!bitTest(trStructure(texreghandle)->flags, TRF_NoPalPending));

    reg->texreghandle = texreghandle;
    reg->timeStamp[0] = taskTimeElapsed;//universe.totaltimeelapsed;
    reg->crc[0] = (udword)palette;

    //save 8bit image for re-palettizing
    reg->data = (ubyte*)memAlloc(width*height, "nopal data", NP_MEMFLAGS);
    memcpy(reg->data, data, width*height);
    trNoPalBytesAllocated += width * height;

    reg->width = width;
    reg->height = height;

    //create a rep w/ given palette
//    dbgAssertOrIgnore(reg->glhandle[0] == 0);
    glGenTextures(1, (GLuint*)&reg->glhandle[0]);
    glBindTexture(GL_TEXTURE_2D, reg->glhandle[0]);
    trNoPalTexImage(data, palette, width, height);

    //add handle to lra queue
    trNoPalQueueAdd(handle);

    return handle;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalTextureRecreate
    Description : 8bit image data still exists but all un-paletted reps are gone,
                  so recreate one
    Inputs      : palette - image palette
                  handle - an np handle
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalTextureRecreate(ubyte* palette, udword handle)
{
    nopalreg* reg;

    reg = trNoPalStructure(handle);

    dbgAssertOrIgnore(reg->data != NULL);
    dbgAssertOrIgnore(reg->texreghandle != TR_InvalidInternalHandle);

    //no longer pending
    bitClear(trStructure(reg->texreghandle)->flags, TRF_NoPalPending);

    //create a rep
    reg->crc[0] = (udword)palette;
//    dbgAssertOrIgnore(reg->glhandle[0] == 0);
    glGenTextures(1, (GLuint*)&reg->glhandle[0]);
    glBindTexture(GL_TEXTURE_2D, reg->glhandle[0]);
    trNoPalTexImage(reg->data, palette, reg->width, reg->height);

    //add handle to lra queue
    trNoPalQueueAdd(handle);

    reg->timeStamp[0] = taskTimeElapsed;//universe.totaltimeelapsed;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalMakeCurrent
    Description : bind an appropriate version of an np texture to the GL.
                  if a version of the image w/ given palette doesn't yet exist,
                  create it
    Inputs      : palette - the image palette
                  handle - an np handle
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalMakeCurrent(ubyte* palette, udword handle)
{
    nopalreg* reg;
    udword newcrc;
    sdword index, minIndex;
    real32 minTime;

    dbgAssertOrIgnore(trNoPalAllocated(handle));

    reg = trNoPalStructure(handle);

    //check for existing rep
    newcrc = (udword)palette;
    for (index = 0; index < trNpNumHandles; index++)
    {
        if (newcrc == reg->crc[index])
        {
            glBindTexture(GL_TEXTURE_2D, reg->glhandle[index]);
            return;
        }
    }

    //find a free spot
    for (index = 0; index < trNpNumHandles; index++)
    {
        if (reg->glhandle[index] == 0)
        {
            reg->crc[index] = newcrc;
            glGenTextures(1, (GLuint*)&reg->glhandle[index]);
            glBindTexture(GL_TEXTURE_2D, reg->glhandle[index]);
            trNoPalTexImage(reg->data, palette, reg->width, reg->height);
            reg->timeStamp[index] = taskTimeElapsed;//universe.totaltimeelapsed;
            return;
        }
    }

    //out of handles, free a rep (lru)
    minTime  = reg->timeStamp[0];
    minIndex = 0;
    for (index = 1; index < trNpNumHandles; index++)
    {
        if (reg->timeStamp[index] < minTime)
        {
            minTime  = reg->timeStamp[index];
            minIndex = index;
        }
    }
    trNoPalSingleRepDelete(handle, minIndex);
    reg->crc[minIndex] = newcrc;
    glGenTextures(1, (GLuint*)&reg->glhandle[minIndex]);
    glBindTexture(GL_TEXTURE_2D, reg->glhandle[minIndex]);
    trNoPalTexImage(reg->data, palette, reg->width, reg->height);
    reg->timeStamp[minIndex] = taskTimeElapsed;//universe.totaltimeelapsed;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalStartup
    Description : initialize and allocate necessary things for un-palettizing textures
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalStartup(void)
{
    if (trNoPalInitialized)
    {
        return;
    }
    trNoPalRegistry = (nopalreg*)memAlloc(TR_RegistrySize * sizeof(nopalreg), "nopal registry", NP_MEMFLAGS);
    memset(trNoPalRegistry, 0, TR_RegistrySize * sizeof(nopalreg));
    trNoPalHighestAllocated = 0;
    trNoPalBytesAllocated = 0;

    trNoPalQueueStartup();

    trNoPalInitialized = TRUE;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalReset
    Description : clear the queue, delete all reps and data
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalReset(void)
{
    sdword index;

    if (!trNoPalInitialized)
    {
        return;
    }

    for (index = 0; index < TR_RegistrySize; index++)
    {
        if (trNoPalAllocated(index))
        {
            trNoPalTextureDeleteFromTexreg(index);//xxx
        }
    }

    trNoPalQueueReset();
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalShutdown
    Description : shutdown the un-palettizing structures
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalShutdown(void)
{
    sdword index;

//    ASSERT: trReset calls trNoPalReset
//    ASSERT: trShutdown calls trReset

    if (!trNoPalInitialized)
    {
        return;
    }

    for (index = 0; index < TR_RegistrySize; index++)
    {
        if (trNoPalAllocated(index))
        {
            trNoPalTextureDelete(index);
        }
    }

    memFree(trNoPalRegistry);
    trNoPalRegistry = NULL;
    trNoPalHighestAllocated = 0;
//    dbgAssertOrIgnore(trNoPalBytesAllocated == 0);

    trNoPalQueueShutdown();

    trNoPalInitialized = FALSE;
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalResizePool
    Description : resize the pool of un-paletted textures
    Inputs      : mb - size of pool in megabytes
    Outputs     : trNoPalMaxBytes is set
    Return      :
----------------------------------------------------------------------------*/
void trNoPalResizePool(sdword mb)
{
#ifdef HW_BUILD_FOR_DEBUGGING
    dbgMessagef("trNoPalResizePool: %dMB", mb);
#endif
    trNoPalMaxBytes = mb << 20;
    trNoPalReadjust();
}

/*-----------------------------------------------------------------------------
    Name        : trNoPalFilter
    Description : enable / disable filtering on an np texture
    Inputs      : bEnable - TRUE or FALSE
                  index - texreg handle
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void trNoPalFilter(sdword bEnable, sdword handle)
{
    nopalreg* reg;
    sdword index;

    dbgAssertOrIgnore(handle != TR_InvalidInternalHandle);

    if (!trNoPalAllocated(handle))
    {
        return;
    }

    reg = trNoPalStructure(handle);

    for (index = 0; index < trNpNumHandles; index++)
    {
        if (reg->glhandle[index] != 0)
        {
            glBindTexture(GL_TEXTURE_2D, reg->glhandle[index]);
            if (bEnable)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : trTextureUsageList
    Description : Print out a texture usage list, much like texreg used to do.
                    We're doing it here because packed textures will confuse the
                    liflist results.
    Inputs      : fileName - file to dump the usage to.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
#if TR_TEXTURE_USAGE
void trTextureUsageList(char *fileName)
{
    char *fileNameFull;
    FILE *fp;
    char directoryName[256], *pSlash;
    sdword index = 0;
    sdword size, directorySize = 0;
    sdword sharedSize, sharedDirectorySize = 0;
    texreg *reg;

    for (index = 0; index < trHighestAllocated && !trAllocated(index); index++)
    {
        ;
    }
    dbgAssertOrIgnore(trAllocated(index));
    strcpy(directoryName, trTextureRegistry[index].fileName);
    pSlash = directoryName;
#ifdef _WIN32
    while ((pSlash = strchr(pSlash + 1, '\\')) != NULL)
    {
        if (!strchr(pSlash + 1, '\\'))
#else
    while ((pSlash = strpbrk(pSlash + 1, "\\/")) != NULL)
    {
        if (!strpbrk(pSlash + 1, "\\/"))
#endif
        {
            *pSlash = 0;
            break;
        }
    }

    fileNameFull = filePathPrepend(fileName, FF_UserSettingsPath);

    if (!fileMakeDestinationDirectory(fileNameFull))
        return;

    fp = fopen(fileNameFull, "wt");
    if (fp == NULL)
    {
        return;
    }
    reg = trTextureRegistry + index;
    for (; index < trHighestAllocated; index++, reg++)
    {
        if (!trAllocated(index))
        {
            continue;
        }
        if (strncmp(directoryName, reg->fileName, strlen(directoryName)))
        {                                                   //is this a new name?
            fprintf(fp, "%12d %12d %s\n", directorySize, sharedDirectorySize, directoryName);
            strcpy(directoryName, reg->fileName);
            pSlash = directoryName;
#ifdef _WIN32
            while ((pSlash = strchr(pSlash + 1, '\\')) != NULL)
            {
                if (!strchr(pSlash + 1, '\\'))
#else
            while ((pSlash = strpbrk(pSlash + 1, "\\/")) != NULL)
            {
                if (!strpbrk(pSlash + 1, "\\/"))
#endif
                {
                    *pSlash = 0;
                    break;
                }
            }
            directorySize = 0;
            sharedDirectorySize = 0;
        }
        size = reg->diskWidth * reg->diskHeight;//size of texture
        if (bitTest(reg->flags, TRF_Alpha))
        {                                                   //account for alpha
            size *= sizeof(color);
            if (reg->flags & (TRF_TeamColor0 | TRF_TeamColor1))
            {                                               //account for texture duplication due to teams
                size *= MAX_MULTIPLAYER_PLAYERS;
            }
        }
        sharedSize = size;
        if (reg->sharedFrom != TR_NotShared)
        {                                                   //if this is a shared texture
            sharedSize = 0;
        }
        directorySize += size;
        sharedDirectorySize += sharedSize;
    }
    //print out the closing directory
    fprintf(fp, "%12d %12d %s\n", directorySize, sharedDirectorySize, directoryName);

    fclose(fp);
}
#endif //TR_TEXTURE_USAGE
