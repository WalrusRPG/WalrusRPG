# WalrusRPG

A cross-platform J-RPG engine. Still very much a work in progress, but it
might get somewhere someday.

## Features

- Cross-platform
- Walruses
- Highly experimental stuff inside (yes it's radioactive)

## Does not feature

- Walrii
- The game
- Actual content

## Getting the code wih Git

To get the dependencies together with the source, you need to do a recursive clone
of the repo:

```bash
git clone --recursive https://github.com/WalrusRPG/WalrusRPG.git
```

If you forgot to include `--recursive` while cloning, it's not too late:

```bash
cd WalrusRPG
git submodule update --init --recursive
```

You'll have to compile ConvertIMG yourself for now, it might be added to the
build system later.

## Compiling

The build system currently depends on Vogtinator's [ConvertImg](https://github.com/Vogtinator/ConvertImg),
as well as GNU Make and the Ndless toolchain when compiling for the Nspire,
or SFML for systems that it supports.

We use the [tinystl](https://github.com/mendsley/tinystl) library to keep binaries small, so be
sure to clone the submodules as well !

To compile the project, simply run make (-j friendly).

## Plans

### What needs to be done

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
- Support for multiple platforms, including the TI-Nspire CX and SFML
