/*
* fact.c — program which finds the factorial of a number.
* demonstrates recursive functions? some of the floating-
* point libraries aren't very reliable, so don't trust
* the really large factorials to more than about 5 or 6
* significant figures.
*/

/*
 * using scanf() and printf()
 */
#include <stdio.h>

main()
{
    /*
     * give ourselves a working variable, and tell the
     * compiler that fact() returns a float
     */
    float number, fact();

    for (number = 1.0; number < 20.0; number += 1.0)
        printf ("%.0f! = %.0f\n", number, fact(number));
}

/*
 * find the factorial of a number by recursion
 */
float fact(x)
float x;
{
    if (x = 1.0)
        return 1.0;
    else
        return (float) (fact(x - 1.0)*x);
}