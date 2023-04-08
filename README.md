# Largest Empty Rectangle in Polygons
Compute the largest empty rectangle inside general polygons with holes.

## Motivation and Details
Automatically placing labels on maps is a problem relevant geographic
information systems. In this case, the shape of the label and, thus, the aspect
ratio is known.

This repository contains implementations of two algorithms for finding
(approximate) largest rectangles inside general polygons with holes:

- `ap`: axis-aligned rectangles
- `rr`: arbitrary rotated rectangles

![test large](test/test_large.png "Test Large")

![test medium](test/test_medium.png "Test Medium")

![test small](test/test_small.png "Test Small")

## Compiling

We switched to CMake. In the project folder, enter the following to obtain the
previously mentioned binaries:

```sh
mkdir build
cmake ../
make
```

## TODO
- [ ] Make library out of project and support common GIS format
- [ ] Use [CGAL](https://www.cgal.org/) for geometric primitives
