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

#include <i86.h>

union  REGS  regs;
struct SREGS sregs;

/* Palette Color Codes
bit #   Color EGA Monitor   Color RGBI Monitor
==============================================
5       Secondary Red         ----
4       Secondary Green       Intensity
3       Secondary Blue        ----
2       Red                   Red
1       Green                 Green
0       Blue                  Blue
*/

// Set code to 0 for black
void SetOverScan(char code)
{
    regs.h.ah = 10;
    regs.h.al = 1
    regs.h.bh = code;

    #ifdef __386__
        int386( 0x10, &regs, &regs );
    #else
        int86( 0x10, &regs, &regs );
    #endif
}