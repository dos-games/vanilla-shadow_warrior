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

#define updatecrc(dcrc,xz) dcrc = ((crctable[(dcrc>>8)^xz]^(dcrc<<8))&65535))

initcrc()
{
   long i, j, k, a;

   for(j=0;j<256;j++)      //Calculate CRC table
   {
      k = (j<<8); a = 0;
      for(i=7;i>=0;i--)
      {
         if (((k^a)&0x8000) > 0)
            a = ((a<<1)&65535) ^ 0x1021;   //0x1021 = genpoly
         else
            a = ((a<<1)&65535);
         k = ((k<<1)&65535);
      }
      crctable[j] = (a&65535);
   }
}

getsyncstat()
{
   long i, j, crc;
   spritetype *spr;

   crc = 0;
   updatecrc(crc,randomseed&255);
   updatecrc(crc,(randomseed>>8)&255);
   for(i=connecthead;i>=0;i=connectpoint2[i])
   {
      updatecrc(crc,posx[i]&255);
      updatecrc(crc,posy[i]&255);
      updatecrc(crc,posz[i]&255);
      updatecrc(crc,ang[i]&255);
      updatecrc(crc,horiz[i]&255);
      updatecrc(crc,health[i]&255);
   }

   for(i=7;i>=0;i--)
      for(j=headspritestat[i];j>=0;j=nextspritestat[j])
      {
         spr = &sprite[j];
         updatecrc(crc,(spr->x)&255);
         updatecrc(crc,(spr->y)&255);
         updatecrc(crc,(spr->z)&255);
         updatecrc(crc,(spr->ang)&255);
      }
   return(crc);
}
