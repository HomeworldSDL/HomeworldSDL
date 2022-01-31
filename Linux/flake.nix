{
  description = "Source port of Homeworld 1";

  inputs = {
    guidestone = {
      url = git+https://gitlab.com/homeworldsdl/the-guidestone.git;
      inputs.nixpkgs.follows = "nixpkgs";
    };
    nixGL = {
      url = github:guibou/nixGL;
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };


  outputs = { self, nixpkgs, guidestone, nixGL }: {

    devShells.x86_64-linux.default = with nixpkgs.legacyPackages.x86_64-linux;
      mkShell {
        buildInputs = self.packages.x86_64-linux.default.buildInputs ++ [
          clang-tools_16 # clangd, clang-format
          meson
          pkg-config
          python3
          ninja
          gdb
          nixpkgs-fmt
        ];
        # With --enable-sanitizers dlopen() fails to find libs. This is used by SDL2 to open the sound library (pipewire), and by libglvnd to open the vendor-specific OpenGL driver
        LD_LIBRARY_PATH = "${pipewire.lib}/lib;/run/opengl-driver/lib";
      };

    source-code = with nixpkgs.legacyPackages.x86_64-linux; runCommand "Source Code"
      { }
      ''
        mkdir -p $out/Linux/stuff
        cp ${./stuff}/{acinclude.m4,configure.ac,Makefile.am} $out/Linux/stuff
        cp -r ${./../src} $out/src
        cp -r ${./../tools} $out/tools
      '';
    version = "${builtins.substring 0 8 (self.lastModifiedDate or self.lastModified or "19700101")}.${self.shortRev or "dirty"}";

    packages.x86_64-linux.default = (guidestone.galactic_map {
      version = self.version;
      src = self.source-code;
      configureOptions = "--enable-hwdebug --enable-sanitizers";
    }).overrideAttrs (attrs: {
      dontStrip = true;
      nativeBuildInputs = [ nixpkgs.legacyPackages.x86_64-linux.makeWrapper ];
      installPhase = attrs.installPhase + ''
        wrapProgram $out/bin/homeworld --set LD_LIBRARY_PATH "${self.devShells.x86_64-linux.default.LD_LIBRARY_PATH}"
      '';
    });

    packages.x86_64-linux.i-am-not-on-nixos = nixpkgs.legacyPackages.x86_64-linux.writeShellScriptBin "non-nixos" ''
      ${nixGL.packages.x86_64-linux.default}/bin/nixGL ${self.packages.x86_64-linux.default}/bin/homeworld
    '';

    apps.x86_64-linux.default = {
      type = "app";
      program = "${self.packages.x86_64-linux.default}/bin/homeworld";
    };

  };
}
