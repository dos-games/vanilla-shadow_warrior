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

Agetnumber256(char namestart[80], short num, long maxnumber)
{
	char buffer[80];
    char neg=0;
	long j, k, n, danum, oldnum;

	danum = (long)num;
    oldnum = danum;
    if(danum<0) {neg=1; danum*= -1;}
	while ((keystatus[0x1c] != 2) && (keystatus[0x1] == 0))
	{
		drawrooms(posx,posy,posz,ang,horiz,cursectnum);
		drawmasks();

        if(!neg) sprintf(&buffer,"%s%ld_ ",namestart,danum);
        else sprintf(&buffer,"%s-%ld_ ",namestart,danum);
		printmessage256(buffer);
		nextpage();

		for(j=2;j<=11;j++)
			if (keystatus[j] > 0)
			{
				keystatus[j] = 0;
				k = j-1;
				if (k == 10) k = 0;
                n = (danum*10)+k;
				if (n < maxnumber) danum = n;
			}
		if (keystatus[0xe] > 0)    // backspace
		{
			danum /= 10;
            if(danum==0) neg=0;
			keystatus[0xe] = 0;
		}
        if (keystatus[0xc] > 0)    // neg
		{
            neg=!neg;
            keystatus[0xc] = 0;
        }
		if (keystatus[0x1c] == 1)
		{
			oldnum = danum;
            if(neg) oldnum *= -1;
			keystatus[0x1c] = 2;
			asksave = 1;
		}
	}
	keystatus[0x1c] = 0;
	keystatus[0x1] = 0;

//    lockclock = totalclock;  //Reset timing

	return((short)oldnum);
}
