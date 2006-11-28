#!/bin/zsh

# This compiles biggie on Mac OS X.

/usr/bin/gcc                             \
    -D _MACOSX                           \
    -D _MACOSX_FIX_ME                    \
    -I ../../src/ThirdParty/CRC          \
    -I ../../src/ThirdParty/LZSS         \
    *.c *.h                              \
    ../../src/Game/BigFile.[ch]          \
    ../../src/Game/Types.[ch]            \
    ../../src/ThirdParty/CRC/CRC32.[ch]  \
    ../../src/ThirdParty/LZSS/*.[ch]     \
    -o biggie

exit 0
