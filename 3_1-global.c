/*
 * program to help demonstrate the scope of variables
 */

int abc;                            /* global variable      */

main()
{
    float jk;                       /* local to main        */

    abc = 12;                       /* global to program    */
    jk = 3.1415;                    /* local to main        */

    printf ("main: abc: %d, jk: %f\n",abc, jk);
    funca();
    printf ("main: abc: %d, jk: %f\n",abc, jk);
    funcb() ;
    printf ("main: abc: %d, jk: %f\n",abc, jk);
    funcc();
    printf ("main: abc: %d, jk: %f\n",abc, jk);
}

funca()
{
    float abc;                       /* local to funca       */

    abc = 2.7;                       /* local to funca       */
    printf("funca(): abc: %f\n", abc);
}

funcb()
{
    static int jk;                  /* local, but static    */
    abc = 23;                       /* global change        */
    jk = 43;                        /* local change         */
    printf ("funcb(): abc: %d, jk: %d\n", abc, jk);
}

func()
{
    abc = 12;                       /* global change        */
    printf ("funcc() : abc: %d\n", abc);
}