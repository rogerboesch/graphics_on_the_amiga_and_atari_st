#include <stdio.h>

main()
{
    show_val(4);
}

show_val(x)
int x;
{
    printf ("The number %d was passed into shcw_val().\n", x);
}