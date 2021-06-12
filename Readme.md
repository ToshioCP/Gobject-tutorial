# GObject Tutorial for beginners

This tutorial is aimed at beginners who is learning Gobject system.
One of the biggest difficulty to learn GObject system is to understand its object oriented strategy.
All the necessary topics is described in [GObject API reference](https://developer.gnome.org/gobject/stable/).
But it is probably difficult especially for beginners.

The contents of this tutorial is not beyond the API reference.
It just gives the readers some example codes and diagrams to help them.
Readers should refer to the API reference when learning this tutorial.

The table of contents is below and you can see all the tutorials by following the link.
However, you can make html or pdf by the following steps.

1. You need linux operationg system, ruby, rake, pandoc and latex system.
2. download this repository and uncompress the files.
3. change your current directory to the top directory of the source files.
4. type `rake html` to produce html files. The files are generated under `html` directory.
5. type `rake pdf` to produce a pdf file. The file is generated under `latex` directory.

This system is the same as the one in the `gtk4 tutorial` repository.
There's a document `Readme_for_developers.md` in `gfm` directory of the repository.
The document describes the details.

If you have any questions, feel free to post an issue.


## Table of contents

1. [Prerequisite and License](gfm/sec1.md)
1. [GObject](gfm/sec2.md)
1. [Type system and registration process](gfm/sec3.md)
1. [Signals](gfm/sec4.md)
1. [Properties](gfm/sec5.md)
1. [Derivable type and abstract type](gfm/sec6.md)
1. [Derivable and non-abstract type](gfm/sec7.md)
1. [Overriding a property and chaining up the finalize process](gfm/sec8.md)
1. [Interface](gfm/sec9.md)
