//
// Prefix header for all source files of the 'Homeworld' target in the 'Homeworld' project
//

#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#endif

// _MACOSX     -         Mac-specific code (#define'd on gcc command line / target info in Xcode)
// _MACOSX_86  -   Intel Mac-specific code
// _MACOSX_PPC - PowerPC Mac-specific code

// FIX_ENDIAN  - use where data needs byte-swapping

// Code wrapped by the following #defines is defective and needs reimplementing
// at some point in order to fix it. Try and use a positive test where possible
// (e.g. #ifdef _MACOSX_FIX_86) so that developers with skills in a particular
// area can more quickly target them.

// _MACOSX_FIX_86    - use to mark problematic code on Intel Macs
// _MACOSX_FIX_PPC   - use to mark problematic code on PowerPC Macs

// _MACOSX_FIX_ANIM  - use to toggle animatic (movie) code
// _MACOSX_FIX_GL    - use to toggle glcaps and other gl related code
// _MACOSX_FIX_LAN   - use to toggle LAN related code
// _MACOSX_FIX_MISC  - use to toggle control / memory / other issues
// _MACOSX_FIX_SOUND - use to toggle sound code

#ifdef _MACOSX
#define GENERIC_ETGCALLFUNCTION
	#define _MACOSX_FIX_ANIM 1
	//#define _MACOSX_FIX_SOUND 1
	#define _MACOSX_FIX_LAN 1
	#define _MACOSX_FIX_GL 1
	#define _MACOSX_FIX_MISC 1
	//#ifndef _MACOSX_FIX_ME
	//	#define _MACOSX_FIX_ME 1
	//#endif
	#ifdef __ppc__
		#define _MACOSX_PPC 1
        #define _MACOSX_FIX_GL 1
		#define _MACOSX_FIX_PPC 1
		#define _MACOSX_FIX_SOUND 1
	#endif
	#ifdef __i386__
		#define _MACOSX_86 1
		#define _MACOSX_FIX_86 1
	#endif
#endif
