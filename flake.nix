{
  description = "Prologin 2021 Final Game";

  inputs = {
    stechec2.url = "github:prologin/stechec2/nix-stechec";
    futils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
  };

  outputs = { self, stechec2, nixpkgs, futils }: 
    futils.lib.eachDefaultSystem (system:
      let
        lib = stechec2.lib."${system}";
        pkgs = import nixpkgs { inherit system;  };
      in
        rec {
          packages = futils.lib.flattenTree {
            prologin2022 = lib.mkStechec2Game {
              name = "prologin2022";
              game = ./.;
              version = "1.0";
              stechec2 = stechec2.defaultPackage."${system}";
            };
          };

          defaultPackage = packages.prologin2022;
          devShell = pkgs.mkShell {
            buildInputs = [ stechec2.defaultPackage."${system}" ];
          };
        }
    );
}
