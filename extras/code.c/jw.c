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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "mathutil.h"
#include "break.h"
#include "quake.h"
#include "net.h"
#include "pal.h"

#include "def.h"
#include "ai.h"
#include "weapon.h"

ANIMATOR NullAnimator;

ANIMATOR DoPuff;
extern STATE s_Puff[];

#define CHEMTICS SEC(40)

#define MUSHROOM_RATE 30

STATE s_NukeMushroom[] =
    {
    {RADIATION_CLOUD + 0, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[1]},
    {RADIATION_CLOUD + 1, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[2]},
    {RADIATION_CLOUD + 2, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[3]},
    {RADIATION_CLOUD + 3, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[4]},
    {RADIATION_CLOUD + 4, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[5]},
    {RADIATION_CLOUD + 5, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[6]},
    {RADIATION_CLOUD + 6, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[7]},
    {RADIATION_CLOUD + 7, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[8]},
    {RADIATION_CLOUD + 8, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[9]},
    {RADIATION_CLOUD + 9, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[10]},
    {RADIATION_CLOUD +10, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[11]},
    {RADIATION_CLOUD +11, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[12]},
    {RADIATION_CLOUD +12, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[13]},
    {RADIATION_CLOUD +13, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[14]},
    {RADIATION_CLOUD +14, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[15]},
    {RADIATION_CLOUD +15, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[16]},
    {RADIATION_CLOUD +16, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[17]},
    {RADIATION_CLOUD +17, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[18]},
    {RADIATION_CLOUD +18, MUSHROOM_RATE, NullAnimator, &s_NukeMushroom[19]},
    {RADIATION_CLOUD +19, 100,           DoSuicide,    &s_NukeMushroom[0]},
    };

ANIMATOR DoRadiationCloud;
#define RADIATION_RATE 16

STATE s_RadiationCloud[] =
    {
    {RADIATION_CLOUD + 0, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[1]},
    {RADIATION_CLOUD + 1, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[2]},
    {RADIATION_CLOUD + 2, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[3]},
    {RADIATION_CLOUD + 3, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[4]},
    {RADIATION_CLOUD + 4, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[5]},
    {RADIATION_CLOUD + 5, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[6]},
    {RADIATION_CLOUD + 6, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[7]},
    {RADIATION_CLOUD + 7, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[8]},
    {RADIATION_CLOUD + 8, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[9]},
    {RADIATION_CLOUD + 9, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[10]},
    {RADIATION_CLOUD +10, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[11]},
    {RADIATION_CLOUD +11, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[12]},
    {RADIATION_CLOUD +12, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[13]},
    {RADIATION_CLOUD +13, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[14]},
    {RADIATION_CLOUD +14, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[15]},
    {RADIATION_CLOUD +15, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[16]},
    {RADIATION_CLOUD +16, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[17]},
    {RADIATION_CLOUD +17, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[18]},
    {RADIATION_CLOUD +18, RADIATION_RATE, DoRadiationCloud, &s_RadiationCloud[19]},
    {RADIATION_CLOUD +19, 100,            DoSuicide,        &s_RadiationCloud[0]},
    };

#define CHEMBOMB_FRAMES 1
#define CHEMBOMB_R0 3038
#define CHEMBOMB_R1 CHEMBOMB_R0 + (CHEMBOMB_FRAMES * 1)
#define CHEMBOMB_R2 CHEMBOMB_R0 + (CHEMBOMB_FRAMES * 2)
#define CHEMBOMB_R3 CHEMBOMB_R0 + (CHEMBOMB_FRAMES * 3)
#define CHEMBOMB_R4 CHEMBOMB_R0 + (CHEMBOMB_FRAMES * 4)

