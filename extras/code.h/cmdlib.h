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

// cmdlib.h

#ifndef __CMDLIB__
#define __CMDLIB__

#ifdef __NeXT__
#define stricmp strcasecmp
#endif

typedef unsigned char byte;
typedef enum {false,true} boolean;


void    Error (char *error, ...);
int     CheckParm (char *check);

int     SafeOpenWrite (char *filename);
int     SafeOpenRead (char *filename);
void    SafeRead (int handle, void *buffer, long count);
void    SafeWrite (int handle, void *buffer, long count);
void    *SafeMalloc (long size);

long    LoadFile (char *filename, void **bufferptr);
void    SaveFile (char *filename, void *buffer, long count);

void    DefaultExtension (char *path, char *extension);
void    DefaultPath (char *path, char *basepath);
void    StripFilename (char *path);
void    ExtractFileBase (char *path, char *dest);

long    ParseNum (char *str);

short   BigShort (short l);
short   LittleShort (short l);
long    BigLong (long l);
long    LittleLong (long l);

void    GetPalette (byte *pal);
void    SetPalette (byte *pal);
void    VGAMode (void);
void    TextMode (void);

#endif
