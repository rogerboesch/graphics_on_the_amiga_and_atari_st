#include <stdio.h>
#include "machine.h"

double sin(), cos();
#define PI 3.14159265359

/*
 * Use our line() routine to draw 360 "spokes" of a wheel.
 */
main()
{
    SHORT i;
    float len;

    init_graphics(COLORS);
    len = .8 * ((y_size < x_size) ? y_size/2 : x_size/2);

    set_pen(WHITE) ;
    for (i = 0; i < 360; ++i)
        line(x_size / 2, y_size / 2,
            (SHORT)(x_size/2 + len * cos(i / 180.0 * PI)),
            (SHORT)(y_size/2 + len * sin(i / 180.0 * PI)));
            
    exit_graphics(NULL);
}
