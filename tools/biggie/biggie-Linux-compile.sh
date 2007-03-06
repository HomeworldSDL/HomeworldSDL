#!/bin/sh
gcc -o biggie -Wall -O2 -D_LINUX_FIX_ME -I../../src/ThirdParty/CRC \
	-I../../src/ThirdParty/LZSS `sdl-config --cflags` \
	main.c options.c ../../src/Game/BigFile.c \
	../../Linux/src/ThirdParty/CRC/libhw_CRC.a \
	../../Linux/src/ThirdParty/LZSS/libhw_LZSS.a `sdl-config --libs`
