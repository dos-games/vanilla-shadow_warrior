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

    {
    // calcuate how far you move in one tic
    #if 0
    doubvel = (TICSPERFRAME << ((syncbits[target] & 256) > 0));
    xvect = 0, yvect = 0;
    
    if (syncvel[target] != 0)
        {
        xvect += ((((long) syncvel[target]) * doubvel * (long) sintable[(ang[target] + 2560) & 2047]) >> 3);
        yvect += ((((long) syncvel[target]) * doubvel * (long) sintable[(ang[target] + 2048) & 2047]) >> 3);
        }
        
    if (syncsvel[target] != 0)
        {
        xvect += ((((long) syncsvel[target]) * doubvel * (long) sintable[(ang[target] + 2048) & 2047]) >> 3);
        yvect += ((((long) syncsvel[target]) * doubvel * (long) sintable[(ang[target] + 1536) & 2047]) >> 3);
        }
    #endif    

    // back up position of where target is
    ox = posx[target];
    oy = posy[target];

    // distance is k
    k = ksqrt((ox - sprite[i].x) * (ox - sprite[i].x) + (oy - sprite[i].y) * (oy - sprite[i].y));

    // (TICSPERFRAME<<19) = this is my bullet velocity - dist per tic
    xvect = scale(xvect, k, TICSPERFRAME << 19);
    yvect = scale(yvect, k, TICSPERFRAME << 19);
    
    // look to see where target is going
    clipmove(&ox, &oy, &posz[target], &cursectnum[target], xvect << 14, yvect << 14, 128L, 4 << 8, 4 << 8, 0);
    ox -= sprite[i].x;
    oy -= sprite[i].y;

    // angle to target
    daang = ((getangle(ox, oy) + (krand() & 7) - 4) & 2047);

    // vector to target
    dax = (sintable[(daang + 512) & 2047] >> 6);
    day = (sintable[daang & 2047] >> 6);
    daz = 0;
    
    // z code i'm already doing
    if (ox != 0)
        daz = scale(dax, posz[target] + (8 << 8) - sprite[i].z, ox);
    else if (oy != 0)
        daz = scale(day, posz[target] + (8 << 8) - sprite[i].z, oy);

    // spawn the bullet 
    // vector is dax,day,daz   
    }
