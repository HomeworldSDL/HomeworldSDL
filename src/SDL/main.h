/*=============================================================================
    MAIN.H: Definitions for main Windows interface file for Homeworld.

    Created June 1997 By Luke Moloney
=============================================================================*/

#ifndef ___MAIN_H
#define ___MAIN_H

#include <SDL2/SDL.h>

#include "mainswitches.h"
#include "Types.h"

// -----------------------------------------------------------------------------
// Make sure the game is being compiled with the correct build configuration
// -----------------------------------------------------------------------------

#if defined( HW_GAME_HOMEWORLD ) + defined( HW_GAME_RAIDER_RETREAT ) + defined( HW_GAME_DEMO ) != 1
    #error Please specify ONE of the HW_GAME_* types
#endif

#if defined( HW_BUILD_FOR_DEBUGGING ) + defined( HW_BUILD_FOR_DISTRIBUTION ) != 1
    #error Please specify ONE of the HW_BUILD_* types
#endif


/*=============================================================================
    Switches:
=============================================================================*/

#ifdef HW_BUILD_FOR_DEBUGGING

#define MAIN_MOUSE_FREE             1           //allow the mouse to be freed from the window
#define MAIN_SENSOR_LEVEL           1           //enable command-line adjustment of sensors level
#define MAIN_PRINT_MESSAGES         0           //print the window messages as they come pouring in

#else

#define MAIN_MOUSE_FREE             1           //allow the mouse to be freed from the window
#define MAIN_SENSOR_LEVEL           0           //enable command-line adjustment of sensors level
#define MAIN_PRINT_MESSAGES         0           //print the window messages as they come pouring in

#endif

/*=============================================================================
    Definitions:
=============================================================================*/
#define ERR_ErrorStart              0xfed5
#define MAIN_CDCheck                0

//command-line parsing definitions
#define TS_Delimiters               " \t,="
#define MCL_IndentSpace             4           //number of indent characters

//command-line option flags
#define COF_Visible                 1           //user can see this option with /?
#define COF_NextToken               2           //pass next token as parameter to function

/*=============================================================================
    Type definitions:
=============================================================================*/
//structure for command-line parsing
typedef struct
{
    //bool32 visible;                               //is this option visible by using '/?'?
    udword flags;                               //flags for this entry.  See above for possible values.
    char *parameter;                            //parameter string (begins with a slash)
    bool32 (*function)(char *string);             //function to call, NULL if none.  Called after variable is set.
    void *variableToModify;                     //variable to modify, NULL if none
    udword valueToSet;                          //value to set variable to
    char *helpString;                           //string printed in help screen
}
commandoption;

#define END_COMMAND_OPTION  {0, NULL, NULL, NULL, 0, NULL}

/*=============================================================================
    Data:
=============================================================================*/
extern void *ghMainWindow;
extern void *ghInstance;
#if MAIN_SENSOR_LEVEL
extern udword initialSensorLevel;
#endif

extern sdword enableTrails;
extern sdword showBackgrounds;

extern bool32 mainRasterSkip;

extern bool32 mainSafeGL;

extern bool32 mainForceKatmai;
extern bool32 mainAllowKatmai;
extern bool32 mainAllowPacking;
extern bool32 mainOnlyPacking;

extern bool32 fullScreen;

extern bool32 enableAVI;

extern sdword mainReinitRenderer;

//size of main window
extern sdword MAIN_WindowWidth;
extern sdword MAIN_WindowHeight;
extern sdword MAIN_WindowDepth;

extern sdword mainWindowWidth;
extern sdword mainWindowHeight;
extern sdword mainWindowDepth;

extern bool32 mainNoPerspective;
extern bool32 systemActive;

/*=============================================================================
    Functions:
=============================================================================*/
void WindowsCleanup(void);

void ActivateMe(void);
void DeactivateMe(void);

// Event handler.
void HandleEvent(SDL_Event const* pEvent);

//load/save options from disk
void utyOptionsFileRead(void);
void utyOptionsFileWrite(void);

//renderer swapping functions
bool32 mainLoadGL(char* data);
bool32 mainLoadRGL(void);
bool32 mainLoadParticularRGL(char* device, char* data);
sdword mainActiveRenderer(void);
bool32 mainReinitRGL(void);
bool32 mainShutdownRenderer(void);

void mainSaveRender(void);
void mainRestoreRender(void);
void mainRestoreSoftware(void);

//video playback module convenience functions
void mainCleanupAfterVideo(void);

#endif
