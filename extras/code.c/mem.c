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

void TotalMem()
{
    char incache[8192];
    int i,j,tottiles,totsprites,totactors;

    for(i=0;i<4096;i++) incache[i] = 0;

    for(i=0;i<numsectors;i++)
    {
        incache[sector[i].ceilingpicnum] = 1;
        incache[sector[i].floorpicnum] = 1;
    }
    for(i=0;i<numwalls;i++)
    {
        incache[wall[i].picnum] = 1;
        if (wall[i].overpicnum >= 0)
            incache[wall[i].overpicnum] = 1;
    }

    tottiles = 0;
    for(i=0;i<4096;i++)
        if (incache[i] > 0)
            tottiles += tilesizx[i]*tilesizy[i];



    for(i=0;i<4096;i++) incache[i] = 0;

    for(i=0;i<MAXSPRITES;i++)
        if (sprite[i].statnum < MAXSTATUS)
            incache[sprite[i].picnum] = 1;
    totsprites = 0;


    for(i=0;i<4096;i++)
    {
        if (incache[i] > 0)
    {
     switch(i)
     {
        case LIZTROOP :
        case LIZTROOPRUNNING :
        case LIZTROOPSTAYPUT :
        case LIZTROOPSHOOT :
        case LIZTROOPJETPACK :
        case LIZTROOPONTOILET :
        case LIZTROOPDUCKING :
        totactors+=ActorMem(LIZTROOP);
        incache[LIZTROOP]=0;
        incache[LIZTROOPRUNNING]=0;
        incache[LIZTROOPSTAYPUT]=0;
        incache[LIZTROOPSHOOT]=0;
        incache[LIZTROOPJETPACK]=0;
        incache[LIZTROOPONTOILET]=0;
        incache[LIZTROOPDUCKING]=0;
        break;
        case OCTABRAIN :
        case OCTABRAINSTAYPUT:
        totactors+=ActorMem(OCTABRAIN);
        incache[OCTABRAIN]=0;
        incache[OCTABRAINSTAYPUT]=0;
        break;
         case DRONE :
        totactors+=ActorMem(DRONE);
        incache[DRONE]=0;
        break;
        case COMMANDER :
        totactors+=ActorMem(COMMANDER);
        incache[COMMANDER]=0;
        break;
        case RECON :
        totactors+=ActorMem(RECON);
        incache[RECON]=0;
        break;
        case PIGCOP :
        totactors+=ActorMem(COMMANDER);
        incache[PIGCOP]=0;
        break;
        case LIZMAN :
        case LIZMANSTAYPUT :
        case LIZMANSPITTING :
        case LIZMANFEEDING :
        case LIZMANJUMP :
        totactors+=ActorMem(LIZMAN);
        incache[LIZMAN]=0;
        incache[LIZMANSTAYPUT]=0;
        incache[LIZMANSPITTING]=0;
        incache[LIZMANFEEDING]=0;
        incache[LIZMANJUMP]=0;
        break;
        case BOSS1 :
        totactors+=ActorMem(BOSS1);
        incache[BOSS1]=0;
        break;
        case BOSS2 :
        totactors+=ActorMem(BOSS2);
        incache[BOSS2]=0;
        break;
        case BOSS3 :
        totactors+=ActorMem(BOSS3);
        incache[BOSS3]=0;
        break;

        default: totsprites += tilesizx[i]*tilesizy[i];
     }
    }
    }





    clearmidstatbar16();
    printext16(1*8,4*8,11,-1,"Memory Status",0);

    PrintStatus("Total Tiles   = ",tottiles,2,6,11);
    PrintStatus("Total Sprites = ",totsprites,2,7,11);
    PrintStatus("Total Actors  = ",totactors,2,8,11);

    PrintStatus("Total Memory  = ",(tottiles+totsprites+totactors),2,10,11);

    PrintStatus("Total W/Duke  = ",(tottiles+totsprites+totactors+ActorMem(APLAYER)),2,12,11);
    
}
