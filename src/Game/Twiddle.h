// =============================================================================
//  Twiddle.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 7/7/1997 by lmoloney
// =============================================================================

#ifndef ___TWIDDLE_H
#define ___TWIDDLE_H

#include "Types.h"

// count number of set bits in a number
udword bitNumberSet(udword target, udword nBits);

// find nearest exponent of 2
udword bitHighExponent2(udword number);
udword bitLowExponent2(udword number);

// find lowest/highest bit set in a number
udword bitLowBitPosition(udword number);

#endif
