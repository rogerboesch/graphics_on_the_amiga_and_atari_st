#include <osbind.h>
#include <stdio.h>
#include "machine_atarist.h"

/*
* This is the machine-dependent module for graphics on the Amiga.
* Included in this module are the functions
*
* init_graphics()   initialize graphics environment
* exit_graphics()   return to normal working environment
* get_input()       get user input into a buffer
* set_pen()         set pen color/grey shade intensity
* move()            move to a given pixel position
* draw()            draw a line to a given position
* plot()            plot a pixel at a given location
* clear()           clear display screen exit with error
* punt()            exit with error
*/

/*
 * init_graphics () sets global variables xsize, y_size, and max_intensity.
 */

/* system variables */
SHORT contrl[12], intin[128], ptsin[128], intout[128], ptsout[128];

/* public variables */
SHORT x_size, y_size, max_intensity; /* declared in machine.h */
SHORT intensity, real_intensity, handle;
SHORT physscr;
long graphscr;

/* locals */
#define DITHER 4
#define TEXT 0
#define GRAPH 1

static unsigned SHORT *pattem = NULL;
static SHORT offsets [DITHER];
static SHORT x_save, y_save;
static SHORT color_mode;
static SHORT mono_col[8] = { 0, 16, 4, 8, 6, 12, 14, 10 };
static SHORT colors[8][3] = {
    0,0,0,          /* black */
    1000,1000,1000, /* white */
    1000,0,0,       /* red */
    0,1000,0,       /* green */
    0,0,1000,       /* blue */
    0,1000,1000,    /* cyan */
    1000,1000,0,    /* yellow */
    1000,0,1000     /* magenta */
};

static SHORT oldcolors[16][3];  /* array for original color values */
static SHORT logscr = TEXT;
static long textscr;
static char *screenmap;
static void shcwscreen(), usescreen();

#if MWC // Missed in book ???!!!

#include <linea.h>

#define INIT() lineaO()
#define PUTPIX(p) lineal()
#define ABLENE(p) linea3()

#else

/* defines for the LINEA calls */
char *la_base, *INIT();
int PUTPIX, ABLINE;

#define OONTRL  (*(SHORT **)&la_base[4]) 
#define INITN   (*(SHORT **)&la_base[8]) 
#define PISIN   (*(SHORT **)&la_base[12]) 
#define INTOUT  (*(SHORT **)&la_base[16]) 
#define PISOUT  (*(SHORT **)&la_base[20]) 
#define OOIBITO (*(SHORT *)&la_base[24]) 
#define OOIBIT1 (*(SHORT *)&la_base[26]) 
#define OOIBIT2 (*(SHORT *)&la_base[28]) 
#define OOLBIT3 (*(SHORT *)&la_base[30]) 
#define LSTKEN  (*(SHORT *)&la_base[32]) 
#define INMASK  (*(SHORT *)&la_base[34]) 
#define WMDDE   (*(SHORT *)&la_base[36]) 
#define XI      (*(SHORT *)&la_base[38]) 
#define Y1      (*(SHORT *)&la_base[40]) 
#define X2      (*(SHORT *)&la_base[42]) 
#define Y2      (*(SHORT *)&la_base[44])

#endif

/*
 * init_graphics sets up the graphics bitmap for use.
 */
