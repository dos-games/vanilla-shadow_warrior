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

#undef SpriteInDiveArea
#undef SpriteInUnderwaterArea
#undef SectorIsUnderwaterArea

BOOL
SpriteInDiveArea(SPRITEp sp)
    {
    USERp u = User[sp - sprite];
    short i, nexti;
    SECT_USERp sectu = SectUser[sp->sectnum];
    SECTORp sectp = &sector[sp->sectnum];
    SPRITEp under_sp, over_sp;
    
    if (!TEST(sectp->extra, SECTFX_DIVE_AREA))
        return(FALSE);

    if (sectu->number == 0)
        return(FALSE);
            
    // search for DIVE_AREA "over" sprite for reference point
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_DIVE_AREA], i, nexti)
        {
        over_sp = &sprite[i];

        if (TEST(sector[over_sp->sectnum].extra, SECTFX_DIVE_AREA) &&
            SectUser[over_sp->sectnum] &&
            SectUser[over_sp->sectnum]->number == sectu->number)
            {
            return(TRUE);
            }
        }
        
    return(FALSE);    
    }    
    
BOOL
SpriteInUnderwaterArea(SPRITEp sp)
    {
    USERp u = User[sp - sprite];
    short i, nexti;
    SECT_USERp sectu = SectUser[sp->sectnum];
    SECTORp sectp = &sector[sp->sectnum];
    SPRITEp under_sp, over_sp;
    char Found = FALSE;

    if (!TEST(sectp->extra, SECTFX_UNDERWATER))
        return(FALSE);
    
    //DSPRINTF(ds,"%d",sectu->number);
    MONO_PRINT(ds);

    if (sectu->number == 0)
        return(FALSE);
        
    // search for UNDERWATER "under" sprite for reference point
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_UNDERWATER], i, nexti)
        {
        under_sp = &sprite[i];

        if (TEST(sector[under_sp->sectnum].extra, SECTFX_UNDERWATER) &&
            SectUser[under_sp->sectnum] &&
            SectUser[under_sp->sectnum]->number == sectu->number)
            {
            return(TRUE);
            }
        }
        
    return(FALSE);    
    }    

BOOL
SectorIsUnderwaterArea(short sectnum)
    {
    short i, nexti;
    SECT_USERp sectu = SectUser[sectnum];
    SECTORp sectp = &sector[sectnum];
    SPRITEp under_sp, over_sp;
    char Found = FALSE;

    if (!TEST(sectp->extra, SECTFX_UNDERWATER|SECTFX_UNDERWATER2))
        return(FALSE);
    
    if (!sectu || sectu->number == 0)
        return(FALSE);
        
    return(TRUE);    
    }    
