//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

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

Original Source: 1997 - Frank Maddin and Jim Norwood
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

#include "break.h"
#include "def.h"

BREAK_INFOp GlobBreakInfo;

int WallBreakPosition(short hitwall, short *sectnum, long *x, long *y, long *z, short *ang);

BREAK_INFO WallBreakInfo[] = 
{
{ 60,  -1, SHRAP_METAL, BF_KILL },
{ 82,  -1, SHRAP_METAL, BF_KILL },
{ 1, 3593, SHRAP_METAL },
//{ 13, -1, SHRAP_PAPER },
//{ 14, -1, SHRAP_PAPER },
//{ 49, -1, SHRAP_PAPER },
//{ 52, -1, SHRAP_PAPER },
//{ 99, -1, SHRAP_PAPER },
//{ 102, -1, SHRAP_PAPER },
//{ 207, -1, SHRAP_PAPER },
{ 253, 255, SHRAP_GLASS },
{ 254, 255, SHRAP_GLASS },
{ 282, -1, SHRAP_GLASS },
{ 283, 4974, SHRAP_METAL },
{ 318, 599, SHRAP_GLASS},
{ 486, -1, SHRAP_METAL },
{ 487, 3676, SHRAP_METAL },
{ 628, 3585, SHRAP_METAL},
{ 630, 3586, SHRAP_METAL},
{ 633, 608, SHRAP_GLASS},
{ 634, 608, SHRAP_GLASS },
{ 637, 3587, SHRAP_METAL},
{ 640, 3588, SHRAP_METAL},
{ 641, 3588, SHRAP_METAL},
{ 665, 3588, SHRAP_METAL},
{ 742, 3589, SHRAP_COIN},
{ 743, 3590, SHRAP_COIN},
{ 750, 608, SHRAP_GLASS},
{ 2667, 608, SHRAP_GLASS},
{ 2769, 3681, SHRAP_GLASS},
{ 2676, 3591, SHRAP_GLASS},
{ 2677, 3592, SHRAP_GLASS},
{ 2687, 2727, SHRAP_GLASS},
{ 2688, 2728, SHRAP_GLASS},
//{ 2714, 3593, SHRAP_GLASS},
{ 2732, 3594, SHRAP_GLASS},
{ 2777, 3683, SHRAP_METAL},
{ 2778, 2757, SHRAP_GLASS},
{ 2801, 3591, SHRAP_GLASS},
{ 2804, 3595, SHRAP_GLASS},
{ 2807, 3596, SHRAP_GLASS},
{ 2810, 4989, SHRAP_METAL},
{ 4890, 4910, SHRAP_METAL},
{ 4891, 4911, SHRAP_METAL},
{ 4892, 4912, SHRAP_METAL},
{ 4893, 4913, SHRAP_METAL},
{ 4894, 4914, SHRAP_METAL},
{ 4895, 4915, SHRAP_METAL},
{ 3336, 4940, SHRAP_COIN}, 
{ 3337, 4941, SHRAP_COIN}, 
{ 4885, 4888, SHRAP_METAL},
{ 4887, 4889, SHRAP_COIN},
{ 3350, 4942, SHRAP_GLASS},
{ 3351, 4943, SHRAP_METAL},
{ 3352, 4944, SHRAP_METAL},
{ 3353, 4945, SHRAP_METAL},
{ 4896, 4898, SHRAP_METAL},
{ 4897, 4899, SHRAP_METAL},
{ 3385, 4981, SHRAP_METALMIX},
{ 3389, 4982, SHRAP_METALMIX},
{ 3393, 4984, SHRAP_METALMIX},
{ 3397, 4983, SHRAP_METALMIX},
{ 3401, 4985, SHRAP_METALMIX},
{ 3405, 4986, SHRAP_METALMIX},
{ 3409, 4988, SHRAP_METALMIX},
{ 3413, 4987, SHRAP_METALMIX},
{ 253, 255,   SHRAP_METALMIX},
{ 283, 4974,  SHRAP_METALMIX},
{ 299, 4975,  SHRAP_METALMIX},
{5078, 5079,  SHRAP_METALMIX},
{5080, 5092,  SHRAP_MARBELS},
{5083, 5093,  SHRAP_MARBELS},
{5086, 5094,  SHRAP_MARBELS},
{5089, 5095,  SHRAP_MARBELS},
{4970, 4973,  SHRAP_METAL},
{297,  4980,  SHRAP_METAL},
{1,    4976,  SHRAP_METAL},
{4917, 4918,  SHRAP_METAL},
{4902, 4903,  SHRAP_METAL},
};

