#!/bin/zsh

# This compiles biggie on Mac OS X.

/usr/bin/gcc                                       \
    -D _MACOSX                                     \
    -I /Library/Frameworks/SDL.framework/Headers   \
    -I ../../src/SDL                               \
    -I ../../src/ThirdParty/CRC                    \
    -I ../../src/ThirdParty/LZSS                   \
    ../../src/Game/Types.c                         \
    ../../src/Game/BigFile.c                       \
    ../../src/ThirdParty/CRC/CRC32.c               \
    ../../src/ThirdParty/LZSS/*.c                  \
    *.c                                            \
    -o biggie

exit 0
