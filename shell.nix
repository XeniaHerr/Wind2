{pkgs ? import <nixpkgs> {} }:


pkgs.mkShell  {

buildInputs = with pkgs; [
xorg.libX11.dev
xorg.libXft
xorg.libXinerama
xorg.libxcb
yaml-cpp

cmake
];
}
