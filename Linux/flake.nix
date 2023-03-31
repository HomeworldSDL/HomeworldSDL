{
  description = "Source port of Homeworld 1";

  inputs = {
    guidestone = {
      url = git+https://gitlab.com/homeworldsdl/the-guidestone.git;
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };


  outputs = { self, nixpkgs, guidestone }: {

    devShells.x86_64-linux.default = with nixpkgs.legacyPackages.x86_64-linux;
      mkShell {
        buildInputs = self.packages.x86_64-linux.default.buildInputs ++ [
          clang-tools_13 # clangd, clang-format
          nixpkgs-fmt
        ];
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

    packages.x86_64-linux.default = guidestone.packages.x86_64-linux.default.overrideAttrs (attrs: {
      version = self.version;
      src = self.source-code;
    });

    apps.x86_64-linux.default = {
      type = "app";
      program = "${self.packages.x86_64-linux.default}/bin/homeworld";
    };

  };
}
