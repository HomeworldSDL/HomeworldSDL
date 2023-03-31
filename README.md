# Tenhauser Gate

📣 **Play in your browser!** Try our experimental wasm port here: [Gardens of Kadesh]

Source port of Homeworld 1 for Linux.

🛈 Not playing on Linux? Check out our central [Homeworld SDL] hub.

[Homeworld] is a 3D space real-time strategy game released in 1999.
It's source code was [released in 2003] by [Relic Entertainment], allowing a small but dedicated community of fans to port it to [SDL], enabling native versions for Linux, MacOS, and more recently, web browsers.

[Homeworld SDL]: https://github.com/HomeworldSDL/HomeworldSDL
[SDL]: https://en.wikipedia.org/wiki/Simple_DirectMedia_Layer
[Homeworld]: https://en.wikipedia.org/wiki/Homeworld
[released in 2003]: http://www.insidemacgames.com/news/story.php?ArticleID=8516
[Relic Entertainment]: https://www.relic.com/
[Gardens of Kadesh]: https://gardensofkadesh.github.io/


## Requirements

This repository contains only the source code for Homeworld's engine (metaphorically, the skeleton). In order to have a playable game, you will need the original assets (artwork, textures, sounds, meshes, ... Metaphorically again, the flesh).

To be clear: **You need to have bought the game to use this project.**

So make sure you either have:

- The original Homeworld game from 1999 (It is not sold anywhere anymore, so either you have a CD dusting on your shelf, or you'll have to get one second-hand).
  In that case, you should also download [the official 1.05 patch].

- [The Homeworld Remastered Collection](https://www.homeworldremastered.com/)

### Asset checklist

This section is intended as a quick checklist to make sure you have everything needed for the game to run. If you don't know where to find one of these files, see the appropriate section below. (Most of them can be found in the game's installation folder.)

- [ ] `HomeworldSDL.big` You will have to build this one from this repo. (Except if you plan to play with a 32b version, which is not the default, and deprecated.)

#### Original (1999)

- [ ] `Homeworld.big`  
    > sha256: af9dcc06e3f99404334a0a8ec17e399876080e85feb5b47858382dc715136040
- [ ] `Update.big` Provided by the official 1.05 Patch  
    > sha256: c71b07758ee7696b3a19c1d75c946cbd68830e03b30cd3c2888f2f6d22b7c976
- [ ] `HW_Comp.vce`  
    > sha256: 15c4b988adb09b0969b0dc288b21ddc10ca9d42a2064d15b46b53dcf02fc2e44

- [ ] `HW_Music.wxd`  
    > sha256: b909c2cdbc8c8db67be168f6005bf8e8facaa9857263b16d186c824a0c4eed4f

#### Remaster (2015)

- [ ] `homeworld.big` (You might have to rename it to `Homeworld.big`, with a capital `H`)
    > sha256: e38c0528c1d4bd9d9195d26d5231ae29bef18f57d9bd1fe2eed33fb2b9b172a8
- [ ] `HW_Comp.vce`  
    > sha256: 15c4b988adb09b0969b0dc288b21ddc10ca9d42a2064d15b46b53dcf02fc2e44

- [ ] `HW_Music.wxd`  
    > sha256: 48f93c07bf718c56c20727aba12f06baf13d4d11d1c7185d2d2153543834e454

## Compiling from source

Please refer to [`Linux/BUILD.md`](Linux/BUILD.md).

## Setting up the assets folder

You should choose an installation folder where you will put (copy|move|symlink) all the [required files](#asset-checklist). This will be refered to as `$ASSETS_FOLDER` from here on out. (For example `~/Games/HomeworldSDL/`)

### Extracting the assets

#### Original (1999)

Install the game and [patch][the official 1.05 patch] with [wine] (tested with wine 4.1)

``` sh
wine HWSetup.EXE
wine si_homeworld_update_105.exe
```

Open the game installation folder (usually `~/.wine/drive_c/Sierra/Homeworld`). This is where you'll find the [required files](#asset-checklist). Put them to `$ASSETS_FOLDER`.

[wine]: https://www.winehq.org/

#### Remaster (2015)

##### Steam

After installing the game, you should find the [required assets](#asset-checklist) in `~/.local/share/Steam/steamapps/common/Homeworld/Homeworld1Classic/Data`. Put them to `$ASSETS_FOLDER`.

## Running the game for the first time

The first time you run the game, you will have to point it to [the directory containing the required assets](#setting-up-the-assets-folder). For example, if you have [compiled the game from source](#compiling-from-source):

```sh
HW_Data=$ASSETS_FOLDER src/homeworld
```

where `src/homeworld` is the path to your HomeworldSDL binary executable.

Once the game starts, you should then [configure the OpenGL renderer](#configuring-the-opengl-renderer).

The game stores its configuration in `~/.homeworld`, and will remember the asset directory and graphics configuration for later runs, which means you don't need to provide the `HW_Data` dir the next time you run the game, and you can safely move the executable around.

### Configuring the OpenGL renderer

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

## [About][wiki]

If you want to know more about this project (and see some screenshots!), please head over to the [wiki].

[wiki]: https://gitlab.com/ThibaultLemaire/HomeworldSDL/wikis/home

[releases]: https://gitlab.com/ThibaultLemaire/HomeworldSDL/-/releases
[the official 1.05 patch]: https://www.homeworldaccess.net/infusions/downloads/downloads.php?file_id=35
