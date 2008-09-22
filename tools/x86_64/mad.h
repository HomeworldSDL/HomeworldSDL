#include "Types.h"

typedef struct
{
    udword name; //*                                //name of the object for this keyframe list
    crc16 nameCRC;                              //16-bit crc of the name.  (should upgrade to 32 later).
    ubyte pad[2];
    udword animationBits;                       //bit-flags: one for each animation
    sdword nKeyframes;                          //number of keyframes in all of file for this object
    udword times; //real32*                     //times array
    udword parameters;   //tcp*                   //tension, continuity, bias
    udword path[6];  //real32*                 //arrays of x,y,z,h,p,b
}
madobjpath_disk;


typedef struct
{
    udword name;   // *                                //name of the animation
    real32 startTime;                           //limits of the animation
    real32 endTime;
    udword flags;                               //flags for this animation
}
madanimation_disk;




typedef struct
{
    char identifier[8];                         //compared to MAD_FileIdentifier
    real32 version;                             //compared to MAD_FileVersion
    sdword stringBlockLength;                   //length of string block
    udword stringBlock;  //*                        //string block: where all strings for this file are located and offsets are relative to
    real32 length;                              //length of all animations
    real32 framesPerSecond;                     //rate the animation was scripted at
    sdword nObjects;                            //number of objects in the animation (should be same as number of objects in the mesh to animate)
    udword objPath; //*                       //list of keyframe lists for each object
    sdword nAnimations;                         //number of separate animations in the file
    madanimation_disk anim[1];                       //ragged array of animation headers
}
madheader_disk;

