/*=============================================================================
    REGKEY.H: Registry key definitions for Homeworld.

    Created Sept 1999 by Janik Joire
=============================================================================*/

// Do not change these registry key definitions! The installer depends on them...
#if defined(Downloadable)

    #define BASEKEYNAME "SOFTWARE\\Sierra On-Line\\Homeworld Downloadable"
    #define CONFIGDIR ".homeworldDownloadable"

#elif defined(OEM)

    #define BASEKEYNAME "SOFTWARE\\Sierra On-Line\\Homeworld OEM"
	
    #ifdef _MACOSX
        // the OEM and full version can live alongside so we use the same dir
        #define CONFIGDIR "Library/Application\ Support/Homeworld"
    #else
        #define CONFIGDIR ".homeworldOEM"
    #endif

#else

    #define BASEKEYNAME "SOFTWARE\\Sierra On-Line\\Homeworld"

    #ifdef _MACOSX
        #define CONFIGDIR "Library/Application\ Support/Homeworld"
    #else
        #define CONFIGDIR ".homeworld"
    #endif

#endif