BREAK_INFO SpriteBreakInfo[] = 
{
{ 60,  -1, SHRAP_METAL, BF_KILL },
{ 82,  -1, SHRAP_METAL, BF_KILL },
{ 138, -1, SHRAP_GENERIC, BF_KILL },
{ 253, 255, SHRAP_GLASS },
{ 254, 255, SHRAP_GLASS },
{ 270, -1, SHRAP_PAPER, BF_BURN },
{ 271, -1, SHRAP_PAPER, BF_BURN },
{ 272, -1, SHRAP_WOOD },
{ 274, -1, SHRAP_PAPER, BF_BURN },
//{ 276, -1, SHRAP_WOOD },
//{ 277, -1, SHRAP_WOOD },
//{ 278, -1, SHRAP_WOOD },
{ 282, -1, SHRAP_GLASS },
{ 283, -1, SHRAP_METAL },
{ 297, -1, SHRAP_METAL },
{ 299, -1, SHRAP_METAL },
{ 363, -1, SHRAP_METAL, BF_KILL},
{ 365, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 366, -1, SHRAP_METAL, BF_KILL,5},
{ 367, -1, SHRAP_WOOD, BF_KILL},
{ 368, -1, SHRAP_GIBS, BF_KILL},
{ 369, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 371, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 372, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 374, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 375, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 376, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 377, -1, SHRAP_STONE, BF_KILL},
{ 379, -1, SHRAP_WOOD, BF_KILL},
{ 380, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 385, -1, SHRAP_BLOOD, BF_KILL},
{ 386, -1, SHRAP_GIBS, BF_KILL},
{ 387, -1, SHRAP_GIBS, BF_KILL},
{ 388, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 391, -1, SHRAP_GIBS, BF_KILL},
{ 392, -1, SHRAP_GIBS, BF_KILL},
{ 393, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 394, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 395, -1, SHRAP_GIBS, BF_KILL},
{ 396, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 397, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 398, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 399, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 400, -1, SHRAP_GENERIC, BF_KILL},
{ 401, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 402, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 407, -1, SHRAP_METAL, BF_KILL},
{ 408, -1, SHRAP_PAPER, BF_BURN},
{ 409, -1, SHRAP_PAPER, BF_BURN},
{ 415, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL,5},
{ 418, -1, SHRAP_GENERIC, BF_KILL|BF_FIRE_FALL,5},
{ 422, -1, SHRAP_METAL, BF_KILL},
{ 423, -1, SHRAP_BLOOD, BF_KILL},
{ 424, -1, SHRAP_BLOOD, BF_KILL},
{ 425, -1, SHRAP_BLOOD, BF_KILL},
{ 428, -1, SHRAP_METAL, BF_BURN},
{ 430, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 431, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 432, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 433, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 434, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 435, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 436, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 437, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 438, -1, SHRAP_GIBS, BF_KILL},
{ 441, -1, SHRAP_WOOD, BF_KILL},
{ 442, -1, SHRAP_STONE, BF_KILL},
{ 443, -1, SHRAP_STONE, BF_KILL|BF_FIRE_FALL},
{ 453, -1, SHRAP_WOOD, BF_KILL},
{ 458, -1, SHRAP_STONE, BF_KILL},
{ 459, -1, SHRAP_STONE, BF_KILL},
{ 460, -1, SHRAP_METAL, BF_KILL},
{ 461, -1, SHRAP_METAL, BF_KILL},
{ 462, -1, SHRAP_METAL, BF_KILL},
{ 463, -1, SHRAP_STONE, BF_KILL},
{ 467, -1, SHRAP_STONE, BF_KILL},
{ 468, -1, SHRAP_WOOD, BF_KILL},
{ 475, -1, SHRAP_GLASS, BF_KILL},
{ 481, -1, SHRAP_GENERIC, BF_KILL},
{ 482, -1, SHRAP_WOOD, BF_KILL},
{ 483, -1, SHRAP_WOOD, BF_KILL|BF_TOUGH},
{ 491, -1, SHRAP_WOOD, BF_KILL},
{ 492, -1, SHRAP_METAL, BF_KILL},
{ 493, -1, SHRAP_METAL, BF_KILL},
{ 498, -1, SHRAP_GENERIC, BF_KILL},
{ 500, -1, SHRAP_METAL, BF_KILL},
{ 501, -1, SHRAP_METAL, BF_KILL},
{ 504, -1, SHRAP_METAL, BF_KILL,5},
{ 505, -1, SHRAP_BLOOD, BF_KILL,5},
{ 506, -1, SHRAP_GENERIC, BF_KILL,5},
{ 507, -1, SHRAP_GLASS, BF_KILL},
{ 508, -1, SHRAP_GLASS, BF_KILL},
{ 509, -1, SHRAP_GLASS, BF_KILL},
{ 510, -1, SHRAP_GLASS, BF_KILL},
{ 511, -1, SHRAP_METAL, BF_KILL},
{ 512, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL,5},
{ 516, -1, SHRAP_WOOD, BF_BURN},
{ 517, -1, SHRAP_WOOD, BF_BURN},
{ 518, -1, SHRAP_WOOD, BF_BURN},
{ 519, -1, SHRAP_WOOD, BF_FIRE_FALL|BF_KILL,5},
{ 520, -1, SHRAP_WOOD, BF_KILL},
{ 521, -1, SHRAP_WOOD, BF_KILL|BF_FIRE_FALL},
{ 537, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 541, -1, SHRAP_WOOD, BF_KILL|BF_FIRE_FALL},
{ 586, -1, SHRAP_METAL, BF_KILL},
{ 590, -1, SHRAP_METAL, BF_KILL},
{ 591, -1, SHRAP_METAL, BF_KILL},
{ 593, 608, SHRAP_GLASS,BF_TOUGH},
{ 604, -1, SHRAP_METAL, BF_KILL},
{ 613, -1, SHRAP_LARGE_EXPLOSION, BF_KILL},
{ 614, -1, SHRAP_METAL, BF_KILL},
{ 615, -1, SHRAP_METAL, BF_KILL},
{ 618, -1, SHRAP_GLASS, BF_KILL},
{ 646, -1, SHRAP_METAL, BF_KILL},
{ 647, -1, SHRAP_LARGE_EXPLOSION, BF_KILL},
{ 648, -1, SHRAP_LARGE_EXPLOSION, BF_KILL},
{ 649, -1, SHRAP_METAL, BF_KILL},
{ 656, -1, SHRAP_METAL, BF_KILL},
{ 657, -1, SHRAP_METAL, BF_KILL},
{ 658, -1, SHRAP_LARGE_EXPLOSION , BF_KILL},
{ 659, -1, SHRAP_METAL, BF_KILL,5},
//{ 660, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
//{ 661, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
//{ 662, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 663, -1, SHRAP_METAL, BF_KILL,10},
{ 664, -1, SHRAP_METAL, BF_KILL,5},
{ 666, -1, SHRAP_PLANT, BF_KILL},
{ 670, -1, SHRAP_METAL, BF_KILL|BF_FIRE_FALL},
{ 671, -1, SHRAP_GLASS, BF_KILL|BF_FIRE_FALL},
{ 673, -1, SHRAP_BLOOD, BF_KILL},
{ 674, -1, SHRAP_GIBS, BF_KILL},
{ 675, -1, SHRAP_GIBS, BF_KILL},
{ 676, -1, SHRAP_GIBS, BF_KILL},
{ 678, -1, SHRAP_GLASS, BF_KILL,5},
{ 679, -1, SHRAP_GLASS, BF_KILL,5},
{ 683, -1, SHRAP_GLASS, BF_KILL,5},
{ 684, -1, SHRAP_GLASS, BF_KILL,5},
{ 685, -1, SHRAP_GLASS, BF_KILL,5},
{ 686, -1, SHRAP_PAPER, BF_KILL,5},
{ 687, -1, SHRAP_STONE, BF_KILL|BF_TOUGH},
{ 688, -1, SHRAP_STONE, BF_KILL|BF_TOUGH},
{ 690, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 692, -1, SHRAP_WOOD, BF_BURN|BF_OVERRIDE_BLOCK},
{ 695, -1, SHRAP_STONE, BF_KILL},
{ 696, -1, SHRAP_STONE, BF_KILL},
{ 697, -1, SHRAP_STONE, BF_KILL},
{ 698, -1, SHRAP_STONE, BF_KILL},
{ 699, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 702, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 703, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 704, -1, SHRAP_STONE, BF_TOUGH|BF_KILL},
{ 706, -1, SHRAP_PLANT, BF_KILL},
{ 707, -1, SHRAP_PLANT, BF_KILL},
{ 710, -1, SHRAP_PLANT, BF_KILL},
{ 711, -1, SHRAP_PLANT, BF_KILL},
{ 714, -1, SHRAP_STONE, BF_KILL,5},
{ 721, -1, SHRAP_GIBS, BF_KILL},
{ 722, -1, SHRAP_GIBS, BF_KILL},
{ 723, -1, SHRAP_GIBS, BF_KILL},
{ 724, -1, SHRAP_GIBS, BF_KILL},
{ 725, -1, SHRAP_PLANT, BF_KILL},
{ 730, -1, SHRAP_GENERIC, BF_KILL},
{ 744, -1, SHRAP_GLASS, BF_KILL,5},
{ 2563, -1, SHRAP_PAPER, BF_BURN},
{ 2564, -1, SHRAP_PAPER, BF_BURN},
{ 3570, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN},
{ 3571, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 3572, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 3573, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 3574, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 3575, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 3576, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 3577, -1, SHRAP_WOOD, BF_TOUGH|BF_KILL},
{ 3578, -1, SHRAP_WOOD, BF_TOUGH|BF_KILL},
{ 3579, -1, SHRAP_WOOD, BF_TOUGH|BF_KILL},
{ 3580, -1, SHRAP_WOOD, BF_TOUGH|BF_KILL},
{ 3581, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN},
{ 3582, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN},
{ 2640, -1, SHRAP_STONE, BF_KILL,5},
{ 2641, -1, SHRAP_STONE, BF_KILL},
{ 2642, -1, SHRAP_STONE, BF_KILL},
{ 2680, -1, SHRAP_GENERIC, BF_KILL},
{ 2681, -1, SHRAP_GENERIC, BF_KILL},
{ 2682, -1, SHRAP_GENERIC, BF_KILL},
{ 2683, -1, SHRAP_GENERIC, BF_KILL},
{ 2684, -1, SHRAP_GENERIC, BF_KILL},
{ 2685, -1, SHRAP_GENERIC, BF_KILL},
{ 2687, 2727, SHRAP_GLASS},
{ 2688, 2728, SHRAP_GLASS},
{ 2699, -1, SHRAP_WOOD, BF_KILL},
{ 2709, -1, SHRAP_WOOD, BF_TOUGH|BF_KILL},
{ 2720, -1, SHRAP_GIBS, BF_KILL},
{ 2721, -1, SHRAP_GIBS, BF_KILL},
{ 2722, -1, SHRAP_GIBS, BF_KILL},
{ 2723, -1, SHRAP_GIBS, BF_KILL},
{ 2724, -1, SHRAP_GIBS, BF_KILL},
{ 2725, -1, SHRAP_BLOOD, BF_KILL},
{ 2726, -1, SHRAP_BLOOD, BF_KILL},
{ 2719, -1, SHRAP_GLASS, BF_KILL},
{ 2750, -1, SHRAP_WOOD, BF_KILL},
{ 2676, 3591, SHRAP_GLASS},
{ 2769, 3681, SHRAP_GLASS},
{ 2777, 3683, SHRAP_METAL, BF_TOUGH},
{ 2778, 2757, SHRAP_GLASS},
{ 3448, 3451, SHRAP_METAL, BF_TOUGH|BF_KILL},
{ 3449, -1, SHRAP_PAPER, BF_KILL},
{ 3497, -1, SHRAP_GENERIC, BF_KILL|BF_TOUGH},
{ 3551, -1, SHRAP_METAL, BF_KILL},
{ 3552, -1, SHRAP_METAL, BF_KILL},
{ 3553, -1, SHRAP_METAL, BF_KILL},
{ 3554, -1, SHRAP_METAL, BF_KILL},
{ 3555, -1, SHRAP_METAL, BF_KILL},
{ 3556, -1, SHRAP_METAL, BF_KILL},
{ 3557, -1, SHRAP_METAL, BF_KILL},
{ 3558, -1, SHRAP_WOOD, BF_KILL},
{ 3568, -1, SHRAP_WOOD, BF_BURN},
{ 4994, -1, SHRAP_METAL, BF_KILL},
{ 4995, -1, SHRAP_METAL, BF_KILL},
{ 5010, -1, SHRAP_WOOD, BF_TOUGH|BF_BURN|BF_OVERRIDE_BLOCK},
{ 5017, -1, SHRAP_PAPER, BF_KILL},
{ 5018, -1, SHRAP_PAPER, BF_KILL},
{ 5019, -1, SHRAP_PAPER, BF_KILL},
{ 5060, -1, SHRAP_METAL, BF_KILL},
{ 5061, -1, SHRAP_METAL, BF_KILL},
{ 5073, -1, SHRAP_GIBS, BF_KILL},

// Evil ninja Hari-Kari - can gib
{ 4218, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},

// old ninja dead frames
{ 1133, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 1134, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},

// dead actors
{ 811, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 1440, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 1512, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 1643, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 1680, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},

{ 4219+7, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 4236, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},  // Evil Ninja cut in half
{ 4421, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},  // Dead Big Ripper
{ 4312, -1, SHRAP_GIBS, BF_KILL|BF_TOUGH},  // Dead Coolie Ghost
{ 766 , -1, SHRAP_COIN,BF_KILL},
{ 767 , -1, SHRAP_COIN,BF_KILL},
{ 2700, -1, SHRAP_PAPER,BF_KILL},
{ 2701, -1, SHRAP_PAPER,BF_KILL},
{ 2702, -1, SHRAP_PAPER,BF_KILL},
{ 2703, -1, SHRAP_PAPER,BF_KILL},
{ 2704, -1, SHRAP_PAPER,BF_KILL},
{ 2705, -1, SHRAP_PAPER,BF_KILL},
{ 2218, -1, SHRAP_METAL,BF_KILL},   // Caltrops are breakable
{ 689,  -1, SHRAP_METAL,BF_TOUGH|BF_KILL},  
//{ 2540, -1, SHRAP_METAL,BF_TOUGH|BF_KILL},  
{ 3354, -1, SHRAP_METAL,BF_KILL},   
{ 3357, -1, SHRAP_PAPER,BF_KILL},   
{ 4886, -1, SHRAP_GLASS,BF_KILL},   
{ 646, 708, SHRAP_METAL,BF_TOUGH|BF_KILL},  
{ 708, -1,  SHRAP_METAL,BF_TOUGH|BF_KILL},  
{ 656, -1,  SHRAP_METAL,BF_KILL},   
{ 663, -1,  SHRAP_METAL,BF_KILL},   
{ 664, -1,  SHRAP_METAL,BF_KILL},   
{ 691, -1,  SHRAP_METAL,BF_KILL},   
{ 5021,-1,  SHRAP_GIBS, BF_KILL},
{ 712, -1,  SHRAP_LARGE_EXPLOSION, BF_KILL},   
{ 713, -1,  SHRAP_LARGE_EXPLOSION, BF_KILL},   
{ 693, -1,  SHRAP_WOODMIX, BF_KILL|BF_TOUGH},   
{ 5041,-1,  SHRAP_GIBS, BF_KILL},   
{ 5042,5077,SHRAP_GIBS, BF_TOUGH|BF_KILL},   
{ 5077,-1,  SHRAP_WOOD, BF_TOUGH|BF_KILL},   
{ 3356,3358,SHRAP_WOOD, BF_TOUGH|BF_KILL},  
{ 3358,-1,  SHRAP_WOOD, BF_TOUGH|BF_KILL},  
{ 900, -1,  SHRAP_GIBS, BF_KILL},
{ 901, -1,  SHRAP_GIBS, BF_KILL},
{ 902, -1,  SHRAP_GIBS, BF_KILL},
{ 915, -1,  SHRAP_GIBS, BF_KILL},
{ 916, -1,  SHRAP_GIBS, BF_KILL},
{ 917, -1,  SHRAP_GIBS, BF_KILL},
{ 930, -1,  SHRAP_GIBS, BF_KILL},
{ 931, -1,  SHRAP_GIBS, BF_KILL},
{ 932, -1,  SHRAP_GIBS, BF_KILL},
{ 1670,-1,  SHRAP_GIBS, BF_KILL},
{ 2219,-1,  SHRAP_METAL,BF_KILL|BF_TOUGH},   
{ 4768,-1,  SHRAP_GLASS,BF_KILL},   
{ 4792,-1,  SHRAP_GLASS,BF_KILL},   
{ 4816,-1,  SHRAP_GLASS,BF_KILL},   
{ 4840,-1,  SHRAP_GLASS,BF_KILL},   
{ 4584,-1,  SHRAP_GIBS, BF_KILL},
{ 5062,-1,  SHRAP_WOOD, BF_KILL|BF_TOUGH},
{ 5063,4947,SHRAP_PAPERMIX, BF_KILL|BF_TOUGH|BF_LEAVE_BREAK},
{ 4947,-1,  SHRAP_PAPERMIX, BF_KILL|BF_TOUGH},
{ 1160,-1,  SHRAP_GIBS, BF_KILL|BF_TOUGH},
{ 5104,-1,  SHRAP_GIBS, BF_KILL},
{ 3795,-1,  SHRAP_GIBS, BF_KILL},
{ 470,-1,   SHRAP_GIBS, BF_KILL},
{ 5205,-1,  SHRAP_GIBS, BF_KILL|BF_TOUGH},
//{ 969,-1,   SHRAP_GIBS, BF_KILL|BF_TOUGH},
//{ 1277,-1,  SHRAP_GIBS, BF_KILL|BF_TOUGH},
};

