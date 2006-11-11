// =============================================================================
//  Hash.h
//  - reasonably generic hash table using uints as keys
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 1/9/1998 by khent
// =============================================================================

#ifndef ___HASH_H
#define ___HASH_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------

typedef struct hash_s
{
    udword key;
    void*  data;
    struct hash_s* next;
} hash_t;

typedef struct
{
    udword  size;
    udword  maxkey;
    hash_t* table[1];
} hashtable;

hashtable* hashNewTable(udword size);
void  hashDeleteTable(hashtable* table);
void* hashLookup(hashtable const* table, udword key);
void  hashInsert(hashtable* table, udword key, void* data);
void  hashRemove(hashtable* table, udword key);
udword hashFindFreeKeyBlock(hashtable* table, udword numkeys);

#endif
