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

// Bit of code where I've tried lots of floor and ceiling distances

    #define CF_DIST 1
    
    #if CF_DIST == 1
    u->floor_dist = Z(4);
    u->ceiling_dist = Z(8);
    #endif

    #if CF_DIST == 2
    u->floor_dist = u->ceiling_dist = DIV2(Z(tilesizy[state->Pic])) - Z(16);
    #endif

    #if CF_DIST == 3
    u->floor_dist = u->ceiling_dist = DIV2(Z(tilesizy[state->Pic]));
    #endif

    #if CF_DIST == 4
    u->floor_dist = DIV2(Z(tilesizy[state->Pic])) - Z(16);
    u->ceiling_dist = DIV2(Z(tilesizy[state->Pic]));
    #endif


VOID
DoActorZrange(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];
    long ceilhit, florhit;
    short save_cstat;

    save_cstat = TEST(sp->cstat, CSTAT_SPRITE_BLOCK);
    RESET(sp->cstat, CSTAT_SPRITE_BLOCK);
    getzrange(sp->x, sp->y, sp->z - DIV2(SPRITEp_SIZE_Z(sp)), sp->sectnum, &u->hiz, &ceilhit, &u->loz, &florhit, sp->clipdist << 2, 0);
    SET(sp->cstat, save_cstat);

    u->lo_sectp = u->lo_sp = u->hi_sectp = u->hi_sp = NULL;
    
    switch (TEST(ceilhit, BIT(14)|BIT(15)))
        {
        case BIT(14)|BIT(15):
            u->hi_sp = &sprite[ceilhit & 4095];
            break;
        default:
            u->hi_sectp = &sector[ceilhit & 4095];
            break;
        }

    switch (TEST(florhit, BIT(14)|BIT(15)))
        {
        case BIT(14)|BIT(15):
            u->lo_sp = &sprite[florhit & 4095];
            break;
        default:
            u->lo_sectp = &sector[florhit & 4095];
            break;
        }
    }
    
int 
move_sprite(short SpriteNum, long xchange, long ychange, long zchange)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;
    long x, y;
    short dist;
    char jump_fall,cliptype;
    
    jump_fall = TEST(u->Flags,SPR_JUMPING|SPR_FALLING);
    
    if (jump_fall)
        {
        scaredfallz <<= 4;
        }
    
    // save off x,y values
    x = sp->x;
    y = sp->y;
    
    cliptype = 2;
    if (TEST(u->Flags, SPR_NO_SCAREDZ))
        cliptype = 0;
    
    u->ret = movesprite(SpriteNum, xchange, ychange, zchange, u->ceiling_dist, u->floor_dist, cliptype, synctics);
    
    ASSERT(sp->sectnum >= 0);

    if (jump_fall)
        {
        scaredfallz >>= 4;
        }
    
    DoActorZrange(SpriteNum);
        
    if (u->ret == 0)
        {
        // Keep track of how far sprite has moved
        dist = Distance(x, y, sp->x, sp->y);
        u->Dist += dist;
        return(TRUE);
        }
    else
        return(FALSE);
    
    }

// shortened version of my routine but basically the same core logic
    
int 
DoActorMoveCloser(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;
    long nx, ny;

    nx = sp->xvel * (long) sintable[NORM_ANGLE(sp->ang + 512)] >> 14;
    ny = sp->xvel * (long) sintable[sp->ang] >> 14;

    // if cannot move the sprite
    if (!move_sprite(SpriteNum, nx, ny, 0L))
        {
        DebugMoveHit(SpriteNum);        
        
        DoActorCantMoveCloser(SpriteNum);
        
        return (0);
        }

    return (0);

    }
    
VOID DebugMoveHit(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    extern BOOL DebugActor;
    
    switch (TEST(u->ret, BIT(14)|BIT(15)))
        {
        case BIT(14)|BIT(15):
            sprintf(ds, "Hit a Sprite %d  ", u->ret & 4095);
            break;
        case BIT(15):
            sprintf(ds, "Hit a Wall %d    ", u->ret & 8191);
            break;
        default:
            sprintf(ds, "Hit a Sector %d  ", u->ret & 4095);
            break;
        }
    
    if (DebugActor)
        printext(0, 200-8, ds, ALPHABET, 80);
        
    MONO_PRINT(ds);    
    }                                         
    
    