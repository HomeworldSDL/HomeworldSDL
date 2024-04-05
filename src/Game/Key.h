/*=============================================================================
    KEY.H: Definitions for keyboard interface.

    Created June 1997 by Gary Shaw
=============================================================================*/

#ifndef ___KEY_H
#define ___KEY_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keycode.h>

#include "Types.h"

/*=============================================================================
    Switches:
=============================================================================*/
#ifdef HW_BUILD_FOR_DEBUGGING
#define KEY_ERROR_CHECKING      1
#define KEY_VERBOSE_LEVEL       1
#else
#define KEY_ERROR_CHECKING      0
#define KEY_VERBOSE_LEVEL       0
#endif

/*=============================================================================
    Definitions:
=============================================================================*/
#define KEY_TOTAL_KEYS          (SDL_NUM_SCANCODES+8)  // Use SDL keysyms, plus some for mouse
#define KEY_BufferLength        16

#define KEY_NUMPRESSED_BITS     5
#define KEY_NUMPRESSED_MAX      ((1 << (KEY_NUMPRESSED_BITS + 1)) - 1)
#define BACKSPACEKEY            SDL_SCANCODE_BACKSPACE
#define TABKEY                  SDL_SCANCODE_TAB
#define ENTERKEY                SDL_SCANCODE_KP_ENTER
#define SHIFTKEY                SDL_SCANCODE_LSHIFT
#define LSHIFTKEY               SDL_SCANCODE_LSHIFT
#define RSHIFTKEY               SDL_SCANCODE_RSHIFT
#define CONTROLKEY              SDL_SCANCODE_LCTRL
#define LCONTROLKEY             SDL_SCANCODE_LCTRL
#define RCONTROLKEY             SDL_SCANCODE_RCTRL
#define ALTKEY                  SDL_SCANCODE_LALT
#define LALTKEY                 SDL_SCANCODE_LALT
#define RALTKEY                 SDL_SCANCODE_RALT
#define METAKEY                 SDL_SCANCODE_LGUI      // Apple/Command/"Windows" key
#define LMETAKEY                SDL_SCANCODE_LGUI
#define RMETAKEY                SDL_SCANCODE_RGUI
#define CAPSLOCKKEY             SDL_SCANCODE_CAPSLOCK
#define ESCKEY                  SDL_SCANCODE_ESCAPE
#define ARRLEFT                 SDL_SCANCODE_LEFT
#define ARRRIGHT                SDL_SCANCODE_RIGHT
#define ARRUP                   SDL_SCANCODE_UP
#define ARRDOWN                 SDL_SCANCODE_DOWN
#define ENDKEY                  SDL_SCANCODE_END
#define HOMEKEY                 SDL_SCANCODE_HOME
#define PAGEUPKEY               SDL_SCANCODE_PAGEUP
#define PAGEDOWNKEY             SDL_SCANCODE_PAGEDOWN
#define INSERTKEY               SDL_SCANCODE_INSERT
#define DELETEKEY               SDL_SCANCODE_DELETE
#define LESSTHAN                SDL_SCANCODE_COMMA
#define GREATERTHAN             SDL_SCANCODE_PERIOD
//#define PLUSMINUS               SDL_SCANCODE_WORLD_0  // key under ESC and next to upper row numeric 1 
#define ZEROKEY                 SDL_SCANCODE_0
#define ONEKEY                  SDL_SCANCODE_1
#define TWOKEY                  SDL_SCANCODE_2
#define THREEKEY                SDL_SCANCODE_3
#define FOURKEY                 SDL_SCANCODE_4
#define FIVEKEY                 SDL_SCANCODE_5
#define SIXKEY                  SDL_SCANCODE_6
#define SEVENKEY                SDL_SCANCODE_7
#define EIGHTKEY                SDL_SCANCODE_8
#define NINEKEY                 SDL_SCANCODE_9
#define FIRSTNUMKEY             ONEKEY /* number scancodes don't start at zero for some insane reason */
#define FIRSTNUMKEYNUM          1
#define AKEY                    SDL_SCANCODE_A
#define BKEY                    SDL_SCANCODE_B
#define CKEY                    SDL_SCANCODE_C
#define DKEY                    SDL_SCANCODE_D
#define EKEY                    SDL_SCANCODE_E
#define FKEY                    SDL_SCANCODE_F
#define GKEY                    SDL_SCANCODE_G
#define HKEY2                   SDL_SCANCODE_H
#define IKEY                    SDL_SCANCODE_I
#define JKEY                    SDL_SCANCODE_J
#define KKEY                    SDL_SCANCODE_K
#define LKEY                    SDL_SCANCODE_L
#define MKEY                    SDL_SCANCODE_M
#define NKEY                    SDL_SCANCODE_N
#define OKEY                    SDL_SCANCODE_O
#define PKEY                    SDL_SCANCODE_P
#define QKEY                    SDL_SCANCODE_Q
#define RKEY                    SDL_SCANCODE_R
#define SKEY                    SDL_SCANCODE_S
#define TKEY                    SDL_SCANCODE_T
#define UKEY                    SDL_SCANCODE_U
#define VKEY                    SDL_SCANCODE_V
#define WKEY                    SDL_SCANCODE_W
#define XKEY                    SDL_SCANCODE_X
#define YKEY                    SDL_SCANCODE_Y
#define ZKEY                    SDL_SCANCODE_Z
#define SPACEKEY                SDL_SCANCODE_SPACE
#define RETURNKEY               SDL_SCANCODE_RETURN
#define NUMPAD0                 SDL_SCANCODE_KP_0
#define NUMPAD1                 SDL_SCANCODE_KP_1
#define NUMPAD2                 SDL_SCANCODE_KP_2
#define NUMPAD3                 SDL_SCANCODE_KP_3
#define NUMPAD4                 SDL_SCANCODE_KP_4
#define NUMPAD5                 SDL_SCANCODE_KP_5
#define NUMPAD6                 SDL_SCANCODE_KP_6
#define NUMPAD7                 SDL_SCANCODE_KP_7
#define NUMPAD8                 SDL_SCANCODE_KP_8
#define NUMPAD9                 SDL_SCANCODE_KP_9
#define FIRSTNUMPADKEY          NUMPAD1
#define FIRSTNUMPADKEYNUM          1
#define LBRACK                  SDL_SCANCODE_LEFTBRACKET
#define RBRACK                  SDL_SCANCODE_RIGHTBRACKET
#define F1KEY                   SDL_SCANCODE_F1
#define F2KEY                   SDL_SCANCODE_F2
#define F3KEY                   SDL_SCANCODE_F3
#define F4KEY                   SDL_SCANCODE_F4
#define F5KEY                   SDL_SCANCODE_F5
#define F6KEY                   SDL_SCANCODE_F6
#define F7KEY                   SDL_SCANCODE_F7
#define F8KEY                   SDL_SCANCODE_F8
#define F9KEY                   SDL_SCANCODE_F9
#define F10KEY                  SDL_SCANCODE_F10
#define F11KEY                  SDL_SCANCODE_F11
#define F12KEY                  SDL_SCANCODE_F12
#define PRINTKEY                SDL_SCANCODE_PRINTSCREEN      // F13 on Apple Extended keyboard
#define SCROLLKEY               SDL_SCANCODE_SCROLLLOCK  // F14 (caught by MacOSX: -brightness)
#define PAUSEKEY                SDL_SCANCODE_PAUSE      // F15 (caught by MacOSX: +brightness)
#define TILDEKEY                SDL_SCANCODE_GRAVE
#define NUMMINUSKEY             SDL_SCANCODE_KP_MINUS
#define NUMPLUSKEY              SDL_SCANCODE_KP_PLUS
#define NUMSTARKEY              SDL_SCANCODE_KP_MULTIPLY
#define NUMSLASHKEY             SDL_SCANCODE_KP_DIVIDE
#define NUMPADSLASH             SDL_SCANCODE_KP_DIVIDE
#define NUMDOTKEY               SDL_SCANCODE_KP_PERIOD
#define MINUSKEY                SDL_SCANCODE_MINUS
#define PLUSKEY                 SDL_SCANCODE_EQUALS
#define BACKSLASHKEY            SDL_SCANCODE_BACKSLASH

