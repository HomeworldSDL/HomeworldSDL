#  Original CFLAGS line
# CFLAGS += -malign-double -Wall -fno-strict-aliasing -Wextra
#  Additional Modified CFLAGS line
# CFLAGS += -malign-double -Wall -fno-strict-aliasing   -Wextra -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls
AM_CFLAGS = -Wall -fno-strict-aliasing -Wextra

noinst_LIBRARIES = libhw_SDL.a
libhw_SDL_a_SOURCES = avi.c avi.h dct.h debugwnd.h devstats.h font.c font.h fqcodec.h fqeffect.h fquant.h glinc.h main.c main.h mainrgn.c mainrgn.h mainswitches.h mixfft.c mixfft.h mouse.c mouse.h NetworkInterface.c NetworkInterface.h prim2d.c prim2d.h prim3d.c prim3d.h Queue.c Queue.h regkey.h render.c render.h resource.h rglu.c rglu.h rinit.c rinit.h screenshot.c screenshot.h smixer.c soundcmn.h soundlow.c soundlow.h sstream.c standard_library.h texreg.c texreg.h TimeoutTimer.c TimeoutTimer.h Titan.c Titan.h TitanInterfaceC.h TitanInterfaceC.c utility.c utility.h
libhw_SDL_a_CPPFLAGS = -I$(top_srcdir)/src/Game -I$(top_srcdir)/src/ThirdParty/CRC -I$(top_srcdir)/src/Ships

# Some extra features if using Win32
if PLATFORM_WIN32
libhw_SDL_a_SOURCES += debugwnd.c wave.c wave.h
else
if OLD_SOUND
libhw_SDL_a_SOURCES += win32glue.c
endif
endif

if OLD_SOUND
else
libhw_SDL_a_SOURCES += fqcodec.c fqeffect.c fquant.c dct.c
endif

# render.c contains a task function, which requires ebp to be used as the
# frame pointer.
render.$(OBJEXT): render.c
	if $(COMPILE) -fno-omit-frame-pointer -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -c -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi
