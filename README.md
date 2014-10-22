sc8
===

Convert images to MSX screen 8

```
usage: sc8 [-b color] [-d 1|2|3] [-p] input output

 -b color           Set the background color to color, defaults to black
 -d 1|2|3           Set the dither type to either:
                     1: 2x2, 2: 4x4 (default), 3: 8x8
 -p                 Generate a preview image, 'output.png'
```

Dependencies
============

You'll need ImageMagick to compile it. On debian systems you'll need the
development files, libmagickwand-dev.

Example
=======

This is the public domain wikipedia color chart converted in all three
dither options.

![Original 24bit](/example/chart.png?raw=true "Original 24bit")
![sc8 dither option 1 (o2x2)](/example/chart_d1.sc8.png?raw=true "sc8 dither option 1 (o2x2)")
![sc8 dither option 2 (o4x4)](/example/chart_d2.sc8.png?raw=true "sc8 dither option 2 (o4x4)")
![sc8 dither option 3 (o8x8)](/example/chart_d3.sc8.png?raw=true "sc8 dither option 3 (o8x8)")

