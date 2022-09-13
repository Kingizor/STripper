STripper
========

A background extractor for various Donkey Kong games.

There are separate graphical interfaces targetting GTK3 (GNU/Linux, etc) and the Windows API, as well as a command line version.

The following games are supported:

* SNES:
    * Donkey Kong Country
    * Donkey Kong Country 2
    * Donkey Kong Country 3
* GB:
    * Donkey Kong Land
    * Donkey Kong Land 2
    * Donkey Kong Land III
* GBC:
    * Donkey Kong Country
    * Donkey Kong Land III
* GBA:
    * Donkey Kong Country
    * Donkey Kong Country 2
    * Donkey Kong Country 3
    * DK - King of Swing
* NDS:
    * DK - Jungle Climber

The program primarily targets the unmodified NA versions. Modified versions of the games have the potential to cause crashes so a simple checksum is in place to verify the data and to prevent anything bad from happening!

Build Instructions
------------------
The project is written entirely in C, so a suitable C compiler is required.

Clone the repository with `git clone --recursive` and build with [meson](https://mesonbuild.com/Quick-guide.html).

External Dependencies
---------------------
* `dkcomp - DK compression library (included)`
* `libpng - for writing PNG images`
* `openMP - parallel processing` (_optional_)
* `GTK3 --- GTK3 Interface` (_optional_)

License
-------
MIT

