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

#define MAIN
#define QUIET
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>

float hitdistance(int weap_maxdist, int targetdistance)
{
	float reducepercent;

//	if(weap_maxdist==0) return;

    // If result is greater than 1, it's invalid.
	// >1 means you are beyond the weapon's effective range.
	reducepercent = (float)targetdistance/(float)weap_maxdist;

	return( reducepercent );
}

float aimdeviation(int angle)
{
	float reducebyaim;

    // A result >1 means you missed the target entirely!
	reducebyaim = (float)angle/512;

	return( reducebyaim );
}

main()
{
	int wd, wm, td, aim;
	float hitpoints,reducepercent;
	char buffer[80];

	strcpy(buffer," ");

	do{
		printf("Enter weapon's maximum damage: ");
 		gets( buffer );	
		strupr( buffer );
		wd = atoi( buffer );
		printf("Enter maximum effective weapon range: ");
 		gets( buffer );	
		strupr( buffer );
		wm = atoi( buffer );
		printf("Enter distance to target: ");
 		gets( buffer );	
		strupr( buffer );
		td = atoi( buffer );
		printf("Enter aim deviation from 0-512: ");
 		gets( buffer );	
		strupr( buffer );
		aim = atoi( buffer );


		// I want aim deviation reduction to have priority over distance
		reducepercent = aimdeviation(aim);

		// Example: Now, if angle deviation has redered your shot to 50%
		// effective, the following code will take it's % effectiveness 
		// times 50% so, e.i. 50%*40% or whatever.
		reducepercent -= reducepercent * hitdistance(wm,td);

		// This line calculates the actual hitdamage to do to the target.
		// NOTE: If the result is 0 or less, just do the weapons 
		// minimum amount of damage possible.
		hitpoints = wd - (wd*reducepercent);

		printf("Resulting damage = %f\n",hitpoints);

	} while(!strcmp(buffer,"QUIT"));

}