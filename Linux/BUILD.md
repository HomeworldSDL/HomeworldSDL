# Compiling Homeworld SDL on Linux

## Installing dependencies

### Using [Nix]

If you have [Nix] installed, there is a [`flake.nix`](flake.nix) file listing the build depencies so you can just run the build in a `nix develop` environment without installing anything:

``` sh
nix develop ./Linux
```

You can then go on with the [Quick Start](#quick-start) in that virtual environment.

[Nix]: https://nixos.org/nix/

## Quick Start

> More information can be found in the documentation files next to this one.

### Meson

``` sh
meson setup build
cd build
meson compile
```

> You are free to replace `build` above with anything you like. It will be the name of the build directory

You can now [run the compiled executable for the first time](../README#running-the-game-for-the-first-time).

### Autotools (Deprecated)

#### x86_64 (intel/amd 64-bit)

``` sh
cd Linux
./bootstrap
../configure
make -j4
```

> The `-j4` flag passed to `make` is just an example.
  `-j` controls the number of "jobs" used by `make` to compile the sources.
  If your machine is equipped with, e.g., 12 cores, then replace `-j4` by `-j12` for a faster build.

> The configuration step has a lot of flags, run `../configure --help` to see them. (Notably the `--disable-linux-fixme` flag)

#### x86 (intel/amd 32-bit)

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

## Hacking

So you want to dive into the code and start hacking, huh?
Here are a few pointers to help you with that:

### Clangd

[Clangd] is a _language server_ that can work with many editors (including [VSCode]) via a plugin.
It adds smart features to your editor: code completion, compile errors, go-to-definition and more.

To give proper hints, though, clangd needs to know the compile flags used (otherwise you'll get "header not found" errors).
To that end, it uses a `compile_commands.json` file describing how each file was compiled.

[Clangd]: https://clangd.llvm.org
[VSCode]: https://vscodium.com/

### With Meson

Meson automatically generates `compile_commands.json`, so if you named your build dir `build` as clangd expects, then you have nothing to do.
Enjoy your modern development environment!

#### With Autotools (Deprecated)

You can use [bear] to auto-generate `compile_commands.json`.
In the build steps outlined above, replace the `make` step with:

```sh
bear -- make -j4
```

> ⚠️ As of 2023-09-22, this breaks the build if you also used the [`--enable-sanitizers`](#sanitizers) option in the `configure step`.
  You will have to run a first build __without__ this option, then re-enable it, and re-build once the `compile_commands.json` has been generated.

Then link or copy the result to the root of the repo, so that clangd finds it automatically

```sh
ln -srv compile_commands.json ..
```

[bear]: https://github.com/rizsotto/Bear

### Sanitizers

[LLVM's Sanitizers] are a powerful suite of tools for memory debugging.
They can detect and help debug many kinds of invalid or dangerous memory handling patterns (like buffer overflows, use after free, or leaks).

#### With Meson

The `address` and `undefined` sanitizers are enabled by default.
You can disable them by passing the `-Db_sanitize=none` option to `meson setup`.

#### With Autotools (Deprecated)

You can build a debug version of the game that includes those sanitizers with

```sh
../configure --enable-sanitizers
```

[LLVM's Sanitizers]: https://clang.llvm.org/docs/AddressSanitizer.html
