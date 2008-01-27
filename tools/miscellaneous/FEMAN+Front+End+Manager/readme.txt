Homeworld FEMan release
July 11/00 by Luke Moloney of Relic Entertainment.
All included binaries and data are © 1997-2000 Relic Entertainment Inc.  The attached binaries are for educational and amusement purposes only.

This zip file is a release of all of the FEMan source files and the FEMan tool used to create all of the front ends in Homeworld.  This program and data are to be used at your own risk.  Relic does not warranty this release and acknowledges that there are many bugs in the FEMan.


File types

The data files for front ends are of the format .fib.  The FEMan program edits file of the extension .fem and exports to .fib's.  .lif files are used for front end textures.  .PSD files are used as the source for .lifs.


Directory structure

To use this program, you will need to have unpacked some of the contents of the Homeworld.big file.

1)	Unpack FEMan\* and all it's subdirectories into a directory called FEMan.  This directory is where the games looks for all front end data data.
2)	Next, create a directory off the Homeworld root called data\FEMan.
3)	When you are ready to run the game, you can copy the contents of data\FEMan to FEMan directory.
If this sounds confusing, it's because the directory structure of the released game is a lot simpler than the directory structure used in development.  This complex directory structure is only needed if you want to change the textures.  If you only want to change the layout of the screens but not the graphics, you can simply export (described below) into the FEMan directory.

Here is a tree view:
c:\
|
--->Sierra
    |
    ------>Homeworld                #where you run makeFEMan.bat from
            |
            --------->data
            |         |
            |         ---->FEMan    #where you create the .fem and .lif files
                           |
                           ---->Textures
            |
            --------->DataSrc
            |         |
            |         ---->FEMan
                           |
                           ---->Textures
            |
            --------->FEMan         #where to copy data\Feman to before running the game
                      |
                      ---->Textures


Usage:

When you run the FEMan program, you can load any of the files in DataSrc\FEMan and edit them.  To export to the game, select File|Export and direct it to the Data\FEMan directory (or FEMan, if you are not changing textures).  The name to export to is the same as the name of the .fem file you are editing, but with the .fib extension.

Also included is a little users guide I created for the external localization organization.  It's pretty specific to localizing, rather than just editing.  It's a good intro to FEMan, however.  The rest of FEMan should be pretty easy to use.

Bugs:

This program was always a bit buggy due to not having enough time to support it.  Remember to save often, as it can sometimes crash.  Also, it can sometimes start off-screen (an MFC bug, I think).  Just create a shortcut with the Run Maximized option set.

Luke Moloney
lmoloney@relic.com

