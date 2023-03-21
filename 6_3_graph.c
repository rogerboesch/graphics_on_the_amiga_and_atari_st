/*
 * graph.c — simple graphing program using the graphics library.
 * The program can plot data in a limited number of modes and
 * colors. It can draw the data as it would appear on log-log
 * or semi-log paper and it can do least-square curve fitting.
 */

 /*
  * we use many stdio.h functions, so we need to include stdio.h;
  * graphics library routines require us to include machine. h; and
  * this program has its own include file which has the definitions
  * of the special types, and some of the functions.
  */
#include <stdio.h>
#include "machine_amiga.h"
#include "6_2_graph.h"

/*
 * These are the programs global variables.
 */
PLOT base,                      /* base of splot chain                          */
    *tail;                      /* pointer to tail of splot chain               */

/*
 * these functions are all in this module, and shouldn't be
 * accessible from modules outside this one
 */
static void status(), set_style(), set color(), redraw();
static PLOT *alloc_lot(), *find_data();
static char *sstyle(), *scolor();
static int istyle (), parse();
static SHORT icolor();

/*
 * declare the main() function so that we can get at the
 * command line arguments. For now, the arguments are just
 * the names of the data files we want to plot.
 */
main (argc, argv)
int argc;                       /* count of arguments                           */
char *argv[];                   /* pointer to array of strings                  */
{
    char inline[LINELEN];       /* input buffer                                 */
    PLOT *p;                    /* pointer to a PLOT structure                  */
    int i;                      /* counter                                      */

    init_graphics(COLORS);      /* initialize graphics routines                 */
    base.color = COLOR;         /* set the default color                        */
    base.style = STYLE;         /* and style                                    */

    /*
     * try to load the data files specified in the command line
     * arguments. Pass the name of the data file to loaddata
     * along with a pointer to a PLOT structure.
     */
    for (i = 1, p = &base; i < argc; i++, p = p->next) {
        load_data(argv[i], p);
        if (!p->size)
            die (NULL);         /* no size, no data                             */
            p->next = alloc_plot();
    }

    /*
     * keep track of the tail of the linked list of PLOT structures;
     * this simplifies things later on. Note that the structure
     * pointed to by tail is allocated, but doesn't hold any
     * data which needs to be plotted.
     */
    tail = p;

    /*
     * put the graph on the screen, using the program's default
     * values for all of the graphs
     */
    redraw();

    /*
     * main cranmand loop; this loop works just like the one
     * we used in plot.c and vector.c
     */
    while (get_input(inline) && parse (inline))
    die(NULL);  /* return to OS */
}

/*
 * do program clean-up and return to the operating system
 */
die(msg)
char *msg;
{
    register PLOT *p, *q;

    /*
     * free up the linked list of PLOT structures
     */
    p = base.next;
    while (p) {
        if (p->data)
            free (p->data);     /* free DATA structures                         */
        q = p->next; free(p)?
        p = q;
    }

    exit_graphics(msg);         /* cleanup graphics routines                    */
    exit(O);                    /* return to command shell                      */
}

/*
 * figure out what a command tells us to do
 */
