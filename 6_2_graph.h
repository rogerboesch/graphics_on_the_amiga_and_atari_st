/* 
 * include file for the modules of the graph program
 */

/*
 * plot inodes which we can use
 */
#define NONE 0
#define DOT 1
#define LINE 2
#define DIAMOND 3

/*
 * some arbitrary constants
 */
#define UNELEN 1024         /* length of input line */
#define STYLE LINE          /* default style        */
#define COLOR WHITE         /* default color        */
#define LEFTM 5             /* left margin          */
#define RIGHTM 5            /* right margin         */
#define TOPM 5              /* top margin           */
#define BOTTOMM 5           /* bottom margin        */

/*
 * structure to hold the data being plotted
 */
typedef struct {
    FLOAT x,y;
} DATA;

/*
 * structure to hold information about a particular plot; we build
 * a linked list of these as we add more to the screen. Ihe program
 * follows the linked list of PLOT structures to redraw the screen.
 */
typedef struct plot_s {
    struct plot_s *next;    /* pointer to next plot         */
    DATA *data;             /* pointer to array of data     */
    char filename[LINELEN]; /* name of the data file        */
    int size;               /* number of data points        */
    SHORT color;            /* color to do the plot in      */
    int style;              /* style of line drawing to use */
} PLOT;

/*
 * data regarding the particular display
 */
typedef struct {
    FLOAT xscale,           /* how much to scale x coord    */
    yscale,                 /* how much to scale y coord    */
    minx,                   /* smallest x                   */
    miny,                   /* smallest y                   */
    xoff,                   /* center of screen in x        */
    yoff;                   /* center of screen in y        */
    int scaled;             /* has the screen been scaled?  */
} DISPLAY;

/*
 * define these, so the compiler kncws what they are going
 * to return. Each module (in theory) needs to have access
 * to at least some of these routines
 */
extern void handle_plot(), load_data(), help();
extern void semi_log(), log_log(), clear_screen(), g_status ();
extern DATA *fit(), *alloc_data();
