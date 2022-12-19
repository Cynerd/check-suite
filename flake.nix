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
      pkg_check_suite = pkgs:
        with pkgs;
          stdenv.mkDerivation {
            pname = "check-suite";
            version =
              head (match ".*project\\([^\\)]*version: *'([^']+)'.*"
                (readFile ./meson.build));

            src = ./.;

            buildInputs = [check];
            nativeBuildInputs = [meson ninja pkg-config];
          };
    in
      {
        overlays = {
          check-suite = final: prev: {check-suite = pkg_check_suite prev;};
          default = self.overlays.check-suite;
        };
      }
      // eachDefaultSystem (system: {
        packages = filterPackages system rec {
          check-suite = pkg_check_suite nixpkgs.legacyPackages.${system};
          default = check-suite;
        };

        legacyPackages = import nixpkgs {
          # The legacyPackages imported as overlay allows us to use pkgsCross
          inherit system;
          overlays = [self.overlays.default];
          crossOverlays = [self.overlays.default];
        };

        devShells = filterPackages system {
          default = nixpkgs.legacyPackages.${system}.mkShell {
            packages = with nixpkgs.legacyPackages.${system}; [
              cppcheck
              flawfinder
              clang-tools_14
              shellcheck
              shfmt
              valgrind
              lcov
            ];
            inputsFrom = [self.packages.${system}.check-suite];
            meta.platforms = platforms.linux;
          };
        };

        formatter = nixpkgs.legacyPackages.${system}.alejandra;
      });
}
