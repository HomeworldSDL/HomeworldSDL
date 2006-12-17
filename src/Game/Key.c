
#include <stdio.h>
#include <string.h>

#include "Debug.h"
#include "Demo.h"
#include "Task.h"
#include "Key.h"
#include "Options.h"
#include "ConsMgr.h"

/*=============================================================================
    Data:
=============================================================================*/
volatile keyScanType keyScanCode[KEY_TOTAL_KEYS];
volatile keyScanType keySaveScan[KEY_TOTAL_KEYS];
real32 keyLastTimeHit = 0.0f;

//key repeat buffer
ubyte keyNumberBufferKeys = 0;
keybufferchar keyBuffer[KEY_BufferLength];

uword keysToNotBuffer[] = {
    SHIFTKEY, LMOUSE_BUTTON, RMOUSE_BUTTON, MMOUSE_BUTTON,
    FLYWHEEL_UP, FLYWHEEL_DOWN, LMOUSE_DOUBLE, RMOUSE_DOUBLE,
    MMOUSE_DOUBLE,
    0};
/*=============================================================================
    Functions:
=============================================================================*/
/*-----------------------------------------------------------------------------
    Name        : keyInit
    Description : Start the keyboard module
    Inputs      : void
    Outputs     : clears the keyScanCode array
    Return      : void
----------------------------------------------------------------------------*/
void keyInit(void)
{
    keyClearAll();
}

/*-----------------------------------------------------------------------------
    Name        : keyClose
    Description : Does nothing
    Inputs      : ..
    Outputs     : ..
    Return      : ..
----------------------------------------------------------------------------*/
void keyClose(void)
{
    ;
}

/*-----------------------------------------------------------------------------
    Name        : keyPressDown
    Description : Called from Windows, sets the on and sticky bits of key
    Inputs      : key-index of key to set
    Outputs     : on and sticky bit set, repeat count updated
    Return      : void
----------------------------------------------------------------------------*/
void keyPressDown(udword key)
{
	bool bypass = FALSE;
	bool shift = FALSE;
	keyScanType originalKey;

    /* Game not specific about left or right Shift/Ctrl/Alt key, so neither
       are we. */
    if (key == SDLK_RSHIFT)
        key = SDLK_LSHIFT;
    else if (key == SDLK_RCTRL)
        key = SDLK_LCTRL;
    else if (key == SDLK_RALT)
        key = SDLK_LALT;
    else if (key == SDLK_RMETA) // Apple/Command/"Windows" key
        key = SDLK_LMETA;


	originalKey = keySaveScan[key];
#if KEY_ERROR_CHECKING
    dbgAssertOrIgnore(key < KEY_TOTAL_KEYS);
#endif
#ifdef gshaw
    dbgMessagef("Key pressed = %d",key);
#endif


    if (opKeyDetour)
    {
        //key redefinition in options screen

        switch (opKeyDetour)
        {
            case 1:
                opDefineThisKey((keyindex)key);
                bypass = TRUE;
                break;
            case 2:
                if (key == SHIFTKEY)
                {
                    bypass = FALSE;
                }
                else
                {
                    shift = keyIsHit(SHIFTKEY);

                    if (cmBuildHotKey((keyindex)key, shift))
                        bypass = TRUE;
                    else
                        bypass = FALSE;
                }
            //research is case 3!
        }
    }
    if (!bypass)
    {
        keyBufferAdd(key, keyIsHit(SHIFTKEY));
        keySaveScan[key].keypressed = 1;
        keySaveScan[key].keystick = 1;
        if (keySaveScan[key].keynumpressed < KEY_NUMPRESSED_MAX)
        {
            keySaveScan[key].keynumpressed++;
    #if KEY_VERBOSE_LEVEL >= 2
            dbgMessagef("keyRepeat: repeat count of 0x%x incremented to %d", key, keySaveScan[key].keynumpressed);
    #endif
        }
        if (!(demDemoRecording | demDemoPlaying))
        {
            keyScanCode[key] = keySaveScan[key];
        }
        else if (demDemoPlaying)
        {
            if (key == ESCKEY)
            {                                               //escape while playing back a demo:stop playing demo
                keySaveScan[key] = originalKey;             //don't register the keypress
                demPlayEnd();                               //stop playing the demo
            }
        }
    }
    keyLastTimeHit = taskTimeElapsed;
}

