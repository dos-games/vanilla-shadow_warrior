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

// JSECTOR.C
// This is all Jim's programming having to do with sectors.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "jnames.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "sector.h"
#include "player.h"
#include "sprite.h"
#include "jsector.h"
#include "jtags.h"

#include "def.h"

// V A R I A B L E   D E C L A R A T I O N S //////////////////////////////////////////////////////

typedef enum {m_normal, m_viewon, m_pissed} MIRRORSTATE;

typedef struct {
    short mirrorwall;                       // Wall number containing the mirror tile
    short mirrorsector;                     // nextsector used internally to draw mirror rooms
    short camera;                           // Contains number of ST1 sprite used as a camera
    short numspawnspots;                    // Number of spawnspots used
    short spawnspots[MAXMIRRORMONSTERS];    // One spot for each possible skill level for a
                                            // max of up to 4 coolie ghosts to spawn.
    BOOL  ismagic;                          // Is this a magic mirror?
    MIRRORSTATE     mstate;                                         // What state the mirror is currently in
    long  maxtics;                          // Tic count used to time mirror events
    long  tics;                                                     // How much viewing time has been used on mirror?
} MIRRORTYPE, *MIRRORTYPEp;

MIRRORTYPE mirror[MAXMIRRORS];

static short mirrorcnt,floormirrorcnt;
static short floormirrorsector[MAXMIRRORS];
static char tempbuf[max(576,MAXXDIM)];

BOOL mirrorinview;

extern long frameplace;  
extern long chainnumpages;
extern AMB_INFO ambarray[];

// F U N C T I O N S //////////////////////////////////////////////////////////////////////////////

VOID analyzesprites(long, long, long, BOOL mirror);

/////////////////////////////////////////////////////
//  Initialize any of my special use sprites
/////////////////////////////////////////////////////
void JS_SpriteSetup( void )
{
    SPRITEp sp;
    short SpriteNum = 0, NextSprite, ndx;
    USERp u;
    short i, num;

    
    TRAVERSE_SPRITE_STAT(headspritestat[0], SpriteNum, NextSprite)
    {
        short tag;
        short bit;

        sp = &sprite[SpriteNum];
        tag = sp->hitag;

        switch (sprite[SpriteNum].picnum)
        {
            case ST1:
                if (tag == MIRROR_CAM)
                {       
                    // Just change it to static, sprite has all the info I need
                    change_sprite_stat(SpriteNum, STAT_ST1);
                }else
                if (tag == MIRROR_SPAWNSPOT)
                {       
                    // Just change it to static, sprite has all the info I need
                    change_sprite_stat(SpriteNum, STAT_ST1);
                }else
                if (tag == AMBIENT_SOUND)
                  {
                    change_sprite_stat(SpriteNum, STAT_ST1);
                    PlaySound(sp->lotag,&sp->x,&sp->y,&sp->z,v3df_ambient);
                  }
                break;
        }
    }
}


