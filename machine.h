/*
 * define AMIGA=1 to include AMIGA specific files
 * define ATARI_ST=1 to include ATARI ST specific files
 */

#define AMIGA       1
#define ATAR_ST     0

#if AMIGA
    #include "machine_amiga.h"
#endif

#ifdef ATARI_ST
    #include "machine_atarist.h"
#endif

