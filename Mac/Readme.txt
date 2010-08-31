Welcome to the Homeworld SDL project!

The directory containing this Readme also contains all the files specific to building Homeworld SDL for Mac OS X. To be able to build the project you need to:

1) Install the pre-compiled SDL.framework on your computer. It can be downloaded from:

    http://www.libsdl.org/index.php

Instructions for installation should be included but basically you need to copy SDL.framework into either:

    /Library/Frameworks/                 # available to all users on the machine
    /Users/<user>/Library/Frameworks/    # only available to <user>


2) Install the Apple Developer Tools. You can either do this from your Mac OS X CD/DVDs or via Apple's Developer connection (http://developer.apple.com/). The current IDE is Xcode 2.4. If you require them, previous projects for ProjectBuilder/Xcode (not up-to-date obviously) are available in the following tagged versions of the repository tree:

    <repository>/homeworld/tags/20051210-MacOSX-Xcode1.5-SDL1.2.9
    <repository>/homeworld/tags/20060605-MacOSX-pre-XCode-2.3-Projects


3) Open the project: Homeworld.xcodeproj. If you use the current Xcode project (highly recommended) then you you should be able to build a working Macintosh application out-of-the-box by clicking on the "Build" button (Command-B).

NB: You will find things a lot easier if you copy the Homeworld game files into the directory where Xcode will build your executable (e.g.: ".../Mac/build/Development"). This will allow you run the game using the "Build and Debug", "Run" commands.

----

Helpful things to know:

1) There are two targets in the Xcode project: "Homeworld" and "Raider Retreat". Homeworld is the full game. Raider Retreat is a special version of Homeworld that consists of the first four missions and a mission removed from the original game. 

2) Mac OS X related #defines are explained in:
   .../homeworld/Mac/Homeworld_Prefix.h

3) Toggle full screen/windowed mode:
   .../homeworld/src/SDL/main.c: bool fullScreen = TRUE;

    NB: Development builds use "fullScreen = FALSE" by default
    NB: Deployment  builds use "fullScreen = TRUE"  by default