/////////////////////////////////////////////////////
//  Initialize the mirrors
/////////////////////////////////////////////////////
void JS_InitMirrors( void )
{
    short startwall, endwall, dasector;
    long i, j, k, s, dax, day, daz, dax2, day2;
    
    // Set all the mirror struct values to -1
    memset( mirror, 0xFF, sizeof(mirror) );

    mirrorinview = FALSE;   // Initially set global mirror flag to no mirrors seen

    floormirrorcnt = 0;
    tilesizx[FLOORMIRROR] = 0;
    tilesizy[FLOORMIRROR] = 0;

    //Scan for floor and ceiling mirrors
    for( i=0; i<=numsectors; i++)
    {

        if (sector[i].floorpicnum == FLOORMIRROR)
            floormirrorsector[floormirrorcnt++] = i;

        ASSERT( floormirrorcnt < MAXMIRRORS );
    }

    // Scan wall tags for mirrors
    mirrorcnt = 0;
    tilesizx[MIRROR] = 0;
    tilesizy[MIRROR] = 0;

    for(i=0;i<numwalls;i++)
    {
        if ((wall[i].overpicnum == MIRROR) && ((wall[i].cstat&32) != 0))
        {
            s = wall[i].nextsector;
            if ((s >= 0) && (sector[s].ceilingpicnum != MIRROR))
            {
                sector[s].ceilingpicnum = MIRROR;
                sector[s].floorpicnum = MIRROR;
                mirror[mirrorcnt].mirrorwall = i;
                mirror[mirrorcnt].mirrorsector = s;
                mirror[mirrorcnt].numspawnspots = 0;
                mirror[mirrorcnt].ismagic = FALSE;
                if( wall[i].lotag == TAG_WALL_MAGIC_MIRROR )
                {
                    short ii,nextii;
                    SPRITEp sp;

                    mirror[mirrorcnt].ismagic = TRUE;
                    TRAVERSE_SPRITE_STAT(headspritestat[STAT_ST1], ii, nextii)
                    {
                        sp = &sprite[ii];
                        // if correct type and matches
                        if (sp->hitag == MIRROR_CAM && sp->lotag == wall[i].hitag)
                        {
                            mirror[mirrorcnt].camera = ii;                                          
                        }else
                        if (sp->hitag == MIRROR_SPAWNSPOT && sp->lotag == wall[i].hitag)
                        {
                            mirror[mirrorcnt].spawnspots[mirror[mirrorcnt].numspawnspots++] = ii;                                          
                            ASSERT(mirror[mirrorcnt].numspawnspots <= MAXMIRRORMONSTERS);
                        }
                    }

                    // For magic mirrors, set allowable viewing time to 30 secs
                    // Base rate is supposed to be 120, but time is double what I expect
                    mirror[mirrorcnt].maxtics = 60*30;

                }

                mirror[mirrorcnt].mstate = m_normal;

                // Set tics used to none
                mirror[mirrorcnt].tics = 0;

                if( mirror[mirrorcnt].ismagic)
                {
                //DSPRINTF(ds,"mirror.mirrorwall %d", mirror[mirrorcnt].mirrorwall);
                MONO_PRINT(ds);
                //DSPRINTF(ds,"mirror.mirrorsector %d", mirror[mirrorcnt].mirrorsector);
                MONO_PRINT(ds);
                //DSPRINTF(ds,"mirror.camera %d", mirror[mirrorcnt].camera);
                MONO_PRINT(ds);
                }
                    
                mirrorcnt++;
                ASSERT( mirrorcnt < MAXMIRRORS );
            }
        }
    }

    //Invalidate textures in sector behind mirror
    for(i=0;i<mirrorcnt;i++)
    {
        startwall = sector[mirror[i].mirrorsector].wallptr;
        endwall = startwall + sector[mirror[i].mirrorsector].wallnum;
        for(j=startwall;j<endwall;j++)
        {
            wall[j].picnum = MIRROR;
            wall[j].overpicnum = MIRROR;
        }
    }

} // InitMirrors


