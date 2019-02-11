# HomeworldSDL

[SDL] port of the [Homeworld] source code that was [released in 2003] by [Relic].

## Installing

### Requirements

This repository contains only the source code for Homeworld's engine (metaphorically, the skeleton). In order to have a playable game, you will need the original assets (artwork, textures, sounds, meshes, ... Metaphorically again, the flesh).

So make sure you have:

- The Homeworld CD
- [The 1.05 Patch](http://www.homeworldaccess.net/downloads/hw1patch/si_homeworld_update_105.exe)

### Asset checklist

To sum it up, here are the files required for the game to run:

- [ ] `Homeworld.big`  
    > sha256: af9dcc06e3f99404334a0a8ec17e399876080e85feb5b47858382dc715136040
- [ ] `Update.big` (This comes from the official 1.05 Patch)  
    > sha256: c71b07758ee7696b3a19c1d75c946cbd68830e03b30cd3c2888f2f6d22b7c976
- [ ] `HW_Comp.vce`  
    > sha256: 15c4b988adb09b0969b0dc288b21ddc10ca9d42a2064d15b46b53dcf02fc2e44

- [ ] `HW_Music.wxd`  
    > sha256: b909c2cdbc8c8db67be168f6005bf8e8facaa9857263b16d186c824a0c4eed4f
- [x] `HomeworldSDL.big` (This file should be included in the [releases])

Usually you will find these files in your installation folder. For more detailed instructions, read on.

### Linux

#### Extracting game assets with wine

Install the game and patch with [wine] (tested with wine 4.1)

``` sh
wine HWSetup.EXE
wine si_homeworld_update_105.exe
```

Open the game installation folder (usually `~/.wine/drive_c/Sierra/Homeworld`). This is where you'll find the [required files](#asset-checklist). We'll be getting back at them later on.

#### Installing the binary distribution

1. Download [__the latest release__][releases]. (Extract it if necessary)
2. Copy [the files you gathered earlier](#extracting-game-assets-with-wine) in the folder you just downloaded
3. Run the game
4. Jump to [Configuring the OpenGL renderer](#configuring-the-opengl-renderer)

## Configuring the OpenGL renderer

The first time you run the game, you'll have to configure the renderer to use OpenGL. This will be saved in your `~/.homeworld`, so you won't have to do it every time you run the game.

Once the game has launched, you need to:

- Select "Options" from the main game menu.
- Select "Video" from the available option sub-menus listed on
    the right side of the screen.
- Select "(GL) Default OpenGL" from the "Rendering System" box. 
- Choose a resolution from the "Resolution" box (any resolution
    you choose will do fine).
- Click on the "Accept!" button in the bottom-right corner of the screen.

 The game will now attempt to switch to the OpenGL renderer.  
 If everything goes okay, you should be presented with a confirmation that the settings you requested were changed.  Select "Yes", and you should be ready to go!

[SDL]: https://en.wikipedia.org/wiki/Simple_DirectMedia_Layer
[Homeworld]: https://en.wikipedia.org/wiki/Homeworld
[released in 2003]: http://www.gearboxsoftware.com/2013/11/inside-the-box-homeworld-a-brief-history-of-code/
[Relic]: https://www.relic.com/
[releases]: https://github.com/ThibaultLemaire/HomeworldSDL/releases
[wine]: https://www.winehq.org/
