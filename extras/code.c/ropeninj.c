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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "string.h"
#include "ai.h"
#include "pal.h"
#include "player.h"
#include "net.h"

#include "def.h"

/*

 !AIC - Decision tables used in mostly ai.c DoActorActionDecide().
 
*/

// Rope Ninjas
DECISION NinjaRoperRoam[] =
    {
    {1023, InitActorDuck},
    {1024, InitActorAlertNoise},
    };

DECISION NinjaRoperBattle[] =
    {
    {499, InitActorDuck},
    {500, InitActorAlertNoise},
    {1024, InitActorAttack}
    };

PERSONALITY NinjaRoperPersonality =
    {
    NinjaRoperBattle,
    NinjaRoperBattle,
    NinjaRoperRoam,
    NinjaRoperRoam,
    NinjaRoperRoam,
    NinjaRoperRoam,
    NinjaRoperBattle,
    NinjaRoperBattle
    };

    
/*
    
 !AIC - Extra attributes - speeds for running, animation tic adjustments for speeeds, etc

*/    

ATTRIBUTE DefaultAttrib =
    {
    {60, 80, 100, 130},                 // Speeds
    {3, 0, -2, -3},                     // Tic Adjusts
     3,                                  // MaxWeapons;
    {0, 0, 0}                           
    };

//////////////////////
//
// NINJA RUN
//
//////////////////////

ANIMATOR DoNinjaRoperMove, DoNinjaRoperCrawl, DoStayOnFloor, NullNinjaRoper, DoActorJump, DoActorFall, DoActorDebris, DoNinjaRoperHariKari, DoActorSlide;
ANIMATOR InitActorDecide;

#define NINJA_RATE 18

    
//////////////////////
//
// NINJA STAND
//
//////////////////////

#define NINJA_STAND_RATE 10

STATE s_NinjaRoperStand[5][1] =
    {
    {
    {NINJA_STAND_R0 + 0, NINJA_STAND_RATE, DoNinjaRoperMove, &s_NinjaRoperStand[0][0]},
    },
    {
    {NINJA_STAND_R1 + 0, NINJA_STAND_RATE, DoNinjaRoperMove, &s_NinjaRoperStand[1][0]},
    },
    {
    {NINJA_STAND_R2 + 0, NINJA_STAND_RATE, DoNinjaRoperMove, &s_NinjaRoperStand[2][0]},
    },
    {
    {NINJA_STAND_R3 + 0, NINJA_STAND_RATE, DoNinjaRoperMove, &s_NinjaRoperStand[3][0]},
    },
    {
    {NINJA_STAND_R4 + 0, NINJA_STAND_RATE, DoNinjaRoperMove, &s_NinjaRoperStand[4][0]},
    },
    };


STATEp sg_NinjaRoperStand[] =
    {
    s_NinjaRoperStand[0],
    s_NinjaRoperStand[1],
    s_NinjaRoperStand[2],
    s_NinjaRoperStand[3],
    s_NinjaRoperStand[4]
    };
    
//////////////////////
//
// NINJA RISE
//
//////////////////////

#define NINJA_RISE_RATE 10

STATE s_NinjaRoperRise[5][3] =
    {
    {
    {NINJA_KNEEL_R0 + 0, NINJA_RISE_RATE, NullNinjaRoper, &s_NinjaRoperRise[0][1]},
    {NINJA_STAND_R0 + 0, NINJA_STAND_RATE, NullNinjaRoper, &s_NinjaRoperRise[0][2]},
    {NULL, NULL, NULL, sg_NinjaRoperRun},
    },
    {
    {NINJA_KNEEL_R1 + 0, NINJA_RISE_RATE, NullNinjaRoper, &s_NinjaRoperRise[1][1]},
    {NINJA_STAND_R1 + 0, NINJA_STAND_RATE, NullNinjaRoper, &s_NinjaRoperRise[1][2]},
    {NULL, NULL, NULL, sg_NinjaRoperRun},
    },
    {
    {NINJA_KNEEL_R2 + 0, NINJA_RISE_RATE, NullNinjaRoper, &s_NinjaRoperRise[2][1]},
    {NINJA_STAND_R2 + 0, NINJA_STAND_RATE, NullNinjaRoper, &s_NinjaRoperRise[2][2]},
    {NULL, NULL, NULL, sg_NinjaRoperRun},
    },
    {
    {NINJA_KNEEL_R3 + 0, NINJA_RISE_RATE, NullNinjaRoper, &s_NinjaRoperRise[3][1]},
    {NINJA_STAND_R3 + 0, NINJA_STAND_RATE, NullNinjaRoper, &s_NinjaRoperRise[3][2]},
    {NULL, NULL, NULL, sg_NinjaRoperRun},
    },
    {
    {NINJA_KNEEL_R4 + 0, NINJA_RISE_RATE, NullNinjaRoper, &s_NinjaRoperRise[4][1]},
    {NINJA_STAND_R4 + 0, NINJA_STAND_RATE, NullNinjaRoper, &s_NinjaRoperRise[4][2]},
    {NULL, NULL, NULL, sg_NinjaRoperRun},
    },
    };    


STATEp sg_NinjaRoperRise[] =
    {
    s_NinjaRoperRise[0],
    s_NinjaRoperRise[1],
    s_NinjaRoperRise[2],
    s_NinjaRoperRise[3],
    s_NinjaRoperRise[4]
    };


//////////////////////
//
// NINJA CRAWL
//
//////////////////////


#define NINJA_CRAWL_RATE 14
STATE s_NinjaRoperCrawl[5][4] =
    {
    {
    {NINJA_CRAWL_R0 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[0][1]},
    {NINJA_CRAWL_R0 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[0][2]},
    {NINJA_CRAWL_R0 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[0][3]},
    {NINJA_CRAWL_R0 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[0][0]},
    },
    {
    {NINJA_CRAWL_R1 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[1][1]},
    {NINJA_CRAWL_R1 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[1][2]},
    {NINJA_CRAWL_R1 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[1][3]},
    {NINJA_CRAWL_R1 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[1][0]},
    },
    {
    {NINJA_CRAWL_R2 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[2][1]},
    {NINJA_CRAWL_R2 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[2][2]},
    {NINJA_CRAWL_R2 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[2][3]},
    {NINJA_CRAWL_R2 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[2][0]},
    },
    {
    {NINJA_CRAWL_R3 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[3][1]},
    {NINJA_CRAWL_R3 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[3][2]},
    {NINJA_CRAWL_R3 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[3][3]},
    {NINJA_CRAWL_R3 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[3][0]},
    },
    {
    {NINJA_CRAWL_R4 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[4][1]},
    {NINJA_CRAWL_R4 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[4][2]},
    {NINJA_CRAWL_R4 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[4][3]},
    {NINJA_CRAWL_R4 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperCrawl[4][0]},
    },
    };


STATEp sg_NinjaRoperCrawl[] =
    {
    s_NinjaRoperCrawl[0],
    s_NinjaRoperCrawl[1],
    s_NinjaRoperCrawl[2],
    s_NinjaRoperCrawl[3],
    s_NinjaRoperCrawl[4]
    };

//////////////////////
//
// NINJA KNEEL_CRAWL
//
//////////////////////

#define NINJA_KNEEL_CRAWL_RATE 20

STATE s_NinjaRoperKneelCrawl[5][5] =
    {
    {
    {NINJA_KNEEL_R0 + 0, NINJA_KNEEL_CRAWL_RATE, NullNinjaRoper, &s_NinjaRoperKneelCrawl[0][1]},
    {NINJA_CRAWL_R0 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[0][2]},
    {NINJA_CRAWL_R0 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[0][3]},
    {NINJA_CRAWL_R0 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[0][4]},
    {NINJA_CRAWL_R0 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[0][1]},
    },
    {
    {NINJA_KNEEL_R1 + 0, NINJA_KNEEL_CRAWL_RATE, NullNinjaRoper, &s_NinjaRoperKneelCrawl[1][1]},
    {NINJA_CRAWL_R1 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[1][2]},
    {NINJA_CRAWL_R1 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[1][3]},
    {NINJA_CRAWL_R1 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[1][4]},
    {NINJA_CRAWL_R1 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[1][1]},
    },
    {
    {NINJA_KNEEL_R2 + 0, NINJA_KNEEL_CRAWL_RATE, NullNinjaRoper, &s_NinjaRoperKneelCrawl[2][1]},
    {NINJA_CRAWL_R2 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[2][2]},
    {NINJA_CRAWL_R2 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[2][3]},
    {NINJA_CRAWL_R2 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[2][4]},
    {NINJA_CRAWL_R2 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[2][1]},
    },
    {
    {NINJA_KNEEL_R3 + 0, NINJA_KNEEL_CRAWL_RATE, NullNinjaRoper, &s_NinjaRoperKneelCrawl[3][1]},
    {NINJA_CRAWL_R3 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[3][2]},
    {NINJA_CRAWL_R3 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[3][3]},
    {NINJA_CRAWL_R3 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[3][4]},
    {NINJA_CRAWL_R3 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[3][1]},
    },
    {
    {NINJA_KNEEL_R4 + 0, NINJA_KNEEL_CRAWL_RATE, NullNinjaRoper, &s_NinjaRoperKneelCrawl[4][1]},
    {NINJA_CRAWL_R4 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[4][2]},
    {NINJA_CRAWL_R4 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[4][3]},
    {NINJA_CRAWL_R4 + 2, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[4][4]},
    {NINJA_CRAWL_R4 + 1, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperKneelCrawl[4][1]},
    },
    };


