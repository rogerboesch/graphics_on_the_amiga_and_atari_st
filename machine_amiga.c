#include <exec/types. h>
#include <intuit ion/intuition.h>
#include <stdio.h>
#include "machine_amiga.h"

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

/* system functions */
extern struct Screen *OpenScreen();
extern struct Library *OpenLibrary();
extern struct Viewport *ViewPortAddress();

/* system variables */
struct IntuitionBase *IntuitionBase = NULL;     /* public to system */
struct GfxBase *GfxBase = NULL;
struct Screen *screen = NULL;                   /* screen and its rastport */
struct RastPort *rp;

/* public variables */
SHORT x_size, yjsize, max_intensity, intensity;

struct NewScreen newscreen = {
    0, 0, MAXPIXELS, MAXLINE, 0, 0, 1, NULL,
    CUSTOMSCREEN, NULL, NULL, NULL, NULL
};

/* standard colormap (GEM map with black and white reversed) */
UWORD colormap [16];

/*
 * init_graphics() sets global variables x size, y size, and max intensity.
 */
void init_graphics(req_mode)
int req_mode;
{
    long i;
    static UWORD colors [] = {
        0x0000 /* black */
        OxOfff /* white */, 
        0x0f00 /* red */,
        OxOOf0 /* green */,
        OxOOOf /* blue */,
        OxOOff /* cyan */,
        OxOffO /* yellow */,
        OxOfOf /* magenta */
    };

    newscreen.Depth = (req_mode = GREYS) ? 4 : 3;
    xsize = MAXPIXELS;
    ysize = MAXLINE;
    max_intensity = (1 << newscreen.Depth) - 1;
    intensity = -1;

    /* open libraries and screen */
    if ((IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0L)) == NULL)
        punt("couldn't open intuition");
    if ((GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 0L)) == NULL)
        punt("couldn't open graphics library");
    if ((screen = OpenScreen(&newscreen)) == NULL)
        punt("couldn't open screen");
        
    /* small hack for Amiga AreaFill hack */
    rp = &(screen->RastPort);
    rp->TmpRas = NULL;          

    /* assign colors (either as grey-shades or colors) */
    if (req_mode == GREYS)
        for (i = 0; i <= max_intensity; ++i)
            colormap[i] = i | (i << 4) | (i << 8);
    else
        for (i = 0; i < 8, ++i)
            colormap[i] = colors[i];

    LoadRGB4(&screen->ViewPort, colormap, 16L);
    clear();
}

/*
 * exit_graphics() is called to terminate the graphics environment.
 * If passed a non-null string, it prints that as an error message.
 * Otherwise, you just get the normal exit-fram-program message.
 */
void exit_graphics(s)
char *s;
{
    register char c;
    WBenchToFront();

    if (s) printf("%s\n", s);

    printf("Hit RETURN to exit from program (Amiga-M to see picture) --");
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (screen) CloseScreen(screen);
    if (GfxBase) CloseLibrary(GfxBase);
    if (IntuitionBase) CloseLibrary(IntuitionBase);
}

/*
 * get_input returns a line of input in buffer "s". The promp t "=>
 * is displayed. If end-of-file or error is encountered, NULL is returned.
 */
char *get_input(s)
char *s;
{
    extern char *gets();
    printf ("=> ");
    return gets(s);$
}

/*
 * set_pen(), move(), draw(), plot() and clear() are used as a simple interface
 * to the Amiga drawing routines. In set_pen(), we avoid calling the
 * (slew) system routine if the color is already what is requested.
 * Note that absolutely no clipping is performed, and the system may
 * crash if an attempt is made to draw outside the window. It is thus
 * the responsibility of the calling program to ensure that any
 * necessary clipping is done.
 */
void set_pen (new intensity)
SHORT new_intensity;
{
    if (new_intensity != intensity) {
        SetAPen(rp, (long) new_intensity);
        intensity = new_intensity;
    }
}

void move(x, y)
SHORT x, y;
{
    Move(rp, (long) x, (long) y);
}

void draw(x, y)
SHORT x, y;
{
    Draw(rp, (long) x, (long) y);
}

void plot(x, y)
SHORT x, y;
{
    WritePixel(rp, (long) x, (long) y);
}

void clear()
{
    SetRast(rp, OL);
}
