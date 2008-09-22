#include <Types.h>
#include <CRC32.h>

typedef struct
{
    char ident[8];                              //compared to "Willy 7"
    sdword version;                             //version number
    sdword flags;                               //to plug straight into texreg flags
    sdword width, height;                       //dimensions of image
    crc32 paletteCRC;                           //a CRC of palettes for fast comparison
    crc32 imageCRC;                             //crc of the unquantized image
    udword data; //*                              //actual image
    udword palette;//*                            //palette for this image
    udword teamEffect0, teamEffect1; //* *        //palettes of team color effect
//    sdword nSizes;                              //number of sizes encoded
//    udword image[LLC_NumberImages];             //offset-pointers to the individual images (from end of this header)
}
lifheader_disk;

