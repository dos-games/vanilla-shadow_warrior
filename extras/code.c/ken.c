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

Hey Frank,

   Here are 3 cool code snippets that I recently programmed for the Build
engine that are not currently in my KENBUILD demo.  Once you're done with
this, there's always network code...   Enjoy!

-Ken S.
----------------------------------------------------------------------------
/*  SLOPED SIN-WAVE FLOORS:

It's best to program sloped sin-wave floors in 2 steps:
   1.  First set the floorz of the floor as the sin code normally does it.
   2.  Adjust the slopes by calling alignflorslope once for each sector.

Note:  For this to work, the first wall of each sin-wave sector must be
       aligned on the same side of each sector for the entire strip.
*/

     walltype *wal;
     for (i = (all sin-wave sectors)...)
     {
           //Sloped sin floors only work on rectangular sectors (4 walls)
         if (sector[i].wallnum == 4)
         {
               //Set wal to the wall on the opposite side of the sector
            wal = &wall[sector[i].wallptr+2];

               //Pass (Sector, x, y, z)
            alignflorslope(sector[i].sectnum,wal->x,wal->y,
                           sector[wal->nextsector].floorz);
         }
     }

----------------------------------------------------------------------------
/*  OVER-THE-SHOULDER MODE, KEN'S METHOD:

   Right after you calculate the normal player positions - (x, y, z, sect,
   horiz, ang), call this function to adjust them so you're in over the
   shoulder mode.  My method uses FAFhitscan.  If FAFhitscan hits something, I
   make the camera distance slightly smaller so it doesn't draw exactly on
   the wall.
*/

long cameradist = 0;
void over_the_shoulder_adjust(long *nx, long *ny, long *nz,
                              short *vsect, short ang, short horiz)
{
   spritetype *spr;
   long i, vx, vy, vz, hx, hy, hz, hitx, hity, hitz;
   short bakcstat, hitsect, hitwall, hitsprite, daang;

      //Calculate the vector (nx,ny,nz) to shoot backwards
   vx = (sintable[(ang+1536)&2047]>>4);
   vy = (sintable[(ang+1024)&2047]>>4);
   vz = (horiz-100)*128;

      //Player sprite of current view
   spr = &sprite[playersprite[screenpeek]];

   bakcstat = spr->cstat;        //Backup&restore cstat to prevent sync bugs
   spr->cstat &= (short)~0x101;  //Don't let FAFhitscan hit your own sprite

      //Make sure sector passed to FAFhitscan is correct
   COVERupdatesector(*nx,*ny,vsect);

   FAFhitscan(*nx,*ny,*nz,*vsect,vx,vy,vz,
           &hitsect,&hitwall,&hitsprite,&hitx,&hity,&hitz);

   hx = hitx-(*nx); hy = hity-(*ny);

      //If something is in the way, make cameradist lower if necessary
   if (klabs(vx)+klabs(vy) > klabs(hx)+klabs(hy))
   {
      *vsect = hitsect;
      if (hitwall >= 0)   //Push you a little bit off the wall
      {
          daang = getangle(wall[wall[hitwall].point2].x-wall[hitwall].x,
                           wall[wall[hitwall].point2].y-wall[hitwall].y);

          i = vx*sintable[daang]+vy*sintable[(daang+1536)&2047];
          if (klabs(vx) > klabs(vy)) hx -= mulscale28(vx,i);
                                else hy -= mulscale28(vy,i);
      }
      else if (hitsprite < 0)  //Push you off the ceiling/floor
      {
          if (klabs(vx) > klabs(vy)) hx -= (vx>>5);
                                else hy -= (vy>>5);
      }
      if (klabs(vx) > klabs(vy)) i = (hx<<16)/vx;
                            else i = (hy<<16)/vy;

      if (i < cameradist) cameradist = i;
   }

      //Actually move you!  (Camerdist is 65536 if nothing is in the way)
   *nx = (*nx) + ((vx * cameradist) >> 16);
   *ny = (*ny) + ((vy * cameradist) >> 16);
   *nz = (*nz) + ((vz * cameradist) >> 16);

      //Slowly increase cameradist until it reaches 65536
      //Synctics is a timer variable so it increases the same rate
      //on all speed computers
   cameradist = min(cameradist+(synctics<<10),65536);

      //Make sure vsect is correct
   updatesectorz(*nx,*ny,*nz,vsect);

   spr->cstat = bakcstat;   //Restore cstat
}

//This function is like updatesector, but it takes a z-coordinate in addition
//   to help it get the right sector when there's overlapping.  (I may be
//   adding this function to the engine or making the standard updatesector
//   use z's.  Until then, use this.  )
void updatesectorz(long x, long y, long z, short *sectnum)
{
   walltype *wal;
   long i, j, cz, fz;

   getzsofslope(*sectnum,x,y,&cz,&fz);
   if ((z >= cz) && (z <= fz))
      if (inside(x,y,*sectnum) != 0) return;

   if ((*sectnum >= 0) && (*sectnum < numsectors))
   {
      wal = &wall[sector[*sectnum].wallptr];
      j = sector[*sectnum].wallnum;
      do
      {
         i = wal->nextsector;
         if (i >= 0)
         {
            getzsofslope(i,x,y,&cz,&fz);
            if ((z >= cz) && (z <= fz))
               if (inside(x,y,(short)i) == 1)
                  { *sectnum = i; return; }
         }
         wal++; j--;
      } while (j != 0);
   }

   for(i=numsectors-1;i>=0;i--)
   {
      getzsofslope(i,x,y,&cz,&fz);
      if ((z >= cz) && (z <= fz))
         if (inside(x,y,(short)i) == 1)
            { *sectnum = i; return; }
   }

   *sectnum = -1;
}

----------------------------------------------------------------------------
//AUTO LOOK UP & DOWN CODE:
//You should call this code for all players in processinput, where snum is
//   the player index

    if ((sector[cursectnum[snum]].floorstat&2))   //If the floor is sloped
    {
           //Get a point, 512 units ahead of player's position
        x = posx[snum]+(sintable[(ang[snum]+512)&2047]>>5);
        y = posy[snum]+(sintable[ang[snum]&2047]>>5);
        tempsect = cursectnum[snum];
        COVERupdatesector(x,y,&tempsect);

        if (tempsect >= 0)  //If the new point is inside a valid sector...
        {
               //Get the floorz as if the new (x,y) point was still in
               //your sector
            k = getflorzofslope(cursectnum[snum],x,y);

               //If extended point is in same sector as you or the slopes
               //   of the sector of the extended point and your sector match
               //   closely (to avoid accidently looking straight out when
               //   you're at the edge of a sector line) then adjust horizon
               //   accordingly
            if ((cursectnum[snum] == tempsect) ||
               (klabs(getflorzofslope(tempsect,x,y)-k) <= (4<<8))
                horizoff[snum] += (((j-k) * 160) >> 16);
        }
   }
       //Make horizoff grow towards 0 since horizoff is not modified when
       //   you're not on a slope
    if (horizoff[snum] > 0) horizoff[snum] -= ((horizoff[snum]>>3)+1);
    if (horizoff[snum] < 0) horizoff[snum] += (((-horizoff[snum])>>3)+1);
--------------------------------------------------------------------------