#define CHEMBOMB CHEMBOMB_R0
#define CHEMBOMB_RATE 8
ANIMATOR DoChemBomb;
#if 1
STATE s_ChemBomb[5][1] =
    {
        {
        {CHEMBOMB_R0 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[0][0]},
        },
        {
        {CHEMBOMB_R1 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[1][0]},
        },
        {
        {CHEMBOMB_R2 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[2][0]},
        },
        {
        {CHEMBOMB_R3 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[3][0]},
        },
        {
        {CHEMBOMB_R4 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[4][0]},
        }
    };

STATEp sg_ChemBomb[] =
    {
    &s_ChemBomb[0][0],
    &s_ChemBomb[1][0],
    &s_ChemBomb[2][0],
    &s_ChemBomb[3][0],
    &s_ChemBomb[4][0]
    };

#else

STATE s_ChemBomb[5] =
    {
        {CHEMBOMB_R0 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[1]},
        {CHEMBOMB_R1 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[2]},
        {CHEMBOMB_R2 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[3]},
        {CHEMBOMB_R3 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[4]},
        {CHEMBOMB_R4 + 0, CHEMBOMB_RATE, DoChemBomb, &s_ChemBomb[0]},
    };

STATEp sg_ChemBomb[] =
    {
    &s_ChemBomb[0],
    &s_ChemBomb[1],
    &s_ChemBomb[2],
    &s_ChemBomb[3],
    &s_ChemBomb[4]
    };

#endif


