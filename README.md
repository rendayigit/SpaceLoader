# [SpaceLoader](https://github.com/rendayigit/SpaceLoader)

![sl](https://user-images.githubusercontent.com/26045359/204103152-a5e0e2e3-ff24-4157-aa15-8c2d4f51f523.PNG)

A project where a Client can call batch scripts located in the Server PC. These commands including the batch script locations are located in an xml file therefore configuration doesn't require recompilation. Written using the Qt framework.

## Environment Setup

### Installing Qt

- [Download the online installer](https://www.qt.io/download-qt-installer "Qt Online Installer")
- Follow the installation instructions
- Done

> Make sure to install `QT > A compiler version xyz` along with `Developer and Design Tools > compiler with the same version xyz`

> Note: You may need to create a free Qt Account.

> Note: You may use the `maintenance tool` in the Qt directory to later install new or uninstall previously installed features on Qt. You may also update Qt with this tool.

***

## Qt Build From Terminal (Only with .pro files and qmake)

| #   | bash script             | function                                      |
| --- | ----------------------- | --------------------------------------------- |
| 1   | ```qmake Foo.pro``` | creates a makefile |
| 2   | ```make``` | compile using the previously created makefile |
| 3   | ```./Foo``` | run executable |

***

## Pacman Basic Commands

| #   | bash script             | function                                      |
| --- | ----------------------- | --------------------------------------------- |
| 1 | ```pacman -Scc``` | clear cache |
| 2 | ```pacman -Ss <package>``` | search package |
| 3 | ```pacman -S <package>``` | install package |
| 4 | ```pacman -R <package>``` | uninstall package |
| 5 | ```pacman -Qe``` | list installed packages |
| 6 | ```pacman -Syyu``` | update |
| 7 | ```pacman -Syyu --disable-download-timeout``` | update (use this if ur network is slow) |

## msys packages

- mingw-w64-x86_64-qt5
- mingw-w64-x86_64-qt5-base
- mingw-w64-x86_64-qt5-declarative
- mingw-w64-x86_64-clang
- mingw-w64-x86_64-cmake
- mingw-w64-x86_64-make
- mingw-w64-x86_64-gdb
- mingw-w64-x86_64-clang-tools-extra
- mingw-w64-x86_64-clang-analyzer
- mingw-w64-x86_64-gtest
- mingw-w64-x86_64-yaml-cpp
- mingw-w64-x86_64-libssh


![Github Repo Stars](https://img.shields.io/github/actions/workflow/status/rendayigit/SpaceLoader/ci.yml)
[![Pull Request Welcome](https://img.shields.io/github/issues-pr/rendayigit/SpaceLoader)](https://github.com/rendayigit/SpaceLoader/pulls)
[![Github issues](https://img.shields.io/github/issues-raw/rendayigit/SpaceLoader)](https://github.com/rendayigit/SpaceLoader/issues)
[![Github Repo Size](https://img.shields.io/github/issues-pr/rendayigit/SpaceLoader)](https://github.com/rendayigit/SpaceLoader)