/////////////////////////////////////////////////////
//  Draw a 3d screen to a specific tile
/////////////////////////////////////////////////////
void drawroomstotile(long daposx, long daposy, long daposz,
             short daang, long dahoriz, short dacursectnum, short tilenume)
{

    long i, j, k, bakchainnumpages, bakvidoption, bakframeplace;
    long bakwindowx1, bakwindowy1, bakwindowx2, bakwindowy2, xsiz, ysiz;
    char *ptr1, *ptr2;

    //DRAWROOMS TO TILE BACKUP&SET CODE
    xsiz = tilesizx[tilenume]; ysiz = tilesizy[tilenume];
    bakchainnumpages = chainnumpages; chainnumpages = 0;
    bakvidoption = vidoption; vidoption = 1;
    if (waloff[tilenume] == 0) loadtile(tilenume);
    bakframeplace = frameplace; frameplace = waloff[tilenume];
    bakwindowx1 = windowx1; bakwindowy1 = windowy1;
    bakwindowx2 = windowx2; bakwindowy2 = windowy2;
    setview(0,0,xsiz-1,ysiz-1);
    setvlinebpl(xsiz);
    j = 0; for(i=0;i<=ysiz;i++) { ylookup[i] = j, j += xsiz; }

    //DRAWS TO TILE HERE
    drawrooms(daposx,daposy,daposz,daang,dahoriz,dacursectnum);
    analyzesprites(daposx,daposy,daposz,FALSE);
    drawmasks();

    //ROTATE TILE (supports square tiles only for rotation part)
    if(xsiz == ysiz)
    {
    k = (xsiz<<1);
    for(i=xsiz-1;i>=0;i--)
    {
        ptr1 = (char *)(waloff[tilenume]+i*(xsiz+1)); ptr2 = ptr1;
        if ((i&1) != 0)
        {
        ptr1--; ptr2 -= xsiz;
        swapchar(ptr1,ptr2);
        }
        for(j=(i>>1)-1;j>=0;j--)
        {
        ptr1 -= 2; ptr2 -= k;
        swapchar2(ptr1,ptr2,xsiz);
        //swapchar(ptr1,ptr2);
        //swapchar(ptr1+1,ptr2+xsiz);
        }
    }
    }

    //DRAWROOMS TO TILE RESTORE CODE
    setview(bakwindowx1,bakwindowy1,bakwindowx2,bakwindowy2);
    chainnumpages = bakchainnumpages;
    vidoption = bakvidoption;
    frameplace = bakframeplace;
    j = 0;
    if (chainnumpages >= 2)
        { for(i=0;i<=ysiz;i++) ylookup[i] = j, j += (xdim>>2); }
    else
        { for(i=0;i<=ysiz;i++) ylookup[i] = j, j += xdim; }
    setvlinebpl(ylookup[1]);
}

/////////////////////////////////////////////////////
//  Draw one mirror, the one closest to player
/////////////////////////////////////////////////////
void JS_MirrorSpawnMonsters( short MirrorNum )
{
    int i;
    SPRITEp sp;

    // Let's spawn some nasties to harass that mirror watching fool
    for(i=0; i<mirror[MirrorNum].numspawnspots; i++)
    {
        sp = &sprite[mirror[MirrorNum].spawnspots[i]];
        // if skill is too low
        if (TEST(sp->extra, SPRX_SKILL) <= Skill || Skill==4)
            NewCoolg(mirror[MirrorNum].spawnspots[i]);
    }
}

