{
  description = "Prologin 2021 Final Game";

  inputs = {
    stechec2.url = "github:prologin/stechec2/nix-stechec";
    futils.url = "github:numtide/flake-utils";
  };

  outputs = { self, stechec2, futils }: 
    futils.lib.eachDefaultSystem (system:
      let
        lib = stechec2.lib."${system}";
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
        }
    );
}
