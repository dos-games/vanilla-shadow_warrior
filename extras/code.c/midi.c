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

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "build.h"
#undef MAXPLAYERS
#include "names2.h"
#include "types.h"
#include "sndcards.h"
#include "fx_man.h"
#include "music.h"
#include "util_lib.h"
#include "gamedefs.h"
#include "config.h"
#include "panel.h"
#include "game.h"
#include "sounds.h"
#include "ai.h"

char gm_on[] = {0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7};
char gs_on[] = {0xF0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7};
char xg_on[] = {0xF0, 0x43, 0x10, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7};

// config adjustable    
//extern long MidiPort;
#define midi_status_port (MidiPort+1)
#define midi_status_busy_flag (BIT(6))

void SendGeneralMidiSysX(void)
    {
    short i;
    
    if (MusicDevice != GenMidi)
        return;
    
    for (i = 0; i < SIZ(gm_on); i++)
        MidiStatusOut(gm_on[i]);
        
    for (i = 0; i < SIZ(gs_on); i++)
        MidiStatusOut(gs_on[i]);
        
    for (i = 0; i < SIZ(xg_on); i++)
        MidiStatusOut(xg_on[i]);
    }

void MidiStatusOut(char data)
    {
    long count = 1024;
    
    while (count > 0 && TEST(inp(midi_status_port+1), midi_status_busy_flag))
        {
        count--;
        }
        
    outp(midi_status_port, data);
    }