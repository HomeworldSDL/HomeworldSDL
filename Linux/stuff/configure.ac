AC_INIT([HomeworldSDL],[0.6],[baucheta@gmail.com])
AC_CONFIG_AUX_DIR(config)
AC_CANONICAL_TARGET
AC_CANONICAL_HOST
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall foreign dist-bzip2])

AC_LIBTOOL_WIN32_DLL
AC_PROG_CC
if test "$CC" = :; then
  AC_MSG_ERROR([This package needs a C compiler.])
fi
AC_PROG_YACC
if test "$YACC" = :; then
  AC_MSG_ERROR([This package needs yacc to compile.])
fi
AM_PROG_LEX
if test "$LEX" = :; then
  AC_MSG_ERROR([This package needs lex to compile.])
fi
AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL

dnl Check if building on Windows (based off GTK+ 2.2.4 configure.in...I'm not
dnl very familiar with autoconf).
AC_MSG_CHECKING([if we're in Windows])
case "$host" in
	*-*-mingw*|*-*-cygwin*)
		platform_win32=yes
		;;
	*)
		platform_win32=no
		;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

dnl Platform-independent data types.
dnl AX_CREATE_STDINT_H

dnl "bool" supported by C++ compiler?
dnl AC_CXX_BOOL

dnl Check for SDL.
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

dnl Configure options for building Raiders version.
AC_ARG_ENABLE(raiders,
	AS_HELP_STRING([--enable-raiders],[build a Raiders Retreat binary for Homeworld]),
	enable_raiders=yes)
if test "x$enable_raiders" = "xyes" ; then
	echo "* Building Raiders Retreat binary for Homeworld (HW_GAME_RAIDER_RETREAT) *"
	HWGAME_CFLAGS="-DHW_GAME_RAIDER_RETREAT"
fi

dnl Configure options for building Demo version.
AC_ARG_ENABLE(demo,
	AS_HELP_STRING([--enable-demo],[build a Demo binary for Homeworld]),
	enable_demo=yes)
if test "x$enable_demo" = "xyes" ; then
	echo "* Building Demo binary for Homeworld (HW_GAME_DEMO) *"
	HWGAME_CFLAGS="-DHW_GAME_DEMO"
fi

dnl Configure options for building Raiders version.
if test "x$HWGAME_CFLAGS" = "x" ; then
	echo "* Building Release binary for Homeworld (HW_GAME_HOMEWORLD) *"
	HWGAME_CFLAGS="-DHW_GAME_HOMEWORLD"
fi

dnl Configure options for building debug version.
AC_ARG_ENABLE(hwdebug,
	AS_HELP_STRING([--enable-hwdebug],[build a debug binary for Homeworld]),
	enable_hwdebug=yes)
if test "x$enable_hwdebug" = "xyes" ; then
	echo "* Building debug binary for Homeworld (HW_BUILD_FOR_DEBUGGING) *"
	HWBIN_CFLAGS="-DHW_BUILD_FOR_DEBUGGING"
else
	echo "* Building distribution binary for Homeworld (HW_BUILD_FOR_DISTRIBUTION) *"
	HWBIN_CFLAGS="-DHW_BUILD_FOR_DISTRIBUTION"
fi

dnl Stolen from gcc, set CC_FOR_BUILD right so we build host tools with host compiler
if test "${build}" != "${host}" ; then
  CC_FOR_BUILD=${CC_FOR_BUILD-gcc}
else
  CC_FOR_BUILD="${CC}"
fi

dnl Configure options for building X86_64 version.
AC_ARG_ENABLE(x86_64,
    AS_HELP_STRING([--enable-x86_64],[build an experimental x86_64 binary for Homeworld]),
    enable_x86_64=yes)
if test "x$enable_x86_64" = "x" ; then
    if test "$host_cpu" = "arm" ; then
        echo "* Building ARM binary for Homeworld (ARM)"
        CFLAGS="$CFLAGS -DARM -DGENERIC_ETGCALLFUNCTION"
    else
        echo "* Building x86 binary for Homeworld (X86) *"
        CFLAGS="$CFLAGS -malign-double -D_X86"
	x86_class_cpu="true"
    fi
else
    echo "* Building x86_64 binary for Homeworld (X86_64) *"
    CFLAGS="$CFLAGS -D_X86_64 -D_X86_64_FIX_ME"
    x86_class_cpu="true"
fi
AM_CONDITIONAL(X86_CPU, test x$x86_class_cpu = xtrue)

dnl Force generic ETG function call
AC_ARG_ENABLE(generic_ETG,
	AS_HELP_STRING([--enable-generic-etg],
        [Some platforms have assembly implementation of etgFunctionCall, if you want to test the C only generic implementation on these platforms use this option]),
        enable_generic_etg=yes)
