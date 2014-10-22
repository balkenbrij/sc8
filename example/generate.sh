#! /bin/sh

for dither in 1 2 3; do
    sc8 -p -d $dither chart.png chart_d${dither}.sc8
done

