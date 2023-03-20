/*
 * Defines for machine-dependent information
 */

/* define compiler type */
#define AZTEC 1
#define LATTICE 0

/* use 16-bit SHORT'S and don't worry about the ZERO bug */
#define SHORT short
#define ZERO

/* allow LATTICE to use doubles for speed and functionality */
#if LATTICE
    #define FLOAT double
#else
    #define FLOAT float
#endif

/* defines for call to init_graphics() */
#define GREYS 0
#define COLORS 1
#define BLACK 0
#define WHITE 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define CYAN 5
#define YELLOW 6
#define MAGENTA 7

extern SHORT x size, y_size, max_intensity;
extern void init_graphics(), exit_graphics();
extern void set_pen(), move(), draw(), plot(), clear(), punt();
extern char *get_input();
extern char *malloc(), *calloc();

/* define this as the machine's screen's x- and y-size */
#define MAXLINE 200
#define MAXPIXEIS 320