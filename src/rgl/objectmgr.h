#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H
/*============================================================================
  File: objectmgr.h
  Manage dynamically loaded libraries.

  Created 9/29/2003 by Ted Cipicchio
============================================================================*/

#define OBJ_INVALID 0xffffffff

/* Initialize the 

/* Get the ID of a loaded libary. */
GLuint findObject (const char* name);
/* Get the ID of a loaded library, loading it if necessary. */
GLuint loadObject (const char* name);

hashtable* hashNewTable(void* (*allocFunc)(GLint),
                        void (*freeFunc)(void*));
void  hashDeleteTable(hashtable* table);
void* hashLookup(hashtable const* table, GLuint key);
void  hashInsert(hashtable* table, GLuint key, void* data);
void  hashRemove(hashtable* table, GLuint key);
GLuint hashFindFreeKeyBlock(hashtable* table, GLuint numkeys);

#endif
