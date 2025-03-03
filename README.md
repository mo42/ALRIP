# Approximate Largest Empty Rectangles in Polygons
`libabel` is a library for approximating the largest empty rectangle inside
general polygons with holes. In practice, you can label geographic areas with
this.

That's why it's called `libabel` so you can link to it using `-label`.

# Build

```sh
git clone https://github.com/mo42/alrip.git && cd alrip/src
make
```

`libabel.a` provides you the following function:

```c
rot_square axis_aligned_box(inner_outer& io, int scale);
```

where `io` is a tuple of inner polygons (the holes) and outer polygons (the
areas to be labelled).

# Examples

![Examples Small](example12.png "Examples Small")
![Example Large](example3.png "Example Large")

# License

We distribute ALRIP under the MIT License.