//////////////////////////////////////////////
// SORT & SEARCH SUPPORT
//////////////////////////////////////////////

int CompareBreakInfo(BREAK_INFOp break_info1, BREAK_INFOp break_info2)
    {
    // will return a number less than 0 if break_info1 < break_info2
    return(break_info1->picnum - break_info2->picnum);
    }

int CompareSearchBreakInfo(short *picnum, BREAK_INFOp break_info)
    {
    // will return a number less than 0 if picnum < break_info->picnum
    return(*picnum - break_info->picnum);
    }

BREAK_INFOp FindWallBreakInfo(short picnum)
    {
    return(bsearch(&picnum, &WallBreakInfo, SIZ(WallBreakInfo), sizeof(BREAK_INFO), CompareSearchBreakInfo));
    }

BREAK_INFOp FindSpriteBreakInfo(short picnum)
    {
    return(bsearch(&picnum, &SpriteBreakInfo, SIZ(SpriteBreakInfo), sizeof(BREAK_INFO), CompareSearchBreakInfo));
    }

//////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////

void SortBreakInfo(void)
    {
    qsort(&SpriteBreakInfo, SIZ(SpriteBreakInfo), sizeof(BREAK_INFO), CompareBreakInfo);
    qsort(&WallBreakInfo, SIZ(WallBreakInfo), sizeof(BREAK_INFO), CompareBreakInfo);
    }

