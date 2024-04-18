{
  description = "Unit Testing Framework for C with implicit Suite discovery";

  inputs.semver.url = "gitlab:cynerd/nixsemver";

  outputs = {
    self,
    flake-utils,
    nixpkgs,
    semver,
  }: let
    inherit (nixpkgs.lib) hasSuffix mapAttrs;
    inherit (flake-utils.lib) eachDefaultSystem filterPackages;
    inherit (semver.lib) changelog;

    version = changelog.currentRelease ./CHANGELOG.md self.sourceInfo;
    src = builtins.path {
      path = ./.;
      filter = path: _: ! hasSuffix ".nix" path;
    };

    check-suite = {
      stdenv,
      meson,
      ninja,
      pkg-config,
      check,
    }:
      stdenv.mkDerivation {
        pname = "check-suite";
        inherit version src;
        GIT_REV = self.shortRev or self.dirtyShortRev;
        buildInputs = [check];
        nativeBuildInputs = [meson ninja pkg-config];
      };
  in
    {
      overlays.default = final: prev: {
        check-suite = prev.callPackage check-suite {};
      };
    }
    // eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system}.extend self.overlays.default;
    in {
      packages = {
        default = pkgs.check-suite;
        musl = pkgs.pkgsMusl.check-suite;
      };
      legacyPackages = pkgs;

      devShells = filterPackages system (let
        devpkgs = with pkgs; [
          # Linters and formatters
          clang-tools_14
          cppcheck
          editorconfig-checker
          flawfinder
          muon
          shellcheck
          shfmt
          statix
          deadnix
          gitlint
          # Testing and code coverage
          valgrind
          gcovr
        ];
      in
        mapAttrs (n: v:
          pkgs.mkShell {
            packages = devpkgs;
            inputsFrom = [v];
          })
        self.packages.${system});

      checks.default = self.packages.${system}.default;

      formatter = pkgs.alejandra;
    });
}
