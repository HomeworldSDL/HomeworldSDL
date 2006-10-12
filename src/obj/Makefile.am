noinst_LIBRARIES = libhw_Sound.a
libhw_Sound_a_SOURCES =

# *** kludge alert ***
# this prevents the generated makefile from unlinking the .o files.
# we don't have the source for them. Makefile will attempt to
# rm *.donotkillofiles.
OBJEXT=donotkillofiles

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