BREAK_INFOp SetupWallForBreak(WALLp wallp)
    {
    BREAK_INFOp break_info;
    
    break_info = FindWallBreakInfo(wallp->picnum);
    if (break_info)
        {
        wallp->lotag = TAG_WALL_BREAK;
        SET(wallp->extra, WALLFX_DONT_STICK);
        
        // set for cacheing
        if (break_info->breaknum >= 0)
            SET_GOTPIC(break_info->breaknum);
        }

    if (wallp->overpicnum > 0 && TEST(wallp->cstat, CSTAT_WALL_MASKED))    
        {
        break_info = FindWallBreakInfo(wallp->overpicnum);
        if (break_info)
            {
            wallp->lotag = TAG_WALL_BREAK;
            SET(wallp->extra, WALLFX_DONT_STICK);
            }
        }    
    
    return(break_info);    
    }

BREAK_INFOp SetupSpriteForBreak(SPRITEp sp)
    {
    short picnum = sp->picnum;
    BREAK_INFOp break_info;
    
    // ignore as a breakable if true
    if (sp->lotag == TAG_SPRITE_HIT_MATCH)
        return(NULL);

    break_info = FindSpriteBreakInfo(picnum);
    if (break_info)
        {
        
        // use certain sprites own blocking for determination
        if (TEST(break_info->flags, BF_OVERRIDE_BLOCK))
            {
            // if not blocking then skip this code
            if (!TEST(sp->cstat, CSTAT_SPRITE_BLOCK))
                {
                return(-1);
                }
            }
            
        if (TEST(break_info->flags, BF_BURN))
            SET(sp->extra, SPRX_BURNABLE);
        else    
            SET(sp->extra, SPRX_BREAKABLE);
            
        sp->clipdist = SPRITEp_SIZE_X(sp);
        
        SET(sp->cstat, CSTAT_SPRITE_BREAKABLE);
        
        // set for cacheing
        if (break_info->breaknum >= 0)
            SET_GOTPIC(break_info->breaknum);
        }
    
    return(break_info);    
    }