STATEp sg_NinjaRoperKneelCrawl[] =
    {
    s_NinjaRoperKneelCrawl[0],
    s_NinjaRoperKneelCrawl[1],
    s_NinjaRoperKneelCrawl[2],
    s_NinjaRoperKneelCrawl[3],
    s_NinjaRoperKneelCrawl[4]
    };


//////////////////////
//
// NINJA DUCK
//
//////////////////////

#define NINJA_DUCK_RATE 10

STATE s_NinjaRoperDuck[5][2] =
    {
    {
    {NINJA_KNEEL_R0 + 0, NINJA_DUCK_RATE, NullNinjaRoper, &s_NinjaRoperDuck[0][1]},
    {NINJA_CRAWL_R0 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperDuck[0][1]},
    },
    {
    {NINJA_KNEEL_R1 + 0, NINJA_DUCK_RATE, NullNinjaRoper, &s_NinjaRoperDuck[1][1]},
    {NINJA_CRAWL_R1 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperDuck[1][1]},
    },
    {
    {NINJA_KNEEL_R2 + 0, NINJA_DUCK_RATE, NullNinjaRoper, &s_NinjaRoperDuck[2][1]},
    {NINJA_CRAWL_R2 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperDuck[2][1]},
    },
    {
    {NINJA_KNEEL_R3 + 0, NINJA_DUCK_RATE, NullNinjaRoper, &s_NinjaRoperDuck[3][1]},
    {NINJA_CRAWL_R3 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperDuck[3][1]},
    },
    {
    {NINJA_KNEEL_R4 + 0, NINJA_DUCK_RATE, NullNinjaRoper, &s_NinjaRoperDuck[4][1]},
    {NINJA_CRAWL_R4 + 0, NINJA_CRAWL_RATE, DoNinjaRoperMove, &s_NinjaRoperDuck[4][1]},
    },
    };


STATEp sg_NinjaRoperDuck[] =
    {
    s_NinjaRoperDuck[0],
    s_NinjaRoperDuck[1],
    s_NinjaRoperDuck[2],
    s_NinjaRoperDuck[3],
    s_NinjaRoperDuck[4]
    };


//////////////////////
//
// NINJA SIT
//
//////////////////////

STATE s_NinjaRoperSit[5][1] =
    {
    {
    {NINJA_KNEEL_R0 + 0, NINJA_RISE_RATE, DoNinjaRoperMove, &s_NinjaRoperSit[0][0]},
    },
    {
    {NINJA_KNEEL_R1 + 0, NINJA_RISE_RATE, DoNinjaRoperMove, &s_NinjaRoperSit[1][0]},
    },
    {
    {NINJA_KNEEL_R2 + 0, NINJA_RISE_RATE, DoNinjaRoperMove, &s_NinjaRoperSit[2][0]},
    },
    {
    {NINJA_KNEEL_R3 + 0, NINJA_RISE_RATE, DoNinjaRoperMove, &s_NinjaRoperSit[3][0]},
    },
    {
    {NINJA_KNEEL_R4 + 0, NINJA_RISE_RATE, DoNinjaRoperMove, &s_NinjaRoperSit[4][0]},
    },
    };


STATEp sg_NinjaRoperSit[] =
    {
    s_NinjaRoperSit[0],
    s_NinjaRoperSit[1],
    s_NinjaRoperSit[2],
    s_NinjaRoperSit[3],
    s_NinjaRoperSit[4]
    };



//////////////////////
//
// NINJA JUMP
//
//////////////////////

#define NINJA_JUMP_RATE 24

STATE s_NinjaRoperJump[5][2] =
    {
    {
    {NINJA_JUMP_R0 + 0, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[0][1]},
    {NINJA_JUMP_R0 + 1, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[0][1]},
    },
    {
    {NINJA_JUMP_R1 + 0, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[1][1]},
    {NINJA_JUMP_R1 + 1, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[1][1]},
    },
    {
    {NINJA_JUMP_R2 + 0, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[2][1]},
    {NINJA_JUMP_R2 + 1, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[2][1]},
    },
    {
    {NINJA_JUMP_R3 + 0, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[3][1]},
    {NINJA_JUMP_R3 + 1, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[3][1]},
    },
    {
    {NINJA_JUMP_R4 + 0, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[4][1]},
    {NINJA_JUMP_R4 + 1, NINJA_JUMP_RATE, DoNinjaRoperMove, &s_NinjaRoperJump[4][1]},
    },
    };


STATEp sg_NinjaRoperJump[] =
    {
    s_NinjaRoperJump[0],
    s_NinjaRoperJump[1],
    s_NinjaRoperJump[2],
    s_NinjaRoperJump[3],
    s_NinjaRoperJump[4]
    };


//////////////////////
//
// NINJA FALL
//
//////////////////////

#define NINJA_FALL_RATE 16

STATE s_NinjaRoperFall[5][2] =
    {
    {
    {NINJA_JUMP_R0 + 1, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[0][1]},
    {NINJA_JUMP_R0 + 2, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[0][1]},
    },
    {
    {NINJA_JUMP_R1 + 1, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[1][1]},
    {NINJA_JUMP_R1 + 2, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[1][1]},
    },
    {
    {NINJA_JUMP_R2 + 1, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[2][1]},
    {NINJA_JUMP_R2 + 2, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[2][1]},
    },
    {
    {NINJA_JUMP_R3 + 1, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[3][1]},
    {NINJA_JUMP_R3 + 2, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[3][1]},
    },
    {
    {NINJA_JUMP_R4 + 1, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[4][1]},
    {NINJA_JUMP_R4 + 2, NINJA_FALL_RATE, DoNinjaRoperMove, &s_NinjaRoperFall[4][1]},
    },
    };


STATEp sg_NinjaRoperFall[] =
    {
    s_NinjaRoperFall[0],
    s_NinjaRoperFall[1],
    s_NinjaRoperFall[2],
    s_NinjaRoperFall[3],
    s_NinjaRoperFall[4]
    };

//////////////////////
//
// NINJA SWIM
//
//////////////////////


#define NINJA_SWIM_RATE 18
STATE s_NinjaRoperSwim[5][3] =
    {
    {
    {NINJA_SWIM_R0 + 1, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[0][1]},
    {NINJA_SWIM_R0 + 2, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[0][2]},
    {NINJA_SWIM_R0 + 3, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[0][0]},
    },
    {
    {NINJA_SWIM_R1 + 1, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[1][1]},
    {NINJA_SWIM_R1 + 2, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[1][2]},
    {NINJA_SWIM_R1 + 3, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[1][0]},
    },
    {
    {NINJA_SWIM_R2 + 1, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[2][1]},
    {NINJA_SWIM_R2 + 2, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[2][2]},
    {NINJA_SWIM_R2 + 3, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[2][0]},
    },
    {
    {NINJA_SWIM_R3 + 1, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[3][1]},
    {NINJA_SWIM_R3 + 2, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[3][2]},
    {NINJA_SWIM_R3 + 3, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[3][0]},
    },
    {
    {NINJA_SWIM_R4 + 1, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[4][1]},
    {NINJA_SWIM_R4 + 2, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[4][2]},
    {NINJA_SWIM_R4 + 3, NINJA_SWIM_RATE, DoNinjaRoperMove, &s_NinjaRoperSwim[4][0]},
    },
    };
    

STATEp sg_NinjaRoperSwim[] =
    {
    s_NinjaRoperSwim[0],
    s_NinjaRoperSwim[1],
    s_NinjaRoperSwim[2],
    s_NinjaRoperSwim[3],
    s_NinjaRoperSwim[4]
    };

//////////////////////
//
// NINJA DIVE
//
//////////////////////


#define NINJA_DIVE_RATE 23

STATE s_NinjaRoperDive[5][4] =
    {
    {
    {NINJA_SWIM_R0 + 0, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[0][1]},
    {NINJA_SWIM_R0 + 1, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[0][2]},
    {NINJA_SWIM_R0 + 2, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[0][3]},
    {NINJA_SWIM_R0 + 3, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[0][0]},
    },
    {
    {NINJA_SWIM_R1 + 0, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[1][1]},
    {NINJA_SWIM_R1 + 1, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[1][2]},
    {NINJA_SWIM_R1 + 2, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[1][3]},
    {NINJA_SWIM_R1 + 3, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[1][0]},
    },
    {
    {NINJA_SWIM_R2 + 0, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[2][1]},
    {NINJA_SWIM_R2 + 1, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[2][2]},
    {NINJA_SWIM_R2 + 2, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[2][3]},
    {NINJA_SWIM_R2 + 3, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[2][0]},
    },
    {
    {NINJA_SWIM_R3 + 0, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[3][1]},
    {NINJA_SWIM_R3 + 1, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[3][2]},
    {NINJA_SWIM_R3 + 2, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[3][3]},
    {NINJA_SWIM_R3 + 3, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[3][0]},
    },
    {
    {NINJA_SWIM_R4 + 0, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[4][1]},
    {NINJA_SWIM_R4 + 1, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[4][2]},
    {NINJA_SWIM_R4 + 2, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[4][3]},
    {NINJA_SWIM_R4 + 3, NINJA_DIVE_RATE, DoNinjaRoperMove, &s_NinjaRoperDive[4][0]},
    },
    };

