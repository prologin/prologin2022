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

          # TODO: use proper buildPythonPackage and imports
          map-editor-2022 = final.stdenv.mkDerivation {
            name = "map-editor-2022";
            src = ./editor;

            buildPhase = " ";
            installPhase = ''
              mkdir -p $out/bin/bg
              cp -rf $src/conf.yml $out/bin/
              cp -rf $src/bg $out/bin

              install -Dm755 $src/map_editor.py $out/bin/map_editor
              # uses nativeBuildInputs to add to PYTHONPATH wrapperscript
              wrapPythonPrograms
              cp -rf $src/validator.py $out/bin
            '';

            pythonPath = with final.python3Packages; [
              tkinter
              pyyaml
              pillow
            ];
            nativeBuildInputs = [ final.python3Packages.wrapPython ];

          };

          viewer-2022 = final.writeShellScriptBin "viewer" ''
            VIEWER_DIR=$HOME/.viewer

            rm -r "$VIEWER_DIR"
            mkdir -p "$VIEWER_DIR"

            if [ "$#" -ne 1 ]; then
              echo "Usage: $0 path/to/dump.json" >&2
              exit 1
            fi

            ln -s ${./www/static/css} "$VIEWER_DIR/css"
            ln -s ${./viewer/fonts} "$VIEWER_DIR/fonts"
            ln -s ${./viewer/js} "$VIEWER_DIR/js"
            ln -s ${./viewer/index.html} "$VIEWER_DIR/index.html"
            mkdir -p "$VIEWER_DIR/static"
            ln -s ${./www/static/img} "$VIEWER_DIR/static/img"
            ln -s ${./www/static/js} "$VIEWER_DIR/static/js"

            cp "$1" "$VIEWER_DIR/dump"
            ${final.python3}/bin/python -m http.server 8742 --bind 0.0.0.0 --directory "$VIEWER_DIR"
          '';
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
            inherit (pkgs) prologin2022 prologin2022-docs map-editor-2022 viewer-2022;
          };

          defaultPackage = self.packages.${system}.prologin2022;

          devShell = pkgs.mkShell {
            buildInputs = [
              stechec2.defaultPackage."${system}"
              pkgs.python3Packages.sphinx

              # Editor packages
              pkgs.python3Packages.tkinter
              pkgs.python3Packages.pyyaml
              pkgs.python3Packages.pillow
            ];
          };
        }
      );
     in
     recursiveUpdate anySystemOutputs multipleSystemsOutpus;
}
