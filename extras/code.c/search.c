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

short sectlist[MAXSECTORS], sectlistcnt, sectlistend;

sectlist[0] = endsect;
sectlistcnt = 0; sectlistend = 1;
while (sectlistcnt < sectlistend)
{
      //Read off next sector on list to process
   dasect = sectlist[sectlistcnt++];

   //process dasect
   //for(i=headspritesect[dasect]...
   
      //Scan all red walls of dasect
   startwall = sector[dasect].wallptr;
   endwall = startwall + sector[dasect].wallnum;
   for(w=startwall;w<endwall;w++)
   {
      nextsector = wall[w].nextsector;
      if (nextsector < 0) continue;

         //Check if new sector is already on sectlist
      for(s=sectlistend-1;s>=0;s--)
         if (sectlist[s] == nextsector) break;

      if (s < 0)  //Did not break out of loop, so no matches were found
         sectlist[sectlistend++] = nextsector;
   }
}