#define LMOUSE_BUTTON           (SDL_NUM_SCANCODES+0)
#define RMOUSE_BUTTON           (SDL_NUM_SCANCODES+1)
#define MMOUSE_BUTTON           (SDL_NUM_SCANCODES+3)

#define FLYWHEEL_UP             (SDL_NUM_SCANCODES+2)
#define FLYWHEEL_DOWN           (SDL_NUM_SCANCODES+4)

#define LMOUSE_DOUBLE           (SDL_NUM_SCANCODES+5)
#define RMOUSE_DOUBLE           (SDL_NUM_SCANCODES+6)
#define MMOUSE_DOUBLE           (SDL_NUM_SCANCODES+7)

#define NUMKEYNUM_GENERIC(x,first,firstnum) (((x - first)+firstnum)%10)
#define NUMKEYNUM(x) NUMKEYNUM_GENERIC(x,FIRSTNUMKEY,FIRSTNUMKEYNUM)
#define NUMPADKEYNUM(x) NUMKEYNUM_GENERIC(x,FIRSTNUMPADKEY,FIRSTNUMPADKEYNUM)

/*=============================================================================
    Type definitions:
=============================================================================*/
typedef struct
{
   ubyte keypressed : 1;
   ubyte keystick : 2;
   sbyte keynumpressed : KEY_NUMPRESSED_BITS;
}
keyScanType;

