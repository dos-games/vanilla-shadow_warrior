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

void MyGetZrange(long x, long y, long z, short sectnum, long *ceilz, long *ceilhit, long *florz, long *florhit, long walldist, char cliptype)
    {
    short bad, clipsectcnt, startwall, endwall, clipyou;
    long x1, y1, x2, y2, xmin, ymin, xmax, ymax, i, j, k, daz, daz2;
    
    short clipsectnum;
    extern short editstatus;
    static short clipsectorlist[512];

    // Extra walldist for sprites on sector lines
    xmin = x - ((walldist << 1) + 1);
    ymin = y - ((walldist << 1) + 1);
    xmax = x + ((walldist << 1) + 1);
    ymax = y + ((walldist << 1) + 1);

    *ceilz = sector[sectnum].ceilingz;
    *florz = sector[sectnum].floorz;
    *ceilhit = sectnum + 16384;
    *florhit = sectnum + 16384;

    clipsectcnt = 0;
    clipsectnum = 0;
    clipsectorlist[clipsectnum++] = sectnum;

    // Collect sectors inside your square first
    while (clipsectcnt < clipsectnum)
	{
	startwall = sector[clipsectorlist[clipsectcnt]].wallptr;
	endwall = startwall + sector[clipsectorlist[clipsectcnt]].wallnum - 1;
	for (j = startwall; j <= endwall; j++)
	    if (wall[j].nextsector >= 0)
		{
		x1 = wall[j].x;
		y1 = wall[j].y;
		x2 = wall[wall[j].point2].x;
		y2 = wall[wall[j].point2].y;

		bad = 0;

		if ((x1 < xmin) && (x2 < xmin))
		    bad = 1;
		else if ((x1 > xmax) && (x2 > xmax))
		    bad = 1;
		else if ((y1 < ymin) && (y2 < ymin))
		    bad = 1;
		else if ((y1 > ymax) && (y2 > ymax))
		    bad = 1;
		else if ((x1 - x) * (y2 - y) < (x2 - x) * (y1 - y))
		    bad = 1;            // If wall's NOT facing you

		if (bad == 0)
		    {
		    if (y2 > y1)
			{
			if (x2 > x1)
			    {
			    if ((x2 - x1) * (ymin - y1) - (xmax - x1) * (y2 - y1) >= 0)
				bad = 1;
			    }
			else
			    {
			    if ((x2 - x1) * (ymax - y1) - (xmax - x1) * (y2 - y1) >= 0)
				bad = 1;
			    }
			}
		    else
			{
			if (x2 > x1)
			    {
			    if ((x2 - x1) * (ymin - y1) - (xmin - x1) * (y2 - y1) >= 0)
				bad = 1;
			    }
			else
			    {
			    if ((x2 - x1) * (ymax - y1) - (xmin - x1) * (y2 - y1) >= 0)
				bad = 1;
			    }
			}


		    if (bad == 0)
			{
			clipyou = 0;
			if (wall[j].nextsector == -1)
			    clipyou = 1;
			if (((cliptype == 0) || (cliptype == 2)) && ((wall[j].cstat & 1) > 0))
			    clipyou = 1;
			if ((cliptype == 1) && ((wall[j].cstat & 64) > 0))
			    clipyou = 1;
			if (editstatus == 0)
			    {
			    if ((sector[wall[j].nextsector].ceilingstat & 1) == 0)
				{
				if (z <= sector[wall[j].nextsector].ceilingz + (3 << 8))
				    clipyou = 1;
				}
			    if ((sector[wall[j].nextsector].floorstat & 1) == 0)
				{
				if (z >= sector[wall[j].nextsector].floorz - (3 << 8))
				    clipyou = 1;

				if (cliptype == 2)
				    if (z + scaredfallz < sector[wall[j].nextsector].floorz)
					clipyou = 1;
				}
			    }

			if (clipyou == 0)
			    {
			    for (i = 0; i < clipsectnum; i++)
				{
				if (wall[j].nextsector == clipsectorlist[i])
				    {
				    bad = 1;
				    break;
				    }
				}
			    if (bad == 0)
				{
				i = wall[j].nextsector;

				clipsectorlist[clipsectnum++] = i;

				if ((x1 < xmin + walldist) && (x2 < xmin + walldist))
				    bad = 1;
				else if ((x1 > xmax - walldist) && (x2 > xmax - walldist))
				    bad = 1;
				else if ((y1 < ymin + walldist) && (y2 < ymin + walldist))
				    bad = 1;
				else if ((y1 > ymax - walldist) && (y2 > ymax - walldist))
				    bad = 1;
				else if ((x1 - x) * (y2 - y) < (x2 - x) * (y1 - y))
				    bad = 1;    // If wall's NOT facing you

				if (bad == 0)
				    {
				    if (y2 > y1)
					{
					if (x2 > x1)
					    {
					    if ((x2 - x1) * (ymin + walldist - y1) - (xmax - walldist - x1) * (y2 - y1) >= 0)
						bad = 1;
					    }
					else
					    {
					    if ((x2 - x1) * (ymax - walldist - y1) - (xmax - walldist - x1) * (y2 - y1) >= 0)
						bad = 1;
					    }
					}
				    else
					{
					if (x2 > x1)
					    {
					    if ((x2 - x1) * (ymin + walldist - y1) - (xmin + walldist - x1) * (y2 - y1) >= 0)
						bad = 1;
					    }
					else
					    {
					    if ((x2 - x1) * (ymax - walldist - y1) - (xmin + walldist - x1) * (y2 - y1) >= 0)
						bad = 1;
					    }
					}

				    if (bad == 0)
					{
					if (sector[i].ceilingz > *ceilz)
					    {
					    *ceilz = sector[i].ceilingz;
					    *ceilhit = i + 16384;
					    }
					if (sector[i].floorz < *florz)
					    {
					    *florz = sector[i].floorz;
					    *florhit = i + 16384;
					    }
					}
				    }
				}
			    }
			}
		    }
		}
	clipsectcnt++;
	}

    for (i = 0; i < clipsectnum; i++)
	{
	k = headspritesect[clipsectorlist[i]];
	while (k != -1)
	    {
	    if ((sprite[k].cstat & 1) > 0)
		{
		if ((sprite[k].x >= xmin) && (sprite[k].x <= xmax))
		    if ((sprite[k].y >= ymin) && (sprite[k].y <= ymax))
			{
			daz = sprite[k].z + (4 << 8);
			daz2 = sprite[k].z - (4 << 8) - ((tilesizy[sprite[k].picnum] * sprite[k].yrepeat) << 2);

			if ((z > daz) && (daz > *ceilz))
			    {
			    *ceilz = daz;
			    *ceilhit = k + 49152;
			    }
			if ((z < daz2) && (daz2 < *florz))
			    {
			    *florz = daz2;
			    *florhit = k + 49152;
			    }
			}
		}
	    k = nextspritesect[k];
	    }
	}
    }
