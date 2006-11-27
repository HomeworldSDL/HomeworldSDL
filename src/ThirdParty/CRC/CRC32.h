// =============================================================================
//  CRC32.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 6/29/1997 by lmoloney
// =============================================================================

#ifndef ___CRC32_H
#define ___CRC32_H

#if !defined _MSC_VER
#include <stdint.h>
#else
#include "Types.h"
#endif

typedef uint32_t crc32;
typedef uint16_t crc16;

crc32 crc32Compute(uint8_t *packet, uint32_t length);
crc16 crc16Compute(uint8_t *packet, uint32_t length);

#endif
