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

#if 0	  
int
DoDeathItem(SHORT Weapon)
    {
    SPRITEp sp = &sprite[Weapon];
    USERp u = User[Weapon];
    LONG dax, day, daz;

    if (TEST(u->Flags, SPR_UNDERWATER))
        {
        ScaleSpriteVector(Weapon, 50000);
        
        u->Counter += 20;
        u->zchange += u->Counter;
        }
    else
        {
        u->Counter += 20;
        u->zchange += u->Counter;
        }    
    
    u->ret = move_missile(Weapon, u->xchange, u->ychange, u->zchange, 
        u->ceiling_dist, u->floor_dist, CLIPMASK_MISSILE, MISSILEMOVETICS);

    WeaponHitDiveArea(Weapon);

    if (u->ret)
        {
        switch (TEST(u->ret, HIT_MASK))
            {
        case HIT_PLAX_WALL:
            KillSprite(Weapon);
            return(TRUE);
        case HIT_SPRITE:
                {
                short wall_ang, dang;
                short hitsprite = -2;
                SPRITEp hsp;
                
                PlaySound(DIGI_CALTROPS, &sp->x, &sp->y, &sp->z, v3df_pan);
                
                hitsprite = NORM_SPRITE(u->ret);
                hsp = &sprite[hitsprite];
                
                if (TEST(hsp->cstat, CSTAT_SPRITE_WALL))
                    {
                    wall_ang = NORM_ANGLE(hsp->ang);
                    WallBounce(Weapon, wall_ang);
                    ScaleSpriteVector(Weapon, 10000);
                    }
                else
                    {
					u->xchange = u->ychange = 0;
				    u->WaitTics -= (MISSILEMOVETICS * 2);
					if(u->WaitTics <= 0)
                        KillSprite((short) Weapon);
					// DoGet
                    return (TRUE);
                    }    
                    
                    
                break;
                }
                
        case HIT_WALL:
                {
                short hitwall,nw,wall_ang,dang;
                WALLp wph;

                hitwall = NORM_WALL(u->ret);
                wph = &wall[hitwall];
                
                if (wph->lotag == TAG_WALL_BREAK)
                    {
                    HitBreakWall(wph, sp->x, sp->y, sp->z, sp->ang, u->ID);
                    u->ret = 0;
                    break;
                    }
                
                PlaySound(DIGI_CALTROPS, &sp->x, &sp->y, &sp->z, v3df_pan);
                
                nw = wall[hitwall].point2;
                wall_ang = NORM_ANGLE(getangle(wall[nw].x - wph->x, wall[nw].y - wph->y)+512);
                
                WallBounce(Weapon, wall_ang);
                ScaleSpriteVector(Weapon, 1000);
                break;
                }
                
        case HIT_SECTOR:
                {
                BOOL hitwall;
                if (SlopeBounce(Weapon, &hitwall))
                    {
                    if (hitwall)
                        {
                        // hit a wall
                        ScaleSpriteVector(Weapon, 1000);
                        u->ret = 0;
                        u->Counter = 0;
                        }
                    else
                        {
                        // hit a sector
                    	if (sp->z > DIV2(u->hiz + u->loz)) 
                            {
                            // hit a floor
                            if (!TEST(u->Flags, SPR_BOUNCE))
                                {
				                PlaySound(DIGI_CALTROPS, &sp->x, &sp->y, &sp->z, v3df_pan);
                                SET(u->Flags, SPR_BOUNCE);
                                ScaleSpriteVector(Weapon, 1000); // was 18000
                                u->ret = 0;
                                u->Counter = 0;
                                }
                            else
                                {
								u->xchange = u->ychange = 0;
							    u->WaitTics -= (MISSILEMOVETICS * 2);
								if(u->WaitTics <= 0)
		                            KillSprite((short) Weapon);
								// DoGet
                                return (TRUE);
                                }
                            }
                        else
                            {
                            // hit a ceiling
                            ScaleSpriteVector(Weapon, 1000); // was 22000
                            }    
                        }    
                    }
                else
                    {    
                    // hit floor
                   	if (sp->z > DIV2(u->hiz + u->loz)) 
                        {
                        if (TEST(u->Flags, SPR_UNDERWATER))
                            SET(u->Flags, SPR_BOUNCE); // no bouncing underwater
                            
                        if (u->lo_sectp && SectUser[sp->sectnum] && SectUser[sp->sectnum]->depth)
                            SET(u->Flags, SPR_BOUNCE); // no bouncing on shallow water
                                
                        if (!TEST(u->Flags, SPR_BOUNCE))
                            {
			                PlaySound(DIGI_CALTROPS, &sp->x, &sp->y, &sp->z, v3df_pan);
                            SET(u->Flags, SPR_BOUNCE);
                            u->ret = 0;
                            u->Counter = 0;
                            u->zchange = -u->zchange;
                            ScaleSpriteVector(Weapon, 1000); // Was 18000
                            }
                        else
                            {
							u->xchange = u->ychange = 0;
						    u->WaitTics -= (MISSILEMOVETICS * 2);
							if(u->WaitTics <= 0)
	                            KillSprite((short) Weapon);
							// DoGet
                            return (TRUE);
                            }
                        }
                    else
                    // hit something above
                        {
                        u->zchange = -u->zchange;
                        ScaleSpriteVector(Weapon, 1000); // was 22000
                        }        
                    }
                break;    
                }
            }    
        }

    return (FALSE);
    }