//////////////////////////////////////////////
// ACTIVATE
//////////////////////////////////////////////

short FindBreakSpriteMatch(short match)
    {
    short i,nexti;
    
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_BREAKABLE], i, nexti)
        {
        if (SPRITE_TAG2(i) == match && sprite[i].picnum == ST1)
            {
            return(i);
            }
        }
    
    return(-1);    
    }

//
// WALL
// 

int AutoBreakWall(WALLp wallp, long hitx, long hity, long hitz, short ang, short type)
    {
    BREAK_INFOp break_info;
    short BreakSprite;
    WALLp nwp;
    SPRITEp bsp;
    
    //DSPRINTF(ds,"wallnum %d, pic %d, lo %d, hi %d",wallp-wall, wallp->picnum, wallp->lotag, wallp->hitag);
    MONO_PRINT(ds);
    
    wallp->lotag = 0;
    if (wallp->nextwall >= 0)
        {
        nwp = &wall[wallp->nextwall];

        // get rid of both sides
        // only break ONE of the walls

        if (nwp->lotag == TAG_WALL_BREAK &&
            nwp->overpicnum > 0 &&
            TEST(nwp->cstat, CSTAT_WALL_MASKED))
            {
            nwp->lotag = 0;
            }
        }
    
    if (wallp->overpicnum > 0 && TEST(wallp->cstat, CSTAT_WALL_MASKED))    
        break_info = FindWallBreakInfo(wallp->overpicnum);
    else    
        break_info = FindWallBreakInfo(wallp->picnum);
        
    if (!break_info)    
        {
        //DSPRINTF(ds,"Break Info not found - wall %d", wallp - wall);
        MONO_PRINT(ds);
        
        return(FALSE);
        }
    
    // Check to see if it should break with current weapon type
    if(!CheckBreakToughness(break_info, type)) return(FALSE);

    if (hitx != MAXLONG)    
        {
        // need correct location for spawning shrap
        BreakSprite = COVERinsertsprite(0, STAT_DEFAULT);
        ASSERT(BreakSprite >= 0);
        bsp = &sprite[BreakSprite];
        bsp->cstat = 0;
        bsp->extra = 0;
        bsp->ang = ang;
        bsp->picnum = ST1;
        bsp->xrepeat = bsp->yrepeat = 64;
        setsprite(BreakSprite, hitx, hity, hitz);
        
        // pass Break Info Globally
        GlobBreakInfo = break_info;
        SpawnShrap(BreakSprite, -1);
        GlobBreakInfo = NULL;
        
        KillSprite(BreakSprite);
        }
    
    // change the wall
    if (wallp->overpicnum > 0 && TEST(wallp->cstat, CSTAT_WALL_MASKED))    
        {
        if (break_info->breaknum == -1)
            {
            RESET(wallp->cstat, CSTAT_WALL_MASKED|CSTAT_WALL_1WAY|CSTAT_WALL_BLOCK_HITSCAN|CSTAT_WALL_BLOCK);
            wallp->overpicnum = 0;
            if (wallp->nextwall >= 0)
                {
                nwp = &wall[wallp->nextwall];
                RESET(nwp->cstat, CSTAT_WALL_MASKED|CSTAT_WALL_1WAY|CSTAT_WALL_BLOCK_HITSCAN|CSTAT_WALL_BLOCK);
                nwp->overpicnum = 0;
                }
            }
        else
            {
            RESET(wallp->cstat, CSTAT_WALL_BLOCK_HITSCAN|CSTAT_WALL_BLOCK);
            wallp->overpicnum = break_info->breaknum;
            if (wallp->nextwall >= 0)
                {
                nwp = &wall[wallp->nextwall];
                RESET(nwp->cstat, CSTAT_WALL_BLOCK_HITSCAN|CSTAT_WALL_BLOCK);
                nwp->overpicnum = break_info->breaknum;
                }
            }    
        }
    else    
        {
        if (break_info->breaknum == -1)
            wallp->picnum = 594; // temporary break pic
        else
            {
            wallp->picnum = break_info->breaknum;
            if( wallp->hitag < 0)    
                DoWallBreakSpriteMatch( wallp->hitag );
            }    
        }    
        
    
    return(TRUE);    
    }