STATEp sg_NinjaRoperDive[] =
    {
    s_NinjaRoperDive[0],
    s_NinjaRoperDive[1],
    s_NinjaRoperDive[2],
    s_NinjaRoperDive[3],
    s_NinjaRoperDive[4]
    };

//////////////////////
//
// NINJA CLIMB
//
//////////////////////


#define NINJA_CLIMB_RATE 20
STATE s_NinjaRoperClimb[5][4] =
    {
    {
    {NINJA_CLIMB_R0 + 0, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[0][1]},
    {NINJA_CLIMB_R0 + 1, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[0][2]},
    {NINJA_CLIMB_R0 + 2, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[0][3]},
    {NINJA_CLIMB_R0 + 3, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[0][0]},
    },
    {
    {NINJA_CLIMB_R1 + 0, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[1][1]},
    {NINJA_CLIMB_R1 + 1, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[1][2]},
    {NINJA_CLIMB_R1 + 2, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[1][3]},
    {NINJA_CLIMB_R1 + 3, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[1][0]},
    },
    {
    {NINJA_CLIMB_R4 + 0, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[2][1]},
    {NINJA_CLIMB_R4 + 1, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[2][2]},
    {NINJA_CLIMB_R4 + 2, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[2][3]},
    {NINJA_CLIMB_R4 + 3, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[2][0]},
    },
    {
    {NINJA_CLIMB_R3 + 0, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[3][1]},
    {NINJA_CLIMB_R3 + 1, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[3][2]},
    {NINJA_CLIMB_R3 + 2, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[3][3]},
    {NINJA_CLIMB_R3 + 3, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[3][0]},
    },
    {
    {NINJA_CLIMB_R2 + 0, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[4][1]},
    {NINJA_CLIMB_R2 + 1, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[4][2]},
    {NINJA_CLIMB_R2 + 2, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[4][3]},
    {NINJA_CLIMB_R2 + 3, NINJA_CLIMB_RATE, DoNinjaRoperMove, &s_NinjaRoperClimb[4][0]},
    },
    };

STATEp sg_NinjaRoperClimb[] =
    {
    s_NinjaRoperClimb[0],
    s_NinjaRoperClimb[1],
    s_NinjaRoperClimb[2],
    s_NinjaRoperClimb[3],
    s_NinjaRoperClimb[4]
    };


//////////////////////
//
// NINJA FLY
//
//////////////////////

#define NINJA_FLY_RATE 12

STATE s_NinjaRoperFly[5][1] =
    {
    {
    {NINJA_FLY_R0 + 0, NINJA_FLY_RATE, DoNinjaRoperMove, &s_NinjaRoperFly[0][0]},
    },
    {
    {NINJA_FLY_R1 + 0, NINJA_FLY_RATE, DoNinjaRoperMove, &s_NinjaRoperFly[1][0]},
    },
    {
    {NINJA_FLY_R2 + 0, NINJA_FLY_RATE, DoNinjaRoperMove, &s_NinjaRoperFly[2][0]},
    },
    {
    {NINJA_FLY_R3 + 0, NINJA_FLY_RATE, DoNinjaRoperMove, &s_NinjaRoperFly[3][0]},
    },
    {
    {NINJA_FLY_R4 + 0, NINJA_FLY_RATE, DoNinjaRoperMove, &s_NinjaRoperFly[4][0]},
    },
    };    

STATEp sg_NinjaRoperFly[] =
    {
    s_NinjaRoperFly[0],
    s_NinjaRoperFly[1],
    s_NinjaRoperFly[2],
    s_NinjaRoperFly[3],
    s_NinjaRoperFly[4]
    };

//////////////////////
//
// NINJA PAIN
//
//////////////////////

#define NINJA_PAIN_RATE 15
ANIMATOR DoNinjaRoperPain;

STATE s_NinjaRoperPain[5][2] =
    {
    {
    {NINJA_PAIN_R0 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[0][1]},
    {NINJA_PAIN_R0 + 1, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[0][1]},
    },
    {
    {NINJA_STAND_R1 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[1][1]},
    {NINJA_STAND_R1 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[1][1]},
    },
    {
    {NINJA_STAND_R2 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[2][1]},
    {NINJA_STAND_R2 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[2][1]},
    },
    {
    {NINJA_STAND_R3 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[3][1]},
    {NINJA_STAND_R3 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[3][1]},
    },
    {
    {NINJA_STAND_R4 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[4][1]},
    {NINJA_STAND_R4 + 0, NINJA_PAIN_RATE, DoNinjaRoperPain, &s_NinjaRoperPain[4][1]},
    },
    };    
    
STATEp sg_NinjaRoperPain[] =
    {
    s_NinjaRoperPain[0],
    s_NinjaRoperPain[1],
    s_NinjaRoperPain[2],
    s_NinjaRoperPain[3],
    s_NinjaRoperPain[4]
    };

//////////////////////
//
// NINJA STAR
//
//////////////////////

#define NINJA_STAR_RATE 18
ANIMATOR InitEnemyStar;

