#!/bin/zsh

# This compiles kas2c on Mac OS X. It could be done as an XCode project
# but that seems to be a bit of an overkill. The only Mac OS X-specific
# parts of this script are the #defines (-D) provided to gcc which
# toggle some header paths.

/bin/echo -----------------------------------------------------------------
/bin/echo gcc will complain about TRUE/FALSE being redefined when compiling
/bin/echo kas2c. This is normal and can be safely ignored.
/bin/echo -----------------------------------------------------------------

/usr/bin/lex -i -olexer.c lexer.l
/usr/bin/yacc -d -o parser.c parser.y
/usr/bin/gcc lexer.c parser.c parser.h kas2c.c kas2c.h -o kas2c -D_MACOSX

exit 0