/*-----------------------------------------------------------------------------
    Name        : keyRepeat
    Description : Increment repeat count for a key
    Inputs      : key - index of key to increment repeat count of.
    Outputs     : increments repeat count, clamping at a max.  That's it.
    Return      :
----------------------------------------------------------------------------*/
void keyRepeat(udword key)
{
    /* Game not specific about left or right Shift/Ctrl/Alt key, so neither
       are we. */
    if (key == SDLK_RSHIFT)
        key = SDLK_LSHIFT;
    else if (key == SDLK_RCTRL)
        key = SDLK_LCTRL;
    else if (key == SDLK_RALT)
        key = SDLK_LALT;
    else if (key == SDLK_RMETA) // Apple/Command/"Windows" key
        key = SDLK_LMETA;

#if KEY_ERROR_CHECKING
    dbgAssertOrIgnore(key < KEY_TOTAL_KEYS);
#endif
    keyBufferAdd(key, keyIsHit(SHIFTKEY));
    if (keySaveScan[key].keynumpressed < KEY_NUMPRESSED_MAX)
    {
        keySaveScan[key].keynumpressed++;
#if KEY_VERBOSE_LEVEL >= 2
        dbgMessagef("keyRepeat: repeat count of 0x%x incremented to %d", key, keySaveScan[key].keynumpressed);
#endif
    }
    if (!(demDemoRecording | demDemoPlaying))
    {
        keyScanCode[key] = keySaveScan[key];
    }
    keyLastTimeHit = taskTimeElapsed;
}

/*-----------------------------------------------------------------------------
    Name        : keyPressUp
    Description : Called from windows, clears indexed key
    Inputs      : key-index of key to clear
    Outputs     : on bit and repeat count cleared, sticky bit remains
    Return      :
----------------------------------------------------------------------------*/
void keyPressUp(udword key)
{
    /* Game not specific about left or right Shift/Ctrl/Alt key, so neither
       are we. */
    if (key == SDLK_RSHIFT)
        key = SDLK_LSHIFT;
    else if (key == SDLK_RCTRL)
        key = SDLK_LCTRL;
    else if (key == SDLK_RALT)
        key = SDLK_LALT;
    else if (key == SDLK_RMETA) // Apple/Command/"Windows" key
        key = SDLK_LMETA;

    keySaveScan[key].keypressed = 0;
    keySaveScan[key].keynumpressed = 0;
#if KEY_VERBOSE_LEVEL >= 2
        dbgMessagef("keyRepeat: repeat count of 0x%x set to 0", key);
#endif
    if (!(demDemoRecording | demDemoPlaying))
    {
        keyScanCode[key] = keySaveScan[key];
    }
}

/*-----------------------------------------------------------------------------
    Name        : keyAnyKeyHit
    Description : Checks to see if any key is hit
    Inputs      : void
    Outputs     : ..
    Return      : nonzero if any key pressed
----------------------------------------------------------------------------*/
bool keyAnyKeyHit(void)
{
    int i;
    keyScanType keyhit = { 0,0,0 };

    for (i=0;i<KEY_TOTAL_KEYS;i++)
        keyhit.keypressed |= keyScanCode[i].keypressed;

    return (bool)keyhit.keypressed;
}

/*-----------------------------------------------------------------------------
    Name        : keyAnyKeyStuck
    Description : Checks to see if any key is stuck, clearing the state if so.
    Inputs      : void
    Outputs     : ..
    Return      : nonzero if any key pressed
----------------------------------------------------------------------------*/
bool keyAnyKeyStuck(void)
{
    int i;
    keyScanType keyhit = { 0,0,0 };

    for (i=0;i<KEY_TOTAL_KEYS;i++)
    {
        keyhit.keystick |= keyScanCode[i].keystick;
        keySaveScan[i].keystick = keyScanCode[i].keystick = 0;
    }

    return (bool)keyhit.keystick;
}