STATE s_NinjaRoperStar[5][6] =
    {
    {
    {NINJA_THROW_R0 + 0, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[0][1]},
    {NINJA_THROW_R0 + 0, NINJA_STAR_RATE,       NullNinjaRoper,          &s_NinjaRoperStar[0][2]},
    {NINJA_THROW_R0 + 1, 0 | SF_QUICK_CALL,         InitEnemyStar,      &s_NinjaRoperStar[0][3]},
    {NINJA_THROW_R0 + 1, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[0][4]},
    {NINJA_THROW_R0 + 2, 0 | SF_QUICK_CALL,         InitActorDecide,    &s_NinjaRoperStar[0][5]},
    {NINJA_THROW_R0 + 2, NINJA_STAR_RATE,       DoNinjaRoperMove,        &s_NinjaRoperStar[0][5]},
    },
    {
    {NINJA_THROW_R1 + 0, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[1][1]},
    {NINJA_THROW_R1 + 0, NINJA_STAR_RATE,       NullNinjaRoper,          &s_NinjaRoperStar[1][2]},
    {NINJA_THROW_R1 + 1, 0 | SF_QUICK_CALL,         InitEnemyStar,      &s_NinjaRoperStar[1][3]},
    {NINJA_THROW_R1 + 1, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[1][4]},
    {NINJA_THROW_R1 + 2, 0 | SF_QUICK_CALL,         InitActorDecide,    &s_NinjaRoperStar[1][5]},
    {NINJA_THROW_R1 + 2, NINJA_STAR_RATE,       DoNinjaRoperMove,        &s_NinjaRoperStar[1][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[2][1]},
    {NINJA_THROW_R2 + 0, NINJA_STAR_RATE,       NullNinjaRoper,          &s_NinjaRoperStar[2][2]},
    {NINJA_THROW_R2 + 1, 0 | SF_QUICK_CALL,         InitEnemyStar,      &s_NinjaRoperStar[2][3]},
    {NINJA_THROW_R2 + 1, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[2][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL,         InitActorDecide,    &s_NinjaRoperStar[2][5]},
    {NINJA_THROW_R2 + 2, NINJA_STAR_RATE,       DoNinjaRoperMove,        &s_NinjaRoperStar[2][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[3][1]},
    {NINJA_THROW_R2 + 0, NINJA_STAR_RATE,       NullNinjaRoper,          &s_NinjaRoperStar[3][2]},
    {NINJA_THROW_R2 + 1, 0 | SF_QUICK_CALL,         InitEnemyStar,      &s_NinjaRoperStar[3][3]},
    {NINJA_THROW_R2 + 1, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[3][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL,         InitActorDecide,    &s_NinjaRoperStar[3][5]},
    {NINJA_THROW_R2 + 2, NINJA_STAR_RATE,       DoNinjaRoperMove,        &s_NinjaRoperStar[3][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[4][1]},
    {NINJA_THROW_R2 + 0, NINJA_STAR_RATE,       NullNinjaRoper,          &s_NinjaRoperStar[4][2]},
    {NINJA_THROW_R2 + 1, 0 | SF_QUICK_CALL,         InitEnemyStar,      &s_NinjaRoperStar[4][3]},
    {NINJA_THROW_R2 + 1, NINJA_STAR_RATE * 2,     NullNinjaRoper,            &s_NinjaRoperStar[4][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL,         InitActorDecide,    &s_NinjaRoperStar[4][5]},
    {NINJA_THROW_R2 + 2, NINJA_STAR_RATE,       DoNinjaRoperMove,        &s_NinjaRoperStar[4][5]},
    },
    };

STATEp sg_NinjaRoperStar[] =
    {
    s_NinjaRoperStar[0],
    s_NinjaRoperStar[1],
    s_NinjaRoperStar[2],
    s_NinjaRoperStar[3],
    s_NinjaRoperStar[4]
    };

//////////////////////
//
// NINJA MIRV
//
//////////////////////

#define NINJA_MIRV_RATE 18
ANIMATOR InitEnemyMirv;

STATE s_NinjaRoperMirv[5][6] =
    {
    {
    {NINJA_THROW_R0 + 0, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[0][1]},
    {NINJA_THROW_R0 + 1, NINJA_MIRV_RATE, NullNinjaRoper, &s_NinjaRoperMirv[0][2]},
    {NINJA_THROW_R0 + 2, 0 | SF_QUICK_CALL, InitEnemyMirv, &s_NinjaRoperMirv[0][3]},
    {NINJA_THROW_R0 + 2, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[0][4]},
    {NINJA_THROW_R0 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperMirv[0][5]},
    {NINJA_THROW_R0 + 2, NINJA_MIRV_RATE, DoNinjaRoperMove, &s_NinjaRoperMirv[0][5]},
    },
    {
    {NINJA_THROW_R1 + 0, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[1][1]},
    {NINJA_THROW_R1 + 1, NINJA_MIRV_RATE, NullNinjaRoper, &s_NinjaRoperMirv[1][2]},
    {NINJA_THROW_R1 + 2, 0 | SF_QUICK_CALL, InitEnemyMirv, &s_NinjaRoperMirv[1][3]},
    {NINJA_THROW_R1 + 2, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[1][4]},
    {NINJA_THROW_R1 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperMirv[1][5]},
    {NINJA_THROW_R1 + 2, NINJA_MIRV_RATE, DoNinjaRoperMove, &s_NinjaRoperMirv[1][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[2][1]},
    {NINJA_THROW_R2 + 1, NINJA_MIRV_RATE, NullNinjaRoper, &s_NinjaRoperMirv[2][2]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitEnemyMirv, &s_NinjaRoperMirv[2][3]},
    {NINJA_THROW_R2 + 2, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[2][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperMirv[2][5]},
    {NINJA_THROW_R2 + 2, NINJA_MIRV_RATE, DoNinjaRoperMove, &s_NinjaRoperMirv[2][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[3][1]},
    {NINJA_THROW_R2 + 1, NINJA_MIRV_RATE, NullNinjaRoper, &s_NinjaRoperMirv[3][2]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitEnemyMirv, &s_NinjaRoperMirv[3][3]},
    {NINJA_THROW_R2 + 2, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[3][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperMirv[3][5]},
    {NINJA_THROW_R2 + 2, NINJA_MIRV_RATE, DoNinjaRoperMove, &s_NinjaRoperMirv[3][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[4][1]},
    {NINJA_THROW_R2 + 1, NINJA_MIRV_RATE, NullNinjaRoper, &s_NinjaRoperMirv[4][2]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitEnemyMirv, &s_NinjaRoperMirv[4][3]},
    {NINJA_THROW_R2 + 2, NINJA_MIRV_RATE * 2, NullNinjaRoper, &s_NinjaRoperMirv[4][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperMirv[4][5]},
    {NINJA_THROW_R2 + 2, NINJA_MIRV_RATE, DoNinjaRoperMove, &s_NinjaRoperStar[4][5]},
    },
    };


STATEp sg_NinjaRoperMirv[] =
    {
    s_NinjaRoperMirv[0],
    s_NinjaRoperMirv[1],
    s_NinjaRoperMirv[2],
    s_NinjaRoperMirv[3],
    s_NinjaRoperMirv[4]
    };
    
//////////////////////
//
// NINJA NAPALM
//
//////////////////////

#define NINJA_NAPALM_RATE 18
ANIMATOR InitEnemyNapalm;

STATE s_NinjaRoperNapalm[5][6] =
    {
    {
    {NINJA_THROW_R0 + 0, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[0][1]},
    {NINJA_THROW_R0 + 1, NINJA_NAPALM_RATE, NullNinjaRoper, &s_NinjaRoperNapalm[0][2]},
    {NINJA_THROW_R0 + 2, 0 | SF_QUICK_CALL, InitEnemyNapalm, &s_NinjaRoperNapalm[0][3]},
    {NINJA_THROW_R0 + 2, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[0][4]},
    {NINJA_THROW_R0 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperNapalm[0][5]},
    {NINJA_THROW_R0 + 2, NINJA_NAPALM_RATE, DoNinjaRoperMove, &s_NinjaRoperNapalm[0][5]},
    },
    {
    {NINJA_THROW_R1 + 0, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[1][1]},
    {NINJA_THROW_R1 + 1, NINJA_NAPALM_RATE, NullNinjaRoper, &s_NinjaRoperNapalm[1][2]},
    {NINJA_THROW_R1 + 2, 0 | SF_QUICK_CALL, InitEnemyNapalm, &s_NinjaRoperNapalm[1][3]},
    {NINJA_THROW_R1 + 2, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[1][4]},
    {NINJA_THROW_R1 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperNapalm[1][5]},
    {NINJA_THROW_R1 + 2, NINJA_NAPALM_RATE, DoNinjaRoperMove, &s_NinjaRoperNapalm[1][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[2][1]},
    {NINJA_THROW_R2 + 1, NINJA_NAPALM_RATE, NullNinjaRoper, &s_NinjaRoperNapalm[2][2]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitEnemyNapalm, &s_NinjaRoperNapalm[2][3]},
    {NINJA_THROW_R2 + 2, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[2][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperNapalm[2][5]},
    {NINJA_THROW_R2 + 2, NINJA_NAPALM_RATE, DoNinjaRoperMove, &s_NinjaRoperNapalm[2][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[3][1]},
    {NINJA_THROW_R2 + 1, NINJA_NAPALM_RATE, NullNinjaRoper, &s_NinjaRoperNapalm[3][2]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitEnemyNapalm, &s_NinjaRoperNapalm[3][3]},
    {NINJA_THROW_R2 + 2, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[3][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperNapalm[3][5]},
    {NINJA_THROW_R2 + 2, NINJA_NAPALM_RATE, DoNinjaRoperMove, &s_NinjaRoperNapalm[3][5]},
    },
    {
    {NINJA_THROW_R2 + 0, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[4][1]},
    {NINJA_THROW_R2 + 1, NINJA_NAPALM_RATE, NullNinjaRoper, &s_NinjaRoperNapalm[4][2]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitEnemyNapalm, &s_NinjaRoperNapalm[4][3]},
    {NINJA_THROW_R2 + 2, NINJA_NAPALM_RATE * 2, NullNinjaRoper, &s_NinjaRoperNapalm[4][4]},
    {NINJA_THROW_R2 + 2, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperNapalm[4][5]},
    {NINJA_THROW_R2 + 2, NINJA_NAPALM_RATE, DoNinjaRoperMove, &s_NinjaRoperStar[4][5]},
    },
    };


STATEp sg_NinjaRoperNapalm[] =
    {
    s_NinjaRoperNapalm[0],
    s_NinjaRoperNapalm[1],
    s_NinjaRoperNapalm[2],
    s_NinjaRoperNapalm[3],
    s_NinjaRoperNapalm[4]
    };
    
    
//////////////////////
//
// NINJA ROCKET
//
//////////////////////

#define NINJA_ROCKET_RATE 14
ANIMATOR InitEnemyRocket;

STATE s_NinjaRoperRocket[5][5] =
    {
    {
    {NINJA_STAND_R0 + 0, NINJA_ROCKET_RATE * 2, NullNinjaRoper, &s_NinjaRoperRocket[0][1]},
    {NINJA_STAND_R0 + 0, 0 | SF_QUICK_CALL, InitEnemyRocket, &s_NinjaRoperRocket[0][2]},
    {NINJA_STAND_R0 + 0, NINJA_ROCKET_RATE, NullNinjaRoper, &s_NinjaRoperRocket[0][3]},
    {NINJA_STAND_R0 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperRocket[0][4]},
    {NINJA_STAND_R0 + 0, NINJA_ROCKET_RATE, DoNinjaRoperMove, &s_NinjaRoperRocket[0][4]},
    },
    {
    {NINJA_STAND_R1 + 0, NINJA_ROCKET_RATE * 2, NullNinjaRoper, &s_NinjaRoperRocket[1][1]},
    {NINJA_STAND_R1 + 0, 0 | SF_QUICK_CALL, InitEnemyRocket, &s_NinjaRoperRocket[1][2]},
    {NINJA_STAND_R1 + 0, NINJA_ROCKET_RATE, NullNinjaRoper, &s_NinjaRoperRocket[1][3]},
    {NINJA_STAND_R1 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperRocket[1][4]},
    {NINJA_STAND_R1 + 0, NINJA_ROCKET_RATE, DoNinjaRoperMove, &s_NinjaRoperRocket[1][4]},
    },
    {
    {NINJA_STAND_R2 + 0, NINJA_ROCKET_RATE * 2, NullNinjaRoper, &s_NinjaRoperRocket[2][1]},
    {NINJA_STAND_R2 + 0, 0 | SF_QUICK_CALL, InitEnemyRocket, &s_NinjaRoperRocket[2][2]},
    {NINJA_STAND_R2 + 0, NINJA_ROCKET_RATE, NullNinjaRoper, &s_NinjaRoperRocket[2][3]},
    {NINJA_STAND_R2 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperRocket[2][4]},
    {NINJA_STAND_R2 + 0, NINJA_ROCKET_RATE, DoNinjaRoperMove, &s_NinjaRoperRocket[2][4]},
    },
    {
    {NINJA_STAND_R3 + 0, NINJA_ROCKET_RATE * 2, NullNinjaRoper, &s_NinjaRoperRocket[3][1]},
    {NINJA_STAND_R3 + 0, 0 | SF_QUICK_CALL, InitEnemyRocket, &s_NinjaRoperRocket[3][2]},
    {NINJA_STAND_R3 + 0, NINJA_ROCKET_RATE, NullNinjaRoper, &s_NinjaRoperRocket[3][3]},
    {NINJA_STAND_R3 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperRocket[3][4]},
    {NINJA_STAND_R3 + 0, NINJA_ROCKET_RATE, DoNinjaRoperMove, &s_NinjaRoperRocket[3][4]},
    },
    {
    {NINJA_STAND_R4 + 0, NINJA_ROCKET_RATE * 2, NullNinjaRoper, &s_NinjaRoperRocket[4][1]},
    {NINJA_STAND_R4 + 0, 0 | SF_QUICK_CALL, InitEnemyRocket, &s_NinjaRoperRocket[4][2]},
    {NINJA_STAND_R4 + 0, NINJA_ROCKET_RATE, NullNinjaRoper, &s_NinjaRoperRocket[4][3]},
    {NINJA_STAND_R4 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperRocket[4][4]},
    {NINJA_STAND_R4 + 0, NINJA_ROCKET_RATE, DoNinjaRoperMove, &s_NinjaRoperRocket[4][4]},
    },
    };


STATEp sg_NinjaRoperRocket[] =
    {
    s_NinjaRoperRocket[0],
    s_NinjaRoperRocket[1],
    s_NinjaRoperRocket[2],
    s_NinjaRoperRocket[3],
    s_NinjaRoperRocket[4]
    };

//////////////////////
//
// NINJA FLASHBOMB
//
//////////////////////

#define NINJA_FLASHBOMB_RATE 14
ANIMATOR InitFlashBomb;

STATE s_NinjaRoperFlashBomb[5][5] =
    {
    {
    {NINJA_STAND_R0 + 0, NINJA_FLASHBOMB_RATE * 2, NullNinjaRoper, &s_NinjaRoperFlashBomb[0][1]},
    {NINJA_STAND_R0 + 0, 0 | SF_QUICK_CALL, InitFlashBomb, &s_NinjaRoperFlashBomb[0][2]},
    {NINJA_STAND_R0 + 0, NINJA_FLASHBOMB_RATE, NullNinjaRoper, &s_NinjaRoperFlashBomb[0][3]},
    {NINJA_STAND_R0 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperFlashBomb[0][4]},
    {NINJA_STAND_R0 + 0, NINJA_FLASHBOMB_RATE, DoNinjaRoperMove, &s_NinjaRoperFlashBomb[0][4]},
    },
    {
    {NINJA_STAND_R1 + 0, NINJA_FLASHBOMB_RATE * 2, NullNinjaRoper, &s_NinjaRoperFlashBomb[1][1]},
    {NINJA_STAND_R1 + 0, 0 | SF_QUICK_CALL, InitFlashBomb, &s_NinjaRoperFlashBomb[1][2]},
    {NINJA_STAND_R1 + 0, NINJA_FLASHBOMB_RATE, NullNinjaRoper, &s_NinjaRoperFlashBomb[1][3]},
    {NINJA_STAND_R1 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperFlashBomb[1][4]},
    {NINJA_STAND_R1 + 0, NINJA_FLASHBOMB_RATE, DoNinjaRoperMove, &s_NinjaRoperFlashBomb[1][4]},
    },
    {
    {NINJA_STAND_R2 + 0, NINJA_FLASHBOMB_RATE * 2, NullNinjaRoper, &s_NinjaRoperFlashBomb[2][1]},
    {NINJA_STAND_R2 + 0, 0 | SF_QUICK_CALL, InitFlashBomb, &s_NinjaRoperFlashBomb[2][2]},
    {NINJA_STAND_R2 + 0, NINJA_FLASHBOMB_RATE, NullNinjaRoper, &s_NinjaRoperFlashBomb[2][3]},
    {NINJA_STAND_R2 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperFlashBomb[2][4]},
    {NINJA_STAND_R2 + 0, NINJA_FLASHBOMB_RATE, DoNinjaRoperMove, &s_NinjaRoperFlashBomb[2][4]},
    },
    {
    {NINJA_STAND_R3 + 0, NINJA_FLASHBOMB_RATE * 2, NullNinjaRoper, &s_NinjaRoperFlashBomb[3][1]},
    {NINJA_STAND_R3 + 0, 0 | SF_QUICK_CALL, InitFlashBomb, &s_NinjaRoperFlashBomb[3][2]},
    {NINJA_STAND_R3 + 0, NINJA_FLASHBOMB_RATE, NullNinjaRoper, &s_NinjaRoperFlashBomb[3][3]},
    {NINJA_STAND_R3 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperFlashBomb[3][4]},
    {NINJA_STAND_R3 + 0, NINJA_FLASHBOMB_RATE, DoNinjaRoperMove, &s_NinjaRoperFlashBomb[3][4]},
    },
    {
    {NINJA_STAND_R4 + 0, NINJA_FLASHBOMB_RATE * 2, NullNinjaRoper, &s_NinjaRoperFlashBomb[4][1]},
    {NINJA_STAND_R4 + 0, 0 | SF_QUICK_CALL, InitFlashBomb, &s_NinjaRoperFlashBomb[4][2]},
    {NINJA_STAND_R4 + 0, NINJA_FLASHBOMB_RATE, NullNinjaRoper, &s_NinjaRoperFlashBomb[4][3]},
    {NINJA_STAND_R4 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperFlashBomb[4][4]},
    {NINJA_STAND_R4 + 0, NINJA_FLASHBOMB_RATE, DoNinjaRoperMove, &s_NinjaRoperFlashBomb[4][4]},
    },
    };


STATEp sg_NinjaRoperFlashBomb[] =
    {
    s_NinjaRoperFlashBomb[0],
    s_NinjaRoperFlashBomb[1],
    s_NinjaRoperFlashBomb[2],
    s_NinjaRoperFlashBomb[3],
    s_NinjaRoperFlashBomb[4]
    };

//////////////////////
//
// NINJA UZI
//
//////////////////////

#define NINJA_UZI_RATE 8
ANIMATOR InitEnemyUzi,CheckRopeFire;

STATE s_NinjaRoperUzi[5][17] =
    {
    {
    {NINJA_FIRE_R0 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][1]},
    {NINJA_FIRE_R0 + 0, 0 | SF_QUICK_CALL, CheckRopeFire, &s_NinjaRoperUzi[0][2]},
    {NINJA_FIRE_R0 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][3]},
    {NINJA_FIRE_R0 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][4]},
    {NINJA_FIRE_R0 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][5]},
    {NINJA_FIRE_R0 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][6]},
    {NINJA_FIRE_R0 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][7]},
    {NINJA_FIRE_R0 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][8]},
    {NINJA_FIRE_R0 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][9]},
    {NINJA_FIRE_R0 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][10]},
    {NINJA_FIRE_R0 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][11]},
    {NINJA_FIRE_R0 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][12]},
    {NINJA_FIRE_R0 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][13]},
    {NINJA_FIRE_R0 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][14]},
    {NINJA_FIRE_R0 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[0][15]},
    {NINJA_FIRE_R0 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[0][16]},
    {NINJA_FIRE_R0 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperUzi[0][16]},
    },
    {
    {NINJA_FIRE_R1 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][1]},
    {NINJA_FIRE_R1 + 0, 0 | SF_QUICK_CALL, CheckRopeFire, &s_NinjaRoperUzi[1][2]},
    {NINJA_FIRE_R1 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][3]},
    {NINJA_FIRE_R1 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][4]},
    {NINJA_FIRE_R1 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][5]},
    {NINJA_FIRE_R1 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][6]},
    {NINJA_FIRE_R1 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][7]},
    {NINJA_FIRE_R1 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][8]},
    {NINJA_FIRE_R1 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][9]},
    {NINJA_FIRE_R1 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][10]},
    {NINJA_FIRE_R1 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][11]},
    {NINJA_FIRE_R1 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][12]},
    {NINJA_FIRE_R1 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][13]},
    {NINJA_FIRE_R1 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][14]},
    {NINJA_FIRE_R1 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[1][15]},
    {NINJA_FIRE_R1 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[1][16]},
    {NINJA_FIRE_R1 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperUzi[1][16]},
    },
    {
    {NINJA_FIRE_R2 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][1]},
    {NINJA_FIRE_R2 + 0, 0 | SF_QUICK_CALL, CheckRopeFire, &s_NinjaRoperUzi[2][2]},
    {NINJA_FIRE_R2 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][3]},
    {NINJA_FIRE_R2 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][4]},
    {NINJA_FIRE_R2 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][5]},
    {NINJA_FIRE_R2 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][6]},
    {NINJA_FIRE_R2 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][7]},
    {NINJA_FIRE_R2 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][8]},
    {NINJA_FIRE_R2 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][9]},
    {NINJA_FIRE_R2 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][10]},
    {NINJA_FIRE_R2 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][11]},
    {NINJA_FIRE_R2 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][12]},
    {NINJA_FIRE_R2 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][13]},
    {NINJA_FIRE_R2 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][14]},
    {NINJA_FIRE_R2 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[2][15]},
    {NINJA_FIRE_R2 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[2][16]},
    {NINJA_FIRE_R2 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperUzi[2][16]},
    },
    {
    {NINJA_FIRE_R3 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][1]},
    {NINJA_FIRE_R3 + 0, 0 | SF_QUICK_CALL, CheckRopeFire, &s_NinjaRoperUzi[3][2]},
    {NINJA_FIRE_R3 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][3]},
    {NINJA_FIRE_R3 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][4]},
    {NINJA_FIRE_R3 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][5]},
    {NINJA_FIRE_R3 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][6]},
    {NINJA_FIRE_R3 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][7]},
    {NINJA_FIRE_R3 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][8]},
    {NINJA_FIRE_R3 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][9]},
    {NINJA_FIRE_R3 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][10]},
    {NINJA_FIRE_R3 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][11]},
    {NINJA_FIRE_R3 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][12]},
    {NINJA_FIRE_R3 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][13]},
    {NINJA_FIRE_R3 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][14]},
    {NINJA_FIRE_R3 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[3][15]},
    {NINJA_FIRE_R3 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[3][16]},
    {NINJA_FIRE_R3 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperUzi[3][16]},
    },
    {
    {NINJA_FIRE_R4 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][1]},
    {NINJA_FIRE_R4 + 0, 0 | SF_QUICK_CALL, CheckRopeFire, &s_NinjaRoperUzi[4][2]},
    {NINJA_FIRE_R4 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][3]},
    {NINJA_FIRE_R4 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][4]},
    {NINJA_FIRE_R4 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][5]},
    {NINJA_FIRE_R4 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][6]},
    {NINJA_FIRE_R4 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][7]},
    {NINJA_FIRE_R4 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][8]},
    {NINJA_FIRE_R4 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][9]},
    {NINJA_FIRE_R4 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][10]},
    {NINJA_FIRE_R4 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][11]},
    {NINJA_FIRE_R4 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][12]},
    {NINJA_FIRE_R4 + 0, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][13]},
    {NINJA_FIRE_R4 + 0, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][14]},
    {NINJA_FIRE_R4 + 1, NINJA_UZI_RATE, NullNinjaRoper, &s_NinjaRoperUzi[4][15]},
    {NINJA_FIRE_R4 + 1, 0 | SF_QUICK_CALL, InitEnemyUzi, &s_NinjaRoperUzi[4][16]},
    {NINJA_FIRE_R4 + 0, 0 | SF_QUICK_CALL, InitActorDecide, &s_NinjaRoperUzi[4][16]},
    },
    };


