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
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "ai.h"
#include "jsprite.h"

extern BOOL MoveSkip4, MoveSkip2;

short GetDeltaAngle(short ang1, short ang2);
short GetRotation(short sn);
int StateControl(SHORT SpriteNum);
VOID DoActorZrange(short SpriteNum);
void PreCacheRange(short, short);

extern TRACK Track[MAX_TRACKS];
extern SECTOR_OBJECT SectorObject[MAX_SECTOR_OBJECTS];

extern int DirArr[] = {NORTH, NE, EAST, SE, SOUTH, SW, WEST, NW, NORTH, NE, EAST, SE, SOUTH, SW, WEST, NW};

#define SCROLL_RATE 20
#define SCROLL_FIRE_RATE 20

extern BOOL DebugActor;
extern long score;

ANIMATOR DoGet, DoKey, DoSpriteFade;

/////////////////////////////////////////////////////
//  Initialize any of my special use sprites
/////////////////////////////////////////////////////
void
JS_SpriteSetup(void)
    {
    SPRITEp sp;
    short SpriteNum = 0, NextSprite, ndx;
    USERp u;
    short i, num;


    TRAVERSE_SPRITE_STAT(headspritestat[0], SpriteNum, NextSprite)
        {
        short tag;
        short bit;

        sp = &sprite[SpriteNum];
        tag = sp->hitag;

        switch (sprite[SpriteNum].picnum)
            {
        case ST1:
            if (tag == MIRROR_CAM)
                {
                // Just change it to static, sprite has all the info I need
                change_sprite_stat(SpriteNum, STAT_ST1);
                }
            else if (tag == MIRROR_SPAWNSPOT)
                {
                // Just change it to static, sprite has all the info I need
                change_sprite_stat(SpriteNum, STAT_ST1);

                ANIMATOR DoSpawnMirrorGhosts;

                u = SpawnUser(SpriteNum, ST1, NULL);

                change_sprite_stat(SpriteNum, STAT_NO_STATE);
                u->ActorActionFunc = DoSpawnMirrorGhosts;

                u->WaitTics = RandomRange(sp->ang) * 120;

                sp->z += Z(30);
                }
            break;

            }                           // switch
        }                               // TRAVERSE
    }

///////////////////////////////////////////
//  Spawn the coolie ghost for mirrors
///////////////////////////////////////////

int
DoSpawnMirrorGhosts(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];

    if ((u->WaitTics -= synctics) <= 0)
        {
        // if we have erupted for "pal" seconds then stop
        if (u->WaitTics < -((BYTE) (sp->pal) * 120))
            {
            u->Counter = 0;
            u->WaitTics = sp->ang * 120;
            }

        // Counter controls the volume of lava erupting
        // starts out slow and increases to a max
        u->Counter += synctics;
        if (u->Counter > sp->lotag)
            u->Counter = sp->lotag;

        if ((RANDOM_P2(1024 << 6) >> 6) < u->Counter)
            SpawnShrapX(SpriteNum);
        }
    }
