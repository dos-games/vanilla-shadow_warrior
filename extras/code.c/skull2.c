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

#include <stdlib.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "ai.h"

//////////////////////    
//
// SERP RUN
//
//////////////////////

ANIMATOR DoSerpMove,NullAnimator,DoActorDebris,NullAnimator;
 
int 
SetupSerp(short SpriteNum)    
{
    SPRITEp sp = &sprite[SpriteNum];
    USERp u;
    ANIMATOR DoActorDecide;
    
    if (TEST(sp->extra, SPRX_RESTORE))
        {
        u = User[SpriteNum];
        }
    else
        {
        User[SpriteNum] = u = SpawnUser(SpriteNum,SERP_RUN_R0,&s_SerpRun[0][0]);
        u->Health = 300;
        u->Attrib = &DefaultAttrib;
        DoActorSetSpeed(SpriteNum, NORM_SPEED);
        }

    u->StateStart = u->State;
    u->StateEnd = s_SerpDie;
    u->Rot = sg_SerpRun;

    EnemyDefaults(SpriteNum, &SerpActionSet, &SerpPersonality);
    sp->clipdist = (512 + 256) >> 2;
    SET(u->Flags, SPR_XFLIP_TOGGLE);
    
    u->loz = sp->z;
    
    return(0);
}    


int DoSerpMove(short SpriteNum)
{
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];
    
    if (TEST(u->Flags,SPR_SLIDING))
        DoActorSlide(SpriteNum);

    if (u->track >= 0)
        ActorFollowTrack(SpriteNum, ACTORMOVETICS);
    else
        (*u->ActorActionFunc)(SpriteNum);
    
    KeepActorOnFloor(SpriteNum);
}    


