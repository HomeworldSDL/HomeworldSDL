# Compiling Homeworld SDL on Linux

## x86_64 (intel/amd 64-bit)

> This is only a quickstart, more information can be found in the documentation files next to this one.

``` sh
cd Linux
./bootstrap
../configure --enable-x86_64
make
```

> The configuration step has a lot of flags, run `../configure --help` to see them. (Notably the `--disable-linux-fixme` flag)

Now put the compiled executable `src/homeworld` in a directory with the [required assets](../README#asset-checklist).

For `HomeworldSDL.big` see [Packaging HomeworldSDL.big](#packaging-homeworldsdl.big)

You are now ready to run the game and [configure the OpenGL renderer](../README#configuring-the-opengl-renderer)

## x86 (intel/amd 32-bit)

You may want to cross-compile the game to 32bit even if your machine is 64bit. (mainly because the x86_64 port is unstable)

The process is the same as for x86_64, except for the `../configure` step, as follows:

``` sh
cd Linux
./bootstrap
CFLAGS='-m32' ../configure  # here
make
```

## Packaging HomeworldSDL.big

> Note: compiling biggie requires compiled objects from `Linux/src/`. Which means you must first compile the game.

From the root of this repository:

``` sh
cd tools/biggie
./biggie-Linux-compile.sh
cd ../../HomeworldSDL_big
./convert_directory_to_big_file
```

You should now have a `HomeworldSDL.big` file in the root of the repo.
