#include "Types.h"


typedef struct
{
    udword textureName;  // *char               //name of texture, an offset from start of string block
    sdword width, height;                       //size of texture
    udword flags;                               //flags for things like alpha and luminance map
    crc32 imageCRC;                             //crc of the unquantized image

    sdword nShared;                             //number of images which share this one
    udword sharedTo;  // *sdword                //list of indices of images which share this one
    sdword sharedFrom;                          //image this one is shared from, or -1 if none
}
llelement_disk;



