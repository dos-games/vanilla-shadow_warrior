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

#define COINCURSOR 2440

int intro;
void ExtCheckKeys(void)
{
 if (qsetmode == 200)    //In 3D mode
 {
  if(intro<600)
  {
   intro++;
   sprintf(tempbuf,"SWBUILD Level Editor : VER: 1.0  10-30-96");
   printext256(48*8,(24*8)-1,0,-1,tempbuf,1);
   printext256(48*8,24*8,WHITE,-1,tempbuf,1);
   rotatesprite((320-8)<<16,(200-8)<<16,64<<9,0,COINCURSOR+(((4-totalclock>>3))&7),0,0,0,0,0,xdim-1,ydim-1);
  }
 }
}// end