STATEp sg_NinjaRoperUzi[] =
    {
    s_NinjaRoperUzi[0],
    s_NinjaRoperUzi[1],
    s_NinjaRoperUzi[2],
    s_NinjaRoperUzi[3],
    s_NinjaRoperUzi[4]
    };


//////////////////////
//
// NINJA HARI KARI
//
//////////////////////

#define NINJA_HARI_KARI_WAIT_RATE 200
#define NINJA_HARI_KARI_FALL_RATE 16
ANIMATOR DoHariKariBlood;
ANIMATOR DoNinjaRoperSpecial;

STATE s_NinjaRoperHariKari[] =
    {
    {NINJA_HARI_KARI_R0 + 0,   NINJA_HARI_KARI_FALL_RATE,      NullNinjaRoper,       &s_NinjaRoperHariKari[1]},
    {NINJA_HARI_KARI_R0 + 0,   SF_QUICK_CALL,                  DoNinjaRoperSpecial,       &s_NinjaRoperHariKari[2]},
    {NINJA_HARI_KARI_R0 + 1,   NINJA_HARI_KARI_WAIT_RATE,      NullNinjaRoper,       &s_NinjaRoperHariKari[3]},
    {NINJA_HARI_KARI_R0 + 2,   SF_QUICK_CALL,                  DoNinjaRoperHariKari, &s_NinjaRoperHariKari[4]},
    {NINJA_HARI_KARI_R0 + 2,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[5]},
    {NINJA_HARI_KARI_R0 + 3,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[6]},
    {NINJA_HARI_KARI_R0 + 4,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[7]},
    {NINJA_HARI_KARI_R0 + 5,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[8]},
    {NINJA_HARI_KARI_R0 + 6,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[9]},
    {NINJA_HARI_KARI_R0 + 7,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[10]},
    {NINJA_HARI_KARI_R0 + 7,   NINJA_HARI_KARI_FALL_RATE,      NullAnimator,       &s_NinjaRoperHariKari[10]},
    };

