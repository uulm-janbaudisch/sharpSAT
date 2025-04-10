{
  lib,
  stdenv,
  fetchgit,
  cmake,
  gmp,
}:
stdenv.mkDerivation {
  pname = "sharpSAT";
  version = "13.03";

  src = ./.;

  nativeBuildInputs = [ cmake ];

  buildInputs = [ gmp.dev ];

  meta = with lib; {
    mainProgram = "sharpSAT";
    description = "#SAT solver based on modern DPLL based SAT solver technology";
    homepage = "https://github.com/SoftVarE-Group/sharpSAT";
    license = licenses.mit;
    platforms = platforms.unix;
  };
}
