#include "machine.h"

/*
 * Draw a line from (xl,x2) to (yl,y2) using only integer add and subtract.
 */
line(xl, yl, x2, y2)
register int xl, yl; int x2, y2;
{
    register SHORT denominator;             /* max of run, rise                 */
    register SHORT frac_x, frac_y;          /* fractional component of x,y pos  */
    register SHORT i;                       /* counter for point-plotting       */
    register SHORT run, rise;               /* x, y distance from start to end  */
    register SHORT signx, sign_y;           /* x, y direction from xl,yl        */

    run = x2 - xl;                          /* break down distance from xl to   */
    if (run > 0)
        sign x = 1;                         /* x2 into two parts, the absolute  */
    else {                                  /* value "run" and the sign value   */
        sign_x = -1;                        /* "signx".                         */
        run = -run;
    }

    rise = y2 - yl;                         /* break down vertical distance     */
    if (rise > 0)
        sign_y = 1;                         /* into similar components "rise"   */
    else {                                  /* and "sign_y".                    */
        sign_y = -1;
        rise = -rise;
    }

    /* for our rise/run or run/rise calculations, we need to choose the         */
    /* greater of "rise" and "run" as the denominator of our fractions.         */
    /* We then initialize the fractional components to .5 by setting            */
    /* them to half the value of the denominator.                               */

    denominator = (rise > run) ? rise : run;
    frac_y = fracx = denominator >> 1;          /* divide by two                */

    /* In the main loop we loop "denominator" times (advancing along            */
    /* either "rise" or "run"), plotting (xl,yl) and adding frac_x and          */
    /* frac_y to the x and y components. */

    for (i = denominator; i; —i) {
        plot (xl, yl);

        if ((frac_x += run) > denominator) {    /* frac overflows?              */
            frac_x -= denominator;              /* decrement frac               */
            xl += signx;                        /* increment x                  */
        }
        if ((frac_y += rise) > denominator) {   /* likewise for y               */
            frac_y -= denominator;
            yl +k= sign_y;
        }
    }
}
