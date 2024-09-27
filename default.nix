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
    src = fetchGit {
      url = "https://github.com/XeniaHerr/Wind2";
#      sha256 = "sha256-bvFHWjTAO7u8uzj8UCqjskLUwavQQw967Bbagx+L1E8=";
#      rev = "14d975d915c6da1caadf081ec5bb0a34c7fb1ab2";

    ref = "X11";
      
    };

    CFLAGS = [ "-I{$./../include/}" ];

    dontUseCmakeConfigure = true;

    installPhase = ''
    mkdir -p $out/bin
    cp app/Wind2 $out/bin
    '';


    buildInputs = [__script cmake python311 yaml-cpp
    xorg.libX11.dev
    xorg.libXft
    xorg.libXinerama
    gtest
    pkg-config
  ];

  buildPhase = ''
#  cd .build/src/build
  ls
  mkdir -p build
  cd ./build
  cmake ..
  make
  '';

}
