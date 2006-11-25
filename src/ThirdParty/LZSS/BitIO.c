// =============================================================================
//  BitIO.c
//  - file-oriented code based on The Data Compression Book, by Mark Nelson & Jean-Loup Gailly
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//	Created by Darren Stone 1998/10/05
// =============================================================================

#include "BitIO.h"

#include <assert.h>
#include <stdlib.h>

// for stdout
#define PACIFIER_COUNT 2047
 
//
//	append to an existing, open file stream (in bit mode)
//
//	don't use regular char-mode output on this stream until after a
//	call to bitioFileAppendStop()
//
BitFile *bitioFileAppendStart(FILE *fp)
{
    BitFile *bit_file;

    bit_file = (BitFile *) calloc( 1, sizeof( BitFile ) );
    if ( bit_file == NULL )
        return( bit_file );
    bit_file->file = fp;
    bit_file->rack = 0;
    bit_file->mask = 0x80;
    bit_file->pacifier_counter = 0;
	bit_file->index = 0;
    return( bit_file );
}

//
//	flush bit mode output and allow regular output to the stream
//
//	returns bytes written in bit mode
//
int bitioFileAppendStop(BitFile *bit_file)
{
	int ret;
    if ( bit_file->mask != 0x80 )
	{
		ret = putc( bit_file->rack, bit_file->file ) ;
		++bit_file->index;
        assert( ret == bit_file->rack ); // fatal_error( "Fatal error in CloseBitFile!\n" );
	}
	ret = bit_file->index;
    free( (char *) bit_file );
	return ret;
}

BitFile *bitioFileOpenOutput( name )
char *name;
{
    BitFile *bit_file;

    bit_file = (BitFile *) calloc( 1, sizeof( BitFile ) );
    if ( bit_file == NULL )
        return( bit_file );
    bit_file->file = fopen( name, "wb" );
    bit_file->rack = 0;
    bit_file->mask = 0x80;
    bit_file->pacifier_counter = 0;
    bit_file->index = 0;
    return( bit_file );
}

//
//	like bitioAppendStart, but for input)
//
//  read from an existing, open file stream (in bit mode)
//
//	don't use regular char-mode input on this stream until after a
//	call to bitioFileInputStop()
//
BitFile     *bitioFileInputStart(FILE *fp)
{
    BitFile *bit_file;

    bit_file = (BitFile *) calloc( 1, sizeof( BitFile ) );
    if ( bit_file == NULL )
        return( bit_file );
    bit_file->file = fp;
    bit_file->rack = 0;
    bit_file->mask = 0x80;
    bit_file->pacifier_counter = 0;
	bit_file->index = 0;
    return( bit_file );
}

//
//	switch back to regular (byte-oriented) input from the stream
//
//	returns bytes read in bit mode
//
int bitioFileInputStop(BitFile *bit_file)
{
	int ret = bit_file->index;
    free( (char *) bit_file );
	return ret;
}


BitFile *bitioFileOpenInput( name )
char *name;
{
    BitFile *bit_file;

    bit_file = (BitFile *) calloc( 1, sizeof( BitFile ) );
    if ( bit_file == NULL )
    	return( bit_file );
    bit_file->file = fopen( name, "rb" );
    bit_file->rack = 0;
    bit_file->mask = 0x80;
    bit_file->index = 0;
    bit_file->pacifier_counter = 0;
    return( bit_file );
}

//
//	returns bytes written
//
int bitioFileCloseOutput( bit_file )
BitFile *bit_file;
{
	int ret;
    if ( bit_file->mask != 0x80 )
	{
		ret = putc( bit_file->rack, bit_file->file ) ;
		++bit_file->index;
        assert( ret == bit_file->rack ); // fatal_error( "Fatal error in CloseBitFile!\n" );
	}
    fclose( bit_file->file );
	ret = bit_file->index;
    free( (char *) bit_file );
	return ret;
}

//
//	return bytes read
//
int bitioFileCloseInput( bit_file )
BitFile *bit_file;
{
	int ret = bit_file->index;
    fclose( bit_file->file );
    free( (char *) bit_file );
	return ret;
}

void bitioFileOutputBit( bit_file, bit )
BitFile *bit_file;
int bit;
{
	int ret;
    if ( bit )
        bit_file->rack |= bit_file->mask;
    bit_file->mask >>= 1;
    if ( bit_file->mask == 0 ) {
		ret = putc( bit_file->rack, bit_file->file ) ;
		++bit_file->index;
	    assert( ret == bit_file->rack ); //fatal_error( "Fatal error in OutputBit!\n" );
#if 0
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		    putc( '.', stdout );
#endif
    	bit_file->rack = 0;
	    bit_file->mask = 0x80;
    }
}

void bitioFileOutputBits( bit_file, code, count )
BitFile *bit_file;
unsigned long code;
int count;
{
    unsigned long mask;
	int ret;

    mask = 1L << ( count - 1 );
    while ( mask != 0) {
        if ( mask & code )
            bit_file->rack |= bit_file->mask;
        bit_file->mask >>= 1;
        if ( bit_file->mask == 0 )
        {
			ret = putc( bit_file->rack, bit_file->file ) ;
			++bit_file->index;
			assert( ret == bit_file->rack ); //fatal_error( "Fatal error in OutputBit!\n" );
#if 0
            if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		        putc( '.', stdout );
#endif
    	    bit_file->rack = 0;
            bit_file->mask = 0x80;
        }
        mask >>= 1;
    }
}