BOOL UserBreakWall(WALLp wp, short ang)
    {
    short SpriteNum;
    SPRITEp sp;
    short match = wp->hitag;
    int block_flags = CSTAT_WALL_BLOCK|CSTAT_WALL_BLOCK_HITSCAN;
    int type_flags = CSTAT_WALL_TRANSLUCENT|CSTAT_WALL_MASKED|CSTAT_WALL_1WAY;
    int flags = block_flags|type_flags;
    short ret;
    
    SpriteNum = FindBreakSpriteMatch(match);
    
    if (SpriteNum < 0)
        {
        // do it the old way and get rid of wall - assumed to be masked
        DoSpawnSpotsForKill(match);
        RESET(wp->cstat, flags);
        if (wp->nextwall >= 0)
            RESET(wall[wp->nextwall].cstat, flags);
       
        // clear tags
        wp->hitag = wp->lotag = 0;
        if (wp->nextwall >= 0)
            wall[wp->nextwall].hitag = wall[wp->nextwall].lotag = 0;
        return(TRUE);
        }
    
    sp = &sprite[SpriteNum];
    
    if (wp->picnum == SP_TAG5(sp))
        return(TRUE);
        
    // make it BROKEN
    if (SP_TAG7(sp) <= 1)
        {
        DoSpawnSpotsForKill(match);
        DoLightingMatch(match, -1);
        
        if (SP_TAG8(sp) == 0)
            {
            wp->picnum = SP_TAG5(sp);
            // clear tags
            wp->hitag = wp->lotag = 0;
            if (wp->nextwall >= 0)
                wall[wp->nextwall].hitag = wall[wp->nextwall].lotag = 0;
            ret = FALSE;
            }
        else    
        if (SP_TAG8(sp) == 1)
            {
            // clear flags
            RESET(wp->cstat, flags);
            if (wp->nextwall >= 0)
                RESET(wall[wp->nextwall].cstat, flags);
            // clear tags
            wp->hitag = wp->lotag = 0;
            if (wp->nextwall >= 0)
                wall[wp->nextwall].hitag = wall[wp->nextwall].lotag = 0;
            
            ret = TRUE;
            }
        else
        if (SP_TAG8(sp) == 2)
            {
            // set to broken pic
            wp->picnum = SP_TAG5(sp);
            
            // clear flags
            RESET(wp->cstat, block_flags);
            if (wp->nextwall >= 0)
                RESET(wall[wp->nextwall].cstat, block_flags);
                
            // clear tags
            wp->hitag = wp->lotag = 0;
            if (wp->nextwall >= 0)
                wall[wp->nextwall].hitag = wall[wp->nextwall].lotag = 0;
                
            ret = FALSE;
            }    
        
        return(ret);    
        }
    else
        {
        // increment picnum
        wp->picnum++;
        
        DoSpawnSpotsForDamage(match);
        }    
    
    return(FALSE);    
    }

