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

// SWEXPFL.H
// This is a list of every function that is exported from SW for DLL use
// These prototypes are devoid of any typedef identifiers since Frank's
// BOOL, VOID, etc. declarations are identical to those in a Dos4G header. DOH!
// 

#ifndef SWEXPFL_H
#define SWEXPFL_H

char ValidPtr(void *ptr);
void * AllocMem(int size);
void * CallocMem(int size, int num);
void * ReAllocMem(void *ptr, int size);
void FreeMem(void *ptr);

#endif
