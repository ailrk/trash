{ mkDerivation, base, hspec, stdenv }:
mkDerivation {
  pname = "type-thoery-for-pl";
  version = "0.1.0.0";
  src = ./.;
  isLibrary = true;
  isExecutable = true;
  libraryHaskellDepends = [ base ];
  executableHaskellDepends = [ base ];
  testHaskellDepends = [ base hspec ];
  homepage = "https://github.com/githubuser/type-thoery-for-pl#readme";
  license = stdenv.lib.licenses.bsd3;
}