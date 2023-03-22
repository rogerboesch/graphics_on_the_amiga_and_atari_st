/*
 * vector.c - demonstrate the use of vectors in 2d graphics.
 * The program is designed to draw a dagger and can be used
 * to manipulate it on the screen.
 */

/*
 * include header files
 */
#include <stdio.h>
#include "machine.h"

/*
 * definitions of program states; as with plot.c, these are used to
 * let the function which figures out what command has been typed
 * and the function which actually executes the command to
 * communicate with one another.
 */
#define ERROR -1
#define NONE 0
#define ROTATE 1
#define TRANS 2
#define SCAIE 3
#define HELP 4
#define STATUS 5
#define LOOP 6
#define QUIT 7

/*
 * these have to be defined so the compiler doesn't complain
 */
extern void die(), prompt(), draw_fig(), help();
extern void status(), rotate(), make_rot(), scale(), scale_fig();
extern void mvnult( );
extern double oos(), sin();

/*
 * the figure, as (x,y) coordinate pairs; global variable figsize
 * holds the number of points in the figure. Don't forget to
 * change it if you change the shape of the figure.
 */
FLOAT fig[9][2] = {
    {-6.0, 0.0},
    {-5.0, 2.0},
    { 6.0, 0.0},
    {-5.0,-2.0},
    {-5.0, 2.0},
    {-4.0, 4.0},
    {-2.0, 0.0},
    {-4.0,-4.0},
    {-6.0, 0.0}
};

/*
 * globed variables to store state of the screen image; cur[][] is the
 * set of vectors Which need to be drawn on the screen, theta indicates
 * which direction the object is pointing, sfactor is the size
 * of the object, off[ ] is a vector which points to where the
 * center of the object should be drawn on the screen. Remember, the
 * screen's center isn't at (0,0). (0,0) is in the upper left corner
 * Of the screen.
 */
FLOAT   cur[9][2],      /* the vectors to draw on the screen */
        theta = 0.0,    /* what direction the object points */
        sfactor =2.0,   /* initial size of the object */
        off[2];         /* offset of the object from (0,0) */

int figsize = 9;        /* size of the figure */

main()
{
    char inline[256];       /* buffer for get_input() */
    init_graphics(CDKRS);   /* initialize the graphics */
    set_pen((SHORT)WHITE);  /* draw the figure in white */
    off[0] = x_size/2;      /* initial position */
    off[l] = y_size/2;
    draw_fig();             /* draw the figure */

    /*
     * the main control loop; get_input() from the user, and then try
     * to parse it with execute (). Remember, execute () will never
     * be called if get_input() returns NULL (end of file condition).
     */
    while (get_input( inline) && execute (inline))
        draw_fig();         /* draw the figure */
    die (NULL);             /* leave "vector" */
}

/*
 * do any cleanup in order to leave vector
 */
void die(msg)
char *msg;
{
    exit_graphics (msg);    /* exit the graphics routines */
    exit(O);                /* leave the program */
}

/*
 * execute a command based on the current mode of the program
 */
execute (c)
char *c;
{
    float tup = 0.0, rad = 0.0, dir = 0.0;
    int command;

    /*
    * call parse () to find out what the command is
    */
    command = parse (*c);

    /*
     * LOOP command calls execute () recursively in order to deal with
     * the command that's supposed to be looped. The routine works
     * by finding the first ';', and then passing execute the rest of
     * input line which follows the ':'.
     */
    if (command == LOOP) {
        register char *begin = c;           /* local vars */
        int count = 0;

        while (*begin != ':' && *begin)     /* find : */
            ++begin;                        /* get past */
        
        if (!*begin) {
            printf("No \":\" for loop construct. \n");
            return 1;
        }

        ++begin; 
        sscanf(&c[l], "%d", &count);
        if (count <= 0) {
            printf ("<count> argument was bad.\n") ;
            return 1;
        }

        for ( ; count; -count) {
            execute(begin);
            draw_fig();
        }
    }
    /*
     * ROTATE adjusts the global theta variable. Actual rotation of the
     * figure is done within drawfigQ.
     */
    else if (command == ROTATE) {
        sscanf(&c[l], "%f", &tmp);
        theta += tup;
    }
    /*
     * SCALE adjusts the global sfactor variable. As with rotation, the
     * actual scaling is performed within draw_fig().
     */
    else if (command == SCALE) {
        sscanf(&c[l], "%f", &tmp);
        sfactor = tmp;
    }
    /*
     * TRANS modifies the offset variables (relatively) ? faked by adjusting
     * the offsets with good old sin() and cos(). When the figure is
     * redrawn, it will be in the new position.
     */
    else if (command == TRANS) {
        sscanf(&c[l], "%f%f", &rad, &dir);
        off[0] += rad * cos(dir * 3.1415927/180);
        off[l] += rad * sin(dir * 3.1415927/180);
    }
    /*
    * print the help menu
    */
    else if (command == HELP)
        help();
    /*
     * print a status report
     */
    else if (command == STATUS)
        status();
    /*
     * return FALSE if we're ready to quit the program
     */
    else if (command == QUIT)
        return 0;
    /*
     * parse() could return NONE or ERROR; we ignore these here, since
     * none means do nothing, and if ERROR is returned the user has
     * already been alerted to the problem
     */
    else if (command == NONE || command == ERROR)
        return 1;
    /*
     * ack! we're not in a known state. Output an error and then
     * put the program in a known state. This is a bad sign, the
     * computer is probably about to crash.
     */
    else
        printf ("Unknown program command!\n");
        
    return 1;
}

