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

	//Shared player variables
static long posx[MAXPLAYERS], etc.

  //Multiplayer syncing variables
static signed char syncvel[MAXPLAYERS+1], osyncvel[MAXPLAYERS+1];
static signed char syncsvel[MAXPLAYERS+1], osyncsvel[MAXPLAYERS+1];
static signed char syncangvel[MAXPLAYERS+1], osyncangvel[MAXPLAYERS+1];
static short syncbits[MAXPLAYERS+1], osyncbits[MAXPLAYERS+1];
static unsigned char synctics;

main()
{
	if (option[4] > 0)
	{
		printext256(68L,84L,31,0,"Waiting for other player...",0);
        
        // does a broadcast
		sendlogon();
		while (numplayers < 2)
			getpackets();
        // set screenpeek to my index - note that you can look at other players
        // screens with thier index number
        // It is a parameter of draw    
		screenpeek = myconnectindex;
	}

	prepareboard("stupid.map");

	while (keystatus[1] == 0)
	{
        // draw rooms, masks, any other overlayed material
		drawscreen(screenpeek);
        // syncronize between the master and slave
		sync();
	}

    // get off    
	sendlogoff();
}

prepareboard(char *daboardfilename)
{
	randomseed = 17L;
}

processinput(short snum)
{
	//Read sync arrays and do key & mouse stuff
    //SHARED KEYS, Movement code
	Modify       : posx[snum], posy[snum], ang[snum], horiz[snum], etc.
	according to : syncvel[snum], syncbits[snum], synctics, etc.
}

drawscreen(short snum)
{
	//NO SHARED VARIABLES CAN CHANGE HERE!
	long dasectnum;

	dasectnum = cursectnum[snum];
	drawrooms(posx[snum],posy[snum],posz[snum],ang[snum],horiz[snum],dasectnum);
	drawmasks();

	nextpage();
}

movethings()
{
	short i, j;

		//SHARED VARIABLES CAN ONLY BE CHANGED WITHIN THIS SUBROUTINE OR ANY
		//SUB-SUBROUTINE CONTAINED IN HERE!

    // move the players
	i = connecthead;
	while (i != -1)
	{
		processinput(i);       //Move player
		checktouchsprite(i);   //Pick up coins
		i = connectpoint2[i];
	}

    // move sectors    
	doanimations(((long)synctics));
    // do tag code
	tagcode();            //Door code, moving sector code, other stuff
	statuslistcode();     //Monster / bullet code / explosions

	lockclock += (long)synctics; synctics = 0;
}

// Can copy verbatim from game.c!
getinput(short snum)
{
	//Store keys & mouse into sync arrays
	syncvel[snum] = vel;
	syncsvel[snum] = svel;
	syncangvel[snum] = angvel;
	syncbits[snum] += keystatus[0x39];
	syncbits[snum] += (keystatus[0x1c]<<1);
}

// Can copy verbatim from game.c!
sync()
{
	long i, j, k;

	if (myconnectindex == connecthead) //I am the MASTER!
	{
		getinput(myconnectindex);
		getpackets();

		if (synctics > (option[5]<3))      //1 for <= 9600 baud, 0 for faster
		{
			tempbuf[0] = 2;
			tempbuf[1] = synctics;

			j = ((numplayers+1)>>1)+2;
			for(k=2;k<j;k++)
				tempbuf[k] = 0;

			i = connecthead; k = (2<<3);
			while (i != -1)
			{
				if (syncvel[i] != osyncvel[i])
				{
					tempbuf[j++] = syncvel[i];
					osyncvel[i] = syncvel[i];
					tempbuf[k>>3] |= (1<<(k&7));
				}
				if (syncsvel[i] != osyncsvel[i])
				{
					tempbuf[j++] = syncsvel[i];
					osyncsvel[i] = syncsvel[i];
					tempbuf[k>>3] |= (2<<(k&7));
				}
				if (syncangvel[i] != osyncangvel[i])
				{
					tempbuf[j++] = syncangvel[i];
					osyncangvel[i] = syncangvel[i];
					tempbuf[k>>3] |= (4<<(k&7));
				}
				if (syncbits[i] != osyncbits[i])
				{
					tempbuf[j++] = (syncbits[i]&255);
					tempbuf[j++] = ((syncbits[i]>>8)&255);
					osyncbits[i] = syncbits[i];
					tempbuf[k>>3] |= (8<<(k&7));
				}
				k += 4;

				i = connectpoint2[i];
			}

			i = connecthead;
			while (i != -1)
			{
				if (i != myconnectindex) sendpacket(i,tempbuf,j);
				i = connectpoint2[i];
			}

			movethings();  //Move EVERYTHING (you too!)
		}
	}
	else                        //I am a SLAVE
	{
		getinput(MAXPLAYERS);

		tempbuf[0] = 3;
		tempbuf[1] = 0;
		j = 2;
		if (syncvel[MAXPLAYERS] != osyncvel[MAXPLAYERS])
		{
			tempbuf[j++] = syncvel[MAXPLAYERS];
			osyncvel[MAXPLAYERS] = syncvel[MAXPLAYERS];
			tempbuf[1] |= 1;
		}
		if (syncsvel[MAXPLAYERS] != osyncsvel[MAXPLAYERS])
		{
			tempbuf[j++] = syncsvel[MAXPLAYERS];
			osyncsvel[MAXPLAYERS] = syncsvel[MAXPLAYERS];
			tempbuf[1] |= 2;
		}
		if (syncangvel[MAXPLAYERS] != osyncangvel[MAXPLAYERS])
		{
			tempbuf[j++] = syncangvel[MAXPLAYERS];
			osyncangvel[MAXPLAYERS] = syncangvel[MAXPLAYERS];
			tempbuf[1] |= 4;
		}
		if ((syncbits[MAXPLAYERS]&255) != (osyncbits[MAXPLAYERS]&255))
		{
			tempbuf[j++] = (syncbits[MAXPLAYERS]&255);
			tempbuf[1] |= 8;
		}
		if ((syncbits[MAXPLAYERS]|255) != (osyncbits[MAXPLAYERS]|255))
		{
			tempbuf[j++] = ((syncbits[MAXPLAYERS]>>8)&255);
			tempbuf[1] |= 16;
		}
		osyncbits[MAXPLAYERS] = syncbits[MAXPLAYERS];

		sendpacket(connecthead,tempbuf,j);
		getpackets();
	}
}

// I think can copy verbatim. Ask KEN?
getpackets()
{
	short otherconnectindex, tempbufleng;
	long i, j, k;

	while ((tempbufleng = getpacket(&otherconnectindex,tempbuf)) > 0)
	{
		//for(i=0;i<tempbufleng;i++)
		//   printf("%d ",tempbuf[i]);
		//printf("\n");

		switch(tempbuf[0])
		{
			case 2:  //[2] (receive master sync buffer)
				synctics = tempbuf[1];

				j = ((numplayers+1)>>1)+2; k = (2<<3);
				i = connecthead;
				while (i != -1)
				{
					if ((tempbuf[k>>3]&(1<<(k&7))) > 0) syncvel[i] = tempbuf[j++];
					if ((tempbuf[k>>3]&(2<<(k&7))) > 0) syncsvel[i] = tempbuf[j++];
					if ((tempbuf[k>>3]&(4<<(k&7))) > 0) syncangvel[i] = tempbuf[j++];
					if ((tempbuf[k>>3]&(8<<(k&7))) > 0)
					{
						syncbits[i] = ((short)tempbuf[j])+(((short)tempbuf[j+1])<<8);
						j += 2;
					}

					k += 4;

					i = connectpoint2[i];
				}

				movethings();        //Move all players and sprites

				break;
			case 3:  //[3] (receive slave sync buffer)
				j = 2;
				if ((tempbuf[1]&1) > 0)
					syncvel[otherconnectindex] = tempbuf[j++];
				if ((tempbuf[1]&2) > 0)
					syncsvel[otherconnectindex] = tempbuf[j++];
				if ((tempbuf[1]&4) > 0)
					syncangvel[otherconnectindex] = tempbuf[j++];
				if ((tempbuf[1]&8) > 0)
				{
					syncbits[otherconnectindex] &= 0xff00;
					syncbits[otherconnectindex] |= ((short)tempbuf[j++]);
				}
				if ((tempbuf[1]&16) > 0)
				{
					syncbits[otherconnectindex] &= 0x00ff;
					syncbits[otherconnectindex] |= (((short)tempbuf[j++])<<8);
				}

				break;
			case 255:  //[255] (logout)
				keystatus[1] = 1;
				break;
		}
	}
}
