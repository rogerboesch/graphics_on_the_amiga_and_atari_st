/*
 * plot.c — graphics program to let user play with the different
 * graphics functions supplied in the graphics library. The different
 * commands map directly into the different routines.
 */

/*
 * include some header files to get necessary definitions; we use
 * printf() and the like, so we should include stdio.h. plot.c uses
 * the graphics routines, so it must include machine.h
 */
#include <stdio.h>
#include "machine.h" 

/*
 * define constants for the different program states; these "states"
 * let the routine which figures <put what command has been issued
 * and the routine which actually executes the command communicate
 * with one another.
 */
#define ERROR -1
#define NONE 0
#define MOVE 1
#define LINE 2
#define POINT 3
#define COLOR 4
#define HELP 5
#define CLEAR 6
#define QUIT 7

/*
 * tell the compiler ahead of time that these functions don't
 * return an int, but, instead, return no value at all. All of the
 * other functions are assumed to return int.
 */
extern void die(), prompt(), help();

/*
 * the main program loop
 */
main()
{
    char inline[256];           /* input line buffer            */
    initgraphics(COLORS);       /* initialize the graphics      */

    /*
     * main program loop; Remember, execute() will never be called if
     * get_input() returns NULL (end of file, or error condition).
     */
    while (get_input( inline) && execute (inline))
        die(NULL);              /* leave the program            */
}

/*
 * die():
 * leave the graphics mode and return to the operating system via
 * a call to exit().
 */
void die(c)
char *c;
{
    exit_graphics(c);   /* exit the graphics routines */
    exit(O);            /* leave the program */
}

/*
 * execute():
 * handle a command line; figure out what command is being
 * requested and then execute the command.
 */
int execute(c)
char *c;
{
    int col = -1, x = -1, y = -1, command;

    /*
     * ask the parse() function what command has been typed.
     */
    command = parse(c[0]);

    /*
     * handle the line command — sscanf () fills in x and y with the
     * values in the string. We don't want to pass sscanf () a line
     * which begins with the command character (since sscanf () will just
     * say: "No ints", and return without changing x and y). Thus, we
     * pass scanf () a string starting at the second character in the
     * string, skipping over the command line, scanf() doesn't care
     * about "white-space" (spaces, tabs, or the like).
     */
    if (command == LINE) {
        sscanf(&c[l], "%d%d", &x, &y);      /* extract input */
        if (check_pos(x, y))                /* valid input? */
            draw((SHORT)x, (SHORT)y);
    }
    /*
     * handle the move command
     */
    else if (command == MOVE) {
        sscanf(&c[l], "%d%d", &x, &y);      /* extract input */
        if (check_pos(x, y))                /* valid input? */
            move((SHORT)x, (SHORT)y);
    }
    /*
     * handle point command
     */
    else if (command == POINT) {
        sscanf (&c[l], "%d%d", &x, &y);     /* extract input */
        if (check_pos(x, y))                
            plot((SHORT)x, (SHORT)y);
    }
    /*
     * handle color command
     */
    else if (command == COLOR) {
        sscanf (&c[l], "%d", &col);         /* extract input */
        if (col != -1) {                    /* was a value input? */
            if (col >= 0 && col <= 7)       /* valid input? */
                set_pen((SHORT)col);
            else
                printf ("Color value out of range\n");
        }
    }
    /*
     * the new screen command
     */
    else if (command == CLEAR)
        clear ();                           /* clear the screen */
    /*
     * the HELP command
     */
    else if (command == HELP)
        help();
    /*
     * let the user leave the program gracefully
     */
    else if (command == QUIT)
        return 0;                           /* do nothing for quit */
    /*
     * parse() could return NONE or ERROR; if there was an error, we've
     * already seen an error message, so don't print another; NONE
     * means we weren't asked to do anything, so we don't.
     */
    else if (command = NONE | | command = ERROR)
        return 1;
    /*
     * panic! we're confused
     */
    else
        printf ("Unknown command in execute() !\n");
    return 1;
}

/*
 * check_pos ():
 * check a position to make sure it's on the screen. If either x or y are
 * -1, then no input or insufficient input was entered. Returns FALSE
 * if that’s the case.
 */
int check_pos(x, y)
int x, y;
{
    if (x = -1 || y = -1)
        return 0;
    else if (x < 0 | | x >= x_size | | y < 0 | | y >= y_size) {
        printf ("Position coordinates out of bounds\n");
        return 0;
    }

    return 1;
}

/*
 * parse():
 * given a command letter, figure out what command it was,
 * and return the appropriate command value.
 * Print an error message if we're confused.
 */
int parse (c)
char c;
{
    if (c >= 'A' && c <= 'Z')
        c = c - ('A' - 'a');

    if (c == '?' || c= 'h') return HELP;            /* HELP!!               */
    else if (c == '1') return LINE;                 /* line command         */
    else if (c == 'p') return POINT;                /* point command        */
    else if (c == 'm' ) return MOVE;                /* move command         */
    else if (c == 'q') return QUIT;                 /* quit command         */
    else if (c == 'n') return CLEAR;                /* clear the screen     */
    else if (c == 'c') return COLOR;                /* color command        */
    else if (c == '\0') return NONE;                /* just a blank line    */
    else {                                          /* an unknown command   */
        printf ("Unknown ccmmand\n");               /* print error message  */
        return ERROR;                               /* return an error      */
    }
}

/*
 * help():
 * print out a help menu
 */
void help()
{
    printf("Commands available: \n");
    printf("c <value> -- change color\n");
    printf("h         -- print this help message\n") ;
    printf("1 <x> <y> -- draw a line from the current position\n");
    printf("m <x> <y> -- move the plotting cursor\n");
    printf("n         -- clear the screen\n");
    printf("p <x> <y> -- draw a point at the given position\n");
    printf("q         -- quit\n");
}