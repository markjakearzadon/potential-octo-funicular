{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    clang
    cmake
    pkg-config

    # GUI/graphics dependencies
    glfw
    wayland
    # glew
    # libGL
    # libxkbcommon
    # xorg.libX11
    # xorg.libXrandr
    # xorg.libXinerama
    # xorg.libXcursor
    # xorg.libXi

    # Optional depending on your setup
    # SDL2
    # freetype
    # fontconfig
  ];

  shellHook = ''
    echo "test"
  '';
}
