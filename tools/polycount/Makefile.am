noinst_PROGRAMS = polycount

# Needed for access to tokens.
AM_YFLAGS="-d"
AM_LFLAGS="-i"

polycount_SOURCES = file.cpp file.h main.cpp mex.cpp mex.h types.h
polycount_CPPFLAGS = -I$(top_srcdir)/src/Game
polycount_LDADD = @LEXLIB@ @SDL_LIBS@
