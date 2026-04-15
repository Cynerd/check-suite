{
  description = "Unit Testing Framework for C with implicit Suite discovery";

  inputs.semver.url = "gitlab:cynerd/nixsemver";

  outputs = {
    self,
    systems,
    nixpkgs,
    semver,
  }: let
    inherit (nixpkgs.lib) genAttrs;
    inherit (semver.lib) changelog;
    forSystems = genAttrs (import systems);
    forSystemsPkgs = func: forSystems (system: func system self.legacyPackages.${system});

    name = "check-suite";
    version = changelog.currentRelease ./CHANGELOG.md self.sourceInfo;
    src = ./.;

    package = {
      stdenv,
      meson,
      ninja,
      pkg-config,
      check,
    }:
      stdenv.mkDerivation {
        pname = name;
        inherit version src;
        GIT_REV = self.shortRev or self.dirtyShortRev;
        buildInputs = [check];
        nativeBuildInputs = [meson ninja pkg-config];
      };
  in {
    overlays.default = final: _: {
      "${name}" = final.callPackage package {};
    };

    packages = forSystemsPkgs (_: pkgs: {
      default = pkgs."${name}";
      musl = pkgs.pkgsMusl.check-suite;
    });

    devShells = forSystemsPkgs (system: pkgs: {
      default = pkgs.mkShell {
        packages = with pkgs; [
          # Linters and formatters
          deadnix
          editorconfig-checker
          flawfinder
          gitlint
          llvmPackages.clang-tools
          muon
          shellcheck
          shfmt
          statix
          # Testing and code coverage
          valgrind
          gcovr
          # Release
          yq
        ];
        inputsFrom = [self.packages.${system}.default];
      };
    });

    checks = forSystems (system: {
      inherit (self.packages.${system}) default;
    });
    formatter = forSystemsPkgs (_: pkgs: pkgs.alejandra);

    legacyPackages =
      forSystems (system:
        nixpkgs.legacyPackages.${system}.extend self.overlays.default);
  };
}
