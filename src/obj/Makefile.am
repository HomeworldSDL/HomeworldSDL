noinst_LIBRARIES = libhw_Sound.a
libhw_Sound_a_SOURCES =

if PLATFORM_WIN32
   libhw_Sound_a_LIBADD = \
      dct.munged \
      fqcodec.munged \
      fqeffect.munged \
      fquant.munged
else
   libhw_Sound_a_LIBADD = \
      $(top_srcdir)/src/obj/dct.o \
      $(top_srcdir)/src/obj/fqcodec.o \
      $(top_srcdir)/src/obj/fqeffect.o \
      $(top_srcdir)/src/obj/fquant.o
endif

%.munged: %.o
	objcopy $^ --prefix-symbols _ --redefine-sym _chkstk=__chkstk $@