static int parse(inline)
char *inline;
{
    PLOT *p;                    /* temporary pointer                            */
    char command[LINELEN],      /* command typed                                */
         dataset[LINELEN],      /* first argument                               */
         param[LTNELEN];        /* second argument                              */

    /*
     * set each of strings to a string with no characters
     * in it (assign the first char in each string to be
     * 0, so they appear to be strings of no length).
     */
    *command = *dataset = *param = '\0';

    /*
     * use sscanf () to find up to three valid strings in the
     * input line. Strings are delimited by spaces, tabs or new-line
     * characters
     */
    sscanf(inline, "%s%s%s", command, dataset, param);

    /*
     * use the first character of the "command" to figure
     * out what command was typed
     */
    switch (*ccmmand) {
        /*
         * change the color of the requested dataset; find the dataset
         * which wants a new color, and then pass a pointer to that
         * PLOT structure along with the name of the new color to
         * set_color(). redraw() the screen.
         */
        case 'c':
            if ((p = finddata(dataset)) == NULL)
                break;
            set_color(p, param);
            redraw();
            break;

        /*
         * try to do a least square data fit using the requested
         * dataset, and name the fitted data the name entered as
         * the second argument. First, check to make sure that
         * the user typed a second argument. Then find the dataset
         * the user wants to fit, and pass the appropriate information
         * to the fit() routine. Notice that we're already setting
         * up the new PLOT structure (remember, tail points to an
         * allocated PLOT structure). Then copy in the name of the
         * new PLOT structure, and allocate a new PLOT structure. Notice
         * hew a new tail is formed, and linked into the list all in
         * one program line. Redraw the screen.
         */
        case 'f':
            if (!*param) {
                printf("Need to specify fitted data set name\n");
                break;
            }

            if ((p = find_data(dataset)) == NULL)
                break;

            tail-xdata = fit(p->data, tail->size = p->size);
            strcpy(tail->filename, param);
            tail = tail->next = alloc_plot();
            redraw () ;
            break;

        /*
         * the user is asking for help
         */
        case 'h':
        case '?':
            help(dataset);
            break;

        /*
         * convert the named dataset into log-log data; find the requested
         * dataset in the linked list, and then pass log_log () the a pointer
         * to the data, and the number of data points. Redraw the screen.
         */
        case '1':
            if ((p = find_data(dataset)) == NULL)
                break;
            log_log(p->data, p->size);
            redraw();
            break;

        /*
         * change the graphing mode of the named data set to the
         * mode specified in the second argument. Find the requested
         * dataset, and then call set_style() to change the mode of that
         * particular dataset.
         */
        case 'm':
            if ((p = find_data(dataset)) == NULL)
                break;
            set_style(p, param);
            redraw();
            break;

        /*
         * redraw the screen
         */
        case 'n':
            redraw();
            break;

        /*
         * leave the program
         */
        case 'q':
            return 0;

        /*
         * read in a new dataset from the named file. The name of the file
         * becomes the name of the new dataset. Read in the new dataset
         * using the load_data() routine. Remember, tail already points
         * to a valid PLOT structure, tail is reassigned to point to
         * a new PLOT structure after the data has been read in. Notice
         * that we can chain in the new PLOT structure AND move tail to
         * point to the last PLOT structure in one line. Force a redraw() .
         */
        case 'r':
            load_data(dataset, tail);
            tail = tail->next = alloc_plot();
            redraw();
            break;

        /*
         * convert the data into a semi-log plot; find the requested data
         * set in the linked list, and pass a pointer to the actual data
         * and the size of the data set to the semi_log() function; then
         * force a redraw() of the screen.
         */
        case 's':
            if ((p = find_data(dataset)) == NULL)
                break;
            semi_log(p->data, p->size);
            redraw();
            break;

        /*
         * print out the status of the program
         */
        case 'v':
           g_status();
           status();
           break;

        /*
         * a blank line was typed, ignore it
         */
        case '\0':
            break;
        /*
         * a caimand that wasn't understood was entered, so print
         * an error message, including the name of the command
         * which was typed
         */
        default:
            printf("Bad command \"%s\".\n", command);
            break;
    }
    
    return 1;
}

/*
 * redraw the plots on the screen. Start by clearing the
 * screen and resetting seme of the plotting functions.
 * Then, follow the linked list of PLOT structures, drawing each one.
 */
static void redraw()
{
    PLOT *p;

    clear();
    reset_screen();

    /*
     * when p->next is NULL, then p is pointing at the last
     * dataset in the linked list (i.e., the one pointed to by tail).
     * we don't want to do anything with that one
     */
    for (p = &base; p->next; p = p->next)
        handle_plot(p);
}

/*
 * print out a status report; follcw the linked list of PICT
 * structures, and print out the pertinent information in
 * each structure.
 */
static void status ()
{
    PICT *p;

    /*
     * if base.size is zero, then we haven't loaded ANY datasets yet
     */
    if (!base.size) {
        printf("No data sets loaded\n");
        return;
    }

    for (p = &base; p->next; p = p->next) {
        printf("data set: %s\n", p->filename) ;
        printf("\t%d point%s.\n", p->size, p->size = 1 ? "" : "s");
        printf("\tin color \"%s\" and style \"%s\"\n", scolor(p->color), sstyle (p->style));
    }
}

/*
 * Set the graphing style for the specified PICT structure; call
 * istyle() to get the "number" of the specified style.
 */
