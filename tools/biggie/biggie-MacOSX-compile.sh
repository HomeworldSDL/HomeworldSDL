#!/bin/zsh

# This compiles biggie on Mac OS X.

/usr/bin/gcc *.h *.c ../../src/Game/BigFile.* ../../src/Game/BitIO.* ../../src/Game/CRC32.* ../../src/Game/LZSS.* ../../src/Game/Types.* -o biggie -D_MACOSX -D_MACOSX_FIX_ME

exit 0
