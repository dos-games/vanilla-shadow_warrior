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


 if(keystatus[0x28]==1 && keystatus[0x2e]==1) // ' C
      {
     keystatus[0x2e] = 0;
         switch (searchstat)
         {
            case 0: case 4:
        for(i=0;i<MAXWALLS;i++)
        {
         if(wall[i].picnum==temppicnum) {wall[i].shade=tempshade;}
        }
                break;
            case 1: case 2:
        for(i=0;i<MAXSECTORS;i++)
        {
        if(searchstat==1)
         if(sector[i].ceilingpicnum==temppicnum) {sector[i].ceilingshade=tempshade;}
        if(searchstat==2)
         if(sector[i].floorpicnum==temppicnum) {sector[i].floorshade=tempshade;}
        }
                break;
            case 3:
        for(i=0;i<MAXSPRITES;i++)
        {
         if(sprite[i].picnum==temppicnum) {sprite[i].shade=tempshade;}
        }
                break;
         }
      }

 if(keystatus[0x28]==1 && keystatus[0x14]==1) // ' T
      {
         keystatus[0x14] = 0;
         switch (searchstat)
         {
            case 0: case 4:
                strcpy(tempbuf,"Wall lotag: ");
                wall[searchwall].lotag =
                getnumber256(tempbuf,wall[searchwall].lotag,65536L);
                break;
            case 1: case 2:
                strcpy(tempbuf,"Sector lotag: ");
                sector[searchsector].lotag =
                getnumber256(tempbuf,sector[searchsector].lotag,65536L);
                break;
            case 3:
                strcpy(tempbuf,"Sprite lotag: ");
                sprite[searchwall].lotag =
                getnumber256(tempbuf,sprite[searchwall].lotag,65536L);
                break;
         }
      }

 if(keystatus[0x28]==1 && keystatus[0x23]==1) // ' H
      {
         keystatus[0x23] = 0;
         switch (searchstat)
         {
            case 0: case 4:
                strcpy(tempbuf,"Wall hitag: ");
                wall[searchwall].hitag =
                getnumber256(tempbuf,wall[searchwall].hitag,65536L);
                break;
            case 1: case 2:
                strcpy(tempbuf,"Sector hitag: ");
                sector[searchsector].hitag =
                getnumber256(tempbuf,sector[searchsector].hitag,65536L);
                break;
            case 3:
                strcpy(tempbuf,"Sprite hitag: ");
                sprite[searchwall].hitag =
                getnumber256(tempbuf,sprite[searchwall].hitag,65536L);
                break;
         }
      }

 if(keystatus[0x28]==1 && keystatus[0x1f]==1) // ' S
      {
     keystatus[0x1f] = 0;
         switch (searchstat)
         {
            case 0: case 4:
        strcpy(tempbuf,"Wall shade: ");
        wall[searchwall].shade =
        getnumber256(tempbuf,wall[searchwall].shade,65536L);
                break;
            case 1: case 2:
        strcpy(tempbuf,"Sector shade: ");
        if(searchstat==1)
         sector[searchsector].ceilingshade =
         getnumber256(tempbuf,sector[searchsector].ceilingshade,65536L);
        if(searchstat==2)
         sector[searchsector].floorshade =
         getnumber256(tempbuf,sector[searchsector].floorshade,65536L);
                break;
            case 3:
        strcpy(tempbuf,"Sprite shade: ");
        sprite[searchwall].shade =
        getnumber256(tempbuf,sprite[searchwall].shade,65536L);
                break;
         }
      }

 if(keystatus[0x28]==1 && keystatus[0x2f]==1) // ' V
      {
     keystatus[0x2f] = 0;
         switch (searchstat)
         {
            case 1: case 2:
        strcpy(tempbuf,"Sector visibility: ");
        sector[searchsector].visibility =
         getnumber256(tempbuf,sector[searchsector].visibility,65536L);
                break;
         }
      }
