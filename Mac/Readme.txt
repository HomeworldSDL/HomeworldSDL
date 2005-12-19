{\rtf1\mac\ansicpg10000\cocoartf824\cocoasubrtf230
{\fonttbl\f0\fmodern\fcharset77 Courier;}
{\colortbl;\red255\green255\blue255;}
\margl1440\margr1440\vieww13680\viewh10540\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\ql\qnatural

\f0\fs26 \cf0 Welcome to the Homeworld SDL project!\
\
The directory containing this Readme also contains all the files specific to building Homeworld SDL for Mac OS X. To be able to build the project you first need to install the pre-compiled SDL Framework on your machine which can be downloaded from:\
\
    http://www.libsdl.org/index.php\
\
During the course of the project Apple has upgraded its build tools and the Homeworld SDL project files have been upgraded in tandem. Currently there are project files for the following IDEs:\
\
    XCode 2.1+     => Homeworld.xcodeproj  (current development IDE)\
    XCode 1.5      => Homeworld.xcode      (NB: this project is out-of-date)\
    ProjectBuilder => Homeworld.pbproj     (NB: this project is out-of-date)\
\
If you use the XCode 2.1+ project (highly recommended) then you you should be able to build a working Macintosh application out-of-the-box.\
\
NB: You will find things a lot easier if you copy the Homeworld game install files (i.e. the game data) into the appropriate build directory for the current active build configuration (e.g. "...Mac/build/Development" when using XCode 2.1+), so that you can start the game with the "build and debug/run" commands.\
\
----\
\
Helpful things to know:\
\
1) The Macintosh projects for the Homeworld port live in:\
   .../homeworld/Mac\
\
2) Mac OS X related #defines are explained in:\
   .../homeworld/Mac/Homeworld_Prefix.h\
\
3) Toggle full screen/windowed mode:\
   .../homeworld/src/SDL/main.c: bool fullScreen = TRUE;\
\
4) There are two targets: "Homeworld" and "Raider Retreat". Raider Retreat is the special OEM version of Homeworld that consists of the first four missions and a mission removed from the original game.\
}