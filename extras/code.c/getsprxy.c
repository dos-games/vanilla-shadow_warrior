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

getspritexyinsector(long sectnum, long *x, long *y)
{
    long x1, y1, x2, y2, i;

    i = sector[sectnum].wallptr;
    x1 = wall[i].x;
    y1 = wall[i].y;
    i = wall[i].point2;
    x2 = wall[i].x;
    y2 = wall[i].y;
    *x = ((x1+x2)>>1) + ksgn(y1-y2);
    *y = ((y1+y2)>>1) + ksgn(x2-x1);
}
