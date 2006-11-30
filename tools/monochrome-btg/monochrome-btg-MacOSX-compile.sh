#!/bin/zsh

# This compiles monochrome-btg on Mac OS X

/usr/bin/gcc                                       \
    -D _MACOSX                                     \
    -I /Library/Frameworks/SDL.framework/Headers   \
    -I ../../src/Game                              \
    ../../src/Game/Types.c                         \
    *.c                                            \
    -o monochrome-btg

exit 0