#include <stdio.h>

#include "BTG.h"

void usage (int argc, char *argv[]);
void fix_header_endian (btgHeader *header);

int main (int argc, char *argv[]) {
    char       btg_filename[PATH_MAX];
    FILE      *btg_file;
    btgHeader  btg_header;
    
    // parse arguments
    if (argc != 4) {
        usage(argc, argv);
    }
    
    unsigned int red   = atoi(argv[1]);
    unsigned int green = atoi(argv[2]);
    unsigned int blue  = atoi(argv[3]);
    
    if (red > 255 || green > 255 || blue > 255) {
        fprintf(stderr, "0-255 level RGB colours please\n");
        usage(argc, argv);
    }

    // create BTG header
    btg_header.btgFileVersion = BTG_FILE_VERSION;
    
    btg_header.numVerts = 0;                // we're not a complicated BTG...
    btg_header.numStars = 0;
    btg_header.numPolys = 0;
    
    btg_header.xScroll = 0;                 // only used by the BTG editor
    btg_header.yScroll = 0;
    
    btg_header.zoomVal = 0;                 // only used by the BTG editor
    
    btg_header.pageWidth  = 360;            // rectangular dimensions of background
    btg_header.pageHeight = 180;            // arbitrary: ascension/declination degrees 
    
    btg_header.mRed   = 0;                  // not sure; only used by the BTG editor
    btg_header.mGreen = 0;
    btg_header.mBlue  = 0;
    
    btg_header.mBGRed   = (sdword) red;     // "see through" background colour
    btg_header.mBGGreen = (sdword) green;   // i.e. what we're really interested in
    btg_header.mBGBlue  = (sdword) blue;
    
    btg_header.bVerts    = 0;               // only used by the BTG editor
    btg_header.bPolys    = 0;
    btg_header.bStars    = 0;
    btg_header.bOutlines = 0;
    btg_header.bBlends   = 0;
    
    btg_header.renderMode = 0;              // only used by the BTG editor

    // sort out any endian issues before saving
    fix_header_endian(&btg_header);
    
    // construct filename from the chosen RGB combination
    sprintf(btg_filename, "rgb_%03u_%03u_%03u.btg", red, green, blue);
    
    // now save the BTG file
    btg_file = fopen(btg_filename, "w");
    
    if (btg_file == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", btg_filename);
        exit(-1);
    }
    
    fwrite(&btg_header, 1, sizeof(btgHeader), btg_file);
    
    fclose(btg_file);
}

void usage (int argc, char *argv[]) {
    fprintf(stderr, "usage: %s <red_255> <green_255> <blue_255>\n", argv[0]);
    exit(0);
}

void fix_header_endian (btgHeader *btg_header) {
#if FIX_ENDIAN
    btg_header->btgFileVersion = FIX_ENDIAN_INT_32( btg_header->btgFileVersion );

    btg_header->numVerts       = FIX_ENDIAN_INT_32( btg_header->numVerts       );
    btg_header->numStars       = FIX_ENDIAN_INT_32( btg_header->numStars       );
    btg_header->numPolys       = FIX_ENDIAN_INT_32( btg_header->numPolys       );

    btg_header->xScroll        = FIX_ENDIAN_INT_32( btg_header->xScroll        );
    btg_header->yScroll        = FIX_ENDIAN_INT_32( btg_header->yScroll        );

    btg_header->zoomVal        = FIX_ENDIAN_INT_32( btg_header->zoomVal        );

    btg_header->pageWidth      = FIX_ENDIAN_INT_32( btg_header->pageWidth      );
    btg_header->pageHeight     = FIX_ENDIAN_INT_32( btg_header->pageHeight     );

    btg_header->mRed           = FIX_ENDIAN_INT_32( btg_header->mRed           );
    btg_header->mGreen         = FIX_ENDIAN_INT_32( btg_header->mGreen         );
    btg_header->mBlue          = FIX_ENDIAN_INT_32( btg_header->mBlue          );

    btg_header->mBGRed         = FIX_ENDIAN_INT_32( btg_header->mBGRed         );
    btg_header->mBGGreen       = FIX_ENDIAN_INT_32( btg_header->mBGGreen       );
    btg_header->mBGBlue        = FIX_ENDIAN_INT_32( btg_header->mBGBlue        );

    btg_header->bVerts         = FIX_ENDIAN_INT_32( btg_header->bVerts         );
    btg_header->bPolys         = FIX_ENDIAN_INT_32( btg_header->bPolys         );
    btg_header->bStars         = FIX_ENDIAN_INT_32( btg_header->bStars         );
    btg_header->bOutlines      = FIX_ENDIAN_INT_32( btg_header->bOutlines      );
    btg_header->bBlends        = FIX_ENDIAN_INT_32( btg_header->bBlends        );

    btg_header->renderMode     = FIX_ENDIAN_INT_32( btg_header->renderMode     );
#endif
}