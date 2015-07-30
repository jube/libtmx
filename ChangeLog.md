# ChangeLog for `libtmx`

## `libtmx` 0.3

### 0.3.0 (26 Dec 2014)

- add a soname to the library
- change the name of Base to Component
- improve memory handling (thanks to unique_ptr)
- add support for Tiled 0.10.0 (flip, orientation, render order)
- change the signature in LayerVisitor

### 0.3.1 (28 Apr 2015)

- set width and height attribute optional for rectangle and ellipse
- change the name of PolyBase to Chain
- fix the offset of the tileset
- map have properties

### 0.3.2 (??)

- fix a bug in the parsing of the rotation attribute

## `libtmx` 0.2

### 0.2.0 (7 Jan 2014)

- add a TileObject object type to handle images as objects
- add some utility functions
- `tmx_render` now uses Qt5
- add a ChangeLog and a CONTRIBUTING file

## `libtmx` 0.1

### 0.1.2 (7 Dec 2013)

- Bug: properties of objects were not parsed

### 0.1.1 (03 Nov 2013)

- Tweaks for building with crossroad
- Bug: `tmx_render` could not be built

### 0.1.0 (11 Oct 2013)

- Initial release
