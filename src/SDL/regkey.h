/*=============================================================================
    REGKEY.H: Registry key definitions for Homeworld.

    Created Sept 1999 by Janik Joire
=============================================================================*/

#ifndef ___REGKEY_H
#define ___REGKEY_H

// Do not change these registry key definitions! The installer depends on them...
#if defined(HW_GAME_DEMO)

    #define BASEKEYNAME "SOFTWARE\\Sierra On-Line\\Homeworld Downloadable"
    #define CONFIGDIR ".homeworldDownloadable"

#elif defined(HW_GAME_RAIDER_RETREAT)

    #define BASEKEYNAME "SOFTWARE\\Sierra On-Line\\Homeworld OEM"
	
    #ifdef _MACOSX
        // Raider Retreat and the full version can live alongside so we use the same dir
        #define CONFIGDIR "Library/Application Support/Homeworld"
    #else
        #define CONFIGDIR ".homeworldOEM"
    #endif

#else

    #define BASEKEYNAME "SOFTWARE\\Sierra On-Line\\Homeworld"

    #ifdef _MACOSX
        #define CONFIGDIR "Library/Application Support/Homeworld"
    #else
        #define CONFIGDIR ".homeworld"
    #endif

#endif

#endif
