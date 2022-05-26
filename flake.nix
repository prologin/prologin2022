{
  description = "Prologin 2022 Final Game";

  inputs = {
    stechec2.url = "github:prologin/stechec2/nix-stechec";
    futils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
  };

  outputs = { self, stechec2, nixpkgs, futils }:
    let
      inherit (nixpkgs) lib;
      inherit (lib) recursiveUpdate;
      inherit (futils.lib) eachDefaultSystem;

      anySystemOutputs = {
        overlay = final: prev: {
          prologin2022 = final.mkStechec2Game {
            name = "prologin2022";
            game = ./.;
            version = "1.0";
          };

          prologin2022-docs = final.stdenv.mkDerivation {
            name = "prologin2022-docs";

            src = ./doc;
            buildInputs = [ final.python3Packages.sphinx ];
            buildFlags = [ "html" ];

            installPhase = ''
              cp -r _build/html $out
            '';
          };
        };
      };

      multipleSystemsOutpus = eachDefaultSystem (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [
              stechec2.overlay
              self.overlay
            ];
          };
        in
        rec {
          packages = {
            inherit (pkgs) prologin2022 prologin2022-docs;
          };

          defaultPackage = self.packages.${system}.prologin2022;

          devShell = pkgs.mkShell {
            buildInputs = [ stechec2.defaultPackage."${system}" pkgs.python3Packages.sphinx ];
          };
        }
      );
     in
     recursiveUpdate anySystemOutputs multipleSystemsOutpus;
}
