{pkgs ? import <nixpkgs> {} }:


pkgs.mkShell  {

buildInputs = with pkgs; [
xorg.libX11.dev
xorg.libXft
xorg.libXinerama
yaml-cpp

cmake
];
}
