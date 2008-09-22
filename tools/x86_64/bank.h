#include "Types.h"

typedef struct
{
    Uint16 format;
    Uint16 channels;
    Uint32 frequency;
    Uint32 avgBytesPerSecond;
    Uint16 blockAlign;
/*    Unit16 bitsPerSample; */
} SDLWAVEFORMAT;



typedef struct
{
    udword          id;
    udword          priority;
    sdword          pitch;
    sdword            dataoffset;
    sdword            datasize;
    sdword            loopstart;
    sdword            loopend;

    sword           bitrate;
    sword           flags;

    ubyte           volume;
    sbyte           pan;
    sbyte           pad[2];

    SDLWAVEFORMAT   waveformat;
    sword           wavepad;
} PATCH_DISK;


typedef struct
{
    udword          id;
    udword          checksum;
    sdword          numpatches;
    sdword          firstpatch;
} BANK;



typedef struct
{
    udword          id;
    udword          priority;
    sdword          pitch;
    smemsize            dataoffset;
    smemsize            datasize;
    smemsize            loopstart;
    smemsize            loopend;

    sword           bitrate;
    sword           flags;

    ubyte           volume;
    sbyte           pan;
    sbyte           pad[2];

    SDLWAVEFORMAT   waveformat;
    sword           wavepad;
} PATCH;



