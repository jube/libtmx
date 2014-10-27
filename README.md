# `libtmx`

`libtmx` is a C++11 library for reading [TMX files](https://github.com/bjorn/tiled/wiki/TMX-Map-Format). TMX is a XML dialect used in [Tiled](http://www.mapeditor.org/), a [tile map](http://en.wikipedia.org/wiki/Tile_engine) editor.

`libtmx` is not meant to modify TMX files but to read TMX files.

## Requirements

`libtmx` is written in C++11 so you need a C++11 compiler like [Clang](http://clang.llvm.org/) or [GCC](http://gcc.gnu.org/).

`libtmx` also needs external libraries:

* [TinyXML2](http://www.grinninglizard.com/tinyxml2/)
* [ZLib](http://www.zlib.net/)
* [Boost.Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/)
* [Boost.Iterator](http://www.boost.org/doc/libs/release/libs/iterator/) (header only library)
* [Boost.Range](http://www.boost.org/doc/libs/release/libs/range/) (header only library)
* [Boost String Algorithms](http://www.boost.org/doc/libs/release/libs/algorithm/string/) (header only library)

## Documentation

Check the [online documentation of the current version of `libtmx`](http://jube.github.io/libtmx/index.html). This a doxygen documentation with some additional examples for rendering a map.

## Build and install

You can download the sources directly from github:

    git clone https://github.com/jube/libtmx.git

Then you have to use [CMake](http://www.cmake.org/) to build the project:

    cd libtmx
    mkdir build
    cd build
    cmake ../src
    make

Finally, you can install the files (you may need root permissions):

    make install

## Use

`libtmx` provides a [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/) file so you can use it to configure your project.

    pkg-config --cflags --libs libtmx0

## Authors

- Julien Bernard, julien dot bernard at univ dash fcomte dot fr

## ChangeLog

See [ChangeLog.md](https://github.com/jube/libtmx/blob/master/ChangeLog.md).

## Copyright

This library is open source and is distributed under the [ISC licence](http://opensource.org/licenses/isc-license).
