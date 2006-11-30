Ever wanted to take a screenshot in Homeworld but with a plain background so you could easily extract the object you were imaging? Well now you can! (But see later for a gotcha...)

Homeworld's backgrounds are stored in .btg files (see also src/Game/BTG.*).  They consist of a primary background colour which is then superimposed by a vertex-coloured polygon mesh that provides structure (in the game, they form the galaxy backdrops) and a sprinkling of targa (.tga) images for large/bright stars.

monochrome-btg creates a .btg with the primary background colour of your choosing - set by three 0-255 red, green and blue values - but without any polygons or stars:

    monochrome-btg <red_255> <green_255> <blue_255>

For example:

    monochrome-btg   0   0   0     ->     rgb_000_000_000.btg  (black)
    monochrome-btg 255 255 255     ->     rgb_255_255_255.btg  (white)

The resulting file is needless to say, very small: 84 bytes. The easiest way to use the file is to find a multiplayer map with its own .btg file and replace it with the one you create. (For example: ChainOfFate. A list of map resources can be found at: http://www.machomeworld.com/forums/viewtopic.php?t=49) Load up a multiplayer game on that map and screenshot to your heart's content.


GOTCHA:

The .btg file does not appear to be the only thing that Homeworld renders in the background - grr. It also appears to draw a random number of scattered grey pixels over the background to represent dim stars, thus preventing apparently empty regions of space in barren backdrops. This is exceedingly annoying if you want a black background (lighter backdrops tend to wash these stars out though).

If anyone finds out where this happens then please update this file so we can disable it when necessary!
