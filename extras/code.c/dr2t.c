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


void drawroomstotile(long daposx, long daposy, long daposz,
			 short daang, long dahoriz, short dacursectnum, short tilenume)
{

	long i, j, k, bakchainnumpages, bakvidoption, bakframeplace;
	long bakwindowx1, bakwindowy1, bakwindowx2, bakwindowy2, xsiz, ysiz;
	char *ptr1, *ptr2;

		//DRAWROOMS TO TILE BACKUP&SET CODE
	xsiz = tilesizx[tilenume]; ysiz = tilesizy[tilenume];
	bakchainnumpages = chainnumpages; chainnumpages = 0;
	bakvidoption = vidoption; vidoption = 1;
	if (waloff[tilenume] == 0) loadtile(tilenume);
	bakframeplace = frameplace; frameplace = waloff[tilenume];
	bakwindowx1 = windowx1; bakwindowy1 = windowy1;
	bakwindowx2 = windowx2; bakwindowy2 = windowy2;
	setview(0,0,xsiz-1,ysiz-1);
	setvlinebpl(xsiz);
	j = 0; for(i=0;i<=ysiz;i++) { ylookup[i] = j, j += xsiz; }

		//DRAWS TO TILE HERE
	drawrooms(daposx,daposy,daposz,daang,dahoriz,dacursectnum);
    display_mirror = 1;
    animatesprites(daposx,daposy,daang);
    display_mirror = 0;
	drawmasks();
    // dowater(dahoriz);

		//ROTATE TILE (supports square tiles only for rotation part)
    if(xsiz == ysiz)
    {
        k = (xsiz<<1);
        for(i=xsiz-1;i>=0;i--)
        {
            ptr1 = (char *)(waloff[tilenume]+i*(xsiz+1)); ptr2 = ptr1;
            if ((i&1) != 0)
            {
                ptr1--; ptr2 -= xsiz;
                swapchar(ptr1,ptr2);
            }
            for(j=(i>>1)-1;j>=0;j--)
            {
                ptr1 -= 2; ptr2 -= k;
                swapchar2(ptr1,ptr2,xsiz);
                //swapchar(ptr1,ptr2);
                //swapchar(ptr1+1,ptr2+xsiz);
            }
        }
    }

		//DRAWROOMS TO TILE RESTORE CODE
	setview(bakwindowx1,bakwindowy1,bakwindowx2,bakwindowy2);
	chainnumpages = bakchainnumpages;
	vidoption = bakvidoption;
	frameplace = bakframeplace;
	j = 0;
	if (chainnumpages >= 2)
		{ for(i=0;i<=ysiz;i++) ylookup[i] = j, j += (xdim>>2); }
	else
		{ for(i=0;i<=ysiz;i++) ylookup[i] = j, j += xdim; }
	setvlinebpl(ylookup[1]);
}