/////////////////////////////////////////////////////
//  Draw one mirror, the one closest to player
/////////////////////////////////////////////////////
void JS_DrawMirrors( PLAYERp pp )
{
    long i, j, k, templong, dx, dy, top, bot;
    long x1, y1, x2, y2, ox1, oy1, ox2, oy2, dist, maxdist;
    long tposx, tposy, thoriz;
    long tiltlock;
    short tang;
    char ch, *ptr, *ptr2, *ptr3, *ptr4;
    char tvisibility,palok;
    long tx,ty,tz,tpang; // Interpolate so mirror doesn't drift!

    // Do tic checks on mirrors, even if that mirror isn't in sight
    for(i=0; i<mirrorcnt; i++)
    {
        if(mirror[i].ismagic)
        {
            if(mirror[i].mstate == m_viewon || mirror[i].mstate == m_pissed)
            mirror[i].tics += synctics;

            if(mirror[i].tics >= mirror[i].maxtics)
            {
                if(mirror[i].mstate == m_viewon)
                {
                    mirror[i].mstate = m_pissed;    // Uh oh! Now you've done it!
                    JS_MirrorSpawnMonsters(i);
                }else
                    mirror[i].mstate = m_normal;
            }

        while (mirror[i].tics >= mirror[i].maxtics)
        {
            mirror[i].tics -= mirror[i].maxtics;
            }
        }
    }

    // Check to see if any mirrors need to be drawn
#if 0
    if ((gotpic[FLOORMIRROR>>3]&(1<<(FLOORMIRROR&7))) > 0)
    {
        mirrorinview = TRUE;    // Player sees a mirror

        dist = 0x7fffffff; i = 0;
        for(k=floormirrorcnt-1;k>=0;k--)
        {
            j = klabs(wall[sector[floormirrorsector[k]].wallptr].x-tx);
            j += klabs(wall[sector[floormirrorsector[k]].wallptr].y-ty);
            if (j < dist) dist = j, i = k;
        }

        j = floormirrorsector[i];
        
        drawrooms(tx,ty,(sector[j].floorz<<1)-tz,tpang,201-pp->horiz,j+MAXSECTORS);
        analyzesprites(tx,ty,(sector[j].floorz<<1)-tz,FALSE);
        drawmasks();

        thoriz = scale(pp->horiz-100,windowx2-windowx1,320)+((windowy1+windowy2)>>1);
        for(y1=windowy1,y2=windowy2;y1<y2;y1++,y2--)
        {
            ptr = (char *)(frameplace+ylookup[y1]);
            ptr2 = (char *)(frameplace+ylookup[y2]);
            ptr3 = palookup[PALETTE_DIVE];
            ptr3 += (min(klabs(y1-thoriz)>>2,31)<<8);
            ptr4 = palookup[PALETTE_DIVE];
            ptr4 += (min(klabs(y2-thoriz)>>2,31)<<8);

            j = sintable[((y2+numframes)<<7)&2047];
            j += sintable[((y2-numframes)<<8)&2047];
            j >>= 14;

            ptr2 += j;

            for(x1=windowx1;x1<=windowx2;x1++)
                { ch = ptr[x1]; ptr[x1] = ptr3[ptr2[x1]]; ptr2[x1] = ptr4[ch]; }
        }

        gotpic[FLOORMIRROR>>3] &= ~(1<<(FLOORMIRROR&7));
    }
#endif        

    if ((gotpic[MIRROR>>3]&(1<<(MIRROR&7))) > 0)
    {
        mirrorinview = TRUE;

        tx = pp->oposx + mulscale(pp->posx - pp->oposx, smoothratio, 16);
        ty = pp->oposy + mulscale(pp->posy - pp->oposy, smoothratio, 16);
        tz = pp->oposz + mulscale(pp->posz - pp->oposz, smoothratio, 16);
        tpang = pp->oang + mulscale(((pp->pang + 1024 - pp->oang) & 2047) - 1024, smoothratio, 16);
        
        dist = 0x7fffffff; i = -1;
        for(k=mirrorcnt-1;k>=0;k--)
        {
            x1 = ((wall[mirror[k].mirrorwall].x+wall[wall[mirror[k].mirrorwall].point2].x)>>1);
            y1 = ((wall[mirror[k].mirrorwall].y+wall[wall[mirror[k].mirrorwall].point2].y)>>1);
            templong = ((sector[mirror[k].mirrorsector].ceilingz+sector[mirror[k].mirrorsector].floorz)>>1);
            if (cansee(tx,ty,tz,pp->cursectnum,x1,y1,templong,mirror[k].mirrorsector) != 0) i = k;

            // I want this distance check anyway, to see if magic mirror should activate
            j = klabs(wall[mirror[k].mirrorwall].x-tx);
            j += klabs(wall[mirror[k].mirrorwall].y-ty);
            if (j < dist) dist = j;
        }

        if (i < 0)
        {
           i = 0;
           for(k=mirrorcnt-1;k>=0;k--)
           {
               j = klabs(wall[mirror[k].mirrorwall].x-tx);
               j += klabs(wall[mirror[k].mirrorwall].y-ty);
               if (j < dist) dist = j, i = k;
           }
        }

        j = mirror[i].mirrorwall;
        x1 = wall[j].x; dx = wall[wall[j].point2].x-x1;
        y1 = wall[j].y; dy = wall[wall[j].point2].y-y1;
        bot = dx*dx + dy*dy;
        if (bot != 0)
        {
            top = (tx-x1)*dx + (ty-y1)*dy;
            tposx = ((x1 + scale(dx,top,bot))<<1) - tx;
            tposy = ((y1 + scale(dy,top,bot))<<1) - ty;
        }

        tang = (((getangle(dx,dy)<<1)-tpang)&2047);


        if(dist < MAXMIRRORDIST)
        {
            if(mirror[i].mstate != m_pissed)
                mirror[i].mstate = m_viewon;
        }else
            if(mirror[i].mstate != m_pissed)
                mirror[i].mstate = m_normal;

//              //DSPRINTF(ds,"mirror.tics == %ul", mirror[i].tics);
//              MONO_PRINT(ds);

        if(mirror[i].mstate == m_normal || mirror[i].mstate == m_pissed)
        {
            //Draw pink pixels on horizon to get mirror l&r bounds.
            thoriz = scale(pp->horiz-100,windowx2-windowx1,320)+((windowy1+windowy2)>>1);
            thoriz = min(max(thoriz,windowy1),windowy2);
            clearbufbyte(frameplace+ylookup[thoriz]+windowx1,windowx2-windowx1+1,0xffffffff);
        }

        tvisibility = visibility;
        visibility <<= 1;       // Make mirror darker

        if(mirror[i].ismagic && mirror[i].mstate == m_normal && RANDOM_P2(256) > 128)
            visibility += RANDOM_P2(128);                        
            
        if(mirror[i].mstate == m_pissed)
        {
        ptr = palookup[PALETTE_DEFAULT];
        palookup[PALETTE_DEFAULT] = palookup[PALETTE_RED_LIGHTING];
        palookup[PALETTE_RED_LIGHTING] = ptr;
        }

        // Mirror mirror, show me something cool!
        if(mirror[i].ismagic && mirror[i].mstate == m_viewon) 
        {
            SPRITEp sp;

            sp = &sprite[mirror[i].camera];

            // Set mirror tile back to it's original size for camera viewing
            tilesizx[MIRROR]=tilesizy[MIRROR]=128;
            drawroomstotile(sp->x,sp->y,sp->z,sp->ang,100,sp->sectnum,MIRROR);

        } else  // It's just a mirror, or it's a pissed off magic one.
        {
            tilesizx[MIRROR]=tilesizy[MIRROR]=0;
            drawrooms(tposx,tposy,tz,tang,pp->horiz,mirror[i].mirrorsector);
            analyzesprites(tposx,tposy,tz,TRUE);
            //analyzesprites(pp->posx,pp->posy,TRUE);
            drawmasks();
        }


        if(mirror[i].mstate == m_pissed)
        {
            ptr = palookup[PALETTE_DEFAULT];
            palookup[PALETTE_DEFAULT] = palookup[PALETTE_RED_LIGHTING];
            palookup[PALETTE_RED_LIGHTING] = ptr;
        }

        visibility = tvisibility;


        if(mirror[i].mstate == m_normal || mirror[i].mstate == m_pissed)
        {
            //Get pink pixels on horizon to get mirror l&r bounds.
            x1 = 0; x2 = windowx2-windowx1;
            if ((tz > sector[mirror[i].mirrorsector].ceilingz) && (tz < sector[mirror[i].mirrorsector].floorz))
            {
                ptr = (char *)frameplace+ylookup[thoriz]+windowx1;
                while ((ptr[x1] == 255) && (x2 >= x1)) x1++;
                while ((ptr[x2] == 255) && (x2 >= x1)) x2--;
            }

            if (x2 >= x1)  //Flip window x-wise
            {
                ptr = (char *)frameplace+ylookup[windowy1]+windowx1;
                y1 = windowx2-windowx1-x2; y2 = windowx2-windowx1-x1;
                for(dy=windowy2-windowy1;dy>=0;dy--)
                {
                    copybuf(&ptr[x1],tempbuf,(x2-x1+4)>>2);
                    copybufreverse(&tempbuf[y2-y1],&ptr[y1],y2-y1+1);
                    ptr += ylookup[1];
                }
            }
        }
        gotpic[MIRROR>>3] &= ~(1<<(MIRROR&7));
        
        drawrooms(tx,ty,tz,tpang,pp->horiz,pp->cursectnum);
        analyzesprites(tx,ty,tz,FALSE);
        drawmasks();
    } else
        mirrorinview = FALSE;
}