STATEp sg_NinjaRoperHariKari[] =
    {
    s_NinjaRoperHariKari,
    s_NinjaRoperHariKari,
    s_NinjaRoperHariKari,
    s_NinjaRoperHariKari,
    s_NinjaRoperHariKari
    };


//////////////////////
//
// NINJA GRAB THROAT
//
//////////////////////

#define NINJA_GRAB_THROAT_RATE 32
#define NINJA_GRAB_THROAT_R0 4237
ANIMATOR DoNinjaRoperGrabThroat;

STATE s_NinjaRoperGrabThroat[] =
    {
    {NINJA_GRAB_THROAT_R0 + 0,   NINJA_GRAB_THROAT_RATE,      NullNinjaRoper,       &s_NinjaRoperGrabThroat[1]},
    {NINJA_GRAB_THROAT_R0 + 0,   SF_QUICK_CALL,               DoNinjaRoperSpecial,  &s_NinjaRoperGrabThroat[2]},
    {NINJA_GRAB_THROAT_R0 + 1,   NINJA_GRAB_THROAT_RATE,      NullNinjaRoper,       &s_NinjaRoperGrabThroat[3]},
    {NINJA_GRAB_THROAT_R0 + 2,   SF_QUICK_CALL,               DoNinjaRoperGrabThroat, &s_NinjaRoperGrabThroat[4]},
    {NINJA_GRAB_THROAT_R0 + 2,   NINJA_GRAB_THROAT_RATE,      NullNinjaRoper,       &s_NinjaRoperGrabThroat[5]},
    {NINJA_GRAB_THROAT_R0 + 1,   NINJA_GRAB_THROAT_RATE,      NullNinjaRoper,       &s_NinjaRoperGrabThroat[0]},
    };

STATEp sg_NinjaRoperGrabThroat[] =
    {
    s_NinjaRoperGrabThroat,
    s_NinjaRoperGrabThroat,
    s_NinjaRoperGrabThroat,
    s_NinjaRoperGrabThroat,
    s_NinjaRoperGrabThroat
    };


//////////////////////
//
// NINJA DIE
//
//////////////////////

#define NINJA_DIE_RATE 14

STATE s_NinjaRoperDie[] =
    {
    {NINJA_DIE + 0, NINJA_DIE_RATE, NullNinjaRoper, &s_NinjaRoperDie[1]},
    {NINJA_DIE + 1, NINJA_DIE_RATE, NullNinjaRoper, &s_NinjaRoperDie[2]},
    {NINJA_DIE + 2, NINJA_DIE_RATE, NullNinjaRoper, &s_NinjaRoperDie[3]},
    {NINJA_DIE + 3, NINJA_DIE_RATE, NullNinjaRoper, &s_NinjaRoperDie[4]},
    {NINJA_DIE + 4, NINJA_DIE_RATE, NullNinjaRoper, &s_NinjaRoperDie[5]},
    {NINJA_DIE + 5, NINJA_DIE_RATE-4, NullNinjaRoper, &s_NinjaRoperDie[6]},
    {NINJA_DIE + 6, NINJA_DIE_RATE-6, NullNinjaRoper, &s_NinjaRoperDie[7]},
    {NINJA_DIE + 6, SF_QUICK_CALL , DoNinjaRoperSpecial, &s_NinjaRoperDie[8]},
    {NINJA_DIE + 6, NINJA_DIE_RATE-10, NullNinjaRoper, &s_NinjaRoperDie[9]},
    {NINJA_DIE + 7, SF_QUICK_CALL , QueueFloorBlood, &s_NinjaRoperDie[10]},
    {NINJA_DIE + 7, NINJA_DIE_RATE-12, DoActorDebris, &s_NinjaRoperDie[10]},
    };


#define NINJA_DIESLICED_RATE 20
ANIMATOR DoCutInHalf;

