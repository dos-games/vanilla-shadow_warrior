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


//  Here's the code to draw the screen into a tile in D3D.
//  Place this function somewhere outside "domovethings", I have mine
//  near "drawrooms".



   drawtosprite(screenpeek,TILETODRAWTO);




// GLOBAL var for a single sprite number.
short camsprite;

void drawtosprite(short snum,short picnume)
{
    short i, j;

    if( camsprite <= -1) return;     // Don't execute if it's not on
    if(tempspritevar[i] < 10)
    {
        //  The delay is to prevent
        //  the screen from updating
        //  the tile all the time, to prevent a major BOG.

        tempspritevar[i]++;
        return;
    }
    else
        tempspritevar[i] = 0;

    i = camsprite;

    // The owner of this sprite contains the new x,y,z,a to draw to
    // In my case, the owner is a CAMERA(names.h).

    j = sprite[i].owner;

    if(j >= 0)
        if( dist( &sprite[ps[snum].i] , &sprite[i] ) < 2048 )
    {
            drawroomstotile(
                sprite[j].x,sprite[j].y,sprite[j].z,
                sprite[j].ang,sprite[j].sectnum,
                tilenume);
    }
}

// Remember that camsprite is the sprite number associated with
// TILETODRAWTO(in my names.h) , which can be set inside domovethings.

// Feel free to call me anytime if you got any questions.
//                 Later...
//                      Todd.