int
DoChemBomb(SHORT Weapon)
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

    if (TEST(u->Flags, SPR_UNDERWATER) && (RANDOM_P2(1024 << 4) >> 4) < 256)
        SpawnBubble(Weapon);

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
                
                PlaySound(DIGI_CHEMBOUNCE, &sp->x, &sp->y, &sp->z, v3df_pan);
                
                hitsprite = NORM_SPRITE(u->ret);
                hsp = &sprite[hitsprite];
                
                if (TEST(hsp->cstat, CSTAT_SPRITE_WALL))
                    {
                    wall_ang = NORM_ANGLE(hsp->ang);
                    WallBounce(Weapon, wall_ang);
                    ScaleSpriteVector(Weapon, 32000);
                    }
                else
                    {
					u->xchange = u->ychange = 0;
				    u->WaitTics -= (MISSILEMOVETICS * 2);
					if(u->WaitTics <= 0)
                        KillSprite((short) Weapon);
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
                    HitBreakWall(wph, sp->x, sp->y, sp->z, sp->ang, 0);
                    u->ret = 0;
                    break;
                    }
                
                PlaySound(DIGI_CHEMBOUNCE, &sp->x, &sp->y, &sp->z, v3df_pan);
                
                nw = wall[hitwall].point2;
                wall_ang = NORM_ANGLE(getangle(wall[nw].x - wph->x, wall[nw].y - wph->y)+512);
                
                WallBounce(Weapon, wall_ang);
                ScaleSpriteVector(Weapon, 32000);
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
                        ScaleSpriteVector(Weapon, 28000);
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
				                PlaySound(DIGI_CHEMBOUNCE, &sp->x, &sp->y, &sp->z, v3df_pan);
                                SET(u->Flags, SPR_BOUNCE);
                                ScaleSpriteVector(Weapon, 32000); // was 18000
                                u->ret = 0;
                                u->Counter = 0;
                                }
                            else
                                {
								// Canister pops when first smoke starts out
								if(u->WaitTics == CHEMTICS && !TEST(sp->cstat, CSTAT_SPRITE_INVISIBLE))
								{
									PlaySound(DIGI_30MMFIRE,&sp->x,&sp->y,&sp->z,v3df_pan|v3df_doppler);
									PlaySound(DIGI_CHEMGAS,&sp->x,&sp->y,&sp->z,v3df_pan|v3df_doppler);
									Set3DSoundOwner(Weapon);
								}
                                SpawnRadiationCloud(Weapon);
								u->xchange = u->ychange = 0;
							    u->WaitTics -= (MISSILEMOVETICS * 2);
								if(u->WaitTics <= 0)
		                            KillSprite((short) Weapon);
                                return (TRUE);
                                }
                            }
                        else
                            {
                            // hit a ceiling
                            ScaleSpriteVector(Weapon, 32000); // was 22000
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
			                PlaySound(DIGI_CHEMBOUNCE, &sp->x, &sp->y, &sp->z, v3df_pan);
                            SET(u->Flags, SPR_BOUNCE);
                            u->ret = 0;
                            u->Counter = 0;
                            u->zchange = -u->zchange;
                            ScaleSpriteVector(Weapon, 32000); // Was 18000
                            }
                        else
                            {
							// Canister pops when first smoke starts out
							if(u->WaitTics == CHEMTICS && !TEST(sp->cstat, CSTAT_SPRITE_INVISIBLE))
							{
								PlaySound(DIGI_30MMFIRE,&sp->x,&sp->y,&sp->z,v3df_pan|v3df_doppler);
								PlaySound(DIGI_CHEMGAS,&sp->x,&sp->y,&sp->z,v3df_pan|v3df_doppler);
								Set3DSoundOwner(Weapon);
							}
                            //WeaponMoveHit(Weapon);
                            SpawnRadiationCloud(Weapon);
							u->xchange = u->ychange = 0;
						    u->WaitTics -= (MISSILEMOVETICS * 2);
							if(u->WaitTics <= 0)
	                            KillSprite((short) Weapon);
                            return (TRUE);
                            }
                        }
                    else
                    // hit something above
                        {
                        u->zchange = -u->zchange;
                        ScaleSpriteVector(Weapon, 32000); // was 22000
                        }        
                    }
                break;    
                }
            }    
        }

    // if you haven't bounced or your going slow do some puffs        
    if (!TEST(u->Flags, SPR_BOUNCE|SPR_UNDERWATER) && !TEST(sp->cstat, CSTAT_SPRITE_INVISIBLE))
        {
        SPRITEp np;
        USERp nu;
        short new;
        
        new = SpawnSprite(STAT_MISSILE, PUFF, s_Puff, sp->sectnum,
            sp->x, sp->y, sp->z, sp->ang, 100);

        np = &sprite[new];
        nu = User[new];

        SetOwner(Weapon, new);  
        np->shade = -40;
        np->xrepeat = 40;
        np->yrepeat = 40;
        nu->ox = u->ox;
        nu->oy = u->oy;
        nu->oz = u->oz;
        SET(np->cstat, CSTAT_SPRITE_YCENTER|CSTAT_SPRITE_TRANSLUCENT);
        RESET(np->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);

        nu->xchange = u->xchange;
        nu->ychange = u->ychange;
        nu->zchange = u->zchange;

		nu->spal = np->pal = PALETTE_PLAYER6;
        
        ScaleSpriteVector(new, 20000);
    
        if (TEST(u->Flags, SPR_UNDERWATER))
            SET(nu->Flags, SPR_UNDERWATER);
        }

    return (FALSE);
    }
    
/////////////////////////////
//
// Deadly green gas clouds
//
/////////////////////////////
int
SpawnRadiationCloud(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum],np;
    USERp u = User[SpriteNum],nu;
    short new;
    

    if (!MoveSkip4)
        return(FALSE);
	
	// This basically works like a MoveSkip8, if one existed
	u->Counter2 = !u->Counter2;
	if(!u->Counter2)
		return(FALSE);
    
    if (TEST(u->Flags, SPR_UNDERWATER))
        return(-1);

    new = SpawnSprite(STAT_MISSILE, RADIATION_CLOUD, s_RadiationCloud, sp->sectnum,
        sp->x, sp->y, sp->z - RANDOM_P2(Z(8)), sp->ang, 0);

    np = &sprite[new];
    nu = User[new];

    nu->WaitTics = 1*120;
    np->shade = -40;
    np->xrepeat = 32;
    np->yrepeat = 32;
    SET(np->cstat, CSTAT_SPRITE_YCENTER);
    RESET(np->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
	nu->spal = np->pal = PALETTE_PLAYER6;

    if (RANDOM_P2(1024) < 512)
        SET(np->cstat, CSTAT_SPRITE_XFLIP);
    if (RANDOM_P2(1024) < 512)
        SET(np->cstat, CSTAT_SPRITE_YFLIP);
        
    np->ang = RANDOM_P2(2048);
    np->xvel = RANDOM_P2(32);
    nu->xchange = MOVEx(np->xvel, np->ang);
    nu->ychange = MOVEy(np->xvel, np->ang);
    np->zvel = Z(4) + RANDOM_P2(Z(4));

	nu->Counter = 0;
	nu->Counter2 = 0;
	nu->Radius = 4000;
    
    return (FALSE);
    }

