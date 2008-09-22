
#include <Types.h>



typedef struct tagfescreen_disk
{
    udword name;   // *                              //name of screen for link purposes
    udword flags;                               //flags for this screen
    uword nLinks;                               //number of links in this screen
    uword nAtoms;                               //number of atoms in screen
    udword links;    // *                //pointer to list of links
    udword atoms;    // *                //pointer to list of atoms
}
fescreen_disk;


;


typedef struct tagfelink_disk
{
    udword name;    // *                             //optional name of this link
    udword flags;                               //flags controlling behaviour of link
    udword linkToName;  // *                         //name of screen to link to
}
felink_disk;


typedef struct tagfeatom_disk
{
    udword  name;   //*                               //optional name of control
    udword flags;                               //flags to control behavior
    udword status;                              //status flags for this atom, checked etc.
    ubyte  type;                                //type of control (button, scroll bar, etc.)
    ubyte  borderWidth;                         //width, in pixels, of the border
    uword  tabstop;                             //denotes the tab ordering of UI controls
    color  borderColor;                         //optional color of border
    color  contentColor;                        //optional color of content
    sword  x,      loadedX;
    sword  y,      loadedY;
    sword  width,  loadedWidth;
    sword  height, loadedHeight;
    udword pData;    //*                    //pointer to type-specific data
    udword attribs;    //*                         //sound(button atom) or font(static text atom) reference
    char   hotKeyModifiers;
    char   hotKey[FE_NumberLanguages];
    char   pad2[2];
    udword drawstyle[2];
    udword  region; //*
    udword pad[2];
}
featom_disk;


