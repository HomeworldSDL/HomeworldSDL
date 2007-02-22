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

udword Real32ToUdword(float a)
{
 return (*(udword*)(&a));
}

