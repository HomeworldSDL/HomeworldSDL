{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
    buildInputs = [
        libtool
        autoconf
        automake
        gnumake
        gcc
        yacc
        flex
        SDL2
    ];
}

