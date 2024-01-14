#!/bin/sh

# This compiles kas2c on macOS. It could be done as an XCode project
# but that seems to be a bit of an overkill. The only Mac OS X-specific
# parts of this script are the #defines (-D) provided to gcc which
# toggle some header paths.

/usr/bin/lex -i -olexer.c lexer.l

/usr/bin/yacc -d -o parser.c parser.y

/usr/bin/gcc -o kas2c -I ../../Mac/includes *.c -ll

# now compile the *.kas mission files
cd ../../src/Missions
pwd
for Y in SinglePlayer Tutorials
do
    # change to the .kas folders in turn
    cd $Y

    for X in *.kas
    do
        # get the mission name
        `/bin/echo $X | /usr/bin/awk -F '\.' '{print "export MISSION=" $1}'`

        # use gcc's preprocessor to produce the input for kas2c; the grep -v is
        # to strip a gcc #pragma that kas2c cannot cope with
        /usr/bin/gcc -E -x c $MISSION.kas | /usr/bin/grep -v \#pragma > $MISSION.kp

        # produce the actual mission .h and .c files
        ../../../tools/kas2c/kas2c $MISSION.kp $MISSION.c $MISSION.h $MISSION.func.c
    done

    # move the .h and .c files to the Generated folder
    mv *.h *.c ../Generated

    # clean up the intermediate .kp files
    rm *.kp

    # return to src/Missions root
    cd ..

done

exit 0