/* Been having some problems using sizes < 4 in variable argument lists.
   (segfault core dump AUGH MY NARDS!@$#1) */
/*typedef uword keyindex;*/
typedef udword keyindex;

typedef struct
{
    keyindex key;
    bool8    bShift;
}
keybufferchar;

/*=============================================================================
    Data:
=============================================================================*/
extern volatile keyScanType keyScanCode[KEY_TOTAL_KEYS];
extern volatile keyScanType keySaveScan[KEY_TOTAL_KEYS];
extern real32 keyLastTimeHit;

/*=============================================================================
    Convenience:
=============================================================================*/

#define keyIsHit(key)           (keyScanCode[(key)].keypressed)
#define keyIsStuck(key)         (keyScanCode[(key)].keystick)
#define keyIsRepeat(key)        (keyScanCode[(key)].keynumpressed > 0)
#define keyDecRepeat(key)       keyScanCode[(key)].keynumpressed--,keySaveScan[(key)].keynumpressed--
#define keyClearRepeat(key)     keyScanCode[(key)].keynumpressed = 0,keySaveScan[(key)].keynumpressed = 0
#define keyClearSticky(key)     if (keyScanCode[(key)].keystick) {keyScanCode[(key)].keystick--,keySaveScan[(key)].keystick--;}
#define keySetSticky(key)       if (keyScanCode[(key)].keystick < 3) {keyScanCode[(key)].keystick++,keySaveScan[(key)].keystick++;}

#define guardKeyIsHit() (keyIsHit(GKEY) || (keyIsHit(CONTROLKEY) && (keyIsHit(RALTKEY) || keyIsHit(ALTKEY))))

/*=============================================================================
    Functions:
=============================================================================*/
void keyInit(void);
void keyClose(void);
bool32 keyAnyKeyHit(void);
bool32 keyAnyKeyStuck(void);
void keyClearAll(void);
void keyClearAllStuckKeys(void);

//called from the Windows interface layer, set/clear the sticky and on bits
void keyPressDown(udword key);
void keyPressUp(udword key);
void keyRepeat(udword key);

//functions for buffered keystrokes
udword keyBufferedKeyGet(bool32 *bShift);
void keyBufferAdd(udword key, bool32 bShift);
void keyBufferClear(void);

udword keyGermanToEnglish(udword virtkey);
udword keyFrenchToEnglish(udword virtkey);
udword keySpanishToEnglish(udword virtkey);
udword keyItalianToEnglish(udword virtkey);

#endif
