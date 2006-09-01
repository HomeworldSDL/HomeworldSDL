// =============================================================================
//  CRC32.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/29/1997 by lmoloney
// =============================================================================

#ifndef ___CRC32_H
#define ___CRC32_H

#include "Types.h"

typedef udword crc32;
typedef uword  crc16;

crc32 crc32Compute(ubyte *packet, udword length);
crc16 crc16Compute(ubyte *packet, udword length);

#endif
