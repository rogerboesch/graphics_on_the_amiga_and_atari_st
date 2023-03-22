/*
 * simple program which uses sub()
 */

/*
 * include stdio.h because we're using printf()
 */
 
 #include <stdio.h>

/*
 * define sub() so the compiler knows that it is returning
 * an int. Notice that we aren't saying WHAT sub() does, we're
 * just telling the compiler that sub() returns an int. In addition
 * we don't actually declare sub() until after the declaration of main().
 */
extern int sub();

/*
 * declaration of main(). This is the function which the computer
 * will execute first once it starts working on our program
 */
main()
{
    int a=5;
    int b,c;

    b = 8;
    c = sub(a,b) ;
    printf("The result of %d minus %d is %d\n",a,b,c);
}

/*
 * the } above is the end of main (). This is where the program
 * execution will stop.
 */

 /*
 * below is the declaration of sub() . We've already told the compiler
 * that sub() returns an int, but we have to tell it again here,
 * so that it knows we're being consistent.
 */
int sub(numl, num2)
int numl; int num2;
{
    int subtr;
    
    subtr = numl - num2;
    return subtr;
}