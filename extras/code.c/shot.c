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

#include "id.h"
#include <fast3D.h>
#include "3d_game.h"

ANIMATOR DoShot;

STATE_PFX(Shot)
{
{&ShotID[0], 1000, DoShot, &s_Shot[0]},
{&ShotID[1], 1000, DoShot, &s_Shot[1]},
{&ShotID[2], 1000, DoShot, &s_Shot[2]},
{&ShotID[3], 1000, DoShot, &s_Shot[3]},
{&ShotID[4], 1000, DoShot, &s_Shot[4]},
{&ShotID[5], 1000, DoShot, &s_Shot[5]}
};

IMGID3D LazorID[F_Lazor],MaskID[F_Mask];

#define Lazor_RATE (30/CLOCK_FACT)

STATE s_Mask[1];

STATE s_Lazor[] =
{
{&LazorID[0], Lazor_RATE, DoShot, &s_Lazor[1]},
{&LazorID[1], Lazor_RATE+5, DoShot, &s_Lazor[2]},
{&LazorID[2], Lazor_RATE, DoShot, &s_Lazor[3]},
{&LazorID[3], Lazor_RATE, DoShot, &s_Lazor[3]},
{&LazorID[4], Lazor_RATE, DoShot, &s_Lazor[5]},
{&LazorID[5], Lazor_RATE, DoShot, &s_Lazor[5]},
};

ANIMATOR
DoShot
{
//
//  tx and ty are new x and y based on direction and speed
//

    objp->x.l = objp->x.l + FixedCOS(objp->Dir.w.msw, (objp->xSpeed*CLOCK_FACT));
    objp->y.l = objp->y.l - FixedSIN(objp->Dir.w.msw, (objp->xSpeed*CLOCK_FACT));

    objp->xSpeed += 25*CLOCK_FACT;

    if (OBJoverlapWall(objp))
        OBJremove(objp);

    return(0);
}

