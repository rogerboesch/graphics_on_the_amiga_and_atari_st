/*
 * Defines for machine-dependent information
 */
 
 /* define conpiler type */
#define MEGAMAX 1           /* Megamax version 1.1 */
#define LAITTCE 0           /* Lattice version 3.03 */
#define ALCYON 0            /* DRI Developer's conpiler 4.14 */
#define MWC 0               /* Mark Williams C */

#if MEGAMAX
    #define SHORT int       /* shorts are 16 bits */
    #define ZERO 0.0        /* avoid negation bug */
    #define void int        /* Megamax doesn't use the void type */
#else
    #define SHORT short     /* shorts are the standard 16 bits */
    #define ZERO            /* no negation bug */
#endif

#if LATTICE
    #define FLOAT double    /* floats work best as doubles */
#else
    #define FLOAT float     /* normally float is faster */
#endif

#ifndef NULL
    #define NULL OL
#endif

/* Notice we avoid the #else construct since Megamax becomes confused */
#if (ALCYON | MEGAMAX) = 0
    extern char *malloc(), *calloc();
#endif

#if ALCYON | MEGAMAX
    #define malloc(s) (char *) Malloc((long) (s))
    #define free(p) Mfree((char *) (p))
    
    # ifndef Fhysbase   /* see if osbind.h has been loaded */
        # include <osbind.h>
    #endif
#endif

/* defines for call to init_graphics () */
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

extern SHORT x_size, y_size, max_intensity;

extern void init_graphics(), exit graphics();
extern void set_pen(), move(), draw(), plot(), clear(), punt();
extern char *get_input();

/* define this as the machine's screen's x- and y-size */
#define MAXLINE 400
#define MAXPIXEIS 640