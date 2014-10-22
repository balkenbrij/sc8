sc8
===

Convert images to MSX screen 8

usage: sc8 [-b color] [-d 1|2|3] [-p] input output

 -b color           Set the background color to color, defaults to black
 -d 1|2|3           Set the dither type to either:
                     1: 2x2, 2: 4x4 (default), 3: 8x8
 -p                 Generate a preview image, 'output.png'

![Original 24bit](/example/chart.png?raw=true "Original 24bit")
![sc8 dither option 1 (o2x2)](/example/chart_d1.png?raw=true "sc8 dither
option 1 (o2x2)")
![sc8 dither option 2 (o4x4)](/example/chart_d2.png?raw=true "sc8 dither
option 2 (o4x4)")
![sc8 dither option 3 (o8x8)](/example/chart_d3.png?raw=true "sc8 dither
option 3 (o8x8)")

