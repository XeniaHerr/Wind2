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
#      rev = "3b247ac6bf6fb10291f4847a4711b2b4f280fa92";

    ref = "X11";
      
    };

    CFLAGS = [ "-I{$./../include/}" ];

    dontUseCmakeConfigure = true;

    installPhase = ''
    mkdir -p $out/bin
    cp src/Wind2 $out/bin/Wind2
    echo "Hello World"
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
