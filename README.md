# WalrusRPG

A J-RPG engine for the TI-Nspire. There's still a lot of work before anything works.

## Getting the code wih Git

To have the whole WRPG source code and its source code dependancies (like tinySTL),
and be able to compile and rock on, you have two options : using `--recursive` to
download the whole repo and sub-modules or downloading the moduels after
cloning the repo : 

```bash
# Download the whole repo and its submodules in one step
git clone --recursive <WalrusRPG repo location>
```

Second option

```bash
git clone <WalrusRPG repo location
cd <WalrusRPG folder>
git submodule init
git submodule update
```

YOu'll have to download ConvertIMG separately for the moment as only the binary
helps converting the art data in the compilation process.

## Compiling

The build system currently depends on Vogtinator's [ConvertImg](https://github.com/Vogtinator/ConvertImg),
as well as GNU Make and the Ndless toolchain.

We use the [tinystl](https://github.com/mendsley/tinystl) library to keep binaries small, so be
sure to clone the submodules as well !

To compile the project, simply run make (-j friendly).

## Plans

### What need to be done

- Entities (planning in progress)
- Events
- Battle Engine (planning in progress)
- I/O file data
- Scripting
- UI (research in progress)
- Character system.
- Items

### What's already done
- Map system.
- Basic text routines.
