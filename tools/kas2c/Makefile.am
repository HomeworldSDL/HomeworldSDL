noinst_PROGRAMS = kas2c

# Needed for access to tokens.
AM_YFLAGS="-d"
AM_LFLAGS="-i"

CC=$(CC_FOR_BUILD)

kas2c_SOURCES = parser.y parser.h lexer.l kas2c.c kas2c.h 
lexer.l: parser.c
kas2c_CPPFLAGS = -I$(top_srcdir)/src/Game
kas2c_LDADD = @LEXLIB@