/*-----------------------------------------------------------------------------
    Name        : keyClearAll
    Description : Clear all keys.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void keyClearAll(void)
{
    memset((void *)keyScanCode,0,sizeof(keyScanCode));
    memset((void *)keySaveScan,0,sizeof(keySaveScan));
    keyBufferClear();
}

/*-----------------------------------------------------------------------------
    Name        : keyClearAllStuckKeys
    Description : Clears the stick bits from all keys that are not currently
                    pressed.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void keyClearAllStuckKeys(void)
{
    sdword index;

    for (index = 0; index < KEY_TOTAL_KEYS; index++)
    {
        if (keyScanCode[index].keystick && (!keyScanCode[index].keypressed))
        {                                                   //if stuck but not pressed
            keyScanCode[index].keystick = 0;
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : keyBufferedKeyGet
    Description : Get a buffered keystroke for typing.
    Inputs      :
    Outputs     : bShift - was the shift key pressed when this one went down?
    Return      : ASCII key (including shift state) or 0 meaning there are no more buffered keys.
    Notes       : this function is not thread-safe
                  This function will return the shifted status of the key if
                    shift was pressed at the time this key was hit.
----------------------------------------------------------------------------*/
udword keyBufferedKeyGet(bool *bShift)
{
    udword keyToReturn, index;

    if (keyNumberBufferKeys == 0)
    {
        return(0);
    }
    keyToReturn = (udword)keyBuffer[0].key;
    *bShift = keyBuffer[0].bShift;
    keyScanCode[keyToReturn].keypressed = 0;
    keyScanCode[keyToReturn].keystick = 0;
    keyScanCode[keyToReturn].keynumpressed = 0;
    keySaveScan[keyToReturn].keypressed = 0;
    keySaveScan[keyToReturn].keystick = 0;
    keySaveScan[keyToReturn].keynumpressed = 0;

    for (index = 1; index < keyNumberBufferKeys; index++)
    {                                                       //shift rest of buffer back one
        keyBuffer[index - 1] = keyBuffer[index];
    }
    keyNumberBufferKeys--;
    return(keyToReturn);
}

/*-----------------------------------------------------------------------------
    Name        : keyBufferAdd
    Description : Add a keystroke to the key buffer
    Inputs      : key - scancode of key that was hit
                  bShift - was the shift key pressed?
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void keyBufferAdd(udword key, bool bShift)
{
    int i;

    /* Game not specific about left or right Shift/Ctrl/Alt key, so neither
       are we. */
    if (key == SDLK_RSHIFT)
        key = SDLK_LSHIFT;
    else if (key == SDLK_RCTRL)
        key = SDLK_LCTRL;
    else if (key == SDLK_RALT)
        key = SDLK_LALT;
    else if (key == SDLK_RMETA) // Apple/Command/"Windows" key
        key = SDLK_LMETA;

    for (i = 0; keysToNotBuffer[i]; i++)
    {
        if (keysToNotBuffer[i] == key)                      //don't bother storing strokes of certain special keys
            return;
    }
    if (keyNumberBufferKeys < KEY_BufferLength - 1)
    {                                                       //increase size of buffer if it's not already maxed
        keyBuffer[keyNumberBufferKeys].key = key;           //insert new key at the end
        keyBuffer[keyNumberBufferKeys].bShift = bShift;
        keyNumberBufferKeys++;
    }
}

/*-----------------------------------------------------------------------------
    Name        : keyBufferClear
    Description : Clears out the key buffer
    Inputs      : void
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void keyBufferClear(void)
{
    keyNumberBufferKeys = 0;
}

/*-----------------------------------------------------------------------------
    Name        : keyGermanToEnglish
    Description : This function converts german keyboard presses to english
    Inputs      : udword virtkey
    Outputs     : udword
    Parameters  : udword virtkey
    Return      : void
-----------------------------------------------------------------------------*/
udword keyGermanToEnglish(udword virtkey)
{
    return virtkey;
}

/*-----------------------------------------------------------------------------
    Name        : keyFrenchToEnglish
    Description : This function converts german keyboard presses to english
    Inputs      : udword virtkey
    Outputs     : udword
    Parameters  : udword virtkey
    Return      : void
-----------------------------------------------------------------------------*/
udword keyFrenchToEnglish(udword virtkey)
{
    return virtkey;
}

/*-----------------------------------------------------------------------------
    Name        : keySpanishToEnglish
    Description : This function converts german keyboard presses to english
    Inputs      : udword virtkey
    Outputs     : udword
    Parameters  : udword virtkey
    Return      : void
-----------------------------------------------------------------------------*/
udword keySpanishToEnglish(udword virtkey)
{
    return virtkey;
}

/*-----------------------------------------------------------------------------
    Name        : keyItalianToEnglish
    Description : This function converts german keyboard presses to english
    Inputs      : udword virtkey
    Outputs     : udword
    Parameters  : udword virtkey
    Return      : void
-----------------------------------------------------------------------------*/
udword keyItalianToEnglish(udword virtkey)
{
    return virtkey;
}
