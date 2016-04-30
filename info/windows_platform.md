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