int bitioFileInputBit( bit_file )
BitFile *bit_file;
{
    int value;

    if ( bit_file->mask == 0x80 ) {
        bit_file->rack = getc( bit_file->file );
		++bit_file->index;
        assert( bit_file->rack != EOF );  // fatal_error( "Fatal error in InputBit!\n" );
#if 0
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
	        putc( '.', stdout );
#endif
    }
    value = bit_file->rack & bit_file->mask;
    bit_file->mask >>= 1;
    if ( bit_file->mask == 0 )
	bit_file->mask = 0x80;
    return( value ? 1 : 0 );
}

unsigned long bitioFileInputBits( bit_file, bit_count )
BitFile *bit_file;
int bit_count;
{
    unsigned long mask;
    unsigned long return_value;

    mask = 1L << ( bit_count - 1 );
    return_value = 0;
    while ( mask != 0) {
	if ( bit_file->mask == 0x80 ) {
	    bit_file->rack = getc( bit_file->file );
		++bit_file->index;
		assert( bit_file->rack != EOF );  //fatal_error( "Fatal error in InputBit!\n" );
#if 0
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
    		putc( '.', stdout );
#endif
	}
	if ( bit_file->rack & bit_file->mask )
            return_value |= mask;
        mask >>= 1;
        bit_file->mask >>= 1;
        if ( bit_file->mask == 0 )
            bit_file->mask = 0x80;
    }
    return( return_value );
}

void bitioFilePrintBinary( file, code, bits )
FILE *file;
unsigned int code;
int bits;
{
    unsigned int mask;

    mask = 1 << ( bits - 1 );
    while ( mask != 0 ) {
        if ( code & mask )
            fputc( '1', file );
        else
            fputc( '0', file );
        mask >>= 1;
    }
}

//
//  Init BitIO resources.
//
//  return
//      0 on failure
//      non-zero on success
//
int bitioInit(void)
{
    return 1;
}

//
//  Shut down BitIO resources.
//
void bitioShutdown(void)
{

}

//
//  open for input or output
//
BitBuffer *bitioBufferOpen(char *buffer)
{
    BitBuffer *bitBuffer = (BitBuffer *)malloc(sizeof(BitBuffer));
	assert(buffer);
    assert(bitBuffer);

    bitBuffer->buffer = buffer;
    bitBuffer->rack = 0;
    bitBuffer->mask = 0x80;
    bitBuffer->pacifier_counter = 0;
    bitBuffer->index = 0;
    return bitBuffer;
}

//
//  Write out any pending partial bytes and close.
//  Return size of buffer written (in bytes, rounded up).
//
int bitioBufferCloseOutput(BitBuffer *bitBuffer)
{
    int size = bitBuffer->index;

    if (bitBuffer->mask != 0x80 )
    {
        *(bitBuffer->buffer + bitBuffer->index++) = bitBuffer->rack;
        ++size;
    }
    free(bitBuffer);
    return size;
}

//
//  close (no need to flush anything for input)
//
//	return bytes read
//
int bitioBufferCloseInput(BitBuffer *bitBuffer)
{
	int ret = bitBuffer->index;
    free(bitBuffer);
	return ret;
}

void bitioBufferOutputBit(BitBuffer *bitBuffer, int bit)
{
    if (bit)
        bitBuffer->rack |= bitBuffer->mask;
    bitBuffer->mask >>= 1;
    if (bitBuffer->mask == 0)
    {
        *(bitBuffer->buffer + bitBuffer->index++) = bitBuffer->rack;
#if 0
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		    putc( '.', stdout );
#endif
    	bitBuffer->rack = 0;
	    bitBuffer->mask = 0x80;
    }
}

void bitioBufferOutputBits(BitBuffer *bitBuffer, unsigned long code, int count)
{
    unsigned long mask;

    mask = 1L << ( count - 1 );
    while ( mask != 0)
    {
        if ( mask & code )
            bitBuffer->rack |= bitBuffer->mask;
        bitBuffer->mask >>= 1;
        if ( bitBuffer->mask == 0 )
        {
            *(bitBuffer->buffer + bitBuffer->index++) = bitBuffer->rack;
#if 0
            if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
		        putc( '.', stdout );
#endif
    	    bitBuffer->rack = 0;
            bitBuffer->mask = 0x80;
        }
        mask >>= 1;
    }
}

int bitioBufferInputBit(BitBuffer *bitBuffer)
{
    int value;

    if (bitBuffer->mask == 0x80)
    {
        bitBuffer->rack = *(bitBuffer->buffer + bitBuffer->index++);
#if 0
        if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
	        putc( '.', stdout );
#endif
    }
    value = bitBuffer->rack & bitBuffer->mask;
    bitBuffer->mask >>= 1;
    if (bitBuffer->mask == 0)
	    bitBuffer->mask = 0x80;
    return value ? 1 : 0;
}

unsigned long bitioBufferInputBits(BitBuffer *bitBuffer, int count)
{
    unsigned long mask;
    unsigned long return_value;

    mask = 1L << (count - 1);
    return_value = 0;
    while (mask != 0)
    {
    	if (bitBuffer->mask == 0x80)
        {
    	    bitBuffer->rack = *(bitBuffer->buffer + bitBuffer->index++);
#if 0
            if ( ( bit_file->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
                putc( '.', stdout );
#endif
    	}
    	if (bitBuffer->rack & bitBuffer->mask)
                return_value |= mask;
        mask >>= 1;
        bitBuffer->mask >>= 1;
        if (bitBuffer->mask == 0 )
            bitBuffer->mask = 0x80;
    }
    return return_value;
}

