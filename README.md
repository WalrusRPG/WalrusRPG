![img](http://a.pomf.se/jtafgz.png)

A J-RPG engine for the TI-Nspire. There's still a lot of work before anything works.

## Compiling

The build system currently depends on Vogtinator's [ConvertImg](https://github.com/Vogtinator/ConvertImg),
as well as GNU Make and the Ndless toolchain.

We use the [tinystl](https://github.com/mendley/tinystl) library to keep binaries small, so be
sure to install the headers into your ~/.ndless/include directory.

To compile the project, simply run make (-j friendly).

