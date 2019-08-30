{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
    buildInputs = [
        pkgs.libtool
        pkgs.autoconf
        pkgs.automake
        pkgs.gnumake
        pkgs.gcc
        pkgs.yacc
        pkgs.flex
        pkgs.SDL2
    ];
}

