//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

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

Original Source: 1994 - Jim Dose
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

/**********************************************************************
   module: SNDCARDS.H

   author: James R. Dos‚
   phone:  (214)-XXX-XXX Ext #221
   date:   March 31, 1994

   Contains enumerated type definitions for sound cards.

   (c) Copyright 1994 James R. Dos‚.  All Rights Reserved.
**********************************************************************/

#ifndef __SNDCARDS_H
#define __SNDCARDS_H

#define ASS_VERSION_STRING "1.09"

typedef enum
   {
//   ASS_NoSound,
   SoundBlaster,
   ProAudioSpectrum,
   SoundMan16,
   Adlib,
   GenMidi,
   SoundCanvas,
   Awe32,
   WaveBlaster,
   SoundScape,
   UltraSound,
   SoundSource,
   TandySoundSource,
   PC,
   NumSoundCards
   } soundcardnames;

#endif
