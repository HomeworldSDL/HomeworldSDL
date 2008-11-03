//
// Prefix header for all source files of the 'Homeworld' target in the 'Homeworld' project
//

#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#endif

// _MACOSX           - use for platform-specific fixes (#define'd on gcc command line) (or in the target info in xcode)

// _MACOSX_FIX_ME    - use to temporarily turn off code in order to get around the compiler (Depricated)
// _MACOSX_FIX_ANIM  - use to toggle animatic code
// _MACOSX_FIX_LAN   - use to toggle lan related code
// _MACOSX_FIX_GL    - use to turn off glcaps and other gl related code
// _MACOSX_FIX_LAN   - use to turn off lan related problems
// _MACOSX_FIX_MISC  - use to toggle control / memory / and other issues
// _MACOSX_FIX_SOUND - use to turn the sound engine on and off

// FIX_ENDIAN       - use where data needs byte-swapping 

#ifdef _MACOSX
	#define _MACOSX_FIX_ANIM 1
	//#define _MACOSX_FIX_SOUND 1
	#define _MACOSX_FIX_LAN 1
	#define _MACOSX_FIX_GL 1
	#define _MACOSX_FIX_MISC 1
	//#ifndef _MACOSX_FIX_ME
	//	#define _MACOSX_FIX_ME 1
	//#endif
	#ifdef __ppc__
		#define _MACOSX_FIX_PPC 1
		#define _MACOSX_FIX_SOUND 1
	#endif
	#ifdef __i386__
		#define _MACOSX_86 1
		#define _MACOSX_FIX_86 1
	#endif
#endif
