# Additional defined flags for extra warnings.
# CFLAGS += -malign-double -Wall -fno-strict-aliasing -Wextra -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls

# Note the ordering (we haven't defined the dependencies too strongly).
SUBDIRS = Missions/Generated Game ThirdParty/CRC ThirdParty/JPG ThirdParty/LZSS SDL Ships obj

bin_PROGRAMS = homeworld

HW_LIBS = \
   ./Game/libhw_Game.a \
   ./SDL/libhw_SDL.a \
   ./obj/libhw_Sound.a \
   ./Ships/libhw_Ships.a \
   ./Missions/Generated/libhw_Generated.a \
   ./ThirdParty/LZSS/libhw_LZSS.a \
   ./ThirdParty/CRC/libhw_CRC.a \
   ./ThirdParty/JPG/libhw_JPG.a

DEP_LIBS = \
   ./Game/libhw_Game.a \
   ./SDL/libhw_SDL.a \
   ./Ships/libhw_Ships.a \
   ./Missions/Generated/libhw_Generated.a \
   ./ThirdParty/LZSS/libhw_LZSS.a

homeworld_LIBTOOLFLAGS = --preserve-dup-deps
homeworld_LDADD = $(HW_LIBS) $(DEP_LIBS) -L/usr/X11R6/lib -lX11 -lXext @SDL_LIBS@ $(MOVIE_LIBS) $(NET_LIBS)
homeworld_LDFLAGS = -Wl,--as-needed