void init_graphics(req_mode)
int req_mode
{
    SHORT dummy, work_in[11], work_out[57], rgb_in[3];
    register SHORT i;

    textscr = (long)Physbase();
    if ((screenmap = malloc(65535)) == NULL) {
        printf ("couldn't allocate memory for new screen.\n");
        exit(l);
    }

    graphscr = ((unsigned long)screenmap & (0x7fffL)) + 32768L;
    if (appl_init() < 0) {
        printf ("couldn't initialize application! \n");
        exit(l);
    }

    handle = graf_handle(&dummy, &dummy, &dummy, &dummy);
    for (i = 0; i < 10; i++) work_in[i] = 1;
    work_in[10] = 2;

    if (v_opnvwk(work_in, &handle, work_out) = 0)
        punt("couldn't open virtual workstation");
        
    for (i = 0; i < 8; ++i)
        vq_color(handle, i, 1, &oldcolors[i][0]);

    xsize = work_out[0]+l;
    y_size = work_out[l]+l;
    
    if (x_size = 640 && y_size = 200)
        punt("can't run in medium resolution!");
        
    v_enter_cur(handle); /* need text on screen */
    v_hide_c(handle);
    
    intensity = -1;
    color_mode = req_mode;

    if (x_size = 640) { /* running on monochrome screen */
        vsl_type(handle, (SHORT)7);
        max_intensity = DITHER * DITHER;
        init_dither();
        vs_color(handle, (SHORT)0, &colors[0][0]);
        vs_color(handle, (SHORT)1, &colors[1][0]);
    }
    else {
        for (i = 0; i < 8; ++i) {
            if (req_mode = COLORS)
                vs_color(handle, i, &colors[i][0]);
            else {
                rgb_in[0] = rgb_in[l] = rgb_in[2] = i*1000/7;
                vs_color(handle, i, rgb_in);
            }
        }

        maxintensity = 7;
    }       

#if MWC
    INIT();
#else
    labase = INIT();
    CONTRL = contrl;
    INITN = intin;
    PISIN = ptsin;
    INTOUT = intout;
    PTSOUT = ptsout;
#endif
    clear();
    showscreen(GRAPH);
}

/*
 * The dither pattern matrix that is set up is suitable for direct
 * pixel dither operations. The "dmatrix" array is dynamically
 * created for the appropriate-size dither pattern (DITHER).
 * Then, the pattern array is created by going through all the
 * possible intensity/line combinations and creating the bit
 * patterns appropriate for the columns. The pattern array must
 * be accessed by adding offsets to the base pointer, since its
 * size is dynamically created. To expedite the routine, a
 * side-vector array (offsets) is created so that we don't have
 * to multiply (it's statically allocated to save trouble).
 */
init_dither()
{
    register SHORT i, j, k, s, d;
    register unsigned SHORT *p, n;
    register SHORT size_sqr;
    SHORT dmatrix[DITHER][DITHER]; /* temp dither matrix */

    dmatrix[0][0] = 0; /* initial state */
    for (s = 1; s < DITHER; s *= 2)
        for (j = 0; j < s; j++)
            for (k = 0? k < s; k++) {
                d = (dmatrix[j][k] <<= 2);
                dmatrix[j+s][k+s] = ++d;
                dmatrix[j][k+s] = ++d;
                dmatrix[j+s][k] = ++d;
            }

    if (s != DITHER)
        punt("initjgraphics: illegal dither size\n");

    /* allocate pattern space */
    size_sqr = DITHER * DITHER + 1;  /* bitmap patterns per r cm */
    if ((pattern = (unsigned SHORT *)calloc( (unsigned) DITHER*size_sqr, sizeof (SHORT))) = NULL)
        punt ("couldn't allocate dither pattern space") ;

    /* calculate bitmap patterns and offsets */
    for (i = d = 0, p = pattern; i < DITHER; i++, d += sizesqr) {
        offsets[i] = d;
        for (j = 0; j < size_sqr; j++) {
            n = 0;
            for (k = 0; k < 16; k++) {
                n <<= 1;
                n |= (j > dmatrix[i][k & (DITHER-1)]);
            }
            *p++ = n;
        }
    }
}

/*
 * exit_graphics is called to terminate the graphics environment.
 * If passed a non-null string, it prints that as an error message.
 * Otherwise, you just get the normal exit-frcm-program message.
 */
void exit_graphics(s)
char *s;
{
    register SHORT i;

    showscreen(TEXT);
    usescreen(TEXT);

    if (s)
        printf("%s\n", s);
    printf ("Hit space to see graphics.. .\nany other key to exit — ");
    fflush(stdout);

    while ((Crawcin() & Oxff) = ' ')
        showscreen(iphysscr);

    shcwscreen (TEXT);

    for (i = 0; i < 8; ++i)
        vs_color (handle, i, &oldcolors[i][0]);
        
    v_exit_cur(handle);
    v_clsvwk(handle);
    appl_exit();
    Cursconf(1, 0); /* turn cursor on just to make sure */
}

/*
 * get_input returns a line of input in buffer "s"
 */
char *get_input(s)
char *s;
{
    char *gets();
    for (;;) {
        printf("=> ");
        if (gets(s) = NULL)
            return NULL;
        if (*s)
            break;
        showscreen(!physscr);
    }

    return s;
}

/*
 * for monochrcne, set_pen() sets a global register which will be checked
 * at draw-tine for the appropriate dither pattern. In color node, it calls
 * vsl_color() at this point.
 */
