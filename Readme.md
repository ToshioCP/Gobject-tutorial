# GObject Tutorial for beginners

#### About this tutorial

This tutorial is aimed at beginners who are learning Gobject system.
One of the biggest difficulty to learn GObject system is to understand its object oriented strategy.
All the necessary topics are described in [GObject API Reference](https://docs.gtk.org/gobject/).
But it is probably difficult especially for beginners.

The contents of this tutorial are not beyond the documentation.
It just gives you some example codes and diagrams to help you.
Readers should refer to the GObject documentation when learning this tutorial.

#### GObject reference manual has been changed

I have to point out that the GObject documentation above is the new version.
The Gnome documentation website is revised and the GObject reference manual is also changed recently (Aug/2021).
The old version of the reference manual is [here](https://developer-old.gnome.org/gobject/stable/).
Making new version of the documentation is in progress.
So, you might need to refer to the old version of the reference manual.

#### Generating GFM, HTML and PDF

The table of contents is below and you can see all the tutorials by following the link.
However, you can make GFM, HTML or PDF by the following steps.
GFM is 'GitHub Flavored Markdown', which is used in the document files in the GitHub. 

1. You need Linux operating system, ruby, rake, pandoc and LaTeX system.
2. download this repository and uncompress the files.
3. change your current directory to the top directory of the source files.
4. type `rake` to produce GFM files. The files are generated under `gfm` directory.
5. type `rake html` to produce HTML files. The files are generated under `html` directory.
6. type `rake pdf` to produce a PDF file. The file is generated under `latex` directory.

This system is the same as the one in the `gtk4 tutorial` repository.
There's a document `Readme_for_developers.md` in `gfm` directory of the repository.
It describes the details.

#### Contribution

If you have any questions, feel free to post an issue.
If you find any mistakes in the tutorial, post an issue or pull-request.
When you give a pull-request, correct the source files, which are under the 'src' directory, and run rake.
Then GFM files under the 'gfm' directory are automatically updated.

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
