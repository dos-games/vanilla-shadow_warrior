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

// JNVOXELS.C  -  My functions for messing around with voxel slabs in memory.

#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include "string.h"
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"


// This lets me mess with voxels in memory!
#define MAXVOXELS 512
#define MAXVOXMIPS 5
extern long voxoff[MAXVOXELS][MAXVOXMIPS];

/*
Each voxel has a structure that looks like this for each of its mip levels:
   long xsiz, ysiz, zsiz, xpivot, ypivot, zpivot;
   long xoffset[xsiz+1];
   short xyoffset[xsiz+1][ysiz+1];
   char rawslabdata[?];

Here's how you traverse the offset arrays for any (x,y) column of the voxel:

startptr = voxoff[?][?] + xoffset[x] + xyoffset[x][y];   //pointer to start of slabs on column (x,y)
  endptr = voxoff[?][?] + xoffset[x] + xyoffset[x][y+1]; //pointer to end of slabs on column (x,y)

Note:  endptr is actually the start of the next column (startptr <= currentcolumn < endptr)

Once you get these 2 pointers, you can run through all of the slabs in the column.  Each
   slab has 3 header bytes, then an array of colors.  Here's the format:
 
   char slabztop;              //Starting z coordinate of top of slab
   char slabzleng;             //Number of bytes in the color array - slab height
   char slabbackfacecullinfo;  //Low 6 bits used to tell which of the 6 faces are exposed.
   char col[slabzleng];        //The array of colors from top to bottom

    ptr = startptr;
    // process slab

    // to skip slabs, do this
    currzleng = ptr+1;
    ptr += currzleng;
*/
#if 0
// Writes out a voxel sprite to a file so you can see the format!
void WriteVoxelToFile (short voxel)
{
    int i;

    FILE *fp;
    char filename[13];

    // Pointers to voxel structure  
    char *startptr, *endptr;


    memset(filename,0,sizeof(filename));

    // Build me a filename
    strcpy(filename,"vox");
    strcat(filename,itoa(voxel));
    strcat(filename,".vfm");

    fp = fopen(filename,"w+"); 

    if(fp != NULL)
    {
        fprintf(fp,"Voxel number: %d\n",voxel);

        // Do for all mip levels
        for(i=0; i<5; i++)
        {
            voxoff[voxel][i].xsiz +=
        }
    }
}
#endif



