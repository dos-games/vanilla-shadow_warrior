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

int
LedgeAhead(short SpriteNum, short min_height)
    {

    SPRITEp sp = &sprite[SpriteNum];
    long dax, day;
    short newsector;

    // dax = sp->x + MOVEx(sp->xvel * synctics, sp->ang);
    // day = sp->y + MOVEy(sp->xvel * synctics, sp->ang);

    dax = sp->x + MOVEx(128, sp->ang);
    day = sp->y + MOVEy(128, sp->ang);

    if (!inside(dax, day, sp->sectnum))
        {

        newsector = sp->sectnum;

        updatesector(dax, day, &newsector);

        #if 0
        if (TEST(sector[newsector].extra, SECTFX_SPECIAL))
            {
            SECT_USERp su = SectUser[newsector];

            ASSERT(su);
            
            switch (su->special)
                {
                case SPECIAL_BLOCK_ACTOR:
                    return (TRUE);
                    break;
                }
            }
        #endif    

        if (labs(sector[newsector].floorz - sector[sp->sectnum].floorz) > min_height)
            {
            return (TRUE);
            }
        }

    return (FALSE);
    }

int
move_actor(short SpriteNum, long xchange, long ychange, long zchange)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;

    long x, y, z;
    short sp_sect, orig_sect, dist, LedgeHeight;

    short ret;

    // Look out in front for some sort of ledge

    if (!TEST(u->Flags, SPR_JUMPING | SPR_FALLING))
        {
        if (TEST(u->Flags, SPR_DEATH))
            LedgeHeight = 0;
        else
            LedgeHeight = Z(20);


        if (LedgeAhead(SpriteNum, LedgeHeight))
            {
            return (FALSE);
            }
        }

    // save off x,y values
    x = sp->x;
    y = sp->y;

    // move z up off of the floor
    z = sp->z - Z(16);

    orig_sect = sp_sect = sp->sectnum;

    // scale by synctics
    xchange *= synctics;
    ychange *= synctics;
    zchange *= synctics;

    // some shifting value Ken put in to maintain compatibility
    xchange <<= 11;
    ychange <<= 11;

    ret = clipmove(&x, &y, &z, &sp_sect, xchange, ychange, walldist, u->ceiling_dist, u->floor_dist, 0);
//    ret = clipmove(&x, &y, &z, &sp_sect, xchange, ychange, sp->clipdist<<2, u->ceiling_dist, u->floor_dist, 0);

    if (!ret)
        {
        // Keep track of how far sprite has moved
        dist = Distance(x, y, sp->x, sp->y);
        u->Dist += dist;
        u->ZigZagDist += dist;

        // Place sprite in new position
        setsprite(SpriteNum, x, y, sp->z);

        return (TRUE);
        }
    else
        {
        return (FALSE);
        }

    }

