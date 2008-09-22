
#include <Types.h>


typedef struct
{
    udword bitmap;  // *                             //pointer to bitmap for character (was offset x/y)
    sword width, height;                        //size of character
    sword offsetX, offsetY;                     //offset in screen space to draw
}
charheader_disk;

typedef struct
{
    sword u, v;                                 //location of top-left corner of font
    sword width, height;                        //size of image
    sword offsetX, offsetY;                     //offset in screen space to draw
}
charfileheader_disk;


typedef struct
{
    sdword nCharacters;                         //number of characters in font
    sdword spacing;                             //inter-character spacing
    sdword fullHeight;                          //total height of font, from top of tallest char to bottom of lowest hanging characters
    sdword baseLine;                            //y-value of baseline
    udword name;   // *                             //optional name pointer
    sdword imageWidth;                          //dimensions of font image
    sdword imageHeight;
    sdword nColors;                             //colors in image palette
    udword palette;  // *                           //pointer to palette
    udword image;    // *                           //pointer to image
    udword glFont;   // *   
    ubyte reserved[12];                         //reserved for expansion of future attributes
    udword character_disk[256]; // *                //full character map
}
fontheader_disk;



typedef struct
{
    char identification[FONT_IdentLength];      //file type string
    uword version;                              //current version
    uword flags;                                //flags for font
    fontheader_disk header;
}
fontfileheader_disk;



