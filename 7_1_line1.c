#include "machine.h"

/*
 * Draw a line from (xl,yl) to (x2,y2).
 */
line(xl, yl, x2, y2)
int xl, yl, x2, y2;
{
    float x, y, slope;
    
    y = yl;                                 /* step variable for y      */
    slope = (float) (y2 - yl) / (x2 - xl);  /* = rise / run             */

    for (x = xl; x < x2; ++x) {             /* loop from left to right  */
        plot((SHORT)x, (SHORT)y);
        y += slope;                         /* add "slope" to y         */
    }
}
