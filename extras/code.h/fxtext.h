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

#ifndef __TEXT_H__
#define __TEXT_H__

#define CH00 0x00000000
#define CH01 0xFFFF0000
#define CH10 0x0000FFFF
#define CH11 0xFFFFFFFF

FxU32 txtNum[100] = 
{ 
        CH01, CH10, CH10, CH01, CH10, CH01, CH10, CH01, CH01, CH10, // 0
        CH01, CH10, CH00, CH10, CH00, CH10, CH00, CH10, CH01, CH11, // 1
        CH11, CH10, CH00, CH01, CH01, CH10, CH10, CH00, CH01, CH11, // 2
        CH11, CH10, CH00, CH01, CH01, CH10, CH00, CH01, CH11, CH10, // 3
        CH10, CH01, CH10, CH01, CH01, CH11, CH00, CH01, CH00, CH01, // 4
        CH01, CH11, CH10, CH00, CH01, CH10, CH00, CH01, CH11, CH10, // 5
        CH10, CH00, CH10, CH00, CH11, CH10, CH10, CH01, CH01, CH10, // 6
        CH11, CH11, CH00, CH01, CH00, CH01, CH00, CH10, CH00, CH10, // 7
        CH01, CH10, CH10, CH01, CH01, CH10, CH10, CH01, CH01, CH10, // 8
        CH01, CH10, CH10, CH01, CH01, CH11, CH00, CH01, CH00, CH01  // 9
};

FxU32 txtChar[160+100+70+260] = 
{ 

        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH11, CH11, CH00, CH00, CH00, CH00, // -
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH11, CH00, // .
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?


        CH01, CH10, CH10, CH01, CH10, CH01, CH10, CH01, CH01, CH10, // 0
        CH01, CH10, CH00, CH10, CH00, CH10, CH00, CH10, CH01, CH11, // 1
        CH11, CH10, CH00, CH01, CH01, CH10, CH10, CH00, CH01, CH11, // 2
        CH11, CH10, CH00, CH01, CH01, CH10, CH00, CH01, CH11, CH10, // 3
        CH10, CH01, CH10, CH01, CH01, CH11, CH00, CH01, CH00, CH01, // 4
        CH01, CH11, CH10, CH00, CH01, CH10, CH00, CH01, CH11, CH10, // 5
        CH10, CH00, CH10, CH00, CH11, CH10, CH10, CH01, CH01, CH10, // 6
        CH11, CH11, CH00, CH01, CH00, CH01, CH00, CH10, CH00, CH10, // 7
        CH01, CH10, CH10, CH01, CH01, CH10, CH10, CH01, CH01, CH10, // 8
        CH01, CH10, CH10, CH01, CH01, CH11, CH00, CH01, CH00, CH01, // 9

        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?
        CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, CH00, // ?

        CH01, CH10, CH10, CH01, CH11, CH11, CH10, CH01, CH10, CH01, // A
        CH11, CH10, CH10, CH01, CH11, CH10, CH10, CH01, CH11, CH10, // B
        CH01, CH10, CH10, CH01, CH10, CH00, CH10, CH01, CH01, CH10, // C
        CH11, CH10, CH10, CH01, CH10, CH01, CH10, CH01, CH11, CH10, // D
        CH11, CH11, CH10, CH00, CH11, CH10, CH10, CH00, CH11, CH11, // E
        CH11, CH11, CH10, CH00, CH11, CH10, CH10, CH00, CH10, CH00, // F
        CH01, CH10, CH10, CH00, CH10, CH11, CH10, CH01, CH01, CH10, // G
        CH10, CH01, CH10, CH01, CH11, CH11, CH10, CH01, CH10, CH01, // H
        CH11, CH10, CH01, CH00, CH01, CH00, CH01, CH00, CH11, CH10, // I
        CH00, CH11, CH00, CH01, CH00, CH01, CH10, CH01, CH01, CH10, // J
        CH10, CH01, CH10, CH10, CH11, CH00, CH10, CH10, CH10, CH01, // K
        CH10, CH00, CH10, CH00, CH10, CH00, CH10, CH00, CH11, CH11, // L
        CH10, CH01, CH11, CH11, CH11, CH11, CH10, CH01, CH10, CH01, // M
        CH10, CH01, CH11, CH01, CH11, CH01, CH10, CH11, CH10, CH01, // N
        CH01, CH10, CH10, CH01, CH10, CH01, CH10, CH01, CH01, CH10, // O
        CH11, CH10, CH10, CH01, CH11, CH10, CH10, CH00, CH10, CH00, // P
        CH01, CH10, CH10, CH01, CH10, CH01, CH10, CH11, CH01, CH11, // Q
        CH11, CH10, CH10, CH01, CH11, CH10, CH10, CH01, CH10, CH01, // R
        CH01, CH11, CH10, CH00, CH01, CH10, CH00, CH01, CH11, CH10, // S
        CH11, CH10, CH01, CH00, CH01, CH00, CH01, CH00, CH01, CH00, // T
        CH10, CH01, CH10, CH01, CH10, CH01, CH10, CH01, CH01, CH10, // U
        CH10, CH01, CH10, CH01, CH10, CH01, CH10, CH10, CH01, CH00, // V
        CH10, CH01, CH10, CH01, CH11, CH11, CH11, CH11, CH10, CH01, // W
        CH10, CH01, CH10, CH01, CH01, CH10, CH10, CH01, CH10, CH01, // X
        CH10, CH01, CH10, CH01, CH01, CH01, CH00, CH10, CH00, CH10, // Y
        CH11, CH10, CH00, CH01, CH01, CH10, CH10, CH00, CH01, CH11  // Z
};

#endif /* __TEXT_H__ */
