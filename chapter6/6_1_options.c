/*
 * simple program to print command line arguments
 */

#include <stdio.h>

main (argc, argv)
int argc;       /* number of arguments */
char *argv[];   /* pointer to an array of strings */
int i;          /* counting variable */
{
    /* print exit each argument, one by one */
    for (i = 0; i < argc; ++i)
        printf("%s\n", argv[i]);
}