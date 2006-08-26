Welcome to the Homeworld SDL project!

The directory containing this Readme also contains all the files specific to building Homeworld SDL for Mac OS X. To be able to build the project you first need to install the pre-compiled SDL Framework on your machine which can be downloaded from:

    http://www.libsdl.org/index.php

The current IDE is Xcode 2.4. If you require them, previous projects for ProjectBuilder/Xcode (not up-to-date obviously) are available in the following tagged versions of the repository tree:

    $SUBVERSION_REPOSITORY/homeworld/tags/20051210-MacOSX-Xcode1.5-SDL1.2.9
    $SUBVERSION_REPOSITORY/homeworld/tags/20060605-MacOSX-pre-XCode-2.3-Projects

If you use the current Xcode project (highly recommended) then you you should be able to build a working Macintosh application out-of-the-box.

NB: You will find things a lot easier if you copy the Homeworld game files into the directory where Xcode will build your executable (e.g.: ".../Mac/build/Development"). This will allow you run the game using the "build and debug/run" commands.

----

Helpful things to know:

1) There are two targets in the Xcode project: "Homeworld" and "Raider Retreat". Homeworld is the full game. Raider Retreat is a special version of Homeworld that consists of the first four missions and a mission removed from the original game. 

2) Mac OS X related #defines are explained in:
   .../homeworld/Mac/Homeworld_Prefix.h

3) Toggle full screen/windowed mode:
   .../homeworld/src/SDL/main.c: bool fullScreen = TRUE;

