INCLUDES = -I$(top_srcdir)/src/Game -I$(top_srcdir)/src/ThirdParty/JPG -I$(top_srcdir)/src/ThirdParty/LZSS -I$(top_srcdir)/src/rgl -I$(top_srcdir)/src/SDL -I$(top_srcdir)/src/Ships

# Note the ordering (we haven't defined the dependencies too strongly).
SUBDIRS = Generated Game ThirdParty/JPG ThirdParty/LZSS rgl SDL Ships obj

bin_PROGRAMS = homeworld

HW_LIBS = \
   ./Game/libhw_Game.a \
   ./Generated/libhw_Generated.a \
   ./ThirdParty/JPG/libhw_JPG.a \
   ./ThirdParty/LZSS/libhw_LZSS.a \
   ./SDL/libhw_SDL.a \
   ./Ships/libhw_Ships.a \
   ./obj/libhw_Sound.a

homeworld$(EXEEXT): $(HW_LIBS)
	$(CC) -o $@ -Wl,--as-needed,-\( $(HW_LIBS) -lX11 -lXext @SDL_LIBS@ -Wl,-\)

