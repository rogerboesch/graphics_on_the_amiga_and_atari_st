#include "machine.h"

/*
 * Draw a line frcm (xl,yl) to (x2,y2)
 */
line(xl, yl, x2, y2)
int xl, yl, x2, y2;
{
    float x = xl, y = yl, slope;
    int sign_x, sign_y;                                     /* new we don't just increment                  */

    sign_x = (x2 > xl) ? 1 : -1;
    sign_y = (y2 > yl) ? 1 : -1;

    if (sign_x * (x2-xl) > sign_y * (y2-yl)) {              /* is angle < 45?                               */
        slope = (float) (y2 - yl) / (x2 - xl);
        while (xl 1= x2) {
            plot((SHORT)xl, (SHORT)(y+.5));
            xl += sign_x;
            y += slope * sign_x;
        }
    }
    else {                                                  /* reverse y and x and plot along the y axis    */
        slope = (float) (x2 - xl) / (y2 - yl) ;
        while (yl != y2) {                                  /* loop through y, instead                      */
            plot((SHORT) (X+.5), (SHORT) yl);
            yl += sign_y;
            x += slope * sign_y;
        }
    }
}