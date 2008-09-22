
CFLAGS = -c -Wall -g -O2 -D_X86_64 -DHW_GAME_HOMEWORLD \
	-DHW_BUILD_FOR_DISTRIBUTION -I. -I../../src/SDL -I../../src/Game \
	-I/usr/include/SDL -I../../src/ThirdParty/CRC
LDFLAGS = -O2 file.o
BIN_CONV = bank fib font geo lif ll mad nis
OBJECTS_CONV= file.o bank.o fib.o font.o geo.o lif.o ll.o mad.o nis.o 

all: $(OBJECTS_CONV) $(BIN_CONV)

clean:
	rm -f *.o $(OBJECTS_CONV)

#.c:
#	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