STATE s_NinjaRoperDieSliced[] = 
    {
    {NINJA_SLICED + 0, NINJA_DIESLICED_RATE*6, NullNinjaRoper, &s_NinjaRoperDieSliced[1]},
    {NINJA_SLICED + 1, NINJA_DIESLICED_RATE,   NullNinjaRoper, &s_NinjaRoperDieSliced[2]},
    {NINJA_SLICED + 2, NINJA_DIESLICED_RATE,   NullNinjaRoper, &s_NinjaRoperDieSliced[3]},
    {NINJA_SLICED + 3, NINJA_DIESLICED_RATE,   NullNinjaRoper, &s_NinjaRoperDieSliced[4]},
    {NINJA_SLICED + 4, NINJA_DIESLICED_RATE-1, NullNinjaRoper, &s_NinjaRoperDieSliced[5]},
    {NINJA_SLICED + 5, NINJA_DIESLICED_RATE-2, NullNinjaRoper, &s_NinjaRoperDieSliced[6]},
    {NINJA_SLICED + 6, NINJA_DIESLICED_RATE-3, NullNinjaRoper, &s_NinjaRoperDieSliced[7]},
    {NINJA_SLICED + 7, NINJA_DIESLICED_RATE-4, NullNinjaRoper, &s_NinjaRoperDieSliced[8]},
    {NINJA_SLICED + 7, SF_QUICK_CALL        , DoNinjaRoperSpecial, &s_NinjaRoperDieSliced[9]},
    {NINJA_SLICED + 8, NINJA_DIESLICED_RATE-5, NullNinjaRoper, &s_NinjaRoperDieSliced[10]},
    {NINJA_SLICED + 9, SF_QUICK_CALL         , QueueFloorBlood, &s_NinjaRoperDieSliced[11]},
    {NINJA_SLICED + 9, NINJA_DIESLICED_RATE , DoActorDebris, &s_NinjaRoperDieSliced[11]},
    };

STATE s_NinjaRoperDead[] =
    {
    {NINJA_DIE + 5, NINJA_DIE_RATE, DoActorDebris, &s_NinjaRoperDead[1]},
    {NINJA_DIE + 6, SF_QUICK_CALL , DoNinjaRoperSpecial, &s_NinjaRoperDead[2]},
    {NINJA_DIE + 6, NINJA_DIE_RATE, DoActorDebris, &s_NinjaRoperDead[3]},
    {NINJA_DIE + 7, SF_QUICK_CALL , QueueFloorBlood,&s_NinjaRoperDead[4]},
    {NINJA_DIE + 7, NINJA_DIE_RATE, DoActorDebris, &s_NinjaRoperDead[4]},
    };


STATE s_NinjaRoperDeathJump[] =
    {
    {NINJA_DIE + 0, NINJA_DIE_RATE, DoActorDeathMove, &s_NinjaRoperDeathJump[1]},
    {NINJA_DIE + 1, NINJA_DIE_RATE, DoActorDeathMove, &s_NinjaRoperDeathJump[2]},
    {NINJA_DIE + 2, NINJA_DIE_RATE, DoActorDeathMove, &s_NinjaRoperDeathJump[2]},
    };

STATE s_NinjaRoperDeathFall[] =
    {
    {NINJA_DIE + 3, NINJA_DIE_RATE, DoActorDeathMove, &s_NinjaRoperDeathFall[1]},
    {NINJA_DIE + 4, NINJA_DIE_RATE, DoActorDeathMove, &s_NinjaRoperDeathFall[1]},
    };

/*
STATEp *Stand[MAX_WEAPONS];
STATEp *Run;
STATEp *Jump;
STATEp *Fall;
STATEp *Crawl;
STATEp *Swim;
STATEp *Fly;
STATEp *Rise;
STATEp *Sit;
STATEp *Look;
STATEp *Climb;
STATEp *Pain;
STATEp *Death1;
STATEp *Death2;
STATEp *Dead;
STATEp *DeathJump;
STATEp *DeathFall;
STATEp *CloseAttack[2];
STATEp *Attack[6];
STATEp *Special[2];
*/

STATEp sg_NinjaRoperDie[] =
    {
    s_NinjaRoperDie
    };

STATEp sg_NinjaRoperDieSliced[] =
    {
    s_NinjaRoperDieSliced
    };

STATEp sg_NinjaRoperDead[] =
    {
    s_NinjaRoperDead
    };

STATEp sg_NinjaRoperDeathJump[] =
    {
    s_NinjaRoperDeathJump
    };

STATEp sg_NinjaRoperDeathFall[] =
    {
    s_NinjaRoperDeathFall
    };

/*
    
 !AIC - Collection of states that connect action to states

*/    

ACTOR_ACTION_SET NinjaRoperRoperActionSet =
    {
    {sg_NinjaRoperDuck},
    sg_NinjaRoperCrawl,
    sg_NinjaRoperJump,
    sg_NinjaRoperFall,
    sg_NinjaRoperKneelCrawl,
    sg_NinjaRoperSwim,
    sg_NinjaRoperFly,
    sg_NinjaRoperUzi,
    sg_NinjaRoperDuck,
    NULL,
    sg_NinjaRoperClimb,
    sg_NinjaRoperPain,
    sg_NinjaRoperDie,
    sg_NinjaRoperHariKari,
    sg_NinjaRoperDead,
    sg_NinjaRoperDeathJump,
    sg_NinjaRoperDeathFall,
    {sg_NinjaRoperUzi},
    {1024},
    {sg_NinjaRoperUzi},
    {1024},
    {NULL},
    sg_NinjaRoperDuck,
    sg_NinjaRoperDive
    };
    
ACTOR_ACTION_SET NinjaRoperActionSet =
    {
    {sg_NinjaRoperStand},
    sg_NinjaRoperRun,
    sg_NinjaRoperJump,
    sg_NinjaRoperFall,
    sg_NinjaRoperKneelCrawl,
    sg_NinjaRoperSwim,
    sg_NinjaRoperFly,
    sg_NinjaRoperRise,
    sg_NinjaRoperSit,
    NULL,
    sg_NinjaRoperClimb,
    sg_NinjaRoperPain,
    sg_NinjaRoperDie,
    sg_NinjaRoperHariKari,
    sg_NinjaRoperDead,
    sg_NinjaRoperDeathJump,
    sg_NinjaRoperDeathFall,
    {sg_NinjaRoperUzi, sg_NinjaRoperStar},
    {1000, 1024},
    {sg_NinjaRoperUzi, sg_NinjaRoperStar},
    {800, 1024},
    {NULL},
    sg_NinjaRoperDuck,
    sg_NinjaRoperDive
    };

ACTOR_ACTION_SET NinjaRoperRedActionSet =
    {
    {sg_NinjaRoperStand},
    sg_NinjaRoperRun,
    sg_NinjaRoperJump,
    sg_NinjaRoperFall,
    sg_NinjaRoperKneelCrawl,
    sg_NinjaRoperSwim,
    sg_NinjaRoperFly,
    sg_NinjaRoperRise,
    sg_NinjaRoperSit,
    NULL,
    sg_NinjaRoperClimb,
    sg_NinjaRoperPain,
    sg_NinjaRoperDie,
    sg_NinjaRoperHariKari,
    sg_NinjaRoperDead,
    sg_NinjaRoperDeathJump,
    sg_NinjaRoperDeathFall,
    {sg_NinjaRoperUzi, sg_NinjaRoperUzi},
    {812, 1024},
    {sg_NinjaRoperUzi, sg_NinjaRoperRocket},
    {812, 1024},
    {NULL},
    sg_NinjaRoperDuck,
    sg_NinjaRoperDive
    };

ACTOR_ACTION_SET NinjaRoperGreenActionSet =
    {
    {sg_NinjaRoperStand},
    sg_NinjaRoperRun,
    sg_NinjaRoperJump,
    sg_NinjaRoperFall,
    sg_NinjaRoperKneelCrawl,
    sg_NinjaRoperSwim,
    sg_NinjaRoperFly,
    sg_NinjaRoperRise,
    sg_NinjaRoperSit,
    NULL,
    sg_NinjaRoperClimb,
    sg_NinjaRoperPain,
    sg_NinjaRoperDie,
    sg_NinjaRoperHariKari,
    sg_NinjaRoperDead,
    sg_NinjaRoperDeathJump,
    sg_NinjaRoperDeathFall,
    {sg_NinjaRoperUzi, sg_NinjaRoperFlashBomb},
    {912, 1024},
    {sg_NinjaRoperFlashBomb, sg_NinjaRoperUzi, sg_NinjaRoperMirv, sg_NinjaRoperNapalm},
    {150, 500, 712, 1024},
    {NULL},
    sg_NinjaRoperDuck,
    sg_NinjaRoperDive
    };

extern STATEp sg_PlayerNinjaRoperRun[];
extern STATEp sg_PlayerNinjaRoperStand[];
extern STATEp sg_PlayerNinjaRoperJump[];
extern STATEp sg_PlayerNinjaRoperFall[];
extern STATEp sg_PlayerNinjaRoperClimb[];
extern STATEp sg_PlayerNinjaRoperCrawl[];
extern STATEp sg_PlayerNinjaRoperSwim[];
ACTOR_ACTION_SET PlayerNinjaRoperActionSet =
    {
    {sg_PlayerNinjaRoperStand},
    sg_PlayerNinjaRoperRun,
    sg_PlayerNinjaRoperJump,
    sg_PlayerNinjaRoperFall,
    //sg_NinjaRoperJump,
    //sg_NinjaRoperFall,
    sg_PlayerNinjaRoperCrawl,
    sg_PlayerNinjaRoperSwim,
    sg_NinjaRoperFly,
    sg_NinjaRoperRise,
    sg_NinjaRoperSit,
    NULL,
    sg_PlayerNinjaRoperClimb,
    sg_NinjaRoperPain,
    sg_NinjaRoperDie,
    sg_NinjaRoperHariKari,
    sg_NinjaRoperDead,
    sg_NinjaRoperDeathJump,
    sg_NinjaRoperDeathFall,
    {sg_NinjaRoperStar, sg_NinjaRoperUzi},
    {1000, 1024},
    {sg_NinjaRoperStar, sg_NinjaRoperUzi},
    {800, 1024},
    {NULL},
    sg_NinjaRoperDuck,
    sg_PlayerNinjaRoperSwim
    };
    