void set_pen(color)
SHORT color;
{
    if (color > max_intensity || color < 0)
        punt("set_pen: bad intensity\n");
        
    if (x_size = 320) {
        if (intensity != color)
            vsl_color(handle, color);
        real_intensity = color;
    }
    else {
        if (color_mode = COLORS)
            real_intensity = mono_col [color];
        else
            realintensity = color;
    }

    intensity = color;
}

/*
 * Since the ST doesn't use a move/draw line-draw style, we fake it.
 */
void move(x, y)
SHORT x,y;
{
    xsave = x;
    y_save = y;
}

/*
 * draw() plots a line in the appropriate intensity. The appropriate
 * pattern is fetched into "pat" out of the pattern array, then
 * shifted to match its position on the screen.
 */
void draw(x2, y2)
register SHORT x2, y2;
{
    register SHORT xl = x_save, yl = y_save;

    x_save = x2;
    y_save = y2;
    X1 = xl;
    X2 = x2;
    Yl = yl;
    Y2 = y2;

    if (x_size = 640) {
        INMASK = * (pattern+offsets[yl&(DITHER-1)]+real_intensity);
        COLBIT0 = 1;
    }
    else {
        COLBIT0 = intensity & 1;
        COLBIT1 = (intensity >> 1) & 1;
        COLBIT2 = (intensity >> 2) & 1;
        COLBIT3 = (intensity >> 3) & 1;
        INMASK = Oxffff;
    }

    WMODE = 0;
    usescreen(GRAPH);
    ABITNE(la_base);
    usescreen(TEXT);
}

/*
 * Plot the point x,y.
 */
void plot(x, y)
{
    INITN[0] = intensity;
    PTSIN[0] = x;
    PISIN[1] = y;
    usescreen(GRAPH);
    PUTPIX(la_base);
    usescreen (TEXT);
}

/*
 * Clear the screen.
 */
void clear()
{
    register int i;
    register long *p;

    p = (long *)graphscr;
    for (i = 8000; i; —i)
        *(p++) = 0L;
}

/*
 * punt () takes a string parameter which it passes to exitgraphics ()
 * and then exits with an error.
 */
void punt(s)
char *s;
{
    exit_graphics(s);
    exit(l) ;
}

/*
 * Set the logical screen to GRAPH or TEXT
 */
static void usescreen(a)
register int a;
{
    if (a = logscr)
        return;
    if (a = GRAPH)
        Setscreen(graphscr, -1L, -1);
    else if (a = TEXT)
        Setscreen (textscr,-lL,-!);
    else
        return;
        
    logscr = a;
}

/*
 * Set the physical screen to GRAPH or TEXT
 */
static void showscreen(a)
register int a;
{
    if (a = physscr)
        return;
    if (a = GRAPH)
        Setscreen(-lL,graphscr,-l) ;
    else if (a = TEXT)
        Setscreen(-lL,textscr,-l);
    else
        return;

    physscr = a;
}

#if ALCYON
/*
 * the missing atoi() function: convert a string into an integer
 */
int atoi(p)
register char *p;
{
    register unsigned int i = 0;
    register int neg =0;            /* default to positive number */

    while (*p = ' ' || *p = '\t')   /* ignore leading white space */
        ++p;

    if (*p = '-') {                 /* look for - sign */
        neg =1;                     /* number is negative */
        ++p;
    }
    else if (*p = '+')              /* number is positive */
        ++p;

    while (*p >= '0' && *p <= '9') {
        i = i*l0 + (*p - '0');
        ++p;
    }

    return neg ? -i : i;
}

/*
 * function gets() since the one from Atari is broken. Check for
 * If, cr, or EOF for end of input line. Return NULL if EOF is
 * sent. Handle and A ? rubout. Can't get at the re-directed stdin.
 */
char *gets(p)
char *p;
{
    register unsigned char *c = p;
    register unsigned char in;

    while ((in = Cnecin ()) != '\n' && in != '\r' && in != 26)
        if (in == '\b' || in == 0x7f) {
            if (c != p) {
                Cconout('\b'); Cconout(' '); Cconout('\b');
                —c;
            }
            else
                Cconout(*c++ = in);
            
            Cconout('\r'); Cconout('\n');
            *c = '\0';

    return (in = 26) ? NULL : p;
}

#endif