//-----------------------------------------------------------------------------
// ---------------------
// File ....: interfce.h
// ---------------------
// Author...: Gus J Grubba
// Date ....: October 1995
// Descr....: C Interface to JPEG library
//
// History .: Oct, 27 1995 - Started
//
//-----------------------------------------------------------------------------

#ifndef __JPEGINTERFACE_H__
#define __JPEGINTERFACE_H__

#include "File.h"

// The JPEG library is written around the assumption that you will be dealing
// with files directly via a FILE *. Unfortunately, Homeworld also uses an
// alternative file storage mechanism, the .BIG archive file, which you cannot
// interact with via FILE *. Instead you have to use "File"'s filehandle
// mechanism. The original Relic team took the JPEG code and edited it directly
// to get the correct behaviour. That means that if you want to upgrade the
// JPEG source code to a more modern version you have to reimplement the changes
// they made. 

// I tried to #define a way of making these changes whilst retaining the original
// JPEG code but unfortunately the headers are just not written with something
// like that in mind and "conflicting types" errors abound. So, instead, here's
// what needs changing when upgrading the JPEG code:

// 1) jinclude.h: include File.h so that the JPEG code knows about filehandles
//    #include "File.h"

// 2) jinclude.h: change the JFREAD #define to:
//    #define JFREAD(file, buf, sizeofbuf) (fileBlockReadNoError((file), (void *) (buf), (sizeofbuf)))

// 3) jpeglib.h: change jpeg_stdio_src's function prototype to accept filehandles:
//    EXTERN(void) jpeg_stdio_src JPP((j_decompress_ptr cinfo, filehandle infile));

// 4) jdatasrc.c: change jpeg_stdio_src's function definition to accept filehandles:
//    jpeg_stdio_src (j_decompress_ptr cinfo, filehandle infile)

// Laurence Passmore (20061119)


typedef struct _jpegdata {
   unsigned char *ptr;
   int    width;
   int    height;
   FILE  *output_file;
   filehandle  input_file;
   int    aritcoding;
   int    CCIR601sampling;
   int    smoothingfactor;
   int    quality;
   /*udword hWnd;*/
   int    ProgressMsg;
   int    status;
   int    components;
} JPEGDATA;

void JpegWrite( JPEGDATA *data );
void JpegInfo(  JPEGDATA *data );
void JpegRead(  JPEGDATA *data );

#endif
