// =============================================================================
//  Types.c
// =============================================================================

#include "Types.h"

float SwapFloat32( float val )
{
	union
	{
		float  f;
		Uint32 i;
	} swap;

	swap.f = val;
	swap.i = SDL_Swap32( swap.i );
    
	return swap.f;
}

udword Real32ToUdword(real32 a)
{
 return (*(udword*)(&a));
}

udword Real32ToSdword(real32 a)
{
 return (*(sdword*)(&a));
}

real32 UdwordToReal32(udword a)
{
 return (*(real32*)(&a));
}

real32 SdwordToReal32(sdword a)
{
 return (*(real32*)(&a));
}
