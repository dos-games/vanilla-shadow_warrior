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

/******************
insert into F6 key pressed in 2d mode
******************/


     for(i=0;i<MAXSPRITES;i++) numsprite[i]=0;
     for(i=0;i<MAXSPRITES;i++) multisprite[i]=0;
	 for(i=0;i<MAXSPRITES;i++)
     { if(sprite[i].statnum==0)
       {
         if(sprite[i].pal!=0) multisprite[sprite[i].picnum]++;
         else numsprite[sprite[i].picnum]++;
       }
	 }
    

         x=1; x2=14; y=4;
     printext16(x*8,y*8,11,-1,"Item Count",0);
     PrintStatus("10%health=",numsprite[COLA],x,y+2,11);
      PrintStatus("",multisprite[COLA],x2,y+2,1);
     PrintStatus("30%health=",numsprite[SIXPAK],x,y+3,11);
      PrintStatus("",multisprite[SIXPAK],x2,y+3,1);
     PrintStatus("Med-Kit  =",numsprite[FIRSTAID],x,y+4,11);
      PrintStatus("",multisprite[FIRSTAID],x2,y+4,1);
     PrintStatus("Atom     =",numsprite[ATOMICHEALTH],x,y+5,11);
      PrintStatus("",multisprite[ATOMICHEALTH],x2,y+5,1);
     PrintStatus("Shields  =",numsprite[SHIELD],x,y+6,11);
      PrintStatus("",multisprite[SHIELD],x2,y+6,1);

     x=17; x2=30; y=4;
     printext16(x*8,y*8,11,-1,"Inventory",0);
     PrintStatus("Steroids =",numsprite[STEROIDS],x,y+2,11);
      PrintStatus("",multisprite[STEROIDS],x2,y+2,1);
     PrintStatus("Airtank  =",numsprite[AIRTANK],x,y+3,11);
      PrintStatus("",multisprite[AIRTANK],x2,y+3,1);
     PrintStatus("Jetpack  =",numsprite[JETPACK],x,y+4,11);
      PrintStatus("",multisprite[JETPACK],x2,y+4,1);
     PrintStatus("Goggles  =",numsprite[HEATSENSOR],x,y+5,11);
      PrintStatus("",multisprite[HEATSENSOR],x2,y+5,1);
     PrintStatus("Boots    =",numsprite[BOOTS],x,y+6,11);
      PrintStatus("",multisprite[BOOTS],x2,y+6,1);
     PrintStatus("HoloDuke =",numsprite[HOLODUKE],x,y+7,11);
      PrintStatus("",multisprite[HOLODUKE],x2,y+7,1);
     PrintStatus("Multi D  =",numsprite[APLAYER],x,y+8,11);

     x=33; x2=46; y=4;
     printext16(x*8,y*8,11,-1,"Weapon Count",0);
     PrintStatus("Pistol   =",numsprite[FIRSTGUNSPRITE],x,y+2,11);
      PrintStatus("",multisprite[FIRSTGUNSPRITE],x2,y+2,1);
     PrintStatus("Shotgun  =",numsprite[SHOTGUNSPRITE],x,y+3,11);
      PrintStatus("",multisprite[SHOTGUNSPRITE],x2,y+3,1);
     PrintStatus("Chaingun =",numsprite[CHAINGUNSPRITE],x,y+4,11);
      PrintStatus("",multisprite[CHAINGUNSPRITE],x2,y+4,1);
     PrintStatus("RPG      =",numsprite[RPGSPRITE],x,y+5,11);
      PrintStatus("",multisprite[RPGSPRITE],x2,y+5,1);
     PrintStatus("Pipe Bomb=",numsprite[HEAVYHBOMB],x,y+6,11);
      PrintStatus("",multisprite[HEAVYHBOMB],x2,y+6,1);
     PrintStatus("Shrinker =",numsprite[SHRINKERSPRITE],x,y+7,11);
      PrintStatus("",multisprite[SHRINKERSPRITE],x2,y+7,1);
     PrintStatus("Disruptor=",numsprite[DEVISTATORSPRITE],x,y+8,11);
      PrintStatus("",multisprite[DEVISTATORSPRITE],x2,y+8,1);
     PrintStatus("Trip mine=",numsprite[TRIPBOMBSPRITE],x,y+9,11);
      PrintStatus("",multisprite[TRIPBOMBSPRITE],x2,y+9,1);
     PrintStatus("Freezeray=",numsprite[FREEZESPRITE],x,y+10,11);
      PrintStatus("",multisprite[FREEZESPRITE],x2,y+10,1);

      x=49; x2=62; y=4;
     printext16(x*8,y*8,11,-1,"Ammo Count",0);
     PrintStatus("Pistol   =",numsprite[AMMO],x,y+2,11);
      PrintStatus("",multisprite[AMMO],x2,y+2,1);
     PrintStatus("Shot     =",numsprite[SHOTGUNAMMO],x,y+3,11);
      PrintStatus("",multisprite[SHOTGUNAMMO],x2,y+3,1);
     PrintStatus("Chain    =",numsprite[BATTERYAMMO],x,y+4,11);
      PrintStatus("",multisprite[BATTERYAMMO],x2,y+4,1);
     PrintStatus("RPG Box  =",numsprite[RPGAMMO],x,y+5,11);
      PrintStatus("",multisprite[RPGAMMO],x2,y+5,1);
     PrintStatus("Pipe Bomb=",numsprite[HBOMBAMMO],x,y+6,11);
      PrintStatus("",multisprite[HBOMBAMMO],x2,y+6,1);
     PrintStatus("Disruptor=",numsprite[DEVISTATORAMMO],x,y+7,11);
      PrintStatus("",multisprite[DEVISTATORAMMO],x2,y+7,1);
     PrintStatus("Shrinker =",numsprite[CRYSTALAMMO],x,y+8,11);
      PrintStatus("",multisprite[CRYSTALAMMO],x2,y+8,1);
     PrintStatus("Freezeray=",numsprite[FREEZEAMMO],x,y+9,11);
      PrintStatus("",multisprite[FREEZEAMMO],x2,y+9,1);
/*
     printext16(65*8,4*8,11,-1,"MISC",0);
     PrintStatus("Secrets =",secrets,65,6,11);
     printext16(65*8,7*8,11,-1,"ACTORS",0);
     PrintStatus("Skill 1 =",totalactors1,65,8,11);
     PrintStatus("Skill 2 =",totalactors2,65,9,11);
     PrintStatus("Skill 3 =",totalactors3,65,10,11);
     PrintStatus("Skill 4 =",totalactors4,65,11,11);
     PrintStatus("Respawn =",totalrespawn,65,12,11);
*/
    }









void PrintStatus(char *string,int num,char x,char y,char color)
{
     sprintf(tempbuf,"%s %d",string,num);
     printext16(x*8,y*8,color,-1,tempbuf,0);
}