if test "x$enable_generic_etg" = "xyes" ; then
    echo "* Forceing generic etgFunctionCall (-DGENERIC_ETGCALLFUNCTION) *"
    CFLAGS="$CFLAGS -DGENERIC_ETGCALLFUNCTION "
fi

dnl LINUX FIX ME
AC_ARG_ENABLE(linuxfixme,
	AS_HELP_STRING([--disable-linuxfixme],
		[some major buggy (on linux) code exists, disable if temporary workaround isn't wanted]), [linuxfixme=no], [linuxfixme=yes])
if test "$platform_win32" = "no" ; then
    if test "x$linuxfixme" = "xyes" ; then
    	echo "* Building linux workaround build (-D_LINUX_FIX_ME) *"
    	CFLAGS="$CFLAGS -D_LINUX_FIX_ME "
    	CPPFLAGS="$CPPFLAGS -D_LINUX_FIX_ME "
    else
    	echo "* Building linux un-fixed build *"
    fi
fi

dnl WIN32 FIX ME
AC_ARG_ENABLE(win32fixme,
	AS_HELP_STRING([--disable-win32fixme],
		[some major buggy (on win32) code exists, disable if temporary workaround isn't wanted]), [win32fixme=no], [win32fixme=yes])
if test "$platform_win32" = "yes" ; then
    if test "$win32fixme" = "yes" ; then
    	echo "* Building win32 workaround build (-D_WIN32_FIXME) *"
    	WIN32_FIXME="-D_WIN32_FIXME"
    else
    	echo "* Building win32 un-fixed build *"
    fi
fi

dnl Configure options for Enabling Movies.
AC_ARG_ENABLE(movies,
        AS_HELP_STRING([--enable-movies],[include movie code into Homeworld]),[movies=yes])
if test "x$movies" = "xyes" ; then
	AC_CHECK_HEADER([libavformat/avformat.h], [
		AC_CHECK_HEADER([libavcodec/avcodec.h], [
		  AC_CHECK_HEADER([libswscale/swscale.h], [
			AC_CHECK_LIB([swscale], [sws_scale], [
				AC_CHECK_LIB([avformat],[av_open_input_file],[
			        	echo "* Including Movie code for Homeworld (HW_ENABLE_MOVIES) *"
				        HWGAME_CFLAGS="$HWGAME_CFLAGS -DHW_ENABLE_MOVIES"
				        MOVIE_LIBS="-lavcodec -lavformat -lswscale"
				],[])
	          ],[])
			], [])
		],[])
	],[])
fi

dnl Configure options for Enabling Network gaming.
AC_ARG_ENABLE(network,
        AC_HELP_STRING([--enable-network], [include network code into Homeworld]),[network=yes])
if test "x$network" = "xyes" ; then
	AC_CHECK_HEADER([SDL/SDL_net.h],[
		AC_CHECK_LIB([SDL_net], [SDLNet_Init], [
		        echo "* Including Network Game code for Homeworld (HW_ENABLE_NETWORK) *"
			HWGAME_CFLAGS="$HWGAME_CFLAGS -DHW_ENABLE_NETWORK"	
			NET_LIBS="-lSDL_net"
		], [])
	],[])
fi

dnl Configure options for Enabling old sound code.
AC_ARG_ENABLE(oldsound,
        AS_HELP_STRING([--enable-oldsound],[include old sound objects into Homeworld]),[oldsound=yes])
AM_CONDITIONAL(OLD_SOUND, test "$oldsound" = "yes")

AC_SUBST([MOVIE_LIBS])
AC_SUBST([NET_LIBS])
AC_SUBST([SSE_FLAGS])
AC_SUBST([TRANSFORM])
AC_SUBST(CC_FOR_BUILD)

dnl Add necessary flags for compiler and linker.
dnl - One (and only one) of "HW_BUILD_FOR_DEBUGGING" or "HW_BUILD_FOR_DISTRIBUTION"
dnl   must be set. This is defined above in the HWBIN_CFLAGS variable.
dnl   HW_BUILD_FOR_DEBUGGING is set via "--enable-hwdebug";
dnl   HW_BUILD_FOR_DISTRIBUTION is the default.
dnl - $SDL_CFLAGS and $SDL_LIBS should be pretty much self-explanatory. =]
dnl - We don't want to include $SDL_LIBS when building librgl.
CFLAGS="$CFLAGS $HWGAME_CFLAGS $HWBIN_CFLAGS $SDL_CFLAGS $WIN32_FIXME"

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile src/Game/Makefile src/Missions/Generated/Makefile src/ThirdParty/Makefile src/ThirdParty/CRC/Makefile src/ThirdParty/JPG/Makefile src/ThirdParty/LZSS/Makefile src/ThirdParty/Titan/Makefile src/SDL/Makefile src/Ships/Makefile src/obj/Makefile tools/Makefile tools/kas2c/Makefile])
AC_OUTPUT
