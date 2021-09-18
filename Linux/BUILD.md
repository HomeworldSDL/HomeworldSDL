# Compiling Homeworld SDL on Linux

## Installing dependencies

### Using [Nix]

If you have [Nix] installed, there is a [`shell.nix`](shell.nix) file listing the build depencies so you can just run the build in a `nix-shell` without installing anything:

``` sh
cd Linux
nix-shell
```

You can then go on with the [Quick Start](#quick-start) in that virtual environment.

[Nix]: https://nixos.org/nix/

## Quick Start

> More information can be found in the documentation files next to this one.

### x86_64 (intel/amd 64-bit)

``` sh
cd Linux
./bootstrap
../configure
make
```

> The configuration step has a lot of flags, run `../configure --help` to see them. (Notably the `--disable-linux-fixme` flag)

Now put the compiled executable `src/homeworld` in a directory with the [required assets](../README#asset-checklist).

For `HomeworldSDL.big` see [Packaging HomeworldSDL.big](#packaging-homeworldsdl.big)

You are now ready to run the game and [configure the OpenGL renderer](../README#configuring-the-opengl-renderer)

### x86 (intel/amd 32-bit)

This is if you want to cross-compile the game to 32bit even if your machine is 64bit.

The process is the same as for x86_64, except for the `../configure` step, as follows:

``` sh
cd Linux
./bootstrap
CFLAGS='-m32' ../configure --disable-x86_64  # here
make
```

> Note: this will output a binary without debug symbols. Building a 32b binary with debug symbols on a 64b machine is feasible (I have done it for debugging), but not supported by autoconf and therefore not very straightforward.

### Packaging HomeworldSDL.big

> Note: compiling biggie requires compiled objects from `Linux/src/`. Which means you must first compile the game.

From the root of this repository:

``` sh
cd tools/biggie
./biggie-Linux-compile.sh
cd ../../HomeworldSDL_big
./convert_directory_to_big_file
```

You should now have a `HomeworldSDL.big` file in the root of the repo.
