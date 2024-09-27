with import <nixpkgs> {};


let 
  __script = pkgs.writeShellScriptBin "makeWind2" ''
  #!${runtimeShell}
  cd build;
  ${pkgs.cmake}/bin/cmake ..;
  make ..;
  '';
in
  stdenv.mkDerivation {

    name = "Wind2";
    src = fetchgit {
      url = "https://github.com/XeniaHerr/Wind2";
      sha256 = "sha256-ip70pqE7PYkZCxZH2h7FVuv3UMeGe9145WCeSRFoZlU=";
      
    };

    CFLAGS = [ "-I{$./../include/}" ];

    installPhase = ''
    mkdir -p $out/bin
    cp app/Wind2 $out/bin
    '';


    buildInputs = [__script cmake python311 yaml-cpp
    xorg.libX11.dev
    xorg.libXft
    xorg.libXinerama
  ];

  buildPhase = ''
#  cd .build/src/build
  ls ..
  ls ../..
  cmake ..
  make
  '';

}
