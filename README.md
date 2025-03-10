# Approximate Largest Empty Rectangles in Polygons
`libabel` is a library for approximating the largest empty rectangle inside
general polygons with holes. In practice, you can label geographic areas with
this.

That's why it's called `libabel` so you can link to it using `-label`.

# Build

```sh
git clone https://github.com/mo42/libabel.git && cd libabel/src
make
```

`libabel.a` provides you the following functions:

```c
rot_square approx_max_axis_aligned(inner_outer& io, int scale);

rot_square approx_max_rotated(inner_outer& io, double scale);
```

where `io` is a tuple of inner polygons (the holes) and outer polygons (the
areas to be labelled). The parameter `scale` determines the aspect ratio of the
rectangle you wish to find (e.g., 3 mean the rectangle's width is 3 times
longer than its height).

# Examples

![Examples Small](example12.png "Examples Small")
![Example Large](example3.png "Example Large")

# License

We distribute libabel under the MIT License.