int
DoRadiationCloud(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];

    sp->z -= sp->zvel;
    
    sp->x += u->xchange;
    sp->y += u->ychange;

    if (u->ID)    
	{
        DoFlamesDamageTest(SpriteNum);    
	}

    return (FALSE);
    }

//////////////////////////////////////////////
//
// Inventory Chemical Bombs
//
//////////////////////////////////////////////
int
PlayerInitChemBomb(PLAYERp pp)
    {
    USERp u = User[pp->PlayerSprite];
    USERp wu;
    SPRITEp wp;
    long nx, ny, nz;
    short w, hitsprite;
    short oclipdist;
    long dist;
    

	PlaySound(DIGI_THROW,&pp->posx,&pp->posy,&pp->posz,v3df_pan|v3df_doppler);

    nx = pp->posx;
    ny = pp->posy;
    nz = pp->posz + pp->bob_z + Z(8);

    // Spawn a shot
    // Inserting and setting up variables
    w = SpawnSprite(STAT_MISSILE, CHEMBOMB, s_ChemBomb, pp->cursectnum, 
        nx, ny, nz, pp->pang, CHEMBOMB_VELOCITY);

    wp = &sprite[w];
    wu = User[w];

    // don't throw it as far if crawling
    if (TEST(pp->Flags, PF_CRAWLING))
       {
       wp->xvel -= DIV4(wp->xvel);
       }
    
    wu->RotNum = 5;
    NewStateGroup(w, &sg_ChemBomb);
    SET(wu->Flags, SPR_XFLIP_TOGGLE);
    
    SetOwner(pp->PlayerSprite, w);  
    wp->yrepeat = 32;
    wp->xrepeat = 32;
    wp->shade = -15;
    wu->WeaponNum = u->WeaponNum;
    wu->Radius = 200;
    wu->ceiling_dist = Z(3);
    wu->floor_dist = Z(3);
    wu->Counter = 0;
    SET(wp->cstat, CSTAT_SPRITE_YCENTER);
    SET(wp->cstat, CSTAT_SPRITE_BLOCK);

    if (TEST(pp->Flags, PF_DIVING) || SpriteInUnderwaterArea(wp))
        SET(wu->Flags, SPR_UNDERWATER);

    wp->zvel = ((100 - pp->horiz) * HORIZ_MULT);
    
    //sprintf(ds,"horiz %d, ho %d, ho+ho %d",pp->horiz, pp->horizoff, pp->horizoff + pp->horiz);
    //MONO_PRINT(ds);
    
    oclipdist = pp->SpriteP->clipdist;
    pp->SpriteP->clipdist = 0;
    wp->clipdist = 0;
    
    wp->ang = NORM_ANGLE(wp->ang + 512);
    HelpMissileLateral(w, 800);
    wp->ang = NORM_ANGLE(wp->ang - 512);
    
    MissileSetPos(w, DoChemBomb, 1000);
    
    pp->SpriteP->clipdist = oclipdist;
    wp->clipdist = 80L>>2;

    wu->xchange = MOVEx(wp->xvel, wp->ang);
    wu->ychange = MOVEy(wp->xvel, wp->ang);
    wu->zchange = wp->zvel;
    
    // adjust xvel according to player velocity
    wu->xchange += pp->xvect>>14;
    wu->ychange += pp->yvect>>14;

	// Smoke will come out for this many seconds
	wu->WaitTics = CHEMTICS;
    
    return (0);
    }