/*
 * draw the figure on the screen. Clears the screen before it tries
 * to do any drawing. If the figure doesn't "fit" on the screen, then
 * it prints an error message, clears the screen and does nothing.
 */
void draw_fig()
{
    int i;
    FLOAT tx, ty;
    if (theta >= 360.0)
        theta -= 360.0;                         /* keep theta 0 <-> 360 */
    else if (theta < 0.0)
        theta += 360.0;

    rotate(theta, fig, cur, figsize);           /* rotate the figure */
    scale_fig(sfactor, cur, cur, figsize);      /* scale the figure */

    /*
     * check to make sure that the entire figure is going to fit
     * on the screen, y-ooordinate is inverted so that (0,0) appears
     * to be in the bottom left, rather than the upper left comer
     * of the screen.
    */
    for (i = 0; i < figsize; i++) {
        tx = cur[i][0] + off[0];
        ty = y_size - (cur[i][1] + off[l]);
        
        if (tx < 0 || tx >= x size || ty < 0 || ty >= y_size) {
            printf("Figure is off the screen\n");
            clear();
            return;
        }

        cur[i][0] = tx;
        cur[i][1] = ty;
    }
    clear();                                    /* clear screen */

    /*
     * draw the figure with the draw() command. By new, we know that
     * the figure will fit on the screen, so there's no need to check
     * the values going into move() and draw(). The type casts are
     * absolutely necessary, since move() and draw() both expect SHORTs,
     * not floats.
     */
    move((SHORT)cur[0][0], (SHORT)cur[0][l]);
    for (i = 1; i < figsize; i++)
        draw((SHORT)cur[i][0], (SHORT)cur[i][l]);
}

/*
 * parse an input command, and return the command which was
 * requested. Print an error message if an unknown command
 * was entered.
 */
int parse (d)
char d;
{
    if (d >= 'A' && d <= 'Z') d -= ('A' - 'a');
    if (d == 'h' || d == '?') return HELP;
    else if (d == '1') return LOOP;
    else if (d == 'q') return QUIT;
    else if (d == 'r') return ROTATE;
    else if (d == 's') return SCALE;
    else if (d == 't') return TRANS;
    else if (d == 'v') return STATUS;
    else if (d == '\0') return NONE;
    else {
        printf("Unknown conmand\n");
        return ERROR;
    }
}

/*
 * print out a help menu
 */
void help()
{
    printf("Available commands: \n");
    printf("h                    -- this help menu\n");
    printf("l <count>: <command> -- loop <command> <count> times\n");
    printf("q                    -- quit\n");
    printf("r <angle>            -- rotate the object (relative)\n");
    printf("s <factor>           -- scale the object\n");
    printf("t <offset> <angle>   -- move the object\n");
    printf("v                    -- current program settings\n");
}

/*
 * print the status of the program (any "important" variables
 * which might be of interest to the casual user).
 */
void status()
{
    printf("Position: (%f,%f)\n", off[0], off[l]);
    printf("Direction: %f degrees\n", theta);
    printf("Scaling: %f times normal\n", sf actor);
}

/*
 * scales each vector in the image by "factor"
 * This routine works by calling scale (), which scales only one
 * vector, on each of the vectors in the figure.
 */
void scale_fig(factor, in, out, size)
FLOAT factor, in[][2], out[][2];
int size;
{
    register int i;
    for (i = 0; i < size; i++)
        scale(factor, in[i], out[i]);
}

/*
 * work horse routine number one: scale a single vector by "factor"
 */
void scale(factor, in, cut)
FLOAT factor, in[2], out[2];
{
    out[0] = factor * in[0];
    out[l] = factor * in[l];
}

/*
 * rotate all of the vectors in a figure around (0,0)
 * This function calls make_rot() to get the right rotation
 * matrix.
 */
void rotate(theta, in, out, size)
FLOAT theta, in[][2], out[][2];
int size;
{
    register int i;
    FLOAT rotmat[2][2];

    make_rot(theta, rotmat); /* build matrix */
    for (i =0; i < size; i++)
        mvmult(rotmat, in[i], out[i]);
}

/*
 * build a rotation matrix for a turn of "angle" degrees (notice that
 * it's converted to radians before any "real" work is done with it).
 * Also notice that sin() and cos() expect doubles, so the cast is
 * necessary for seme compilers. This might not be clear, but the
 * matrix is defined as "matrix[column] [row]".
 */
void make_rot(angle, matrix)
FLOAT angle, matrix[2][2];
{
    angle *= 3.1415927/180.0;

    /* MEGAMAX bug, can't have cos() alone */
    matrix[l][1] = matrix[0][0] = 0.0 + cos((double)angle);

    /* MEGAMAX can't negate a floating point value */
    matrix[0][l] = ZERO - (matrix[l][0] = ZERO - sin((double)angle));
}

/*
 * work horse routine number two: multiply a 2x2 matrix by a vector.
 * Assumes that the matrix is defined as matrix [column] [rew]
 */
void mvmult(matrix, in, out)
FLOAT matrix[2][2], in[2], out[2];
{
    out[0] = in[0] * matrix[0][0] + in[l] * matrix[l][0];
    out[l] = in[0] * matrix[0][1] + in[l] * matrix[1][1];
}
