{
  description = "Unit Testing Framework for C with implicit Suite discovery";

  outputs = {
    self,
    flake-utils,
    nixpkgs,
  }:
    with builtins;
    with flake-utils.lib;
    with nixpkgs.lib; let
      packages = pkgs:
        with pkgs; {
          check-suite = stdenv.mkDerivation {
            pname = "check-suite";
            version = replaceStrings ["\n"] [""] (readFile ./version);
            src = builtins.path {
              path = ./.;
              filter = path: type: ! hasSuffix ".nix" path;
            };
            buildInputs = [check];
            nativeBuildInputs = [meson ninja pkg-config];
          };
        };
    in
      {
        overlays = {
          check-suite = final: prev: packages (id prev);
          default = self.overlays.check-suite;
        };
      }
      // eachDefaultSystem (system: let
        pkgs = nixpkgs.legacyPackages.${system}.extend self.overlays.default;
      in {
        packages = filterPackages system rec {
          inherit (pkgs) check-suite;
          default = check-suite;
        };
        legacyPackages = pkgs;

        devShells = filterPackages system {
          default = nixpkgs.legacyPackages.${system}.mkShell {
            packages = with nixpkgs.legacyPackages.${system}; [
              # Linters and formatters
              cppcheck
              flawfinder
              clang-tools_14
              shellcheck
              shfmt
              # Testing and coverage
              valgrind
              gcovr
            ];
            inputsFrom = [self.packages.${system}.check-suite];
            meta.platforms = platforms.linux;
          };
        };

        checks = {
          inherit (self.packages.${system}) default;
          statix =
            pkgs.runCommandNoCC "check-statix" {}
            "${pkgs.statix}/bin/statix check ${./.} && touch $out";
        };
        formatter = nixpkgs.legacyPackages.${system}.alejandra;
      });
}
