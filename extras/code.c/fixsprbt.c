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

	//Here's a nice generic map converter
	//It loads a map, you can do stuff to it, then save the map.

#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <dos.h>

#define MAPVERSION 6
#define MAXMENUFILES 1024

#define MAXSECTORS 1024
#define MAXWALLS (4096*2)
#define MAXSPRITES 4096

#define MAXSTATUS 1024
#define MAXTILES 4096

static char menuname[MAXMENUFILES][32];
static long menunamecnt;

static struct sectortype
{
	unsigned short wallptr, wallnum;
	short ceilingpicnum, floorpicnum;
	short ceilingheinum, floorheinum;
	long ceilingz, floorz;
	signed char ceilingshade, floorshade;
	char ceilingxpanning, floorxpanning;
	char ceilingypanning, floorypanning;
	char ceilingstat, floorstat;
	char ceilingpal, floorpal;
	char visibility;
	short lotag, hitag, extra;
};

static struct walltype
{
	long x, y;
	short point2, nextsector, nextwall;
	short picnum, overpicnum;
	signed char shade;
	char pal;
	short cstat;
	unsigned char xrepeat, yrepeat, xpanning, ypanning;
	short lotag, hitag, extra;
};

static struct spritetype
{
	long x, y, z;
	short cstat;
	signed char shade;
	char pal, clipdist;
	unsigned char xrepeat, yrepeat;
	signed char xoffset, yoffset;
	short picnum, ang, xvel, yvel, zvel, owner;
	short sectnum, statnum;
	short lotag, hitag, extra;
};

//**************************************************************************

static struct sectortype sector[MAXSECTORS];
static struct walltype wall[MAXWALLS];
static struct spritetype sprite[MAXSPRITES];

static long posx, posy, posz, mapversion;
static short ang, cursectnum, numsectors, numwalls, numsprites;

main(int argc, char **argv)
{
	long i;

	if (argc != 2)
	{
		printf("FIXSPRBT.C [filespec]\n");
		printf("This program copies one bit in sprite[].cstat.  Bit 8 = bit 0.\n");
		printf("Run this program on all your maps to make hitscan work like it used to\n");
		exit(0);
	}
	menunamecnt = 0;
	getfilenames(argv[1]);
	sortfilenames();

	if (menunamecnt == 0)
	{
		printf("FIXSPRBT.C [filespec]\n");
		printf("This program copies one bit in sprite[].cstat.  Bit 8 = bit 0.\n");
		printf("Run this program on all your maps to make hitscan work like it used to\n");
		printf("File not found\n");
		exit(0);
	}

	printf("Fixing bit 8 of sprite[].cstat\n");

	for(i=0;i<menunamecnt;i++)
		convmap(menuname[i]);
}

convmap(char *filename)
{
	long i, j;

	if (strchr(filename,'.') == 0) strcat(filename,".map");
	if ((i = loadoldboard(filename)) < 0)
	{
		if (i == -1)
		{
			printf("%s not found.\n",filename);
		}
		if (i == -2)
		{
			if (mapversion != MAPVERSION)
				printf("%s - Wrong map version!\n",filename);
		}
		return;
	}

	for(i=0;i<numsprites;i++)
	{
		sprite[i].cstat = (sprite[i].cstat & ~256);
		sprite[i].cstat |= ((sprite[i].cstat&1)<<8);
	}

	if (savenewboard(filename) == 0)
		printf("%s successfully converted.\n",filename);
	else
		printf("%s NOT converted for some stupid reason.\n",filename);
}

loadoldboard(char *filename)
{
	long i, fil;

	if ((fil = open(filename,O_BINARY|O_RDWR,S_IREAD)) == -1)
		return(-1);

		 //Version control
	read(fil,&mapversion,4);
	if (mapversion != MAPVERSION)
	{
		close(fil);
		return(-2);
	}

		 //Starting position
	read(fil,&posx,4);
	read(fil,&posy,4);
	read(fil,&posz,4);
	read(fil,&ang,2);
	read(fil,&cursectnum,2);

		//Sectors
	read(fil,&numsectors,2);
	read(fil,&sector[0],sizeof(struct sectortype)*numsectors);

		//Walls
	read(fil,&numwalls,2);
	read(fil,&wall[0],sizeof(struct walltype)*numwalls);

		//Sprites
	read(fil,&numsprites,2);
	read(fil,&sprite[0],sizeof(struct spritetype)*numsprites);

	close(fil);

	return(0);
}

savenewboard(char *filename)
{
	long i, fil;

	if ((fil = open(filename,O_BINARY|O_TRUNC|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
		return(-1);

		 //Version control
	write(fil,&mapversion,4);

		 //Starting position
	write(fil,&posx,4);
	write(fil,&posy,4);
	write(fil,&posz,4);
	write(fil,&ang,2);
	write(fil,&cursectnum,2);

		//Sectors
	write(fil,&numsectors,2);
	write(fil,&sector[0],sizeof(struct sectortype)*numsectors);

		//Walls
	write(fil,&numwalls,2);
	write(fil,&wall[0],sizeof(struct walltype)*numwalls);

		//Sprites
	write(fil,&numsprites,2);
	write(fil,&sprite[0],sizeof(struct spritetype)*numsprites);

	close(fil);

	return(0);
}

getfilenames(char *kind)
{
	short type;
	struct find_t fileinfo;

	if (strcmp(kind,"SUBD") == 0)
	{
		strcpy(kind,"*.*");
		if (_dos_findfirst(kind,_A_SUBDIR,&fileinfo) != 0)
			return(-1);
		type = 1;
	}
	else
	{
		if (_dos_findfirst(kind,_A_NORMAL,&fileinfo) != 0)
			return(-1);
		type = 0;
	}
	do
	{
		if ((type == 0) || ((fileinfo.attrib&16) > 0))
			if ((fileinfo.name[0] != '.') || (fileinfo.name[1] != 0))
			{
				if (menunamecnt < MAXMENUFILES)
				{
					strcpy(menuname[menunamecnt],fileinfo.name);
					menuname[menunamecnt][16] = type;
					menunamecnt++;
				}
				//else
				//   printmessage("Too many files! (max=MAXMENUFILES)");
			}
	}
	while (_dos_findnext(&fileinfo) == 0);

	return(0);
}

sortfilenames()
{
	char sortbuffer[32];
	long i, j, k, m;

	for(i=1;i<menunamecnt;i++)
		for(j=0;j<i;j++)
		{
			 k = 0;
			 while ((menuname[i][k] == menuname[j][k]) && (menuname[i][k] != 0) && (menuname[j][k] != 0))
				 k++;
			if (menuname[i][k] < menuname[j][k])
			{
				memcpy(&sortbuffer[0],&menuname[i][0],sizeof(menuname[0]));
				memcpy(&menuname[i][0],&menuname[j][0],sizeof(menuname[0]));
				memcpy(&menuname[j][0],&sortbuffer[0],sizeof(menuname[0]));
			}
		}
}