int WallBreakPosition(short hitwall, short *sectnum, long *x, long *y, long *z, short *ang)
    {
    short w,nw;
    WALLp wp;
    long nx,ny;
    short wall_ang; 
    long ret=0;
    
    w = hitwall;
    wp = &wall[w];
    
    nw = wall[w].point2;
    wall_ang = NORM_ANGLE(getangle(wall[nw].x - wall[w].x, wall[nw].y - wall[w].y)+512);

    *sectnum = SectorOfWall(w);
    ASSERT(*sectnum >= 0);
    
    // midpoint of wall
    *x = DIV2(wall[w].x + wall[w].x);
    *y = DIV2(wall[w].y + wall[w].y);

    //getzsofsector(*sectnum, *x, *y, cz, fz);
    
    if (wp->nextwall < 0)
        {
        // white wall
        *z = DIV2(sector[*sectnum].floorz + sector[*sectnum].ceilingz);
        }
    else
        {
        short next_sectnum = wp->nextsector;
        
        // red wall
        ASSERT(wp->nextsector >= 0);
        
        // floor and ceiling meet
        if (sector[next_sectnum].floorz == sector[next_sectnum].ceilingz)
            *z = DIV2(sector[*sectnum].floorz + sector[*sectnum].ceilingz);
        else
        // floor is above other sector
        if (sector[next_sectnum].floorz < sector[*sectnum].floorz)
            *z = DIV2(sector[next_sectnum].floorz + sector[*sectnum].floorz);
        else    
        // ceiling is below other sector
        if (sector[next_sectnum].ceilingz > sector[*sectnum].ceilingz)
            *z = DIV2(sector[next_sectnum].ceilingz + sector[*sectnum].ceilingz);
        }    
    
    *ang = wall_ang;
    
    nx = MOVEx(128, wall_ang);
    ny = MOVEy(128, wall_ang);
    
    *x += nx;
    *y += ny;
    
    updatesectorz(*x,*y,*z,sectnum);
    if (*sectnum < 0)
        {
        *x = MAXLONG;  // don't spawn shrap, just change wall
        return(FALSE);
        }
        
    return(TRUE);
    }
    
// If the tough parameter is not set, then it can't break tough walls and sprites    
BOOL HitBreakWall(WALLp wp, long hitx, long hity, long hitz, short ang, short type)
    {
    short SpriteNum;
    short match = wp->hitag;

    if (match > 0)
        {
        UserBreakWall(wp, ang);
        return(TRUE);
        }
    
    //if (hitx == MAXLONG)
        {
        short sectnum;
        WallBreakPosition(wp - wall, &sectnum, &hitx, &hity, &hitz, &ang);
        }
    
    AutoBreakWall(wp, hitx, hity, hitz, ang, type);    
    return(TRUE);
    }

//
// SPRITE
// 

int KillBreakSprite(short BreakSprite)
    {
    SPRITEp bp = &sprite[BreakSprite];
    USERp bu = User[BreakSprite];
    short i;
    
    // Does not actually kill the sprite so it will be valid for the rest
    // of the loop traversal. 
    
    // IMPORTANT: Do not change the statnum if possible so that NEXTI in 
    // SpriteControl loop traversals will maintain integrity.
    
    SpriteQueueDelete(BreakSprite);
    
    if (bu)
        {
        if (bp->statnum == STAT_DEFAULT)
            // special case allow kill of sprites on STAT_DEFAULT list
            // a few things have users and are not StateControlled
            KillSprite(BreakSprite); 
        else
            SetSuicide(BreakSprite);
        }
    else
        {
        change_sprite_stat(BreakSprite, STAT_SUICIDE);
        }
    
    return(0);    
    }


