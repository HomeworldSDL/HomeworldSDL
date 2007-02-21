/*=========================================================================
   TYPES.H definitions
   Based on TYPES.H definitions for Watcom C in original Homeworld source.
   (Modified for use outside Watcom C, i.e. GCC, by Ted Cipicchio)

   The main difference is that we now use SDL data types since they
   already deal different data type sizes across platforms.  The original
   data type declarations (sbyte, udword, etc.) are still used to keep
   consistent with the original game source.
==========================================================================*/

#ifndef ___TYPES_H
#define ___TYPES_H

#include "SDL_stdinc.h"
#include "SDL_endian.h"

#ifdef _MSC_VER
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
        #include <windows.h>
    #endif
#endif

// PATH_MAX is Homeworld's standard #define for stating
// "maximum length of a filesystem path string".
#ifdef WIN32
    #define PATH_MAX MAX_PATH
#elif defined _MACOSX
    #include <sys/syslimits.h>
#elif defined linux
		#include <limits.h>
#endif

/*-----------------------------------------------------------------------------
    Warning turn ons:
-----------------------------------------------------------------------------*/
#ifdef _MSC_VER
#pragma warning( 2 : 4013)  //function undefined
#pragma warning( 2 : 4035)  //no return value
#pragma warning( 2 : 4101)  //unreferenced local variable
#pragma warning( 2 : 4245)  //signed/unsigned mismatch
#pragma warning( 2 : 4505)  //spots unused local functions
#pragma warning( 2 : 4706)  //spots '=' instead of '==' in conditional statements
#endif

/*-------------------------------------------------------------------------
   Declare basic integer data types
--------------------------------------------------------------------------*/
#ifndef SDL_HAS_64BIT_TYPE
#error 64-bit integer type not supported on this platform.
#endif

typedef Sint8   sbyte;
typedef Uint8   ubyte;
typedef Sint16  sword;
typedef Uint16  uword;
typedef Sint32  sdword;
typedef Uint32  udword;
typedef Sint64  sqword;
typedef Uint64  uqword;
typedef float   real32;
typedef double  real64;

#ifndef bool                 // was: __cplusplus
typedef udword  bool;
#endif

typedef sbyte   bool8;
typedef sword   bool16;

/*-------------------------------------------------------------------------
	Functions for converting endian-specific values
--------------------------------------------------------------------------*/

// NB: Homeworld was originally written for IBM-compatible PCs which use
//     the little-endian convention. All data structures read/written from/to
//     disk should be assumed to be byte-arranged in this way. Consequently we
//     only need to support conversion to/from little-endian data types.
#define HW_ENDIAN  SDL_LIL_ENDIAN

// bizarrely SDL endian support only exists for integer types
float  SwapFloat32( float val );

// we could use SDL's byte swapping definitions directly but for consistency of
// use with regard to floats, we redefine them
#if SDL_BYTEORDER == HW_ENDIAN
    #define FIX_ENDIAN                0

	#define FIX_ENDIAN_INT_16(x)      (x)
	#define FIX_ENDIAN_INT_32(x)      (x)
	#define FIX_ENDIAN_FLOAT_32(x)    (x)
#else
    #define FIX_ENDIAN                1

	#define FIX_ENDIAN_INT_16(x)      SDL_Swap16(x)
	#define FIX_ENDIAN_INT_32(x)      SDL_Swap32(x)
	#define FIX_ENDIAN_FLOAT_32(x)    SwapFloat32(x)
#endif

#if defined (__GNUC__) && defined (__i386__)
// #define TreatAsUdword(x) (*((udword *)(&(x))))
 udword TreatAsUdword(real32 a);
#else 
 #define TreatAsUdword(x) (*((udword *)(&(x))))
#endif

#define TreatAsReal32(x) (*((real32 *)(&(x))))


/*-------------------------------------------------------------------------
   Declare common numbers etc.
--------------------------------------------------------------------------*/

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
#define TRUE               (!FALSE)
#endif

#ifndef FALSE
#define FALSE              0
#endif

#ifndef OKAY
#define OKAY               0
#endif

#ifndef ERROR
#define ERROR              -1
#endif

#define  SBYTE_Min     -128                     // minimums for signed types
#define  SWORD_Min     -32768L
#define  SDWORD_Min    -2147483648L

