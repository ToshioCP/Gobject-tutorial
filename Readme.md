The GitHub page of this tutorial is available.
Click [here](https://toshiocp.github.io/Gobject-tutorial/).

#### About this tutorial

GObject is the base system for the GTK library, the current version of which is four.
GTK provides GUI on Linux and is used by GNOME desktop system and many applications.
See [GTK 4 tutorial](https://github.com/ToshioCP/Gtk4-tutorial).
One of the problem to understand GTK 4 is the difficulty of the GObject.
This tutorial is useful for those who learns GTK 4.
And the readers of this tutorial should read GTK 4 tutorial because GTK is the only application of GObject so far.

[GObject API Reference](https://docs.gtk.org/gobject/) offers everything necessary for GObject.
The contents of this tutorial are not beyond the documentation.
It just shows examples and how to write GObject programs.
But I believe it is useful for the beginners who feels difficulty to learn the GObject system.
Readers should refer to the GObject documentation when learning this tutorial.

#### Generating GFM, HTML and PDF

The table of contents are at the end of this file and you can see all the tutorials through the link.
However, you can make GFM, HTML or PDF by the following steps.
GFM is 'GitHub Flavored Markdown', which is used in the document files in the GitHub repository.

1. You need Linux operating system, ruby, rake, pandoc and LaTeX system.
2. download the [GObject-tutorial repository](https://github.com/ToshioCP/Gobject-tutorial) and uncompress the files.
3. change your current directory to the top directory of the files.
4. type `rake` to produce GFM files. The files are generated under `gfm` directory.
5. type `rake html` to produce HTML files. The files are generated under `docs` directory.
6. type `rake pdf` to produce a PDF file. The file is generated under `latex` directory.

This system is the same as the one in the `GTK 4 tutorial` repository.
There's a document `Readme_for_developers.md` in `gfm` directory in it.
It describes the details.

#### Contribution

If you have any questions, feel free to post an issue.
If you find any mistakes in the tutorial, post an issue or pull-request.
When you give a pull-request, correct the source files, which are under the 'src' directory, and run `rake` and `rake html`.
Then GFM and HTML files are automatically updated.

## Table of contents

1. [Prerequisites and License](gfm/sec1.md)
1. [GObject](gfm/sec2.md)
1. [Type system and registration process](gfm/sec3.md)
1. [Signals](gfm/sec4.md)
1. [Properties](gfm/sec5.md)
1. [Derivable type and abstract type](gfm/sec6.md)
1. [Derivable and non-abstract type](gfm/sec7.md)
1. [Child class extends parent's function](gfm/sec8.md)
1. [Interface](gfm/sec9.md)
