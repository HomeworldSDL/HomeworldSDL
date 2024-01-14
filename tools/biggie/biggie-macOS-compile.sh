#!/bin/bash

# This compiles biggie on macOS.

/usr/bin/gcc                                       \
    -D __APPLE__                                   \
    -I ../../Mac/includes                          \
    -I ../../src/SDL                               \
    -I ../../src/ThirdParty/CRC                    \
    -I ../../src/ThirdParty/LZSS                   \
    ../../src/Game/Types.c                         \
    ../../src/Game/BigFile.c                       \
    ../../src/ThirdParty/CRC/CRC32.c               \
    ../../src/ThirdParty/LZSS/*.c                  \
    main.c                                         \
    options.c                                      \
    -o biggie
    
cd ../../HomeworldSDL_big
./convert_directory_to_big_file

exit 0
