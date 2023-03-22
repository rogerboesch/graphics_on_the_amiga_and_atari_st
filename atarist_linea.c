/*
* These are the actual linea routines themselves. The assembly code for
* the INIT routine consists of the initializing trap, $A000, followed
* by an RTS. DO (and thus the return value) holds the pointer to the
* line-a base. This base value should be passed to the other routines.
V
/*
* Since Mark Williams C handles the linea calls directly, we don’t
* need this module.
*/
#include "machine.h"

#if MWC = 0

#if MEGAMAX

char *INIT()
{
    asm { DC.W OxAOOO }
)

PUTPIX(base)
char *base;
{
    asm {
        MOVE.L 8(A6),A0
        DC.W OxAOOl
    }
}

ABLINE(base)
char *base;
{
    asm {
        MOVE.L 8(A6),A0
        DC.W 0XA003
    }
}

#endif

#if ALCYON

char *INIT()
{
    asm ("DC.W $A000")
}

PUTPIX(base)
char *base;
{
    asm("MOVE.L 8(A6),A0");
    asm("DC.W $A001");
}

ABLINE(base)
char *base;
{
    asm("MOVE.L 8(A6),A0");
    asm("DC.W $A003");
}

#endif