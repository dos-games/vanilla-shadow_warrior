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
#include "sector.h"
#include "player.h"

#define LAVASIZ 128
#define LAVALOGSIZ 7
#define LAVAMAXDROPS 32
#define DEFAULT_DOOR_SPEED 200
#define DEFAULT_ELEVATOR_SPEED 200

ANIM_CALLBACK CallbackCeilingStomperDown;
ANIM_CALLBACK CallbackCeilingStomperUp;
ANIM_CALLBACK CallbackFloorStomperDown;
ANIM_CALLBACK CallbackFloorStomperUp;
    
PLAYERp GlobPlayerP;

char lavabakpic[(LAVASIZ + 2) * (LAVASIZ + 2)], lavainc[LAVASIZ];
long lavanumdrops, lavanumframes;
long lavadropx[LAVAMAXDROPS], lavadropy[LAVAMAXDROPS];
long lavadropsiz[LAVAMAXDROPS], lavadropsizlookup[LAVAMAXDROPS];
long lavaradx[32][128], lavarady[32][128], lavaradcnt[32];

SECT_USERp SectUser[MAXSECTORS];

short
 FindSectorByTag_Wall(short wallnum, long tag);
short
 FindSectorByTag(long x, long y, long tag);
long
 SectorDistance(short sect1, long sect2);
long
 SectorDistanceByMid(short sect1, long sect2);
short
 FindNextSectorByTag(short sectnum, long tag);
short
 FindSectorMidByTag(short sector, long tag);

ANIM Anim[MAXANIM];   
short AnimCnt = 0;
    
extern short KeyDoorMessageTime, KeyDoorMessageNum;
extern CHARp KeyDoorMessage[];

#define KEY_DOOR_MESSAGE_TIME (3*120)

int OperateSector(short, short);
VOID SectorMidPoint(short sectnum, long *xmid, long *ymid, long *zmid);

typedef struct
    {
    long floor_origz, ceiling_origz, range;
    short sector, sintable_ndx, speed_shift;
    char flags;
    } SINE_WAVE_FLOOR, *SINE_WAVE_FLOORp;

#define MAX_SINE_WAVE 6
SINE_WAVE_FLOOR SineWaveFloor[MAX_SINE_WAVE][21];

typedef struct
    {
    long orig_xy, range;
    short wall, sintable_ndx, speed_shift, type;
    } SINE_WALL, *SINE_WALLp;

#define MAX_SINE_WALL 10
#define MAX_SINE_WALL_POINTS 64
SINE_WALL SineWall[MAX_SINE_WALL][MAX_SINE_WALL_POINTS];

typedef struct
    {
    short sector, dir, time_down, time_up, timer;
    long floorz;
    } SPIKE;

SPIKE Spike[50];

FLOOR_PAN FloorPan[MAX_FLOOR_PAN];
CEILING_PAN CeilingPan[MAX_CEILING_PAN];
WALL_PAN WallPan[MAX_WALL_PAN];
short NextWallPan = 0;

WATER Water[MAX_WATER];

typedef struct
    {
    short Sector, TimeOut;
    } SPRING_BOARD;

SPRING_BOARD SpringBoard[20];

SWING Rotate[17];

typedef struct
    {
    short sector, speed;
    long xmid, ymid;
    } SPIN;

SPIN Spin[17];

LIGHTS Lights[50];

ELEVATOR_AUTO ElevatorAuto[25];
ELEVATOR_AUTO PauseAction[50];

DOOR_AUTO_CLOSE DoorAutoClose[MAX_DOOR_AUTO_CLOSE];

VOID
WallSetup(VOID)
    {
    short i = 0, tag;
    short NextSineWall = 0;

    memset(SineWall, 0xFF, sizeof(SineWall));

    // Reset all wall.extra's so we can set correct bits
    for (i = 0; i < numwalls; i++)
	{
	wall[i].extra = 0;
	}

    for (i = 0; i < numwalls; i++)
	{
	tag = wall[i].lotag;

	switch (tag)
	    {
	case TAG_WALL_LOOP_DONT_SPIN:
		{
		short wall_num, start_wall;

		// set first wall
		SET(wall[i].extra, WALLF_LOOP_DONT_SPIN);
		SET(wall[wall[i].nextwall].extra, WALLF_LOOP_DONT_SPIN);

		// move the the next wall
		start_wall = wall[i].point2;

		// Travel all the way around loop setting wall bits
		for (wall_num = start_wall;
		    wall[wall_num].lotag != TAG_WALL_LOOP_DONT_SPIN;
		    wall_num = wall[wall_num].point2)
		    {
		    SET(wall[wall_num].extra, WALLF_LOOP_DONT_SPIN);
		    SET(wall[wall[wall_num].nextwall].extra, WALLF_LOOP_DONT_SPIN);
		    }

		break;
		}

	case TAG_WALL_LOOP_TELEPORTER_PAN:
		{
		short wall_num;
		WALL_PAN *wp;
		short vel,w2;

		vel = wall[i].hitag;
		if (vel == 0)
		    vel = 4;

		wall_num = i;
		
		do
		    {
		    wp = &WallPan[NextWallPan];
		    
		    wp->wall = wall_num;
		    wp->wallp = &wall[wp->wall];
		    wp->vel = vel;
		    wp->ang = 1024+512;
		    
		    RESET(wp->wallp->cstat, CSTAT_WALL_ALIGN_BOTTOM);
		    
		    NextWallPan++;
		    ASSERT(NextWallPan < MAX_WALL_PAN-1);
		    
		    #if 1
		    // do the other side of the masked wall
		    w2 = wall[wall_num].nextwall;
		    if (w2 >= 0)
			{
			wp = &WallPan[NextWallPan];
			
			wp->wall = w2;
			wp->wallp = &wall[wp->wall];
			wp->vel = vel;
			wp->ang = 1024+512;
			
			RESET(wp->wallp->cstat, CSTAT_WALL_ALIGN_BOTTOM);
			
			NextWallPan++;
			ASSERT(NextWallPan < MAX_WALL_PAN-1);
			}
		    #endif    
		    
		    wall_num = wall[wall_num].point2;
		    }
		  while(wall[wall_num].lotag != TAG_WALL_LOOP_TELEPORTER_PAN);    

		break;
		}

	case TAG_WALL_LOOP_OUTER:
		{
		short wall_num, start_wall;

		// set first wall
		SET(wall[i].extra, WALLF_LOOP_OUTER);
		SET(wall[wall[i].nextwall].extra, WALLF_LOOP_OUTER);

		// move the the next wall
		start_wall = wall[i].point2;

		// Travel all the way around loop setting wall bits
		for (wall_num = start_wall;
		    wall[wall_num].lotag != TAG_WALL_LOOP_OUTER;
		    wall_num = wall[wall_num].point2)
		    {
		    SET(wall[wall_num].extra, WALLF_LOOP_OUTER);
		    SET(wall[wall[wall_num].nextwall].extra, WALLF_LOOP_OUTER);
		    }

		wall[i].lotag = 0;

		break;
		}

	case TAG_WALL_DONT_MOVE:
		{
		// set first wall
		SET(wall[i].extra, WALLF_DONT_MOVE);
		break;
		}

	case TAG_WALL_LOOP_SPIN_2X:
		{
		short wall_num, start_wall;

		// set first wall
		SET(wall[i].extra, WALLF_LOOP_SPIN_2X);
		SET(wall[wall[i].nextwall].extra, WALLF_LOOP_SPIN_2X);

		// move the the next wall
		start_wall = wall[i].point2;

		// Travel all the way around loop setting wall bits
		for (wall_num = start_wall;
		    wall[wall_num].lotag != TAG_WALL_LOOP_SPIN_2X;
		    wall_num = wall[wall_num].point2)
		    {
		    SET(wall[wall_num].extra, WALLF_LOOP_SPIN_2X);
		    SET(wall[wall[wall_num].nextwall].extra, WALLF_LOOP_SPIN_2X);
		    }

		break;
		}

	case TAG_WALL_LOOP_SPIN_4X:
		{
		short wall_num, start_wall;

		// set first wall
		SET(wall[i].extra, WALLF_LOOP_SPIN_4X);
		SET(wall[wall[i].nextwall].extra, WALLF_LOOP_SPIN_4X);

		// move the the next wall
		start_wall = wall[i].point2;

		// Travel all the way around loop setting wall bits
		for (wall_num = start_wall;
		    wall[wall_num].lotag != TAG_WALL_LOOP_SPIN_4X;
		    wall_num = wall[wall_num].point2)
		    {
		    SET(wall[wall_num].extra, WALLF_LOOP_SPIN_4X);
		    SET(wall[wall[wall_num].nextwall].extra, WALLF_LOOP_SPIN_4X);
		    }

		break;
		}

	case TAG_WALL_LOOP_REVERSE_SPIN:
		{
		short wall_num, start_wall;

		// set first wall
		SET(wall[i].extra, WALLF_LOOP_REVERSE_SPIN);
		SET(wall[wall[i].nextwall].extra, WALLF_LOOP_REVERSE_SPIN);

		// move the the next wall
		start_wall = wall[i].point2;

		// Travel all the way around loop setting wall bits
		for (wall_num = start_wall;
		    wall[wall_num].lotag != TAG_WALL_LOOP_REVERSE_SPIN;
		    wall_num = wall[wall_num].point2)
		    {
		    SET(wall[wall_num].extra, WALLF_LOOP_REVERSE_SPIN);
		    SET(wall[wall[wall_num].nextwall].extra, WALLF_LOOP_REVERSE_SPIN);
		    }

		break;
		}

	case TAG_WALL_SINE_Y_BEGIN:
	case TAG_WALL_SINE_X_BEGIN:
		{
		short wall_num, cnt, last_wall, num_points, type, tag_end;
		SINE_WALLp sw;
		short range = 250, speed = 3, peak = 0;

		tag_end = tag + 2;

		type = tag - TAG_WALL_SINE_Y_BEGIN;


		// count up num_points
		for (wall_num = i, num_points = 0;
		    num_points < MAX_SINE_WALL_POINTS && wall[wall_num].lotag != tag_end;
		    wall_num = wall[wall_num].point2, num_points++)
		    {
		    if (num_points == 0)
			{
			if (wall[wall_num].hitag)
			    range = wall[wall_num].hitag;
			}
		    else if (num_points == 1)
			{
			if (wall[wall_num].hitag)
			    speed = wall[wall_num].hitag;
			}
		    else if (num_points == 2)
			{
			if (wall[wall_num].hitag)
			    peak = wall[wall_num].hitag;
			}
		    }

		if (peak)
		    num_points = peak;

		for (wall_num = i, cnt = 0;
		    cnt < MAX_SINE_WALL_POINTS && wall[wall_num].lotag != tag_end;
		    wall_num = wall[wall_num].point2, cnt++)
		    {
		    // set the first on up
		    sw = &SineWall[NextSineWall][cnt];

		    sw->type = type;
		    sw->wall = wall_num;
		    sw->speed_shift = speed;
		    sw->range = range;

		    if (!sw->type)
			sw->orig_xy = wall[wall_num].y - (sw->range >> 2);
		    else
			sw->orig_xy = wall[wall_num].x - (sw->range >> 2);

		    sw->sintable_ndx = cnt * (2048 / num_points);
		    }

		NextSineWall++;

		ASSERT(NextSineWall < MAX_SINE_WALL);

		}
	    }
	}
    }


