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

/*
Here is a sample function I made for Frank -

 It is untested code that is supposed to work like getzrange, but only search
 floor sprites in only the sector passed to it.It is just a special
case optimization of getzrange.If you think getzrange is fast enough
 then you can just ignore this.
*/
getzrangepoint(long x, long y, long z, short sectnum,
    long *ceilz, long *ceilhit, long *florz, long *florhit)
    {
    spritetype *spr;
    long i, j, k, l, dax, day, daz, xspan, yspan, xoff, yoff;
    long x1, y1, x2, y2, x3, y3, x4, y4, cosang, sinang, tilenum;
    short cstat;
    char clipyou;

    if (sectnum < 0)
        {
        *ceilz = 0x80000000;
        *ceilhit = -1;
        *florz = 0x7fffffff;
        *florhit = -1;
        return;
        }

    // Initialize z's and hits to the current sector's top&bottom
    *ceilz = sector[sectnum].ceilingz;
    *ceilhit = sectnum + 16384;
    *florz = sector[sectnum].floorz;
    *florhit = sectnum + 16384;

    // Go through sprites of only the current sector
    for (j = headspritesect[sectnum]; j >= 0; j = nextspritesect[j])
        {
        spr = &sprite[j];
        cstat = spr->cstat;
        if ((cstat & 49) != 33)
            continue;                   // Only check blocking floor sprites

        daz = spr->z;

        // Only check if sprite's 2-sided or your on the 1-sided side
        if (((cstat & 64) != 0) && ((z > daz) == ((cstat & 8) == 0)))
            continue;

        // Calculate and store centering offset information into xoff&yoff
        tilenum = spr->picnum;
        xoff = (long) ((signed char) ((picanm[tilenum] >> 8) & 255)) + ((long) spr->xoffset);
        yoff = (long) ((signed char) ((picanm[tilenum] >> 16) & 255)) + ((long) spr->yoffset);
        if (cstat & 4)
            xoff = -xoff;
        if (cstat & 8)
            yoff = -yoff;

        // Calculate all 4 points of the floor sprite.
        // (x1,y1),(x2,y2),(x3,y3),(x4,y4)
        // These points will already have (x,y) subtracted from them
        cosang = sintable[(spr->ang + 512) & 2047];
        sinang = sintable[spr->ang];
        xspan = tilesizx[tilenum];
        dax = ((xspan >> 1) + xoff) * spr->xrepeat;
        yspan = tilesizy[tilenum];
        day = ((yspan >> 1) + yoff) * spr->yrepeat;
        x1 = spr->x + dmulscale16(sinang, dax, cosang, day) - x;
        y1 = spr->y + dmulscale16(sinang, day, -cosang, dax) - y;
        l = xspan * spr->xrepeat;
        x2 = x1 - mulscale16(sinang, l);
        y2 = y1 + mulscale16(cosang, l);
        l = yspan * spr->yrepeat;
        k = -mulscale16(cosang, l);
        x3 = x2 + k;
        x4 = x1 + k;
        k = -mulscale16(sinang, l);
        y3 = y2 + k;
        y4 = y1 + k;

        // Check to see if point (0,0) is inside the 4 points by seeing if
        // the number of lines crossed as a line is shot outward is odd
        clipyou = 0;
        if ((y1 ^ y2) < 0)              // If y1 and y2 have different signs
            // (- / +)
            {
            if ((x1 ^ x2) < 0)
                clipyou ^= (x1 * y2 < x2 * y1) ^ (y1 < y2);
            else if (x1 >= 0)
                clipyou ^= 1;
            }
        if ((y2 ^ y3) < 0)
            {
            if ((x2 ^ x3) < 0)
                clipyou ^= (x2 * y3 < x3 * y2) ^ (y2 < y3);
            else if (x2 >= 0)
                clipyou ^= 1;
            }
        if ((y3 ^ y4) < 0)
            {
            if ((x3 ^ x4) < 0)
                clipyou ^= (x3 * y4 < x4 * y3) ^ (y3 < y4);
            else if (x3 >= 0)
                clipyou ^= 1;
            }
        if ((y4 ^ y1) < 0)
            {
            if ((x4 ^ x1) < 0)
                clipyou ^= (x4 * y1 < x1 * y4) ^ (y4 < y1);
            else if (x4 >= 0)
                clipyou ^= 1;
            }
        if (clipyou == 0)
            continue;                   // Point is not inside, don't clip

        // Clipping time!
        if (z > daz)
            {
            if (daz > *ceilz)
                {
                *ceilz = daz;
                *ceilhit = j + 49152;
                }
            }
        else
            {
            if (daz < *florz)
                {
                *florz = daz;
                *florhit = j + 49152;
                }
            }
        }
    }
