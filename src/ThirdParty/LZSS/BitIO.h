// =============================================================================
//  BitIO.h
//  - file-oriented code based on The Data Compression Book, by Mark Nelson & Jean-Loup Gailly
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//	Created by Darren Stone 1998/10/05
// =============================================================================

#ifndef ___BITIO_H
#define ___BITIO_H

#include <stdio.h>

// INTERFACE -------------------------------------------------------------------

typedef struct BitFile
{
    FILE *file;
    unsigned char mask;
    int rack;
    int pacifier_counter;
	int index;                 // like a file position, in bytes
} BitFile;

typedef struct BitBuffer
{
    char *buffer;
    int index;
    unsigned char mask;
    int rack;
    int pacifier_counter;
} BitBuffer;

int  bitioInit(void);
void bitioShutdown(void);

// file oriented bit i/o
BitFile      *bitioFileOpenInput( char *name );
BitFile      *bitioFileOpenOutput( char *name );
void          bitioFileOutputBit( BitFile *bit_file, int bit );
void          bitioFileOutputBits( BitFile *bit_file, unsigned long code, int count );
int           bitioFileInputBit( BitFile *bit_file );
unsigned long bitioFileInputBits( BitFile *bit_file, int bit_count );
int           bitioFileCloseInput( BitFile *bit_file );
int           bitioFileCloseOutput( BitFile *bit_file );
void          bitioFilePrintBinary( FILE *file, unsigned int code, int bits );

// for transitioning between bit & byte mode on open streams
BitFile      *bitioFileAppendStart(FILE *fp);
int			  bitioFileAppendStop(BitFile *bit_file);
BitFile      *bitioFileInputStart(FILE *fp);
int			  bitioFileInputStop(BitFile *bit_file);

// buffer oriented bit i/o
BitBuffer    *bitioBufferOpen(char *buffer);
void          bitioBufferOutputBit(BitBuffer *bitBuffer, int bit);
void          bitioBufferOutputBits(BitBuffer *bitBuffer, unsigned long code, int count);
int           bitioBufferInputBit(BitBuffer *bitBuffer);
unsigned long bitioBufferInputBits(BitBuffer *bitBuffer, int count);
int           bitioBufferCloseOutput(BitBuffer *bitBuffer);
int           bitioBufferCloseInput(BitBuffer *bitBuffer);

#endif