int
DoHariKariBlood(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];
    return(0);
    }

/*
    
 !AIC - Every actor has a setup where they are initialized

*/    
    
int
SetupNinjaRoper(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum];
    USERp u;
    ANIMATOR DoActorDecide;
    short pic = sp->picnum;

    if (TEST(sp->cstat, CSTAT_SPRITE_RESTORE))
        {
        u = User[SpriteNum];
        ASSERT(u);
        }
    else
        {
        User[SpriteNum] = u = SpawnUser(SpriteNum, NINJA_RUN_R0, s_NinjaRoperRun[0]);
        u->Health = HEALTH_NINJA;
        }

    u->StateEnd = s_NinjaRoperDie;
    u->Rot = sg_NinjaRoperRun;
    sp->xrepeat = 46;
    sp->yrepeat = 46;

    if (sp->pal == PALETTE_PLAYER5)
        {
        u->Attrib = &InvisibleNinjaRoperAttrib;
        EnemyDefaults(SpriteNum, &NinjaRoperGreenActionSet, &NinjaRoperPersonality);
        if (!TEST(sp->cstat, CSTAT_SPRITE_RESTORE))
            u->Health = HEALTH_RED_NINJA;
        SET(sp->cstat, CSTAT_SPRITE_TRANSLUCENT);
        sp->shade = 127;
        sp->pal = u->spal = PALETTE_PLAYER5;
        sp->hitag = 9998;
        }
    else
    if (sp->pal == PALETTE_PLAYER3)
        {
        u->Attrib = &NinjaRoperAttrib;
        EnemyDefaults(SpriteNum, &NinjaRoperRedActionSet, &NinjaRoperPersonality);
        if (!TEST(sp->cstat, CSTAT_SPRITE_RESTORE))
            u->Health = HEALTH_RED_NINJA;
        sp->pal = u->spal = PALETTE_PLAYER3;
        }
    else    
        {
        u->Attrib = &NinjaRoperAttrib;
        sp->pal = u->spal = PALETTE_PLAYER0;
        EnemyDefaults(SpriteNum, &NinjaRoperActionSet, &NinjaRoperPersonality);
        }
    
    if(pic == NINJA_CRAWL_R0)
    {
        u->Attrib = &NinjaRoperAttrib;
        EnemyDefaults(SpriteNum, &NinjaRoperRoperActionSet, &NinjaRoperRoperPersonality);
        ChangeState(SpriteNum, s_NinjaRoperDuck[0]);
    }

    ChangeState(SpriteNum, s_NinjaRoperRun[0]);
    DoActorSetSpeed(SpriteNum, NORM_SPEED);

    u->Radius = 280;
    SET(u->Flags, SPR_XFLIP_TOGGLE);

    return (0);
    }

int 
DoNinjaRoperHariKari(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;
    int SpawnBlood(short SpriteNum, short Weapon, short hitang, long hitx, long hity, long hitz);
    short cnt,i;

    change_sprite_stat(SpriteNum, STAT_DEAD_ACTOR);
    RESET(sprite[SpriteNum].cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
    SET(u->Flags, SPR_DEAD);
    UpdateSinglePlayKills();
    RESET(u->Flags, SPR_FALLING | SPR_JUMPING);
    u->floor_dist = Z(40);
    u->RotNum = 0;
    u->ActorActionFunc = NULL;
    
    SET(sp->extra, SPRX_BREAKABLE);
    SET(sp->cstat, CSTAT_SPRITE_BREAKABLE);

    PlaySound(DIGI_NINJAUZIATTACK,&sp->x,&sp->y,&sp->z,v3df_follow);
    
    SpawnBlood(SpriteNum, SpriteNum, -1, -1, -1, -1);

    cnt = RANDOM_RANGE(4)+1;
    for(i=0; i<=cnt; i++)
        InitBloodSpray(SpriteNum,TRUE,-2);
    
    return(0);
    }

int 
DoNinjaRoperGrabThroat(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;
    int SpawnBlood(short SpriteNum, short Weapon, short hitang, long hitx, long hity, long hitz);
    short cnt,i;

    if ((u->WaitTics -= ACTORMOVETICS) <= 0)
        {
        RESET(u->Flags2, SPR2_DYING);
        RESET(sp->cstat, CSTAT_SPRITE_YFLIP);
        change_sprite_stat(SpriteNum, STAT_DEAD_ACTOR);
        RESET(sprite[SpriteNum].cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
        SET(u->Flags, SPR_DEAD);
        UpdateSinglePlayKills();
        RESET(u->Flags, SPR_FALLING | SPR_JUMPING);
        u->floor_dist = Z(40);
        u->RotNum = 0;
        u->ActorActionFunc = NULL;
    
        SET(sp->extra, SPRX_BREAKABLE);
        SET(sp->cstat, CSTAT_SPRITE_BREAKABLE);

        SpawnBlood(SpriteNum, SpriteNum, -1, -1, -1, -1);

        ChangeState(SpriteNum, u->StateEnd);
        sp->xvel = 0;
        //u->jump_speed = -300;
        //DoActorBeginJump(SpriteNum);
        PlaySound(DIGI_NINJASCREAM,&sp->x,&sp->y,&sp->z,v3df_follow);
        }

    return(0);
    }

/*
    
 !AIC - Most actors have one of these and the all look similar

*/    
    
int 
DoNinjaRoperMove(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;

    // jumping and falling
    if (TEST(u->Flags, SPR_JUMPING | SPR_FALLING) && !TEST(u->Flags, SPR_CLIMBING))
        {
        if (TEST(u->Flags, SPR_JUMPING))
            DoActorJump(SpriteNum);
        else if (TEST(u->Flags, SPR_FALLING))
            DoActorFall(SpriteNum);
        }

    // sliding    
    if (TEST(u->Flags, SPR_SLIDING) && !TEST(u->Flags, SPR_CLIMBING))
        DoActorSlide(SpriteNum);

    // !AIC - do track or call current action function - such as DoActorMoveCloser()    
    if (u->track >= 0)
        ActorFollowTrack(SpriteNum, ACTORMOVETICS);
    else
    {   
        (*u->ActorActionFunc) (SpriteNum);
    }

    // stay on floor unless doing certain things    
    if (!TEST(u->Flags, SPR_JUMPING | SPR_FALLING | SPR_CLIMBING))
        {
        KeepActorOnFloor(SpriteNum);
        }
        
    // take damage from environment 
    DoActorSectorDamage(SpriteNum);
    
    return(0);
    }

int 
NinjaRoperJumpActionFunc(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;
    long nx, ny;
    
    // Move while jumping
    nx = sp->xvel * (long) sintable[NORM_ANGLE(sp->ang + 512)] >> 14;
    ny = sp->xvel * (long) sintable[sp->ang] >> 14;

    // if cannot move the sprite
    if (!move_actor(SpriteNum, nx, ny, 0L))
        {
        return (0);
        }
    
    if (!TEST(u->Flags, SPR_JUMPING|SPR_FALLING))
        {
        InitActorDecide(SpriteNum);
        }

    return (0);
    }

/*
    
 !AIC - Short version of DoNinjaRoperMove without the movement code.  For times when
 the actor is doing something but not moving.

*/    

int 
NullNinjaRoper(short SpriteNum)
    {
    USERp u = User[SpriteNum];
    SPRITEp sp = User[SpriteNum]->SpriteP;

    if(u->WaitTics > 0) u->WaitTics -= ACTORMOVETICS;
    
    if (TEST(u->Flags, SPR_SLIDING) && !TEST(u->Flags, SPR_CLIMBING) && !TEST(u->Flags, SPR_JUMPING|SPR_FALLING))
        DoActorSlide(SpriteNum);

    if (!TEST(u->Flags, SPR_CLIMBING) && !TEST(u->Flags, SPR_JUMPING|SPR_FALLING))
        KeepActorOnFloor(SpriteNum);

    DoActorSectorDamage(SpriteNum);
    
    return (0);
    }


int DoNinjaRoperPain(short SpriteNum)
{
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];
    
    NullNinjaRoper(SpriteNum);
    
    if ((u->WaitTics -= ACTORMOVETICS) <= 0)
        InitActorDecide(SpriteNum);
        
    return(0);
}    

int DoNinjaRoperSpecial(short SpriteNum)
    {
    SPRITEp sp = &sprite[SpriteNum];
    USERp u = User[SpriteNum];
    
    if(u->spal == PALETTE_PLAYER5)
        {
        RESET(sp->cstat,CSTAT_SPRITE_TRANSLUCENT);
        sp->hitag = 0;
        }

    return(0);
    }  
      
int CheckRopeFire(short SpriteNum)
    {
    if(!CanSeePlayer(SpriteNum))
        InitActorDuck(SpriteNum);
    return(0);
    }       