#define  SBYTE_Max     127                      // maximums for signed types
#define  SWORD_Max     32767
#define  SDWORD_Max    2147483647L

#define  UBYTE_Max     255U                     // maximums for unsigned types
#define  UWORD_Max     65535UL
#define  UDWORD_Max    4294967295UL

//big numbers (theoretically the largest representable with reals)
#define REALlyBig           ((real32)10e20)
#define REALlySmall         ((real32)10e-20)
#define REALlyNegative      ((real32)-10e20)

#define  BIT0       0x00000001
#define  BIT1       0x00000002
#define  BIT2       0x00000004
#define  BIT3       0x00000008
#define  BIT4       0x00000010
#define  BIT5       0x00000020
#define  BIT6       0x00000040
#define  BIT7       0x00000080

#define  BIT8       0x00000100
#define  BIT9       0x00000200
#define  BIT10      0x00000400
#define  BIT11      0x00000800
#define  BIT12      0x00001000
#define  BIT13      0x00002000
#define  BIT14      0x00004000
#define  BIT15      0x00008000

#define  BIT16      0x00010000L
#define  BIT17      0x00020000L
#define  BIT18      0x00040000L
#define  BIT19      0x00080000L
#define  BIT20      0x00100000L
#define  BIT21      0x00200000L
#define  BIT22      0x00400000L
#define  BIT23      0x00800000L

#define  BIT24      0x01000000L
#define  BIT25      0x02000000L
#define  BIT26      0x04000000L
#define  BIT27      0x08000000L
#define  BIT28      0x10000000L
#define  BIT29      0x20000000L
#define  BIT30      0x40000000L
#define  BIT31      0x80000000L

// bit manipulation
#define bitSet(dest, bitFlags)      ((dest) |= (bitFlags))
#define bitClear(dest, bitFlags)    ((dest) &= ~(bitFlags))
#define bitTest(dest, bitFlags)     ((dest) & (bitFlags))
#define bitToggle(dest, bitFlags)   { if ((dest) & (bitFlags)) bitClear((dest), (bitFlags)); else bitSet((dest), (bitFlags)); }
#define bitSetTo(dest, bitFlags, bSet) ((dest) = ((dest) & (~bitFlags)) | ((bSet)?bitFlags:0))


/*-------------------------------------------------------------------------
   Declare basic macros
--------------------------------------------------------------------------*/

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b) ((a) > (b) ? (b) : (a))
#endif

#if !defined(ABS)
    #define ABS(a)   ((a) < 0 ? -(a) : (a))
#endif

#define frandyrandom(stream,n) (ranRandom(stream) * (((real32)(n)) * (1.0f/((real32)UDWORD_Max))))
#define frandyrandombetween(stream,a,b) (frandyrandom(stream,(b)-(a)) + (a))

#define randyrandom(stream,n) (ranRandom(stream) % (n))
#define randyrandombetween(stream,a,b) ( randyrandom(stream,((b)-(a)+1)) + (a) )

#define isBetweenExclusive(num,lowerbound,upperbound) \
            ( ((num) > (lowerbound)) && ((num) < (upperbound)) )

#define isBetweenInclusive(num,lowerbound,upperbound) \
            ( ((num) >= (lowerbound)) && ((num) <= (upperbound)) )

#define capNumber(value,cap) ((value > cap) ? cap : value)

//standard swap, works on any type
#ifndef swap
#define swap(a,b,temp) \
        (temp) = (a);  \
        (a) = (b);     \
        (b) = (temp);
#endif
//integral swap, including pointers
#define swapInt(a, b)   \
        (a) ^= (b);     \
        (b) ^= (a);     \
        (a) ^= (b);
//floating-point swap
#define swapReal32(a, b)                              \
        *((udword *)(&(a))) ^= *((udword *)(&(b)));   \
        *((udword *)(&(b))) ^= *((udword *)(&(a)));   \
        *((udword *)(&(a))) ^= *((udword *)(&(b)));

#define str$(x) #x

//fuzzy equals using a tolerance
#define roughlyEqual(a, b, t)   (((b) <= ((a) + (t))) && ((b) >= ((a) - (t))))

#endif
