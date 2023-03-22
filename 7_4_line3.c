#include "machine.h"

/*
 * Draw a line fron (xl,x2) to (yl,y2) using only integer math.
 */
line(xl, yl, x2, y2)
int x1, yl, x2, y2;
{
    int rise, run;
    int yd, xd;                          /* "deltas" that we add to x and y     */
    int sign_x, sign_y;                  /* sign of y2-yl, and x2-xl            */
    int x_dis, y_dis;                    /* distance xl to x2, yl to y2         */

    rise = y2 - yl;
    run = x2 - xl;                      /* we leave rise and run as ints        */
    sign_y = (rise > 0) ? 1 : -1;
    sign x = (run > 0) ? 1 : -1;        /* calculate the signs                  */
    x_dis = run * sign_x;
    y_dis = rise * sign_y;              /* dis = abs(run or rise)               */
    yd = rise / 2;
    xd = run / 2;                       /* initialize "fractions" to ".5"       */

    if (x_dis > y_dis)                  /* the line is less than 45 degrees     */
        while (xl != x2) {
            plot((SHORT)xl, (SHORT)(yl + yd/x_dis));
            xl += sign_x;
            yd += rise;                 /* as if adding rise/run (slope)        */
        }
    else
        while (yl != y2) {
            plot((SHORT)(xl + xd/y_dis), (SHORT) yl);
            yl += sign_y;
            xd += run;                  /* as if adding (1/slope)               */
        }
}