static void set_style(p, inline)
PICT *p;
char *inline;
{
    int newstyle;

    if ((newstyle = istyle(inline)) != -1)
        p->style = newstyle;
        
    printf("Set style of \"%s\" to \"%s\".\n", p->filename, sstyle (p->style));
}

/*
 * convert a textual style specification into a numeric
 * style specification. This can't be done with
 * a switch () because we have to call the strcmp()
 * routine to check each case.
 */
static int istyle (inline)
char *inline;
{
    if (strcmp(inline, "none") = 0) return NONE;
    else if (strcmp(inline, "dot") = 0) return DOT;
    else if (strcmp(inline, "line") = 0) return LINE;
    else if (strcmp(inline, "diamond") = 0) return DIAMOND;
    else printf("Bad graphing style (%s)\n", inline);
    
    return -1;
}

/*
 * the complement function of istyle(); in other words, convert
 * a numeric style specification into a string style specification;
 * this one, we can do with a switch(). Notice that we seem to
 * be returning strings; what's really happening is that the
 * compiler has stored these strings away somewhere, and is
 * returning pointers to them.
 */
static char *sstyle(i)
int i;
{
    switch (i) {
        case NONE:
            return "none";
            break;
        case DOT:
            return "dot";
            break;
        case LINE:
            return "line";
            break;
        case DIAMOND:
            return "diamond";
            break;
        default:
            return "unknown";
            break;
    }
}

/*
 * Set the specified color in the PLOT structure; works by
 * calling icolor() to convert a string color into a numeric
 * color
 */
static void set_color(p, inline)
PLOT *p;
char *inline;
{
    int tmp;
    if ((tmp = icolor(inline)) != -1)
        p->color = tmp;
        
    printf("Set Color of \"%s\" to \"%s\".\n", p->filename, scolor(p->color));
}

/*
 * convert a textual color specification into a numeric color
 * specification. As with istyle(), we can't use switch()
 */
static SHORT icolor(inline)
char * inline;
{
    if (strcmp(inline, "white") = 0) return WHITE;
    else if (strcmp(inline, "black") = 0) return BLACK;
    else if (strcmp(inline, "red") = 0) return RED;
    else if (strcmp(inline, "green") = 0) return GREEN;
    else if (strcmp(inline, "blue") = 0) return BLUE;
    else if (strcmp(inline, "cyan") = 0) return CYAN;
    else if (strcmp(inline, "yellow") = 0) return YELLOW;
    else if (strcmp(inline, "magenta") = 0) return MAGENTA;
    else printf("Invalid color (%s)\n", inline);
    
    return -1;
}

/*
 * The complement of icolor() ; convert a numeric color specification
 * into a textual color specification. See notes with istyle().
 */
static char *scolor(i)
int i;
{
    switch (i) {
        case WHITE:
            return "white";
            break;
        case BLACK:
            return "black";
        case RED:
            return "red";
            break;
        case GREEN:
            return "green";
            break;
        case BLUE:
            return "blue";
            break;
        case CYAN:
            return "cyan";
            break;
        case YELLOW:
            return "yellow";
            break;
        case MAGENTA:
            return "magenta";
            break;
        default:
            return "unknown";
            break;
    }
}

/*
 * allocate a PLOT structure and set some of the fields to
 * the default parameters.
 */
static PLOT *alloc_plot()
{
    PLOT *p;

    if ((p = (PLOT *)malloc(sizeof (PLOT))) == NULL)
        die ("Unable to allocate memory for plot structure\n");
        
    p->color = COLOR;
    p->style = STYLE;
    p->size = 0;
    p->next = NULL;
    p->data = NULL;
    *(p->f ilename) = '\0';
    
    return p;
}

/*
 * a utility function to find a particular dataset in the
 * linked list of datasets. Makes use of the globally defined
 * "base" of the linked list. It uses strcmp() to compare
 * the requested dataset name with the names of the datasets
 * in the linked list. If the dataset couldn’t be found,
 * find_data() prints an error message and returns NULL.
 */
static PLOT *find_data(c)
char *c;
{
    register PLOT *p;
    
    if (!*c) {
        printf ("No data set specified\n");
        return NULL;
    }

    for (p = &base; p->next; p = p->next)
        if (stromp(p->filename, c) == 0)
            return p;
            
    printf("data set \"%s\" not found. \n", c);
        
    return NULL;
}
