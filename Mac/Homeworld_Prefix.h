//
// Prefix header for all source files of the 'Homeworld' target in the 'Homeworld' project
//

#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#endif

// _MACOSX     -         Mac-specific code (#define'd on gcc command line / target info in Xcode)

// FIX_ENDIAN  - use where data needs byte-swapping

// _MACOSX_FIX_ANIM  - use to toggle animatic (movie) code
// _MACOSX_FIX_GL    - use to toggle glcaps and other gl related code
// _MACOSX_FIX_LAN   - use to toggle LAN related code
// _MACOSX_FIX_MISC  - use to toggle control / memory / other issues

#ifdef __APPLE__
#define GENERIC_ETGCALLFUNCTION
	#define _MACOSX_FIX_ANIM 1
	#define _MACOSX_FIX_LAN 1
	#define _MACOSX_FIX_GL 1
	#define _MACOSX_FIX_MISC 1
#endif
