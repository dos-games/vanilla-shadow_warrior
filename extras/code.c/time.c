//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2005 - 3D Realms Entertainment

This file is NOT part of Shadow Warrior version 1.2
However, it is either an older version of a file that is, or is
some test code written during the development of Shadow Warrior.
This file is provided purely for educational interest.

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#include "mytypes.h"
#include <dos.h>
#include <conio.h>

static VOID interrupt(*oldclk) (VOID);  /* Old Clock interrupt */
static VOID interrupt(*oldbreak) (VOID);/* Old Clock interrupt */
static WORD ChainCounter,               /* Current counter value */
 factor;

static VOID FN TimerUpdate(VOID);

/*
VOID fFN f_dos_setvect(unsigned interruptno, void interrupt (*isr) ());
VOID interrupt(far *f_dos_getvect(unsigned interruptno)) ();
*/

ULONG TimerTics = 0;
WORD TicAmt = 1;
WORD TimerCounter;
WORD AnimCounter;

#define PICCMD  0x20                    /* 8259 PIC command port */
#define EOI     0x20                    /* EOI command */

#define TCTRL   0x43                    /* Timer control port */
#define T0DATA  0x40                    /* Timer 0 data port */
#define T0LOAD  0x36                    /* Timer 0 load command */

/****** _spdup is an interrupt handler which processes clock interrupts which
 have been sped up. *****/


static VOID interrupt far 
_spdup(VOID)
    {

    TimerTics += TicAmt;

    /* if ChainCounter has counted down to 0, time to update system clock */
    /* at 18.2 hz and reset ChainCounter */

    if (ChainCounter--)
        {
        outportb(PICCMD, EOI);          /* Send EOI and return */
        }
    else
        {
        ChainCounter = factor;          /* Otherwise reset ChainCounter */
        _chain_intr(oldclk);            /* and exec old handler */
        }
    }

#pragma option -O2

VOID 
speedup(WORD newfact)
    {
    WORD divider;

    if (newfact <= 1)                   /* Reset timer handler */
        {
        if (oldclk)                     /* reset only if set */
            {
            _dos_setvect(0x08, oldclk); /* in the first place */
            oldclk = 0;
            }
        }
    else
        {
        if (!oldclk)
            oldclk = _dos_getvect(0x08);/* Save old handler */
            
        _dos_setvect(0x08, _spdup);     /* use our handler */
        }
    }
