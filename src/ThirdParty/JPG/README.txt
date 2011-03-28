The JPEG library is written around the assumption that you will be dealing with files directly via a FILE *. Unfortunately, Homeworld also uses an alternative file storage mechanism, the .BIG archive file, which you cannot interact with via FILE *. Instead you have to use File's 'filehandle' mechanism. The original Relic team took the JPEG code and edited it directly to get the correct behaviour. That means that if you want to upgrade the JPEG source code to a more modern version you have to reimplement the changes they made. 

I tried to #define a way of making these changes whilst retaining the original JPEG code but unfortunately the headers are just not written with something like that in mind and "conflicting types" errors abound. So, instead, here's what needs changing when upgrading the JPEG code:

1) jinclude.h: include File.h so that the JPEG code knows about filehandles

   #include "File.h"


2) jinclude.h: rename the JFREAD #define to JFREAD_ANSI and add a new JFREAD #define that uses the functions in File.h:

    // Homeworld SDL
    #define JFREAD(file,buf,sizeofbuf)  \
      (fileBlockReadNoError((file), (void *) (buf), (sizeofbuf)))
    #define JFREAD_ANSI(file,buf,sizeofbuf)  \
      ((size_t) fread((void *) (buf), (size_t) 1, (size_t) (sizeofbuf), (file)))


3) JFREAD is referenced by all the JPEG library code so that it is easy to call whatever file handling code we want to use. However, the JPEG library also includes a virtual memory implementation that relies on tmpfile(). File.c does not support any temporary file handling (it expects to read/write known files) and it's not worth adding it for this one use-case, so we need to call the standard ANSI fread in that bit of code. In jmemansi.c, replace read_backing_store's use of JFREAD with JFREAD_ANSI:

    if (JFREAD_ANSI(info->temp_file, buffer_address, byte_count)
        != (size_t) byte_count)


4) jpeglib.h: change jpeg_stdio_src's function prototype to accept filehandles:

   EXTERN(void) jpeg_stdio_src JPP((j_decompress_ptr cinfo, filehandle infile));


5) jdatasrc.c: change jpeg_stdio_src's function definition to accept filehandles:

   jpeg_stdio_src (j_decompress_ptr cinfo, filehandle infile)


6) delete Makefile.in (built from Makefile.am)


7) check if Makefile.am needs updating (unlikely unless new files have been added/deleted).


NB: interfce.[ch] and jconfig.h are the "hook" files between Homeworld SDL and the JPEG code.
Do not delete these when upgrading the JPEG code.
