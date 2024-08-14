{
  description = "#SAT solver based on modern DPLL based SAT solver technology";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";

  outputs =
    { self, nixpkgs, ... }:
    let
      lib = nixpkgs.lib;

      systems = [
        "aarch64-darwin"
        "aarch64-linux"
        "x86_64-darwin"
        "x86_64-linux"
      ];

      sharpSAT = pkgs: pkgs.callPackage ./sharpSAT.nix { };
    in
    {
      formatter = lib.genAttrs systems (system: nixpkgs.legacyPackages.${system}.nixfmt-rfc-style);
      packages = lib.genAttrs systems (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          default = self.packages.${system}.sharpSAT;

          sharpSAT = sharpSAT pkgs;
          sharpSAT-static = sharpSAT pkgs.pkgsStatic;

          container = pkgs.dockerTools.buildLayeredImage {
            name = "sharpSAT";
            contents = [ self.packages.${system}.sharpSAT ];
            config = {
              Entrypoint = [ "/bin/sharpSAT" ];
              Labels = {
                "org.opencontainers.image.source" = "https://github.com/SoftVarE-Group/sharpSAT";
                "org.opencontainers.image.description" = "#SAT solver based on modern DPLL based SAT solver technology";
                "org.opencontainers.image.licenses" = "MIT";
              };
            };
          };
        }
      );
    };
}