VOID
SectorSetup(VOID)
    {
    short i = 0, k, tag;
    ELEVATOR_AUTOp ep;
    short NextLight = 0, NextElevatorAuto = 0, NextBoatWall = 0, NextSpike = 0, NextSpin = 0, NextWater = 0, NextBlock = 0, NextSineWave = 0, rotcnt = 0, swingcnt = 0;

    short startwall, endwall, j, ndx, door_sector;
    
    WallSetup();

    for (ndx = 0; ndx < 20; ndx++)
	{
	memset(&SectorObject[ndx], 0xFF, sizeof(SectorObject[0]));
	SectorObject[ndx].xmid = MAXLONG;
	}
	
//    memset(SectorObject, 0xFF, sizeof(SectorObject));
    memset(Lights, -1, sizeof(Lights));
    memset(ElevatorAuto, 0, sizeof(ElevatorAuto));
    memset(DoorAutoClose, 0, sizeof(DoorAutoClose));
    memset(Rotate, 0xFF, sizeof(Rotate));
    memset(SineWaveFloor, 0xFF, sizeof(SineWaveFloor));
    memset(Spike, 0xFF, sizeof(Spike));
    memset(SpringBoard, 0xFF, sizeof(SpringBoard));
    memset(PauseAction, 0xFF, sizeof(PauseAction));
    memset(Water, 0xFF, sizeof(Water));
//    memset(Block, 0, sizeof(Block));
    memset(Spin, 0, sizeof(Spin));

    for (i = 0; i < MAX_SW_PLAYERS; i++)
	memset((Player+i)->HasKey, 0, sizeof((Player+i)->HasKey));

    for (i = 0; i < numsectors; i++)
	{
	tag = LOW_TAG(i);

	if (tag >= TAG_OBJECT && tag < TAG_OBJECT + 100)
	    {
	    SetupSectorObject(i, tag);
	    }

	if (TEST(sector[i].extra, SECTFX_SINK))
	    {
	    Water[NextWater].sector = i;

	    Water[NextWater].depth = Z(SectUser[i]->depth);

	    NextWater++;

	    ASSERT(NextWater < SIZ(Water));
	    }

#if 0
	if (TEST(sector[i].extra, SECTFX_SPECIAL))
	    {
	    SECT_USERp su = SectUser[i];

	    ASSERT(su);

	    switch (su->special)
		{
	    case SPECIAL_BLOCK_ACTOR:
		Block[NextBlock].sectp = &sector[i];

		Block[NextBlock].origz = sector[i].floorz;

		NextBlock++;

		ASSERT(NextBlock < SIZ(Block));
		break;
		}
	    }
#endif


	switch (tag)
	    {
	case TAG_SPIKE:

	    Spike[NextSpike].sector = i;

	    Spike[NextSpike].dir = 0;

	    Spike[NextSpike].time_down = 600 + RANDOM_P2(128);
	    Spike[NextSpike].time_up = 300 + RANDOM_P2(128);

	    Spike[NextSpike].timer = 0 + RANDOM_P2(256);

	    sector[i].floorz -= Z(16);

	    Spike[NextSpike].floorz = sector[nextsectorneighborz(i, sector[i].floorz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;

	    sector[i].floorz = Spike[NextSpike].floorz;

	    NextSpike++;

	    ASSERT(NextSpike < SIZ(Spike));


	    break;

	case TAG_SINE_WAVE_FLOOR:
	case TAG_SINE_WAVE_CEILING:
	case TAG_SINE_WAVE_BOTH:
		{
		SINE_WAVE_FLOOR *swf;
		short near_sect = i, base_sect = i;
		short swf_ndx = 0;
		short cnt = 0, sector_cnt;
		long range;
		long range_diff = 0;
		long wave_diff = 0;
		short peak_dist = 0;
		short speed_shift = 3;
		short num;

#define SINE_FLOOR (1<<0)
#define SINE_CEILING (1<<1)

		num = (tag - TAG_SINE_WAVE_FLOOR) / 20;

		// set the first on up
		swf = &SineWaveFloor[NextSineWave][swf_ndx];

		swf->flags = 0;

		switch (num)
		    {
		case 0:
		    SET(swf->flags, SINE_FLOOR);
		    break;
		case 1:
		    SET(swf->flags, SINE_CEILING);
		    break;
		case 2:
		    SET(swf->flags, SINE_FLOOR | SINE_CEILING);
		    break;
		    }

		swf->sector = near_sect;
		ASSERT(sector[swf->sector].hitag != 0);
		swf->range = range = Z(sector[swf->sector].hitag);
		swf->floor_origz = sector[swf->sector].floorz - (range >> 2);
		swf->ceiling_origz = sector[swf->sector].ceilingz - (range >> 2);

		// look for the rest by distance
		for (swf_ndx = 1, sector_cnt = 1; TRUE; swf_ndx++)
		    {
		    //near_sect = FindNextSectorByTag(base_sect, TAG_SINE_WAVE_FLOOR + swf_ndx);
		    near_sect = FindNextSectorByTag(base_sect, tag + swf_ndx);

		    if (near_sect >= 0)
			{
			swf = &SineWaveFloor[NextSineWave][swf_ndx];

			if (swf_ndx == 1 && sector[near_sect].hitag)
			    range_diff = sector[near_sect].hitag;
			else 
			if (swf_ndx == 2 && sector[near_sect].hitag)
			    speed_shift = sector[near_sect].hitag;
			else 
			if (swf_ndx == 3 && sector[near_sect].hitag)
			    peak_dist = sector[near_sect].hitag;

			swf->sector = near_sect;
			swf->floor_origz = sector[swf->sector].floorz - (range >> 2);
			swf->ceiling_origz = sector[swf->sector].ceilingz - (range >> 2);
			range -= range_diff;
			swf->range = range;

			base_sect = swf->sector;
			sector_cnt++;
			}
		    else
			break;
		    }


		ASSERT(swf_ndx <= SIZ(SineWaveFloor[NextSineWave]));

		// more than 6 waves and something in high tag - set up wave
		// dissapate
		if (sector_cnt > 8 && sector[base_sect].hitag)
		    {
		    wave_diff = sector[base_sect].hitag;
		    }

		// setup the sintable_ndx based on the actual number of
		// sectors (swf_ndx)
		for (swf = &SineWaveFloor[NextSineWave][0], cnt = 0; swf->sector >= 0 && swf < &SineWaveFloor[SIZ(SineWaveFloor)]; swf++, cnt++)
		    {
		    if (peak_dist)
			swf->sintable_ndx = cnt * (2048 / peak_dist);
		    else
			swf->sintable_ndx = cnt * (2048 / swf_ndx);

		    swf->speed_shift = speed_shift;
		    }

		// set up the a real wave that dissapates at the end
		if (wave_diff)
		    {
		    for (cnt = sector_cnt - 1; cnt >= 0; cnt--)
			{
			// only do the last (actually the first) few for the
			// dissapate
			if (cnt > 8)
			    continue;

			swf = &SineWaveFloor[NextSineWave][cnt];

			swf->range -= wave_diff;

			wave_diff += wave_diff;

			if (swf->range < Z(4))
			    swf->range = Z(4);

			// reset origz's based on new range
			swf->floor_origz = sector[swf->sector].floorz - (swf->range >> 2);
			swf->ceiling_origz = sector[swf->sector].ceilingz - (swf->range >> 2);
			}
		    }

		NextSineWave++;

		ASSERT(NextSineWave < MAX_SINE_WAVE);

		break;
		}

	case TAG_DOOR_ROTATE:

	    startwall = sector[i].wallptr;
	    endwall = startwall + sector[i].wallnum - 1;

	    for (j = startwall; j <= endwall; j++)
		{
		if (wall[j].lotag == TAG_WALL_ROTATE_PIVOT)
		    {
		    door_sector = wall[j].nextsector;

		    if (sector[door_sector].lotag == TAG_DOOR_ROTATE_POS)
			{
			Rotate[rotcnt].angopen = 512;
			Rotate[rotcnt].angclosed = 0;
			Rotate[rotcnt].angopendir = 1;
			}
		    else if (sector[door_sector].lotag == TAG_DOOR_ROTATE_NEG)
			{
			Rotate[rotcnt].angopen = 1536;
			Rotate[rotcnt].angclosed = 0;
			Rotate[rotcnt].angopendir = -1;
			}

		    Rotate[rotcnt].wall[0] = j;

		    for (k = 1; k < sector[door_sector].wallnum; k++)
			{
			Rotate[rotcnt].wall[k] = wall[Rotate[rotcnt].wall[k - 1]].point2;
			}

//                    Rotate[rotcnt].wall[0] = j;
//                    Rotate[rotcnt].wall[1] = wall[j].point2;
//                    Rotate[rotcnt].wall[2] = wall[wall[j].point2].point2;
//                    Rotate[rotcnt].wall[3] = wall[wall[wall[j].point2].point2].point2;

//                    for (k = 0; k < 4; k++)
		    for (k = 0; k < sector[door_sector].wallnum; k++)
			{
			Rotate[rotcnt].origx[k] = wall[Rotate[rotcnt].wall[k]].x;
			Rotate[rotcnt].origy[k] = wall[Rotate[rotcnt].wall[k]].y;
			}

		    Rotate[rotcnt].sector = i;
		    Rotate[rotcnt].sang = Rotate[rotcnt].angclosed;
		    Rotate[rotcnt].anginc = 0;

		    rotcnt++;

		    ASSERT(rotcnt < SIZ(Rotate));

		    }
		}

	    break;

	case TAG_LIGHT_FADE:
	case TAG_LIGHT_TORCH_FADE:
		{
		short cur_sect, near_sect;
		LIGHTS *l = &Lights[NextLight];
		long dist;

		cur_sect = i;
		l->SectorCount = 0;
		l->ShadeShift = 0;

		while (TRUE)
		    {
		    // look for diffused lighting close by
		    // near_sect =
		    // FindSectorByTag_Wall(sector[cur_sect].wallptr,TAG_LIGHT_FADE
		    // _DIFFUSE);
		    near_sect = FindNextSectorByTag(cur_sect, TAG_LIGHT_FADE_DIFFUSE);



//                sprintf(ds,"%d = near",(long)near_sect);
//                MONO_PRINT(ds);



		    // if not found quit
		    if (near_sect == -1)
			{
			goto GeneralLight;
			}

		    // Reset tag
		    if (HIGH_TAG(near_sect))
			l->ShadeShift = HIGH_TAG(near_sect);

		    sector[near_sect].lotag = 0;
		    cur_sect = near_sect;
		    l->SectorList[l->SectorCount++] = near_sect;
		    }

		goto GeneralLight;
		}

	case TAG_LIGHT_TORCH:
	case TAG_LIGHT_TORCH_NO_FLOOR:

    GeneralLight:
		{
		LIGHTS *l = &Lights[NextLight];

		l->Sector = i;
		l->Tics = 0;

		// Speed for fading etc
		if (HIGH_TAG(i))
		    l->MaxTics = HIGH_TAG(i);
		else
		    l->MaxTics = 3;

		// For fading - ranges from MaxDark to MinDark
		l->MaxDark = sector[i].floorshade;
		l->MinDark = sector[i].ceilingshade;
		// init shade and dir
		l->Shade = sector[i].ceilingshade + 1;
		l->Dir = 0;
		NextLight++;

		ASSERT(NextLight < SIZ(Lights));

		break;
		}

	case TAG_ELEVATOR_AUTO:
	    ep = &ElevatorAuto[NextElevatorAuto];
	    ep->Sector = i;
	    ep->MaxTics = 4 * 120;
	    ep->Speed = 128;
	    ep->Tics = 0;
	    NextElevatorAuto++;

	    ASSERT(NextElevatorAuto < SIZ(ElevatorAuto));

	    break;
	    }
	}
    }


VOID 
SectorMidPoint(short sectnum, long *xmid, long *ymid, long *zmid)
    {
    short startwall, endwall, j;
    long xsum = 0, ysum = 0;
    WALLp wp;

    startwall = sector[sectnum].wallptr;
    endwall = startwall + sector[sectnum].wallnum - 1;

    for (wp = &wall[startwall], j = startwall; j <= endwall; wp++, j++)
	{
	xsum += wp->x;
	ysum += wp->y;
	}

    *xmid = xsum / (endwall - startwall + 1);
    *ymid = ysum / (endwall - startwall + 1);

    *zmid = DIV2(sector[sectnum].floorz + sector[sectnum].ceilingz);
    }


DOOR_AUTO_CLOSEp
FindDoorAutoClose(short SectorNum)
    {
    short door;
    DOOR_AUTO_CLOSEp dacp = NULL;

    for (door = 0; door < MAX_DOOR_AUTO_CLOSE; door++)
	{
	if (DoorAutoClose[door].Sector == SectorNum)
	    return (&DoorAutoClose[door]);

	if (!dacp && !DoorAutoClose[door].Sector)
	    dacp = &DoorAutoClose[door];

	}

    return (dacp);
    }


DOOR_AUTO_CLOSEp
SetDoorAutoClose(short SectorNum, short Type)
    {
    DOOR_AUTO_CLOSEp dacp;

    dacp = FindDoorAutoClose(SectorNum);

    ASSERT(dacp != NULL);

    if (dacp->Sector == NULL)
	{
	// if empty set up an entry to close the door later
	dacp->Type = Type;
	dacp->Sector = SectorNum;
	dacp->Speed = 128;
	dacp->TimeOut = 4 * 128;
	}
    else
	{
	dacp->TimeOut = 4 * 128;
	}

    return (dacp);
    }

VOID
ResetDoorAutoClose(short SectorNum)
    {
    DOOR_AUTO_CLOSEp dacp;

    dacp = FindDoorAutoClose(SectorNum);

    ASSERT(dacp != NULL);

    if (dacp->Sector)
	memset(dacp, 0, sizeof(*dacp));
    }


VOID
DoSwingingDoors(VOID)
    {
    short i, j, k;
    long nx, ny;

    for (i = 0; Rotate[i].sector != -1; i++)
	{
	if (Rotate[i].anginc != 0)
	    {
	    for (j = 0; j < (synctics << 2); j++)
		{
		// Increment swing angle and NORMALIZE it
		Rotate[i].sang = NORM_ANGLE(Rotate[i].sang + Rotate[i].anginc);

		// If ang is closed then reset inc
		if (Rotate[i].sang == Rotate[i].angclosed)
		    {
		    WALLp wallp;

		    wallp = &wall[Rotate[i].wall[0]];
		    PlaySound(DIGI_DOOR_CLOSE, SoundAngle(wallp->x, wallp->y), SoundVol(wallp->x, wallp->y));

		    Rotate[i].anginc = 0;
		    }

		// If ang is open then reset
		if (Rotate[i].sang == Rotate[i].angopen)
		    Rotate[i].anginc = 0;
		}


	    // sprintf(ds,"%d, %d",(long)Rotate[i].ang,
	    // (long)Rotate[i].angopen );
	    // MONO_PRINT(ds);


	    // Rotate the other 3 points about the first one
	    // for (k = 1; k <= 3; k++)
	    for (k = 1; Rotate[i].wall[k] != -1; k++)
		{
		rotatepoint(Rotate[i].origx[0], Rotate[i].origy[0],
		    Rotate[i].origx[k], Rotate[i].origy[k],
		    Rotate[i].sang, &nx, &ny);

		dragpoint(Rotate[i].wall[k], nx, ny);
		}
	    }
	}
    }

SectorShade(short sec, short intensity, short flags)
    {
    short s, startwall, endwall;

#define SHADE_CEILING (1<<0)
#define SHADE_FLOOR (1<<1)
#define SHADE_WALLS (1<<2)

    if (TEST(flags, SHADE_FLOOR))
	sector[sec].floorshade = intensity;     // floor change

    if (TEST(flags, SHADE_CEILING))
	sector[sec].ceilingshade = intensity;   // ceiling change

    // change walsec
    if (TEST(flags, SHADE_WALLS))
	{
	startwall = sector[sec].wallptr;
	endwall = startwall + sector[sec].wallnum - 1;
	for (s = startwall; s <= endwall; s++)
	    wall[s].shade = intensity;
	}
    }

VOID
DoLights(VOID)
    {
    short ls, s, startwall, endwall, i, shade;
    short flags = SHADE_WALLS | SHADE_FLOOR | SHADE_CEILING;

    LIGHTSp lp;

#define RANDOM_TORCH_LIGHT 8

#define FADE_MAX 27
#define FADE_MIN 0

    for (lp = &Lights[0]; (ls = lp->Sector)>=0; lp++)
	{

	switch (LOW_TAG(ls))
	    {

	case TAG_LIGHT_TORCH:
	    lp->Tics += synctics;
	    while(lp->Tics >= lp->MaxTics)
	    {
	       lp->Tics -= lp->MaxTics;
	       if (RANDOM_P2(128) > 64)
		   SectorShade(ls, lp->MinDark + RANDOM_P2(RANDOM_TORCH_LIGHT), flags);
	    }
	    break;

	    // don't change the floor shading
	case TAG_LIGHT_TORCH_NO_FLOOR:
	    lp->Tics += synctics;
	    while(lp->Tics >= lp->MaxTics)
	    {
	       lp->Tics -= lp->MaxTics;
	       if (RANDOM_P2(128) > 64)
	       {
		  RESET(flags, SHADE_FLOOR);
		  SectorShade(ls, lp->MinDark + RANDOM_P2(RANDOM_TORCH_LIGHT), flags);
		  SET(flags, SHADE_FLOOR);
	       }
	    }
	    break;

	case TAG_LIGHT_FADE:
		{
		// adjust Shade based on clock
		lp->Tics += synctics;

		while (lp->Tics >= lp->MaxTics)
		    {
		    lp->Tics -= lp->MaxTics;

		    if (lp->Dir == 1)
			{
			lp->Shade++;

			if (lp->Shade >= lp->MaxDark)   // FADE_MAX)
			    lp->Dir = 0;
			}
		    else
			{
			lp->Shade--;

			if (lp->Shade <= lp->MinDark)   // FADE_MIN)
			    {
			    lp->Dir = 1;
			    }
			}
		    }

		// Do diffused shading
		for (i = 0; i < lp->SectorCount; i++)
		    {
		    shade = lp->Shade + ((i + 1) << lp->ShadeShift);

		    if (shade > lp->MaxDark)
			shade = lp->MaxDark;

		    SectorShade(lp->SectorList[i], shade, flags);
		    }

		SectorShade(ls, lp->Shade, flags);

		break;
		} // End light fade

	case TAG_LIGHT_TORCH_FADE:
		{
		   // I added tics because letting it run at full speed
		   // was flickering way too fast on my P90.
		   lp->Tics += synctics;

		   while(lp->Tics >= lp->MaxTics)
		   {
		      lp->Tics -= lp->MaxTics;
		      
		      if(RANDOM_P2(128) > 64)
		      {
		      int rndshade;
		      rndshade = RANDOM_P2(RANDOM_TORCH_LIGHT);
		      
		      SectorShade(ls, lp->MinDark+rndshade+lp->Shade, flags);

		      // Do diffused shading
		      for (i = 0; i < lp->SectorCount; i++)
		      {
			    shade = lp->Shade + ((i + 1) << lp->ShadeShift);

			    if (shade > lp->MaxDark)
			    shade = lp->MaxDark;

			    SectorShade(lp->SectorList[i], lp->MinDark+(rndshade+shade), flags);

		      }
		      
		      } // if
		   } // while

		break;
		}       // Torchlight fade


	    }           // switch
	}               // got through sector list

    }                   

VOID
ToggleElevator(short elevator_sector, short elevator_speed)
    {
    int sec, destz, i;

    i = AnimGetGoal(&sector[elevator_sector].floorz);
    
    if (sector[elevator_sector].hitag == 0)
	elevator_speed = DEFAULT_ELEVATOR_SPEED;
    else    
	elevator_speed = sector[elevator_sector].hitag;

    if (i < 0)
	{
	// Look up for a floor
	sec = nextsectorneighborz(elevator_sector, sector[elevator_sector].floorz, SEARCH_FLOOR, SEARCH_UP);

	// If did not find a floor, look down for floor ELSE already have
	// the
	// floor
	if (sec == -1)
	    {
	    destz = sector[nextsectorneighborz(elevator_sector, sector[elevator_sector].floorz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;
	    }
	else
	    {
	    destz = sector[sec].floorz;
	    }

	// Tell floor to go to destz
	AnimSet(&sector[elevator_sector].floorz, destz, elevator_speed);
	}

    }

short
MoveElevator(short elevator_sector, short elevator_speed, short search_dir)
    {
    int sec, destz, i;

    i = AnimGetGoal(&sector[elevator_sector].floorz);

    if (sector[elevator_sector].hitag == 0)
	elevator_speed = DEFAULT_ELEVATOR_SPEED;
    else    
	elevator_speed = sector[elevator_sector].hitag;

    if (i < 0)
	{
	// Look up for a floor
	sec = nextsectorneighborz(elevator_sector, sector[elevator_sector].floorz, SEARCH_FLOOR, search_dir);

	// If did not find a floor, look down for floor ELSE already have
	// the
	// floor
	if (sec == -1)
	    {
	    return (NULL);
	    }

	destz = sector[sec].floorz;

	// Tell floor to go to destz
	AnimSet(&sector[elevator_sector].floorz, destz, elevator_speed);

	return (search_dir);
	}

    return (NULL);
    }

#if 0
int
MoveDoorVert(short door_sector, short dir, short door_speed)
    {
    int destz, i;

    i = AnimGetGoal(&sector[door_sector].ceilingz);

    if (i < 0)
	{
	// if door is closed look up for ceiling ELSE look down for floor
	if (sector[door_sector].ceilingz == sector[door_sector].floorz)
	    {
	    if (dir == CLOSE)
		return (0);

	    // Move ceiling
	    destz = sector[nextsectorneighborz(door_sector, sector[door_sector].ceilingz, SEARCH_CEILING, SEARCH_UP)].ceilingz;
	    AnimSet(&sector[door_sector].ceilingz, destz, door_speed);

	    // Move floor
	    destz = sector[nextsectorneighborz(door_sector, sector[door_sector].floorz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;
	    AnimSet(&sector[door_sector].floorz, destz, door_speed);
	    }
	else
	    {
	    if (dir == OPEN)
		return (0);

	    destz = ((sector[door_sector].ceilingz + sector[door_sector].floorz) >> 1);
	    AnimSet(&sector[door_sector].floorz, destz, door_speed);
	    AnimSet(&sector[door_sector].ceilingz, destz, door_speed);
	    }

	return (door_sector);
	}

    return (0);
    }
#endif

int
MoveDoorUp(short door_sector, short auto_close, short door_speed)
    {
    int destz = -999999, i;
    short ret_val = CLOSE;

    i = AnimGetGoal(&sector[door_sector].ceilingz);
    
    // Note: I'm overriding the passed in door_speed - it aint used
    
    if (sector[door_sector].hitag == 0)
	door_speed = DEFAULT_DOOR_SPEED;
    else    
	door_speed = sector[door_sector].hitag;

    if (i < 0)
	{

	// if door is closed look up for ceiling ELSE look down for floor
	if (sector[door_sector].ceilingz == sector[door_sector].floorz)
	    {
	    short startwall,endwall,j;
	    ret_val = OPEN;

	    // look for a specific stopping place first
	    startwall = sector[door_sector].wallptr;
	    endwall = startwall + sector[door_sector].wallnum - 1;

	    for (j = startwall; j <= endwall; j++)
		{
		if (sector[wall[j].nextsector].lotag == TAG_DOOR_UP_DEST)
		    {
		    destz = sector[wall[j].nextsector].ceilingz;
		    break;
		    }
		}
		
	    // look up for nearest ceiling    
	    if (destz == -999999)    
		destz = sector[nextsectorneighborz(door_sector, sector[door_sector].ceilingz, SEARCH_CEILING, SEARCH_UP)].ceilingz;
	    }
	else
	    {
	    destz = sector[nextsectorneighborz(door_sector, sector[door_sector].ceilingz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;
	    }


	// Tell ceiling to go to destz
	i = AnimSet(&sector[door_sector].ceilingz, destz, door_speed);
//        AnimSetVelAdj(i, Z(8));

	}
    else
	{
	destz = sector[door_sector].floorz;

	if (Anim[i].goal == destz)
	    {
	    ret_val = OPEN;

	    Anim[i].goal = sector[nextsectorneighborz(door_sector, sector[door_sector].floorz, SEARCH_CEILING, SEARCH_UP)].ceilingz;
	    }
	else
	    {
	    Anim[i].goal = destz;
	    }

	}

    if (auto_close)
	{
	if (ret_val == OPEN)
	    SetDoorAutoClose(door_sector, OPERATE_TYPE);
	else
	    ResetDoorAutoClose(door_sector);
	}

    return (ret_val);
    }

int
MoveDoorDown(short door_sector, short dir, short door_speed)
    {
    int destz, i;

    i = AnimGetGoal(&sector[door_sector].ceilingz);

    if (sector[door_sector].hitag == 0)
	door_speed = DEFAULT_DOOR_SPEED;
    else    
	door_speed = sector[door_sector].hitag;

    if (i < 0)
	{

	// if door is closed look down for floor ELSE look up for ceiling
	if (sector[door_sector].ceilingz == sector[door_sector].floorz)
	    {
	    if (dir == CLOSE)
		return (0);

	    destz = sector[nextsectorneighborz(door_sector, sector[door_sector].floorz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;
	    }
	else
	    {
	    if (dir == OPEN)
		return (0);

	    destz = sector[nextsectorneighborz(door_sector, sector[door_sector].floorz, SEARCH_CEILING, SEARCH_UP)].ceilingz;
	    }


	// Tell floor to go to destz
	AnimSet(&sector[door_sector].floorz, destz, door_speed);

	return (door_sector);
	}

    return (0);
    }

int
MoveDoorHoriz(short door_sector, short dir, short door_speed)
    {
    short startwall, endwall, wallfind[2];
    long dax, day, dax2, day2, i, j;

    // Set up the start and end walls for the sector
    startwall = sector[door_sector].wallptr;
    endwall = startwall + sector[door_sector].wallnum - 1;

    // first find center point by averaging all points
    dax = 0L, day = 0L;
    for (i = startwall; i <= endwall; i++)
	{
	dax += wall[i].x;
	day += wall[i].y;
	}
    dax /= (endwall - startwall + 1);
    day /= (endwall - startwall + 1);

    // find any points with either same x or same y coordinate
    // as center (dax, day) - should be 2 points found.
    wallfind[0] = -1;
    wallfind[1] = -1;

    for (i = startwall; i <= endwall; i++)
	{
	if ((wall[i].x == dax) || (wall[i].y == day))
	    {
	    if (wallfind[0] == -1)
		wallfind[0] = i;
	    else
		wallfind[1] = i;
	    }
	}

    for (j = 0; j < 2; j++)
	{
	if ((wall[wallfind[j]].x == dax) && (wall[wallfind[j]].y == day))
	    {
	    // find what direction door should open by averaging the
	    // 2 neighboring points of wallfind[0] & wallfind[1].
	    i = wallfind[j] - 1;

	    if (i < startwall)
		i = endwall;

	    dax2 = ((wall[i].x + wall[wall[wallfind[j]].point2].x) >> 1) - wall[wallfind[j]].x;
	    day2 = ((wall[i].y + wall[wall[wallfind[j]].point2].y) >> 1) - wall[wallfind[j]].y;

	    if (dax2 != 0)
		{
		dax2 = wall[wall[wall[wallfind[j]].point2].point2].x;
		dax2 -= wall[wall[wallfind[j]].point2].x;

		AnimSet(&wall[wallfind[j]].x, wall[wallfind[j]].x + dax2, 4);
		AnimSet(&wall[i].x, wall[i].x + dax2, 4);
		AnimSet(&wall[wall[wallfind[j]].point2].x, wall[wall[wallfind[j]].point2].x + dax2, 4);
		}
	    else if (day2 != 0)
		{
		day2 = wall[wall[wall[wallfind[j]].point2].point2].y;
		day2 -= wall[wall[wallfind[j]].point2].y;

		AnimSet(&wall[wallfind[j]].y, wall[wallfind[j]].y + day2, 4);
		AnimSet(&wall[i].y, wall[i].y + day2, 4);
		AnimSet(&wall[wall[wallfind[j]].point2].y, wall[wall[wallfind[j]].point2].y + day2, 4);
		}
	    }
	else
	    {
	    i = wallfind[j] - 1;

	    if (i < startwall)
		i = endwall;

	    dax2 = ((wall[i].x + wall[wall[wallfind[j]].point2].x) >> 1) - wall[wallfind[j]].x;
	    day2 = ((wall[i].y + wall[wall[wallfind[j]].point2].y) >> 1) - wall[wallfind[j]].y;

	    if (dax2 != 0)
		{
		AnimSet(&wall[wallfind[j]].x, dax, 4);
		AnimSet(&wall[i].x, dax + dax2, 4);
		AnimSet(&wall[wall[wallfind[j]].point2].x, dax + dax2, 4);
		}
	    else if (day2 != 0)
		{
		AnimSet(&wall[wallfind[j]].y, day, 4);
		AnimSet(&wall[i].y, day + day2, 4);
		AnimSet(&wall[wall[wallfind[j]].point2].y, day + day2, 4);
		}
	    }
	}

    return (0);
    }

int
MoveDoorRotate(short door_sector, short auto_close)
    {
    int i;
    short ret_val;
    WALLp wallp;

    // Look at all the swinging doors
    for (i = 0; Rotate[i].sector != -1; i++)
	{
	// Is this the correct sector
	if (Rotate[i].sector == door_sector)
	    {
	    if (Rotate[i].anginc == 0)
		{
		if (Rotate[i].sang == Rotate[i].angclosed)
		    {
		    Rotate[i].anginc = Rotate[i].angopendir;
		    ret_val = OPEN;

		    wallp = &wall[Rotate[i].wall[0]];
		    PlaySound(DIGI_DOOR_OPEN, SoundAngle(wallp->x, wallp->y), SoundVol(wallp->x, wallp->y));
		    }
		else
		    {
		    Rotate[i].anginc = -Rotate[i].angopendir;
		    ret_val = CLOSE;
		    }
		}
	    else
		{
		Rotate[i].anginc = -Rotate[i].anginc;

		ret_val = Rotate[i].anginc < 0 ? OPEN : CLOSE;
		}
	    }
	}

    if (auto_close)
	{
	if (ret_val == OPEN)
	    SetDoorAutoClose(door_sector, OPERATE_TYPE);
	else
	    ResetDoorAutoClose(door_sector);
	}

    return (ret_val);

    }

VOID 
DoSpringBoard(PLAYERp pp, short sectnum)
    {
    int sb;
    long i;
    VOID DoPlayerBeginForceJump(PLAYERp);

    i = AnimGetGoal(&sector[sectnum].floorz);

    // if in motion return
    if (i >= 0)
	return;

    AnimSet(&sector[sectnum].floorz, sector[sectnum].floorz - Z(32), 512);

    for (sb = 0; sb < SIZ(SpringBoard); sb++)
	{
	// if empty set up an entry to close the sb later
	if (SpringBoard[sb].Sector == -1)
	    {
	    pp->jump_speed = -sector[pp->cursectnum].hitag;
	    DoPlayerBeginForceJump(pp);

	    SpringBoard[sb].Sector = sectnum;
	    SpringBoard[sb].TimeOut = 1 * 120;

	    sector[sectnum].lotag = 0;
	    }
	}


    return;
    }


VOID 
DoSpringBoardDown(VOID)
    {
    int sb;
    SPRING_BOARD *sbp;

    for (sb = 0; sb < SIZ(SpringBoard); sb++)
	{
	sbp = &SpringBoard[sb];

	// if empty set up an entry to close the sb later
	if (sbp->Sector != -1)
	    {
	    if ((sbp->TimeOut -= synctics) <= 0)
		{
		long destz;

		destz = sector[nextsectorneighborz(sbp->Sector, sector[sbp->Sector].floorz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;

		AnimSet(&sector[sbp->Sector].floorz, destz, 256);

		sector[sbp->Sector].lotag = TAG_SPRING_BOARD;

		sbp->Sector = -1;
		}
	    }
	}


    return;
    }

int 
DoSlidingDoor(short sectnum, short auto_close)
    {
    short startwall, endwall, j, i, dasector, wallfind[2], w;
    long dax2, day2, centx, centy, centz, goal_x1, goal_x2, goal_y1, goal_y2;
    short door_is_closing = FALSE, ret_val;

    SectorMidPoint(sectnum, &centx, &centy, &centz);

    // get 2 closest line segments to center (dax, day)
    wallfind[0] = -1;
    wallfind[1] = -1;

    startwall = sector[sectnum].wallptr;
    endwall = startwall + sector[sectnum].wallnum - 1;

    // find the two end walls
    for (i = startwall; i <= endwall; i++)
	{
	if (wall[i].lotag == TAG_WALL_SLIDING_DOOR)
	    {
	    if (wallfind[0] == -1)
		wallfind[0] = i;
	    else
		wallfind[1] = i;
	    }
	}


    // determine if doors are in motion to close
    goal_x1 = AnimGetGoal(&wall[wallfind[0]].x);
    goal_x2 = AnimGetGoal(&wall[wallfind[1]].x);

    if (goal_x1 >= 0 || goal_x2 >= 0)
	{
	if (Anim[goal_x1].goal == Anim[goal_x2].goal)
	    {
	    door_is_closing = TRUE;
	    }
	}


    goal_y1 = AnimGetGoal(&wall[wallfind[0]].y);
    goal_y2 = AnimGetGoal(&wall[wallfind[1]].y);

    if (goal_y1 >= 0 || goal_y2 >= 0)
	{
	if (Anim[goal_y1].goal == Anim[goal_y2].goal)
	    {
	    door_is_closing = TRUE;
	    }
	}


    // start moving the doors
    for (j = 0; j < 2; j++)
	{

	w = wallfind[j];

	if (door_is_closing || (((wall[w].x + wall[wall[w].point2].x) >> 1) == centx) && (((wall[w].y + wall[wall[w].point2].y) >> 1) == centy))
	    {
	    // OPEN door
	    // walls are at the center, door was closed
	    // find what direction door should open

	    ret_val = OPEN;

	    // look at the wall one before
	    i = w - 1;

	    if (i < startwall)
		i = endwall;

	    dax2 = wall[i].x - wall[w].x;
	    day2 = wall[i].y - wall[w].y;

	    if (dax2 != 0)
		{
		dax2 = wall[wall[wall[wall[w].point2].point2].point2].x;
		dax2 -= wall[wall[wall[w].point2].point2].x;

		// move the four points that make up the door
		AnimSet(&wall[w].x, wall[w].x + dax2, 4);
		AnimSet(&wall[i].x, wall[i].x + dax2, 4);
		AnimSet(&wall[wall[w].point2].x, wall[wall[w].point2].x + dax2, 4);
		AnimSet(&wall[wall[wall[w].point2].point2].x, wall[wall[wall[w].point2].point2].x + dax2, 4);
		}
	    else if (day2 != 0)
		{
		day2 = wall[wall[wall[wall[w].point2].point2].point2].y;
		day2 -= wall[wall[wall[w].point2].point2].y;

		// move the four points that make up the door
		AnimSet(&wall[w].y, wall[w].y + day2, 4);
		AnimSet(&wall[i].y, wall[i].y + day2, 4);
		AnimSet(&wall[wall[w].point2].y, wall[wall[w].point2].y + day2, 4);
		AnimSet(&wall[wall[wall[w].point2].point2].y, wall[wall[wall[w].point2].point2].y + day2, 4);
		}
	    }
	else
	    {
	    // CLOSE door
	    // door was not closed

	    ret_val = CLOSE;

	    i = w - 1;


	    if (i < startwall)
		i = endwall;

	    dax2 = wall[i].x - wall[w].x;
	    day2 = wall[i].y - wall[w].y;

	    if (dax2 != 0)
		{
		// move the four points that make up the door
		AnimSet(&wall[w].x, centx, 4);
		AnimSet(&wall[i].x, centx + dax2, 4);
		AnimSet(&wall[wall[w].point2].x, centx, 4);
		AnimSet(&wall[wall[wall[w].point2].point2].x, centx + dax2, 4);
		}
	    else if (day2 != 0)
		{
		// move the four points that make up the door
		AnimSet(&wall[w].y, centy, 4);
		AnimSet(&wall[i].y, centy + day2, 4);
		AnimSet(&wall[wall[w].point2].y, centy, 4);
		AnimSet(&wall[wall[wall[w].point2].point2].y, centy + day2, 4);
		}
	    }
	}


    if (auto_close)
	{
	if (ret_val == OPEN)
	    SetDoorAutoClose(sectnum, OPERATE_TYPE);
	else
	    ResetDoorAutoClose(sectnum);
	}

    return (ret_val);
    }


VOID
DoElevatorAuto(VOID)
    {
    int vator, es;
    ELEVATOR_AUTOp ep;

    for (ep = &ElevatorAuto[0]; es = ep->Sector; ep++)
	{
	ep->Tics += synctics;

	if (ep->Tics >= ep->MaxTics)
	    {
	    ep->Tics = 0;
	    ToggleElevator(es, ep->Speed);
	    }
	}
    }

VOID
DoSetPauseAction(short sector, short time, short speed)
    {
    int ndx, es;
    ELEVATOR_AUTOp pa;

    for (ndx = 0; ndx < SIZ(PauseAction); ndx++)
	{
	pa = &PauseAction[ndx];

	if (pa->Sector != -1)
	    continue;

	pa->Tics = 0;
	pa->Sector = sector;
	pa->MaxTics = time * 120;
	}
    }


VOID
DoPauseAction(VOID)
    {
    int ndx, es;
    ELEVATOR_AUTOp pa;

    for (ndx = 0; ndx < SIZ(PauseAction); ndx++)
	{
	pa = &PauseAction[ndx];

	if (pa->Sector == -1)
	    continue;

	pa->Tics += synctics;

	if (pa->Tics >= pa->MaxTics)
	    {
	    pa->Tics = 0;

	    // operate and delete action
	    switch (LOW_TAG(pa->Sector))
		{
	    case TAG_ELEVATOR_TRIGGER_UP:
		MoveElevator(pa->Sector, 128, SEARCH_DOWN);
		pa->Sector = -1;
		break;

	    case TAG_ELEVATOR_TRIGGER_DOWN:
		MoveElevator(pa->Sector, 128, SEARCH_UP);
		pa->Sector = -1;
		break;

		}
	    }
	}
    }


VOID
DoDoorsClose(VOID)
    {
    int door;
    short pnum;
    DOOR_AUTO_CLOSEp dac;
    PLAYERp pp;

    // run through all doors to close
    for (door = 0; door < MAX_DOOR_AUTO_CLOSE; door++)
	{
	// holding variable
	dac = &DoorAutoClose[door];

	// if sector is 0 skip it
	if (dac->Sector == NULL)
	    continue;

	// decrement timout
	dac->TimeOut -= synctics;

	// if its time to close the door
	if (dac->TimeOut <= 0)
	    {

	    // Make sure the player is not standing on the door
	    TRAVERSE_CONNECT(pnum)
		{
		pp = Player + pnum;

		if (pp->cursectnum == dac->Sector)
		    {
		    dac->TimeOut = 0;
		    return;
		    }
		}

	    switch (dac->Type)
		{
	    case OPERATE_TYPE:
		OperateSector(dac->Sector, FALSE);
		break;
	    case ELEVATOR_ONE_WAY_TYPE:
		ToggleElevator(dac->Sector, 128);
		// Tag was null so player can not toggle at will.
		// Reset it so it can be toggled again.
		sector[dac->Sector].lotag = TAG_ONE_WAY_ELEVATOR;
		break;
		}

	    // reset the structure element
	    memset(dac, 0, sizeof(*dac));
	    }
	}
    }

short
FindSectorByTag(long x, long y, long tag)
    {
    short i = 0, near_sector = -1;
    long diff, near_diff = 9999999;
    short wallnum;

    for (i = 0; i < numsectors; i++)
	{
	if (LOW_TAG(i) == tag)
	    {
	    // get the delta of the door/elevator
	    wallnum = sector[i].wallptr;

	    // diff = labs(wall[wallnum].x - x) + labs(wall[wallnum].y - y);
	    diff = Distance(wall[wallnum].x, wall[wallnum].y, x, y);

	    // if the door/elevator is closer than the last save it off
	    if (diff < near_diff)
		{
		near_diff = diff;
		near_sector = i;
		}
	    }
	}

    return (near_sector);

    }
    
short
FindSectorByTag_Wall(short wallnum, long tag)
    {
    return(FindSectorByTag(wall[wallnum].x, wall[wallnum].y, tag));
    }

short
FindSectorByTag_Sprite(short SpriteNum, long tag)
    {
    return(FindSectorByTag(sprite[SpriteNum].x, sprite[SpriteNum].y, tag));
    }
    
#if 1
short
FindSectorMidByTag(short sectnum, long tag)
    {
    short i = 0, near_sector = -1;
    long diff, near_diff = 9999999, x, y;
    long trash, fx, fy;

    // Get the mid x,y of the sector
    SectorMidPoint(sectnum, &x, &y, &trash);

    for (i = 0; i < numsectors; i++)
	{
	if (sector[i].lotag == tag)
	    {
	    // get the delta of the door/elevator
	    SectorMidPoint(i, &fx, &fy, &trash);

	    // diff = labs(wall[wallnum].x - x) + labs(wall[wallnum].y - y);
	    diff = Distance(fx, fy, x, y);

	    // if the door/elevator is closer than the last save it off
	    if (diff < near_diff)
		{
		near_diff = diff;
		near_sector = i;
		}
	    }
	}

    return (near_sector);

    }

#endif

short
FindNextSectorByTag(short sectnum, long tag)
    {
    short next_sectnum, startwall, endwall, j;

    startwall = sector[sectnum].wallptr;
    endwall = startwall + sector[sectnum].wallnum - 1;

    for (j = startwall; j <= endwall; j++)
	{
	next_sectnum = wall[j].nextsector;

	if (next_sectnum >= 0)
	    {
	    if (sector[next_sectnum].lotag == tag)
		{
		return (next_sectnum);
		}
	    }
	}

    return (-1);

    }


long
SectorDistance(short sect1, long sect2)
    {
    short wallnum1, wallnum2;

    if (sect1 < 0 || sect2 < 0)
	return (9999999);

    wallnum1 = sector[sect1].wallptr;
    wallnum2 = sector[sect2].wallptr;

    // return the distance between the two sectors.
    return (Distance(wall[wallnum1].x, wall[wallnum1].y, wall[wallnum2].x, wall[wallnum2].y));
    }


long
SectorDistanceByMid(short sect1, long sect2)
    {
    long sx1, sy1, sx2, sy2, trash;

    SectorMidPoint(sect1, &sx1, &sy1, &trash);
    SectorMidPoint(sect2, &sx2, &sy2, &trash);

    // return the distance between the two sectors.
    return (Distance(sx1, sy1, sx2, sy2));
    }



short
Switch(short wallnum)
    {
    short i = 0, near_sector = -1;
    long diff, near_diff = 9999999, x, y;


    // Get the x,y of the switch

    x = wall[wallnum].x;
    y = wall[wallnum].y;

    for (i = 0; i < numsectors; i++)
	{
	switch (sector[i].lotag)
	    {
	case TAG_ONE_WAY_DOOR:
	case TAG_ONE_WAY_ELEVATOR:
//        case TAG_DOOR_SWING:
	case TAG_DOOR_ROTATE:

	    // get the delta of the door/elevator
	    wallnum = sector[i].wallptr;

	    diff = labs(wall[wallnum].x - x) + labs(wall[wallnum].y - y);

	    // if the door/elevator is closer than the last save it off
	    if (diff < near_diff)
		{
		near_diff = diff;
		near_sector = i;
		}

	    break;
	    }
	}

    if (!near_sector)
	return (NULL);

    OperateSector(near_sector, TRUE);

    switch (sector[near_sector].lotag)
	{
    case TAG_ONE_WAY_ELEVATOR:
	ToggleElevator(near_sector, 128);
	SetDoorAutoClose(near_sector, ELEVATOR_ONE_WAY_TYPE);
	sector[near_sector].lotag = NULL;       // so you can't toggle it
						// manually
	// again
	break;
	}

    return (near_sector);

    }

void CallbackCopyFloorPic(ANIMp ap, void *data)
    {
    long dest_sect = -1;
    SECTORp sectp;
    SECT_USERp sectu;
    short i,match;
    
    sectp = data;
    match = SectUser[sectp - sector]->number;
    
    // look again for the destination
    for (i = 0; i < numsectors; i++)
	{
	sectu = SectUser[i];
	
	// if lotag matches && has a SectUser && number matches
	if (sectu && sectu->stag == SECT_RAISE_LOWER_PLAT_DEST && sectu->number == match)
	    {
	    dest_sect = i;
	    break;
	    }
	}
    
    ASSERT(dest_sect != -1);    

    sectp->floorpicnum = sector[dest_sect].floorpicnum;
	
    }

void CallbackCeilingStomperDown(ANIMp ap, void *data)
    {
    SECTORp sectp;
    SECT_USERp sectu;
    short anim_num;
    long destz;
    
    sectp = data;
    sectu = SectUser[sectp - sector];
    
    //destz = sector[nextsectorneighborz(sectp - sector, sectp->ceilingz, SEARCH_CEILING, SEARCH_UP)].ceilingz;
    destz = sectu->dist;
    
    anim_num = AnimSet(&sectp->ceilingz, destz, sectu->speed);
    AnimSetCallback(anim_num, CallbackCeilingStomperUp, sectp);
    AnimSetVelAdj(anim_num, 0);
    }

void CallbackCeilingStomperUp(ANIMp ap, void *data)
    {
    SECTORp sectp;
    SECT_USERp sectu;
    short anim_num;
    
    sectp = data;
    sectu = SectUser[sectp - sector];
    
    anim_num = AnimSet(&sectp->ceilingz, sectp->floorz, sectu->speed);
    AnimSetCallback(anim_num, CallbackCeilingStomperDown, sectp);
    AnimSetVelAdj(anim_num, Z(sectu->ang));
    }


void CallbackFloorStomperUp(ANIMp ap, void *data)
    {
    SECTORp sectp;
    SECT_USERp sectu;
    short anim_num;
    long destz;
    
    sectp = data;
    sectu = SectUser[sectp - sector];

    //destz = sector[nextsectorneighborz(sectp - sector, sectp->floorz, SEARCH_FLOOR, SEARCH_DOWN)].floorz;
    destz = sectu->dist;
    
    anim_num = AnimSet(&sectp->floorz, destz, sectu->speed);
    AnimSetCallback(anim_num, CallbackFloorStomperDown, sectp);
    AnimSetVelAdj(anim_num, 0);
    }

void CallbackFloorStomperDown(ANIMp ap, void *data)
    {
    SECTORp sectp;
    SECT_USERp sectu;
    short anim_num;
    
    sectp = data;
    sectu = SectUser[sectp - sector];
    
    anim_num = AnimSet(&sectp->floorz, sectp->ceilingz, sectu->speed);
    AnimSetCallback(anim_num, CallbackFloorStomperUp, sectp);
    AnimSetVelAdj(anim_num, Z(sectu->ang));
    }

short
DoRaiseLowerPlat(short match)
    {
    SECT_USERp sectu;
    long dest_sect = -1;
    short i, speed, anim_ndx;

    for (i = 0; i < numsectors; i++)
	{
	sectu = SectUser[i];
	
	// if lotag matches && has a SectUser && number matches
	if (sectu && sectu->stag == SECT_RAISE_LOWER_PLAT_DEST && sectu->number == match)
	    {
	    dest_sect = i;
	    break;
	    }
	}
    
    ASSERT(dest_sect != -1);    

    for (i = 0; i < numsectors; i++)
	{
	sectu = SectUser[i];
	
	// if lotag matches && has a SectUser && number matches
	if (sectu && sectu->stag == SECT_RAISE_LOWER_PLAT && sectu->number == match)
	    {
	    speed = 128;
	    if (sectu->speed != 0)
		speed = sectu->speed;
		
	    anim_ndx = AnimSet(&sector[i].floorz, sector[dest_sect].floorz, speed);
	    
	    // Set up a callback if the pic is supposed to be copied from the dest
	    if (TEST(sectu->flags2,BIT(1)))
		{
		AnimSetCallback(anim_ndx, CallbackCopyFloorPic, &sector[i]);
		}
	    
	    // if the sector we are moving has a depth
	    if (sectu->depth)
		{
		//and if the dest_sect does not
		if (!SectUser[dest_sect] || (SectUser[dest_sect] && !SectUser[dest_sect]->depth))
		    {
		    WATERp w;
		    
		    for (w = &Water[0]; w < &Water[MAX_WATER]; w++)
			{
			if (w->sector == i)
			    {
			    AnimSet(&w->depth, 0L, speed);
			    AnimSet((long*)&sectu->depth_fract, 0L, speed<<8);
			    //w->depth = 0;
			    //sectu->depth = 0;
			    break;
			    }
			}
		    }
		}
	    }
	}

    return (0);
    }


int 
OperateSector(short sectnum, short player_is_operating)
    {
    PLAYERp pp = GlobPlayerP;

    switch (LOW_TAG(sectnum))
	{
    case TAG_DOOR_SLIDING:
    
	if (SectUser[sectnum] && SectUser[sectnum]->stag == SECT_LOCK_DOOR && SectUser[sectnum]->number)
	    {
	    short key_num;

	    key_num = SectUser[sectnum]->number;

	    if (pp->HasKey[key_num - 1])
		{
		SectUser[sectnum]->number = 0; //unlock door
		DoSlidingDoor(sectnum, TRUE);
		return (TRUE);
		}
	    else
		{
		KeyDoorMessageTime = KEY_DOOR_MESSAGE_TIME;
		KeyDoorMessageNum = key_num - 1;
		return (TRUE);
		}
	    }
	else
	    {
	    DoSlidingDoor(sectnum, TRUE);
	    return (TRUE);
	    }

    case TAG_DOOR_UP:

	if (SectUser[sectnum] && SectUser[sectnum]->stag == SECT_LOCK_DOOR && SectUser[sectnum]->number)
	    {
	    short key_num;

	    key_num = SectUser[sectnum]->number;

	    if (pp->HasKey[key_num - 1])
		{
		SectUser[sectnum]->number = 0; //unlock door
		MoveDoorUp(sectnum, TRUE, 128);
		return (TRUE);
		}
	    else
		{
		KeyDoorMessageTime = KEY_DOOR_MESSAGE_TIME;
		KeyDoorMessageNum = key_num - 1;
		return (TRUE);
		}
	    }
	else
	    {
	    MoveDoorUp(sectnum, TRUE, 128);
	    return (TRUE);
	    }

	break;

    case TAG_DOOR_ROTATE:

	if (SectUser[sectnum] && SectUser[sectnum]->stag == SECT_LOCK_DOOR && SectUser[sectnum]->number)
	    {
	    short key_num;

	    key_num = SectUser[sectnum]->number;

	    if (pp->HasKey[key_num - 1])
		{
		SectUser[sectnum]->number = 0; //unlock door
		MoveDoorRotate(sectnum, TRUE);
		return (TRUE);
		}
	    else
		{
		KeyDoorMessageTime = KEY_DOOR_MESSAGE_TIME;
		KeyDoorMessageNum = key_num - 1;
		return (TRUE);
		}
	    }
	else
	    {
	    MoveDoorRotate(sectnum, TRUE);
	    return (TRUE);
	    }


	break;


    case TAG_ONE_WAY_DOOR:
	if (!player_is_operating)
	    MoveDoorUp(sectnum, TRUE, 128);
	return (TRUE);


    case TAG_STAIRS_DOWN_START:
    case TAG_STAIRS_UP_START:

	if (!player_is_operating)
	    {
	    long destz;
	    short near_sector;
	    SECT_USERp nsu, su;

	    // get the destination of the first stair, must be positive
	    if (LOW_TAG(sectnum) == TAG_STAIRS_DOWN_START)
		destz = sector[sectnum].floorz + Z(HIGH_TAG(sectnum));
	    else
		destz = sector[sectnum].floorz - Z(HIGH_TAG(sectnum));

	    // Tell floor to go to destz
	    AnimSet(&sector[sectnum].floorz, destz, 128);

	    // Look for the second stair
	    if (LOW_TAG(sectnum) == TAG_STAIRS_DOWN_START)
		near_sector = FindSectorByTag_Wall(sector[sectnum].wallptr, TAG_STAIRS_DOWN);
	    else
		near_sector = FindSectorByTag_Wall(sector[sectnum].wallptr, TAG_STAIRS_UP);

	    if (near_sector >= 0)
		{

		// Tell floor to go to destz
		AnimSet(&sector[near_sector].floorz, destz - Z(HIGH_TAG(near_sector)), 128);

		su = GetSectUser(sectnum);
		nsu = GetSectUser(near_sector);
		nsu->dist = su->dist;

		OperateSector(near_sector, FALSE);
		}

	    }
	return (TRUE);



    case TAG_STAIRS_DOWN:
    case TAG_STAIRS_UP:

	if (!player_is_operating)
	    {
	    long destz;
	    short destz_ndx, near_sector;
	    SECTORp sn = &sector[sectnum], ns;
	    long tag, dist;
	    SECT_USERp su, nsu;

	    // find the goal of the current stair
	    destz_ndx = AnimGetGoal(&sn->floorz);

	    if (destz_ndx == -1)
		break;

	    destz = Anim[destz_ndx].goal;

	    // reset current stair tag
	    tag = LOW_TAG(sectnum);
	    sn->lotag = 0;

	    // look for the next stair
	    if (tag == TAG_STAIRS_DOWN)
		// near_sector = FindSectorByTag_Wall(sn->wallptr,
		// TAG_STAIRS_DOWN);
		near_sector = FindSectorMidByTag(sectnum, TAG_STAIRS_DOWN);
	    else
		// near_sector = FindSectorByTag_Wall(sn->wallptr, TAG_STAIRS_UP);
		near_sector = FindSectorMidByTag(sectnum, TAG_STAIRS_UP);

	    if (near_sector == -1)
		break;


	    // convience var
	    ns = &sector[near_sector];

	    su = GetSectUser(sectnum);

	    // need to check for distance here also
	    if ((dist = SectorDistanceByMid(sectnum, near_sector)) < su->dist)
		{
		sprintf(ds, "stair ns = %d, dist %d", near_sector, dist);
		MONO_PRINT(ds);

		// set the animation for the next stair
		AnimSet(&ns->floorz, destz - Z(HIGH_TAG(near_sector)), 128);

		nsu = GetSectUser(near_sector);
		nsu->dist = su->dist;

		OperateSector(near_sector, FALSE);
		}
	    }

	return (TRUE);


	}

    return (FALSE);
    }

int 
OperateWall(short wallnum, short player_is_operating)
    {
    WALLp wallp = &wall[wallnum];

    switch (LOW_TAG_WALL(wallnum))
	{
	}

    return (FALSE);
    }

VOID AnimateSwitch(SPRITEp sp)
    {
    #define SWITCH_FUSE         558
    #define SWITCH_FLIP         561
    #define SWITCH_RED_CHAIN    563
    #define SWITCH_GREEN_CHAIN  565
    #define SWITCH_TOUCH        567
    #define SWITCH_DRAGON       569

    switch (sp->picnum)
	{
	case SWITCH_FLIP:
	case SWITCH_RED_CHAIN:
	case SWITCH_GREEN_CHAIN:
	case SWITCH_TOUCH:
	case SWITCH_DRAGON:
	    sp->picnum += 1;
	    break;
	    
	case SWITCH_FLIP+1:
	case SWITCH_RED_CHAIN+1:
	case SWITCH_GREEN_CHAIN+1:
	case SWITCH_TOUCH+1:
	case SWITCH_DRAGON+1:
	    sp->picnum -= 1;
	    break;
	}
    }            

VOID ShootableSwitch(short SpriteNum)    
    {
    SPRITEp sp = &sprite[SpriteNum];
    
    switch (sp->picnum)
	{
	case SWITCH_FUSE:
	case SWITCH_FUSE+1:
	    OperateSprite(SpriteNum, TRUE);
	    sp->picnum = SWITCH_FUSE + 2;
	    RESET(sp->cstat, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
	    break;
	}    
    }


// NOTE: switches are always wall sprites
int 
OperateSprite(short SpriteNum, short player_is_operating)
    {
    SPRITEp sp = &sprite[SpriteNum];

    switch (sp->lotag)
	{
    // opens same doors as TAG_OPEN_DOOR_TRIGGER        
    case TAG_OPEN_DOOR_SWITCH:
	    {
	    short near_sector, i;
	    short match = sp->hitag;
	    
	    ASSERT(match > 0);

	    for (i = 0; i < numsectors; i++)
		{
		// if match && has a SectUser && number matches
		if (sector[i].lotag == TAG_DOOR_TO_TRIGGER &&
		    SectUser[i] &&
		    SectUser[i]->number == match)
		    {
		    MoveDoorUp(i, FALSE, sector[i].hitag ? sector[i].hitag : 128);
		    }
		}
		
	    // deactivate switch
	    sp->lotag = NULL;
	    sp->hitag = NULL;
	    RESET(sp->extra, CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
	    break;
	    }
	
    case TAG_LEVEL_EXIT_SWITCH:
	    {
	    extern short Level;
	    extern BOOL QuitFlag;
	    extern BOOL ExitLevel;
	    
	    AnimateSwitch(sp);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));
	    
	    Level = sp->hitag;
	    ExitLevel = TRUE;

	    return (TRUE);
	    }
	    
    case TAG_SPRITE_ROTATE_WATER_VALVE:
    case TAG_SPRITE_ROTATE_WATER_VALVE2:
	    {
	    short *pic, so_num, near_sect;
	    SECTOR_OBJECTp sop;
	    SECTORp sectp;

	    so_num = sp->hitag;

	    ASSERT(so_num <= 20);
	    ASSERT(SectorObject[so_num].num_sectors != -1);

	    sop = &SectorObject[so_num];

	    sop->ang_tgt = NORM_ANGLE(sop->ang_tgt + 1024);

	    if (sp->lotag == TAG_SPRITE_ROTATE_WATER_VALVE)
		{
		// Just search for the nearest TAG_WATER_GATE
		near_sect = FindSectorByTag_Wall(sector[sp->sectnum].wallptr, TAG_WATER_GATE);

		if (near_sect != -1)
		    {
		    sectp = &sector[near_sect];

		    if (sectp->hitag > 10000)
			{
			sectp->hitag -= 10000;
			sectp->hitag = -sectp->hitag;
			}

		    AnimSet(&sectp->floorz, sectp->floorz + Z(sectp->hitag), 128);
		    }
		}
	    else
		{
		// Search for the ALL the TAG_WATER_GATE_ALLs
		while (TRUE)
		    {
		    near_sect = FindSectorByTag_Wall(sector[sp->sectnum].wallptr, TAG_WATER_GATE_ALL);

		    if (near_sect == -1)
			break;

		    sectp = &sector[near_sect];

		    if (sectp->hitag > 10000)
			{
			sectp->hitag -= 10000;
			sectp->hitag = -sectp->hitag;
			}

		    AnimSet(&sectp->floorz, sectp->floorz + Z(sectp->hitag), 128);
		    }

		}

	    sp->lotag = 0;
	    sp->hitag = 0;

	    return (TRUE);
	    }

    case TAG_SPRITE_GRATING:
	    {
	    USERp u;
	    ANIMATOR DoGrating;
	    long dir;

	    change_sprite_stat(SpriteNum, STAT_NO_STATE);

	    u = SpawnUser(SpriteNum, 0, NULL);

	    u->ActorActionFunc = DoGrating;

	    // reduce to 0 to 3 value
	    dir = sp->ang >> 9;

	    if (MOD2(dir) == 0)
		{
		if (dir == 0)
		    AnimSet(&sp->x, sp->x + sp->hitag, 4);
		else
		    AnimSet(&sp->x, sp->x - sp->hitag, 4);
		}
	    else
		{
		if (dir == 1)
		    AnimSet(&sp->y, sp->y + sp->hitag, 4);
		else
		    AnimSet(&sp->y, sp->y - sp->hitag, 4);
		}

	    sp->lotag = 0;
	    sp->hitag = 0;

	    return (TRUE);
	    }
	
    case TAG_WALL_SWITCH:               // Switch
	    {
	    //Switch(wallnum);

	    AnimateSwitch(sp);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);
	    }

    case TAG_ONE_WAY_DOOR_SWITCH:
	    {
	    short near_sector;

	    AnimateSwitch(sp);

	    near_sector = FindSectorByTag_Sprite(SpriteNum, TAG_ONE_WAY_DOOR);
	    OperateSector(near_sector, FALSE);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);
	    }


    case TAG_RAISE_LOWER_PLAT_SWITCH:
	    {
	    short sectnum, near_sector;

	    AnimateSwitch(sp);

	    DoRaiseLowerPlat(sprite[SpriteNum].hitag);
	    
	    sprite[SpriteNum].hitag = 0;
	    sprite[SpriteNum].lotag = 0;

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }


    case TAG_ROTATE_SO_SWITCH:
	    {
	    short so_num;
	    SECTOR_OBJECTp sop;

	    so_num = sprite[SpriteNum].hitag;

	    ASSERT(so_num <= 20);
	    ASSERT(SectorObject[so_num].num_sectors != -1);

	    AnimateSwitch(sp);
	    
	    sop = &SectorObject[so_num];

	    sop->ang_tgt = NORM_ANGLE(sop->ang_tgt + 512);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }


    case TAG_STAIRS_DOWN_SWITCH:
    case TAG_STAIRS_UP_SWITCH:
	    {
	    short near_sector;
	    SECT_USERp su;
	    
	    AnimateSwitch(sp);

	    if (sprite[SpriteNum].lotag == TAG_STAIRS_DOWN_SWITCH)
		near_sector = FindSectorByTag_Sprite(SpriteNum, TAG_STAIRS_DOWN_START);
	    else
		near_sector = FindSectorByTag_Sprite(SpriteNum, TAG_STAIRS_UP_START);

	    // put distance in sector user
	    su = GetSectUser(near_sector);
	    su->dist = sprite[SpriteNum].hitag;

	    OperateSector(near_sector, FALSE);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);
	    }


    case TAG_ELEVATOR_SWITCH:
	    {
	    short ea, dest_sector;
	    ELEVATOR_AUTOp ep;

	    AnimateSwitch(sp);

	    dest_sector = FindSectorByTag_Sprite(SpriteNum, TAG_ELEVATOR_AUTO_STOPPED);

	    for (ea = 0; ElevatorAuto[ea].Sector; ea++);

	    ep = &ElevatorAuto[ea];
	    ep->Sector = dest_sector;
	    ep->MaxTics = 4 * 120;
	    ep->Speed = 128;
	    ep->Tics = 4 * 120;

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }

    case TAG_ELEVATOR_UP_SWITCH:
	    {
	    short ea, dest_sector;
	    ELEVATOR_AUTOp ep;

	    AnimateSwitch(sp);
	    
	    dest_sector = FindSectorByTag_Sprite(SpriteNum, TAG_ELEVATOR_UP);

	    MoveElevator(dest_sector, 128, SEARCH_DOWN);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }

    case TAG_ELEVATOR_DOWN_SWITCH:
	    {
	    short ea, dest_sector;
	    ELEVATOR_AUTOp ep;

	    AnimateSwitch(sp);

	    dest_sector = FindSectorByTag_Sprite(SpriteNum, TAG_ELEVATOR_DOWN);

	    MoveElevator(dest_sector, 128, SEARCH_UP);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }

    case TAG_ELEVATOR_TRIGGER_UP_SWITCH:
	    {
	    short ea, dest_sector;
	    ELEVATOR_AUTOp ep;

	    AnimateSwitch(sp);

	    dest_sector = FindSectorByTag_Sprite(SpriteNum, TAG_ELEVATOR_TRIGGER_UP);

	    // pause for a few seconds and return
	    if (MoveElevator(dest_sector, 128, SEARCH_UP))
		DoSetPauseAction(dest_sector, HIGH_TAG(dest_sector), 128);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }

    case TAG_ELEVATOR_TRIGGER_DOWN_SWITCH:
	    {
	    short ea, dest_sector;
	    ELEVATOR_AUTOp ep;

	    AnimateSwitch(sp);

	    dest_sector = FindSectorByTag_Sprite(SpriteNum, TAG_ELEVATOR_TRIGGER_DOWN);

	    // pause for a few seconds and return
	    if (MoveElevator(dest_sector, 128, SEARCH_DOWN))
		DoSetPauseAction(dest_sector, HIGH_TAG(dest_sector), 128);

	    PlaySound(DIGI_SWITCH, SoundAngle(sp->x, sp->y), SoundVol(sp->x, sp->y));

	    return (TRUE);

	    break;
	    }
	    
	}

    return (FALSE);
    }

BOOL 
LadderOnScreen(VOID)
    {
    static short LadderTile[] =
	{
	62, 63, 77, -1
	};

    short i;

    for (i = 0; i < SIZ(LadderTile); i++)
	{
	if ((gotpic[LadderTile[i] >> 3] & (1 << (LadderTile[i] & 7))) > 0)
	    {
	    gotpic[LadderTile[i] >> 3] &= ~(1 << (LadderTile[i] & 7));

	    if (waloff[LadderTile[i]] != NULL)
		return (TRUE);
	    }
	}

    return (FALSE);
    }


VOID OperateTripTrigger(PLAYERp pp)
{
    switch (LOW_TAG(pp->cursectnum))
	{
	case TAG_TRIGGER_ACTORS:
	    {
	    long dist;
	    short i, nexti;
	    SPRITEp sp;
	    USERp u;

	    dist = sector[pp->cursectnum].hitag;

	    TRAVERSE_SPRITE_STAT(headspritestat[STAT_ENEMY], i, nexti)
		{
		sp = &sprite[i];
		u = User[i];

		if (TEST(u->Flags, SPR_WAIT_FOR_TRIGGER))
		    {
		    if (Distance(sp->x, sp->y, pp->posx, pp->posy) < dist)
			{
			u->tgt_player = pp;
			RESET(u->Flags, SPR_WAIT_FOR_TRIGGER);
			}
		    }
		}

	    break;
	    }

	case TAG_TRIGGER_MISSILE_TRAP:
	    {
	    short i, nexti;
	    SPRITEp sp;
	    USERp u;

	    TRAVERSE_SPRITE_STAT(headspritestat[STAT_ST1], i, nexti)
		{
		sp = &sprite[i];
		u = User[i];

		// if correct type and matches
		if (sp->hitag == FIREBALL_TRAP && sp->lotag == sector[pp->cursectnum].hitag)
		    u->WaitTics = 0;
		    
		// if correct type and matches
		if (sp->hitag == BOLT_TRAP && sp->lotag == sector[pp->cursectnum].hitag)
		    u->WaitTics = 0;
		}

	    break;
	    }

	case TAG_ELEVATOR_UP:
	    MoveElevator(pp->cursectnum, 128, SEARCH_UP);
	    break;
	case TAG_ELEVATOR_DOWN:
	    MoveElevator(pp->cursectnum, 128, SEARCH_DOWN);
	    break;

	case TAG_ELEVATOR_TRIGGER_UP:
	    // pause for a few seconds and return
	    if (MoveElevator(pp->cursectnum, 128, SEARCH_UP))
		DoSetPauseAction(pp->cursectnum, HIGH_TAG(pp->cursectnum), 128);
	    break;

	case TAG_ELEVATOR_TRIGGER_DOWN:
	    // pause for a few seconds and return
	    if (MoveElevator(pp->cursectnum, 128, SEARCH_DOWN))
		DoSetPauseAction(pp->cursectnum, HIGH_TAG(pp->cursectnum), 128);
	    break;
	    
	case TAG_RAISE_LOWER_PLAT_TRIGGER:
	    {
	    
	    DoRaiseLowerPlat(sector[pp->cursectnum].hitag);
	    
	    sector[pp->cursectnum].hitag = 0;
	    sector[pp->cursectnum].lotag = 0;

	    PlaySound(DIGI_SWITCH, SoundAngle(pp->posx, pp->posy), SoundVol(pp->posx, pp->posy));
	    break;
	    }
	    
    case TAG_OPEN_DOOR_TRIGGER:
	    {
	    short near_sector, i;
	    SECT_USERp sectu = SectUser[pp->cursectnum];

	    if (sectu && sectu->number > 0)
		{
		for (i = 0; i < numsectors; i++)
		    {
		    // if lotag matches && has a SectUser && number matches
		    if (sector[i].lotag == TAG_DOOR_TO_TRIGGER &&
			SectUser[i] &&
			SectUser[i]->number == sectu->number)
			{
			MoveDoorUp(i, FALSE, sector[i].hitag ? sector[i].hitag : 128);
			}
		    }
		}
	    else
		{
		near_sector = FindSectorByTag_Wall(sector[pp->cursectnum].wallptr, TAG_DOOR_TO_TRIGGER);

		MoveDoorUp(near_sector, FALSE, sector[i].hitag ? sector[i].hitag : 128);
		}

	    sector[pp->cursectnum].lotag = NULL;
	    sector[near_sector].lotag = NULL;
	    break;
	    }
	}

    switch (LOW_TAG(pp->lastcursectnum))
	{
	case TAG_ELEVATOR_UP:
	    MoveElevator(pp->cursectnum, 128, SEARCH_UP);
	    break;
	}
}

VOID OperateContinuousTrigger(PLAYERp pp)
{
    switch (LOW_TAG(pp->cursectnum))
	{
	case TAG_TRIGGER_MISSILE_TRAP:
	    {
	    short i, nexti;
	    SPRITEp sp;
	    USERp u;
	    int InitFireballTrap(short SpriteNum);

	    TRAVERSE_SPRITE_STAT(headspritestat[STAT_ST1], i, nexti)
		{
		sp = &sprite[i];
		u = User[i];

		// if correct type and matches
		if (sp->hitag == FIREBALL_TRAP && sp->lotag == sector[pp->cursectnum].hitag)
		    {
		    u->WaitTics -= synctics;
		    
		    if (u->WaitTics <= 0)
			{
			u->WaitTics = 1*120;
			InitFireballTrap(i);
			}
		    }
		    
		// if correct type and matches
		if (sp->hitag == BOLT_TRAP && sp->lotag == sector[pp->cursectnum].hitag)
		    {
		    u->WaitTics -= synctics;
		    
		    if (u->WaitTics <= 0)
			{
			u->WaitTics = 1*120;
			InitBoltTrap(i);
			}
		    }
		}

	    break;
	    }
	}    
}

VOID OperateTeleporter(PLAYERp pp)
    {
    int DoPlayerTeleport(PLAYERp, short);

    switch (SectUser[pp->cursectnum]->special)
	{
	case SPECIAL_TELEPORT_CEILING:
	    if (pp->posz <= sector[pp->cursectnum].ceilingz + Z(12))
		{
		DoPlayerTeleport(pp, SectUser[pp->cursectnum]->number);
		}
	    break;
	case SPECIAL_TELEPORT_FLOOR:
	    if (pp->posz + PLAYER_HEIGHT >= sector[pp->cursectnum].floorz - Z(2))
		{
		DoPlayerTeleport(pp, SectUser[pp->cursectnum]->number);
		}
	    break;
	default:
	    DoPlayerTeleport(pp, SectUser[pp->cursectnum]->number);
	    break;
	}
    }


VOID
PlayerOperateEnv(PLAYERp pp)
    {
    //
    // Switch & door activations
    //

    neartag(pp->posx, pp->posy, pp->posz, pp->cursectnum, pp->pang,
	&neartagsector, &neartagwall, &neartagsprite,
	&neartaghitdist, 2048L, NTAG_SEARCH_LO_HI);

    #if 0
    // Jump to get up
    if (TEST_SYNC_KEY(pp, SK_JUMP))
	{
	if (FLAG_KEY_PRESSED(pp, SK_JUMP))
	    {
	    FLAG_KEY_RELEASE(pp, SK_JUMP);
	    }
	}
    else
	{
	FLAG_KEY_RESET(pp, SK_JUMP);
	}
	
    if (TEST_SYNC_KEY(pp, SK_OPERATE))
	{
	if (FLAG_KEY_PRESSED(pp, SK_OPERATE))
	    {
	    FLAG_KEY_RELEASE(pp, SK_OPERATE);
	    }
	}
    else
	{
	FLAG_KEY_RESET(pp, SK_OPERATE);
	}
	
    #endif    
	

    if (TEST_SYNC_KEY(pp, SK_OPERATE))
	{
	// must use this because the syncbit key will be set everytime
	// through
	// and you will get jitter for doors and such
	if (FLAG_KEY_PRESSED(pp, SK_OPERATE))
	    {
	    if (neartagsprite >= 0 && neartaghitdist < 1024 + 768)
		{
		sprintf(ds, "SPRITE LOW tag = %d, HIGH tag = %d", sprite[neartagsprite].lotag, sprite[neartagsprite].hitag);
		MONO_PRINT(ds);

		if (OperateSprite(neartagsprite, TRUE))
		    {
		    FLAG_KEY_RELEASE(pp, SK_OPERATE);
		    }
		}
	    else
		{
		long z[3];
		int i;
		
		z[0] = pp->SpriteP->z - SPRITEp_SIZE_Z(pp->SpriteP) - Z(10);
		z[1] = pp->SpriteP->z;
		z[2] = DIV2(z[0] + z[1]);
		
		for (i = 0; i < SIZ(z); i++)
		    {
		    neartag(pp->posx, pp->posy, z[i], pp->cursectnum, pp->pang,
			&neartagsector, &neartagwall, &neartagsprite,
			&neartaghitdist, 1024+768L, NTAG_SEARCH_LO_HI);
			
		    if (neartagsprite >= 0 && neartaghitdist < 1024 + 768)
			{
			sprintf(ds, "SPRITE LOW tag = %d, HIGH tag = %d", sprite[neartagsprite].lotag, sprite[neartagsprite].hitag);
			MONO_PRINT(ds);

			if (OperateSprite(neartagsprite, TRUE))
			    {
			    FLAG_KEY_RELEASE(pp, SK_OPERATE);
			    break;
			    }
			}
		    }        
		}
		    

	    if (neartagsector >= 0 && neartaghitdist < 1024)
		{
		if (OperateSector(neartagsector, TRUE))
		    {
		    // Release the key
		    FLAG_KEY_RELEASE(pp, SK_OPERATE);

		    sprintf(ds, "SECTOR LOW tag = %d, HIGH tag = %d", LOW_TAG(neartagsector), HIGH_TAG(neartagsector));
		    MONO_PRINT(ds);
		    }
		}

	    if (neartagwall >= 0 && neartaghitdist < 1024)
		{
		if (OperateWall(neartagwall, TRUE))
		    {
		    FLAG_KEY_RELEASE(pp, SK_OPERATE);

		    sprintf(ds, "WALL Wall - LOW tag = %d, HIGH tag = %d", wall[neartagwall].lotag, wall[neartagwall].hitag);
		    MONO_PRINT(ds);
		    }
		}

	    //
	    // Trigger activations -
	    //

	    switch (LOW_TAG(pp->cursectnum))
		{
	    case TAG_SPRING_BOARD:
		DoSpringBoard(pp, pp->cursectnum);
		FLAG_KEY_RELEASE(pp, SK_OPERATE);
		break;
	    case TAG_DOOR_ROTATE:
		if (OperateSector(pp->cursectnum, TRUE))
		    FLAG_KEY_RELEASE(pp, SK_OPERATE);
		break;
		}
	    }
	}
    else
	{
	// Reset the key when syncbit key is not in use
	FLAG_KEY_RESET(pp, SK_OPERATE);
	}
    
    OperateContinuousTrigger(pp);

    // just changed sectors
    if (pp->lastcursectnum != pp->cursectnum)
	{
	OperateTripTrigger(pp);
	}

    if (TEST(sector[pp->cursectnum].extra, SECTFX_TELEPORT))
	{
	OperateTeleporter(pp);
	}
    }



VOID 
DoSineWaveFloor(VOID)
    {
    SINE_WAVE_FLOOR *swf;
    long newz;
    short wave;
    char flags;


    for (wave = 0; wave < MAX_SINE_WAVE; wave++)
	{
	if (wave == 3)
	    {
	    wave = 3;
	    }

	for (swf = &SineWaveFloor[wave][0], flags = swf->flags; swf->sector >= 0 && swf < &SineWaveFloor[wave][SIZ(SineWaveFloor[wave])]; swf++)
	    {

	    swf->sintable_ndx = NORM_ANGLE(swf->sintable_ndx + (synctics << swf->speed_shift));

	    if (TEST(flags, SINE_FLOOR))
		{
		newz = swf->floor_origz + ((swf->range * sintable[swf->sintable_ndx]) >> 14);
		sector[swf->sector].floorz = newz;
		}

	    if (TEST(flags, SINE_CEILING))
		{
		newz = swf->ceiling_origz + ((swf->range * sintable[swf->sintable_ndx]) >> 14);
		sector[swf->sector].ceilingz = newz;
		}

	    }
	}
    }


VOID 
DoSineWaveWall(VOID)
    {
    SINE_WALL *sw;
    long new;
    short sw_num;

    for (sw_num = 0; sw_num < MAX_SINE_WAVE; sw_num++)
	{
	for (sw = &SineWall[sw_num][0]; sw->wall >= 0 && sw < &SineWall[sw_num][MAX_SINE_WALL_POINTS]; sw++)
	    {
	    // move through the sintable
	    sw->sintable_ndx = NORM_ANGLE(sw->sintable_ndx + (synctics << sw->speed_shift));

	    if (!sw->type)
		{
		new = sw->orig_xy + ((sw->range * sintable[sw->sintable_ndx]) >> 14);
		// wall[sw->wall].y = new;
		dragpoint(sw->wall, wall[sw->wall].x, new);
		}
	    else
		{
		new = sw->orig_xy + ((sw->range * sintable[sw->sintable_ndx]) >> 14);
		// wall[sw->wall].x = new;
		dragpoint(sw->wall, new, wall[sw->wall].y);
		}
	    }
	}
    }


VOID 
DoSpikes(VOID)
    {
    SPIKE *spike;

    for (spike = &Spike[0]; spike->sector >= 0 && spike < &Spike[SIZ(Spike)]; spike++)
	{

	spike->timer += synctics;

	if (spike->dir)
	    {
	    if (spike->timer > spike->time_up)
		{
		// time to move down
		spike->timer = spike->timer - spike->time_up;
		// spike->timer = 0;

		AnimSet(&sector[spike->sector].floorz, spike->floorz, 64);

		spike->dir ^= 1;
		}
	    }
	else
	    {
	    if (spike->timer > spike->time_down)
		{
		// time to move up
		spike->timer = spike->timer - spike->time_down;
		// spike->timer = 0;

		AnimSet(&sector[spike->sector].floorz, spike->floorz - Z(42), 256 + 128);

		spike->dir ^= 1;
		}
	    }
	}
    }


VOID 
DoAnim(long numtics)
    {
    long i, animval;

    for (i = AnimCnt - 1; i >= 0; i--)
	{
	animval = *Anim[i].ptr;

	// if LESS THAN goal
	if (animval < Anim[i].goal)
	    {
	    // move it
	    animval += (numtics * PIXZ(Anim[i].vel));
	    
	    Anim[i].vel += Anim[i].vel_adj * numtics;
	    
	    // if the other way make it equal
	    if (animval > Anim[i].goal)
		animval = Anim[i].goal;
	    }
	    
	// if GREATER THAN goal
	if (animval > Anim[i].goal)
	    {
	    animval -= (numtics * PIXZ(Anim[i].vel));
	    
	    Anim[i].vel += Anim[i].vel_adj * numtics;
	    
	    if (animval < Anim[i].goal)
		animval = Anim[i].goal;
	    }

	*Anim[i].ptr = animval;

	// EQUAL this entry has finished
	if (animval == Anim[i].goal)
	    {
	    ANIM_CALLBACKp acp = Anim[i].callback;

	    // do a callback when done if not NULL
	    if (Anim[i].callback)
		(*Anim[i].callback)(&Anim[i], Anim[i].callbackdata);

	    // only delete it if the callback has not changed
	    // Logic here is that if the callback changed then something
	    // else must be happening with it - dont delete it
	    if (Anim[i].callback == acp)
		{
		// decrement the count
		AnimCnt--;

		// move the last entry to the current one to free the last entry up
		Anim[i] = Anim[AnimCnt];
		}
	    }
	}
    }

VOID 
AnimClear(VOID)
    {
    long i, animval;
    
    #if 1
    AnimCnt = 0;
    #else
    for (i = AnimCnt - 1; i >= 0; i--)
	{
	if (Anim[i].extra)
	    FreeMem(Anim[i].extra);
	}
	
    AnimCnt = 0;    
    #endif    
    }
    
short 
AnimGetGoal(long *animptr)
    {
    long i, j;

    j = -1;
    for (i = 0; i < AnimCnt; i++)
	{
	if (animptr == Anim[i].ptr)
	    {
	    j = i;
	    break;
	    }
	}    

    return (j);
    }

void 
AnimDelete(long *animptr)
    {
    long i, j;

    j = -1;
    for (i = 0; i < AnimCnt; i++)
	{
	if (animptr == Anim[i].ptr)
	    {
	    j = i;
	    break;
	    }
	}    

    if (j == -1)    
	return;
	
    // decrement the count
    AnimCnt--;
    
    // move the last entry to the current one to free the last entry up
    Anim[j] = Anim[AnimCnt];
    
    sprintf(ds,"Deleted a Anim");
    MONO_PRINT(ds);
    
    }
    
    
short 
AnimSet(long *animptr, long thegoal, long thevel)
    {
    long i, j;

    ASSERT(AnimCnt < MAXANIM - 1);    

    j = AnimCnt;
    
    // look for existing animation and reset it
    for (i = 0; i < AnimCnt; i++)
	{
	if (animptr == Anim[i].ptr)
	    {
	    j = i;
	    break;
	    }
	}    

    Anim[j].ptr = animptr;
    Anim[j].goal = thegoal;
    Anim[j].vel = Z(thevel);
    Anim[j].vel_adj = 0;
    Anim[j].callback = NULL;
    Anim[j].callbackdata = NULL;
    
    if (j == AnimCnt)
	AnimCnt++;
	
    return (j);
    }

#if 0    
short 
AnimSetCallback(long *animptr, long thegoal, long thevel, ANIM_CALLBACKp call, ANIM_DATAp data)
    {
    short anim_ndx;
    
    anim_ndx = AnimSet(animptr, thegoal, thevel);
    
    Anim[anim_ndx].callback = call;
    Anim[anim_ndx].callbackdata = data;
    
    return(anim_ndx);
    }
#else    
short 
AnimSetCallback(short anim_ndx, ANIM_CALLBACKp call, ANIM_DATAp data)
    {
    ASSERT(anim_ndx < AnimCnt);    
    
    if (anim_ndx == -1)
	return(anim_ndx);
	
    Anim[anim_ndx].callback = call;
    Anim[anim_ndx].callbackdata = data;
    
    return(anim_ndx);
    }
#endif
    
short 
AnimSetVelAdj(short anim_ndx, short vel_adj)
    {
    ASSERT(anim_ndx < AnimCnt);    
    
    if (anim_ndx == -1)
	return(anim_ndx);
	
    Anim[anim_ndx].vel_adj = vel_adj;
    
    return(anim_ndx);
    }
    
    
initlava()
    {
    long x, y, z, r;
    int i;
    extern char tempbuf[];

    for (x = -16; x <= 16; x++)
	for (y = -16; y <= 16; y++)
	    {
	    r = ksqrt(x * x + y * y);
	    lavaradx[r][lavaradcnt[r]] = x;
	    lavarady[r][lavaradcnt[r]] = y;
	    lavaradcnt[r]++;
	    }

//      for(z=0;z<16;z++)
//              lavadropsizlookup[z] = 8 / (ksqrt(z)+1);

    for (z = 0; z < 16; z++)
	lavadropsizlookup[z] = 8 / (ksqrt(16 - z) + 1);

    for (z = 0; z < LAVASIZ; z++)
	lavainc[z] = klabs((((z ^ 17) >> 4) & 7) - 4) + 12;

    lavanumdrops = 0;
    lavanumframes = 0;

    }

movelava(char *dapic)
    {
//    #define COLOR_OFFSET 192
#define COLOR_OFFSET LT_BROWN

    char dat, *ptr;
    long x, y, z, zx, dalavadropsiz, dadropsizlookup, offs, offs2;
    long dalavax, dalavay;

    z = 3;
    if (lavanumdrops + z >= LAVAMAXDROPS)
	z = LAVAMAXDROPS - lavanumdrops - 1;
    while (z >= 0)
	{
	lavadropx[lavanumdrops] = (krand() & (LAVASIZ - 1));
	lavadropy[lavanumdrops] = (krand() & (LAVASIZ - 1));
	lavadropsiz[lavanumdrops] = 1;
	lavanumdrops++;
	z--;
	}

    z = lavanumdrops - 1;
    while (z >= 0)
	{
	dadropsizlookup = lavadropsizlookup[lavadropsiz[z]] * (((z & 1) << 1) - 1);
	dalavadropsiz = lavadropsiz[z];
	dalavax = lavadropx[z];
	dalavay = lavadropy[z];
	for (zx = lavaradcnt[lavadropsiz[z]] - 1; zx >= 0; zx--)
	    {
	    offs = (((lavaradx[dalavadropsiz][zx] + dalavax) & (LAVASIZ - 1)) << LAVALOGSIZ);
	    offs += ((lavarady[dalavadropsiz][zx] + dalavay) & (LAVASIZ - 1));

	    dapic[offs] += dadropsizlookup;

	    if (dapic[offs] < COLOR_OFFSET)
		dapic[offs] = COLOR_OFFSET;
	    }

	lavadropsiz[z]++;
	if (lavadropsiz[z] > 10)
	    {
	    lavanumdrops--;
	    lavadropx[z] = lavadropx[lavanumdrops];
	    lavadropy[z] = lavadropy[lavanumdrops];
	    lavadropsiz[z] = lavadropsiz[lavanumdrops];
	    }
	z--;
	}

    // Back up dapic with 1 pixel extra on each boundary
    // (to prevent anding for wrap-around)
    offs = ((long) dapic);
    offs2 = (LAVASIZ + 2) + 1 + ((long) lavabakpic);
    for (x = 0; x < LAVASIZ; x++)
	{
	copybuf(offs, offs2, LAVASIZ >> 2);
	offs += LAVASIZ;
	offs2 += LAVASIZ + 2;
	}
    for (y = 0; y < LAVASIZ; y++)
	{
	lavabakpic[y + 1] = dapic[y + ((LAVASIZ - 1) << LAVALOGSIZ)];
	lavabakpic[y + 1 + (LAVASIZ + 1) * (LAVASIZ + 2)] = dapic[y];
	}
    for (x = 0; x < LAVASIZ; x++)
	{
	lavabakpic[(x + 1) * (LAVASIZ + 2)] = dapic[(x << LAVALOGSIZ) + (LAVASIZ - 1)];
	lavabakpic[(x + 1) * (LAVASIZ + 2) + (LAVASIZ + 1)] = dapic[x << LAVALOGSIZ];
	}
    lavabakpic[0] = dapic[LAVASIZ * LAVASIZ - 1];
    lavabakpic[LAVASIZ + 1] = dapic[LAVASIZ * (LAVASIZ - 1)];
    lavabakpic[(LAVASIZ + 2) * (LAVASIZ + 1)] = dapic[LAVASIZ - 1];
    lavabakpic[(LAVASIZ + 2) * (LAVASIZ + 2) - 1] = dapic[0];

    for (z = (LAVASIZ + 2) * (LAVASIZ + 2) - 4; z >= 0; z -= 4)
	{
	lavabakpic[z + 0] &= 31;
	lavabakpic[z + 1] &= 31;
	lavabakpic[z + 2] &= 31;
	lavabakpic[z + 3] &= 31;
	}

    for (x = LAVASIZ - 1; x >= 0; x--)
	{
	FAKETIMERHANDLER();
	offs = (x + 1) * (LAVASIZ + 2) + 1;
	ptr = (char *) ((x << LAVALOGSIZ) + (long) dapic);

	zx = ((x + lavanumframes) & (LAVASIZ - 1));

	offs2 = LAVASIZ - 1;
	for (y = offs; y < offs + LAVASIZ; y++)
	    {
	    dat = lavainc[(offs2--) & zx];

	    dat += lavabakpic[y - (LAVASIZ + 2) - 1];
	    dat += lavabakpic[y - (LAVASIZ + 2)];
	    dat += lavabakpic[y - (LAVASIZ + 2) + 1];
	    dat += lavabakpic[y - 1];
	    dat += lavabakpic[y + 1];
	    dat += lavabakpic[y + (LAVASIZ + 2)];
	    dat += lavabakpic[y + (LAVASIZ + 2) - 1];

	    *ptr++ = (dat >> 3) + COLOR_OFFSET;
	    }
	}

    lavanumframes++;
    }


VOID 
DoPanning(VOID)
    {
    FLOOR_PAN *fp;
    CEILING_PAN *cp;
    WALL_PAN *wp;
    long nx, ny;

    for (fp = FloorPan; fp->sector != -1 && fp < &FloorPan[MAX_FLOOR_PAN]; fp++)
	{
	nx = ((synctics * (long) sintable[NORM_ANGLE(fp->ang + 512)]) << fp->vel) >> 17;
	ny = ((synctics * (long) sintable[fp->ang]) << fp->vel) >> 17;

	fp->sectp->floorxpanning += nx;
	fp->sectp->floorypanning += ny;

	fp->sectp->floorxpanning &= 255;
	fp->sectp->floorypanning &= 255;

	// fp->ang = Player[0].pang;
	}


    for (cp = CeilingPan; cp->sector != -1 && cp < &CeilingPan[MAX_FLOOR_PAN]; cp++)
	{
	nx = ((synctics * (long) sintable[NORM_ANGLE(cp->ang + 512)]) << cp->vel) >> 17;
	ny = ((synctics * (long) sintable[cp->ang]) << cp->vel) >> 17;

	cp->sectp->ceilingxpanning += nx;
	cp->sectp->ceilingypanning += ny;

	cp->sectp->ceilingxpanning &= 255;
	cp->sectp->ceilingypanning &= 255;
	}


    for (wp = WallPan; wp->wall != -1 && wp < &WallPan[MAX_WALL_PAN]; wp++)
	{
	nx = ((synctics * (long) sintable[NORM_ANGLE(wp->ang + 512)]) << wp->vel) >> 17;
	ny = ((synctics * (long) sintable[wp->ang]) << wp->vel) >> 17;

	wp->wallp->xpanning += nx;
	wp->wallp->ypanning += ny;

	wp->wallp->xpanning &= 255;
	wp->wallp->ypanning &= 255;
	}
    }


VOID
DoSector(VOID)
    {
    short i;

    #if 1
    MoveSectorObjects(synctics);
    #else
    if (MoveSkip2 == 0)
	MoveSectorObjects(synctics*2);
    #endif

    DoPanning();
    DoSpikes();

    DoLights();

    DoPauseAction();

    DoSwingingDoors();

    DoSineWaveFloor();
    DoSineWaveWall();

    DoElevatorAuto();
    DoDoorsClose();
    DoSpringBoardDown();
    }
