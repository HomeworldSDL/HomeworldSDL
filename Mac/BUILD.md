# Compiling Homeworld SDL on macOS

## Preparation

1. Copy SDL2.framework and SDL2_image.framework (download the .dmg files from the official github repo) and copy the .framework directories to the Mac directory.
2. Make a separate directory called 'includes' and create an 'SDL2' directory there with all the SDL2 header files.  This is due to the game code using SDL2/ on Mac and not finding the header files in the frameworks from the commandline.  Since this only affects kas2c and biggie and is only a development setup, it isn't an issue.
3. Execute the macOS kas2c and biggie shell scripts.

## Xcode

#### x86_64 / Apple Silicon (intel/amd 64-bit/arm64)

Open the Xcode project and build Homeworld (everything is pre-setup).  Application bundles will be placed inside the DerviedData directory for Xcode (it will show you where).

After this, copy the Application bundles to your Applications/ directory.

This is a build readme, it will not cover the data.  Ensure you have the data in the correct directories for macOS.
