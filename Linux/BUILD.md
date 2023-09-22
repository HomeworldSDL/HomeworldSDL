# Compiling Homeworld SDL on Linux

## Installing dependencies

### Using [Nix]

If you have [Nix] installed, there is a [`flake.nix`](flake.nix) file listing the build depencies so you can just run the build in a `nix develop` environment without installing anything:

``` sh
cd Linux
nix develop
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


You can now [run the compiled executable for the first time](../README#running-the-game-for-the-first-time).

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

### Debug

#### Sanitizers

[LLVM's Sanitizers] are a powerful suite of tools for memory debugging.
They can detect and help debug many kinds of invalid or dangerous memory handling patterns (like buffer overflows, use after free, or leaks).

You can build a debug version of the game that includes those sanitizers with

```sh
../configure --enable-sanitizers
```

Unfortunately `kas2c` itself -- which is used in the build process -- leaks memory.
When LSan (the Leak Sanitizer) detects leaks, it exits with a non-zero code, leading the build process to think kas2c failed to transpile the KAS files. So you probably want to

```sh
export ASAN_OPTIONS="detect_leaks=0"
```

before building.

> ⚠️ Beware, though, that as of 2023-09-22, enabling sanitizers surfaces a lot of issues, making the game crash where it does not without them.

[LLVM's Sanitizers]: https://clang.llvm.org/docs/AddressSanitizer.html
