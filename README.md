# MedievalSurvival

[![CMake on multiple platforms](https://github.com/JohnHeikens/MedievalSurvival/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/JohnHeikens/MedievalSurvival/actions/workflows/cmake-multi-platform.yml)

we use this template:
https://github.com/lutrarutra/SFML-Project-Template/tree/master

to build for android:
we use android studio, also to download the NDK.
https://github.com/acsbendi/Build-SFML-For-Android-On-Windows

CAUTION!
vcpkg doesn't work with SFML. therefore, you should have MinGW and MSYS installed on your PC when on windows.

to install CMake, Ninja, SFML and zlib:
make sure to have MSYS2 MINGW64 installed!
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-zlib mingw-w64-x86_64-sfml