int UserBreakSprite(short BreakSprite)
    {
    SPRITEp sp;
    SPRITEp bp = &sprite[BreakSprite];
    short match = bp->lotag;
    short match_extra;
    short SpriteNum;
    
    SpriteNum = FindBreakSpriteMatch(match);
    
    if (SpriteNum < 0)
        {
        // even if you didn't find a matching ST1 go ahead and kill it and match everything
        // its better than forcing everyone to have a ST1
        DoMatchEverything(NULL, match, -1);
        // Kill sound if one is attached
        DeleteNoSoundOwner(BreakSprite);
        //change_sprite_stat(BreakSprite, STAT_SUICIDE);
        KillBreakSprite(BreakSprite);
        return(TRUE);
        }
    
    sp = &sprite[SpriteNum];
    match_extra = SP_TAG6(bp);
    
    if (bp->picnum == SP_TAG5(sp))
        return(TRUE);
   
    // make it BROKEN
    if (SP_TAG7(sp) <= 1)
        {
        DoMatchEverything(NULL, match_extra, -1);
        //DoSpawnSpotsForKill(match_extra);
        DoLightingMatch(match_extra, OFF);
        
        if (SP_TAG8(sp) == 0)
            {
            bp->picnum = SP_TAG5(sp);
            RESET(bp->extra, SPRX_BREAKABLE);
            }
        else    
        // kill sprite
        if (SP_TAG8(sp) == 1)
            {
            // Kill sound if one is attached
            DeleteNoSoundOwner(BreakSprite);
            KillBreakSprite(BreakSprite);
            //KillSprite(BreakSprite);
            //change_sprite_stat(BreakSprite, STAT_SUICIDE);
            return(TRUE);
            }
        else
        if (SP_TAG8(sp) == 2)
        // leave it    
            {
            // set to broken pic
            bp->picnum = SP_TAG5(sp);
            
            // reset 
            if (SP_TAG8(sp) == 2)
                {
                RESET(bp->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
                }
                
            RESET(bp->extra, SPRX_BREAKABLE);
            }    
        }
    else
        {
        // increment picnum
        bp->picnum++;
            
        DoSpawnSpotsForDamage(match_extra);
        }    
    
    return(FALSE);    
    }

int AutoBreakSprite(short BreakSprite, short type)
    {
    SPRITEp bp = &sprite[BreakSprite];
    BREAK_INFOp break_info;
    extern void DoWallBreakMatch( short match );
    int SpawnBreakFlames(SHORT SpriteNum);
    
    break_info = FindSpriteBreakInfo(bp->picnum);
    

    if(bp->hitag < 0)
        DoWallBreakMatch(bp->hitag);
    
    if (!break_info)    
        {
        //DSPRINTF(ds,"Break Info not found - sprite %d", bp - sprite);
        MONO_PRINT(ds);
        return(FALSE);
        }

    // Check to see if it should break with current weapon type
    if(!CheckBreakToughness(break_info, type)) 
        {
        if (break_info->breaknum != -1)
            {
            if (!TEST(break_info->flags, BF_LEAVE_BREAK))
                {
                RESET(bp->extra, SPRX_BREAKABLE);
                RESET(bp->cstat, CSTAT_SPRITE_BREAKABLE);
                }
            
            bp->picnum = break_info->breaknum;
            // pass Break Info Globally
            GlobBreakInfo = break_info;
            SpawnShrap(BreakSprite, -1);
            GlobBreakInfo = NULL;
            if(bp->picnum == 3683)
                RESET(bp->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
            }
            
        return(FALSE);
        }

    RESET(bp->extra, SPRX_BREAKABLE);
    RESET(bp->cstat, CSTAT_SPRITE_BREAKABLE);
    
    // pass Break Info Globally
    GlobBreakInfo = break_info;
    SpawnShrap(BreakSprite, -1);
    GlobBreakInfo = NULL;
    
    // kill it or change the pic
    if (TEST(break_info->flags, BF_KILL) || break_info->breaknum == -1)
        {
        if (TEST(break_info->flags, BF_FIRE_FALL))
            SpawnBreakFlames(BreakSprite);
        
        RESET(bp->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
        SET(bp->cstat, CSTAT_SPRITE_INVISIBLE);
        // Kill sound if one is attached
        DeleteNoSoundOwner(BreakSprite);
        KillBreakSprite(BreakSprite);
        //change_sprite_stat(BreakSprite, STAT_SUICIDE);
        //KillSprite(BreakSprite);
        return(TRUE);
        }
    else
        {
        bp->picnum = break_info->breaknum;
        if(bp->picnum == 3683)
            RESET(bp->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
        }    
    
    return(FALSE);    
    }

BOOL NullActor(USERp u)
    {
    // a Null Actor is defined as an actor that has no real controlling programming attached
    
    // check to see if attached to SO
    if (TEST(u->Flags, SPR_SO_ATTACHED))
        return(TRUE);

    // does not have a STATE or FUNC to control it
    if (!u->State)
        return(TRUE);

    // does not have a STATE or FUNC to control it
    if (!u->ActorActionFunc)
        return(TRUE);

    return(FALSE);
    }

int HitBreakSprite(short BreakSprite, short type)
    {
    SPRITEp sp;
    SPRITEp bp = &sprite[BreakSprite];
    USERp bu = User[BreakSprite];
    short match = bp->lotag;
    short match_extra;
    short SpriteNum;
    BREAK_INFOp break_info;

    // ignore as a breakable if true
    //if (sp->lotag == TAG_SPRITE_HIT_MATCH)
    //    return(FALSE);
    
    if (TEST_BOOL1(bp))
        {
        if (TEST_BOOL2(bp))
            return(FALSE);
            
        return(UserBreakSprite(BreakSprite));
        }

    if (bu && !NullActor(bu))
        {
        // programmed animating type - without BOOL1 set
        if (bp->lotag)
            DoLightingMatch(bp->lotag, -1);
            
        SpawnShrap(BreakSprite, -1);
        RESET(bp->extra, SPRX_BREAKABLE);
        return(FALSE);
        }
    
    return(AutoBreakSprite(BreakSprite,type));    
    }

int SectorOfWall(short theline)
    {
        short i, startwall, endwall, sectnum;

        sectnum = -1;
        
        for(i=0;i<numsectors;i++)
            {
            startwall = sector[i].wallptr;
            endwall = startwall + sector[i].wallnum - 1;
            if ((theline >= startwall) && (theline <= endwall))
                {
                sectnum = i;
                break;
                }
            }
            
        return(sectnum);
    }

void DoWallBreakMatch( short match )    
    {
    short i,sectnum;
    long x,y,z;
    WALLp wp;
    short nw,wall_ang;
    
    for(i=0; i<=numwalls; i++)
        {
        if (wall[i].hitag == match)
            {
            WallBreakPosition(i, &sectnum, &x, &y, &z, &wall_ang);
            //nw = wall[i].point2;
            //wall_ang = NORM_ANGLE(getangle(wall[nw].x - wall[i].x, wall[nw].y - wall[i].y)+512);
            wp = &wall[i];
            wp->hitag = 0; // Reset the hitag
            AutoBreakWall(wp, x, y, z, wall_ang, 0);    
            }
        }
    }

void DoWallBreakSpriteMatch( short match )    
    {
    short i,nexti;
    
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_ENEMY], i, nexti)
        {
        SPRITEp sp = &sprite[i];
        
        if (sp->hitag == match)
            {
            KillSprite(i);
            }
        }    
    }    
