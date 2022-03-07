#!/bin/zsh

# This compiles kas2c on Mac OS X. It could be done as an XCode project
# but that seems to be a bit of an overkill. The only Mac OS X-specific
# parts of this script are the #defines (-D) provided to gcc which
# toggle some header paths.

/usr/bin/lex -i -olexer.c lexer.l

/usr/bin/yacc -d -o parser.c parser.y

/usr/bin/gcc                                       \
    -D_MACOSX                                      \
    -I ../../Mac/SDL2.framework/Headers            \
    *.c                                            \
    -o kas2c

exit 0