#endif

/*

Make Dead Actor caugh up an item

*/

VOID
DoActorSpawnIcon(SHORT SpriteNum)
{
    USERp u = User[SpriteNum];
    SPRITEp sp = &sprite[SpriteNum];
    USERp wu;
    SPRITEp wp;
    long nx, ny, nz;
    short w, hitsprite;
    short oclipdist,i;
    long dist;
	int item;
	STATEp state;


	if(RANDOM_P2(1024) < 900) return; // Random chance it will spawn, else return

	if(!u) return;

	switch(u->ID)
	{    
		case NINJA_RUN_R0:
			item = ICON_UZI;
			state = s_IconUziFloor;
		break;
		case RIPPER_RUN_R0:
		case RIPPER2_RUN_R0:
			item = ICON_HEART;
			state = s_IconHeart;
		break;
		default:
			return;
	}

	PlaySound(DIGI_THROW,&sp->x,&sp->y,&sp->z,v3df_pan|v3df_doppler);

    nx = sp->x;
    ny = sp->y;
    nz = sp->z;

    // Inserting and setting up variables
    w = SpawnSprite(STAT_MISSILE, item, state, sp->sectnum, 
        nx, ny, nz, sp->ang, (CHEMBOMB_VELOCITY+RANDOM_RANGE(CHEMBOMB_VELOCITY))/3);
    
	wp = &sprite[w];
    wu = User[w];
    
	// don't throw it as far if crawling
    if (TEST(pp->Flags, PF_CRAWLING))
       {
       wp->xvel -= DIV4(wp->xvel);
       }
    
    SET(wu->Flags, SPR_XFLIP_TOGGLE);
    
    SetOwner(SpriteNum, w);  
    wp->yrepeat = 64;
    wp->xrepeat = 64;
    wp->shade = -15;
    wu->WeaponNum = u->WeaponNum;
    wu->Radius = 200;
    wu->ceiling_dist = Z(3);
    wu->floor_dist = Z(3);
    wu->Counter = 0;
    
	if (TEST(pp->Flags, PF_DIVING) || SpriteInUnderwaterArea(wp))
        SET(wu->Flags, SPR_UNDERWATER);
    

	wp->zvel = 100 - (30 + RANDOM_RANGE(15));
    
    wp->clipdist = 0;
    MissileSetPos(w, DoDeathItem, 1000);
    wp->clipdist = 80L>>2;
    
	wu->xchange = MOVEx(wp->xvel, wp->ang);
    wu->ychange = MOVEy(wp->xvel, wp->ang);
    wu->zchange = wp->zvel>>1;
    
    // adjust xvel according to player velocity
    wu->xchange += sp->xvect>>14;
    wu->ychange += sp->yvect>>14;
    
	// Items stay around for this many seconds
	wu->WaitTics = CHEMTICS*5;
    
    return (0);
}