int
InitChemBomb(short SpriteNum)
    {
	SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];
    USERp wu;
    SPRITEp wp;
    long nx, ny, nz;
    short w, hitsprite;
    short oclipdist;
    long dist;
    

// Need to make it take away from inventory weapon list
//    PlayerUpdateAmmo(pp, u->WeaponNum, -1);

    nx = sp->x;
    ny = sp->y;
    nz = sp->z;

    // Spawn a shot
    // Inserting and setting up variables
    w = SpawnSprite(STAT_MISSILE, CHEMBOMB, s_ChemBomb, sp->sectnum, 
        nx, ny, nz, sp->ang, 0);

    wp = &sprite[w];
    wu = User[w];

    wu->RotNum = 5;
    NewStateGroup(w, &sg_ChemBomb);
    SET(wu->Flags, SPR_XFLIP_TOGGLE);
    
    SetOwner(SpriteNum, w);  
    wp->yrepeat = 32;
    wp->xrepeat = 32;
    wp->shade = -15;
    wu->Radius = 200;
    wu->ceiling_dist = Z(3);
    wu->floor_dist = Z(3);
    wu->Counter = 0;
    SET(wp->cstat, CSTAT_SPRITE_YCENTER|CSTAT_SPRITE_INVISIBLE); // Make nuke radiation invis.
    RESET(wp->cstat, CSTAT_SPRITE_BLOCK);

    if (SpriteInUnderwaterArea(wp))
        SET(wu->Flags, SPR_UNDERWATER);

    wp->zvel = 0;
    wp->clipdist = 0;
    
    wu->xchange = 0;
    wu->ychange = 0;
    wu->zchange = 0;
    
	// Smoke will come out for this many seconds
	wu->WaitTics = CHEMTICS*2;
    
    return (0);
    }

//////////////////////////////////////////////
//
// Inventory Flash Bombs
//
//////////////////////////////////////////////
int
PlayerInitFlashBomb(PLAYERp pp)
    {
	short pnum,i,nexti,stat;
	long dist,tx,ty,tmin;
	short damage;
    SPRITEp sp = pp->SpriteP, hp;
    USERp u = User[pp->PlayerSprite], hu;

	PlaySound(DIGI_30MMFIRE,&pp->posx,&pp->posy,&pp->posz,v3df_pan|v3df_doppler);
	
	// Set it just a little to let player know what he just did
//	SetFadeAmt(pp, -30, 1); // White flash

    for (stat = 0; stat < SIZ(StatDamageList); stat++)
        {
        TRAVERSE_SPRITE_STAT(headspritestat[StatDamageList[stat]], i, nexti)
            {
            hp = &sprite[i];
            hu = User[i];

			if(i == pp->PlayerSprite) 
				break;

			printf("i = %d, pp->PlayerSprite = %d\n",i,pp->PlayerSprite);

            DISTANCE(hp->x, hp->y, sp->x, sp->y, dist, tx, ty, tmin);
            if (dist > 16384) // Flash radius
                continue;

            if (!TEST(sp->cstat, CSTAT_SPRITE_BLOCK))
                continue;

			if(!FAFcansee(hp->x,hp->y,hp->z,hp->sectnum,sp->x,sp->y,sp->z,sp->sectnum))
				continue;

			damage = GetDamage(i, pp->PlayerSprite, DMG_FLASHBOMB);

			//DSPRINTF(ds,"flashdamage = %d",damage);
			MONO_PRINT(ds);
           
            if (hu->sop_parent)        	
                {	
                break;	
                }	
            else    	
            if (hu->PlayerP)	
                {	
				SetFadeAmt(hu->PlayerP, damage, 1); // White flash
                }	
            else    	
                {	
//                ActorPain(i);	
                }	
            }
        }

    return (0);
    }
