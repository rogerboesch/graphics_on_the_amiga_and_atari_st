#include "machine.h"

/*
 * Draw a line from (xl,yl) to (x2,y2) using system primitives.
 */
line(xl, yl, x2, y2)
int xl, yl, x2, y2;
{
    move(xl, yl);       /* use the routines from machine.c instead */
    draw(x2, y2);
}