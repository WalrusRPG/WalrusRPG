# How to compile on Windows

## Pre-requisites
- MinGW
- Zlib for MinGW
- SFML

## Installation of pre-requisites

1. Install MinGW/MSys
2. Install zlib with MSys 
    - Download last version's sources and uncompress them in a folder.
    - Launch a Msys shell and go into said folder
    - Run this command, it'll install zlib in Msys's folders
``` bash
make -f win32/Makefile.gcc BINARY_PATH=/usr/bin INCLUDE_PATH=/usr//include LIBRARY_PATH=/usr/lib install
```
3. Install SFML for MSys
    - Follow [this](http://www.sfml-dev.org/tutorials/2.0/start-cb.php) page's **Installing SFML** section.

## Compile WalrusRPG's project

1. Go into the folder in a Msys shell
2. If you haven't done it first, use this command to configure the project to use Windows target's settings:
```bash
    $ ./mkconfig windows
```
3. Launch a good old `make`:
4. Run the game with `make run` (note that you can skip 3. if you directly want to build and run)

## Bundling and releasing
1. Note that you might want to copy first your dlls in a folder `bundle_files/windows` to allow the compilation process to acknowledge them when making a release. At this time, I need to copy theses DLL from Msys:
    - libgcc_s_dw2-1.dll
    - sfml-graphics-2.dll
    - sfml-system-2.dll
    - sfml-window-2.dll
2. `make release` will copy the executable and the data files (and also the files stored in bundle_files/common and bundle_files/windows) in the release/windows folder.
3. `make bundle` will create a ready-to-share archive.