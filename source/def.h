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

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#if 1 

//#include <dos.h>
#include <stdio.h>
#include <stdarg.h>
//#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <graph.h>
#include "proto.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "sector.h"
#include "sprite.h"
#include "weapon.h"
#include "player.h"
#include "lists.h"
#include "pal.h"
#include "types.h"
#include "break.h"
#include "ai.h"
#include "jsector.h"
#include "parent.h"
#include "menus.h"
#include "mfile.h"
#include "slidor.h"
#include "task_man.h"
#include "pragmas.h"


#include "ACTOR.DEF"
//#include "AI.DEF"
#include "BORDER.DEF"
#include "BREAK.DEF"
#include "CD.DEF"
#include "CHEATS.DEF"
#include "COLORMAP.DEF"
#include "CONSOLE.DEF"
#include "COOLG.DEF"
#include "COOLIE.DEF"
#include "COPYSECT.DEF"
#include "DEMO.DEF"
#include "DRAW.DEF"
#include "EEL.DEF"
#include "GAME.DEF"
#include "GORO.DEF"
#include "HORNET.DEF"
#include "INV.DEF"
#include "JPLAYER.DEF"
#include "JSECTOR.DEF"
#include "JWEAPON.DEF"
#include "KBD.DEF"
#include "LAVA.DEF"
#include "LIGHT.DEF"
#include "MCLIP.DEF"
#include "MDASTR.DEF"
#include "MENUS.DEF"
#include "MIDI.DEF"
#include "MISCACTR.DEF"
#include "MORPH.DEF"
#include "NET.DEF"
#include "NINJA.DEF"
#include "PANEL.DEF"
#include "PLAYER.DEF"
#include "PREDICT.DEF"
#include "QUAKE.DEF"
#include "RIPPER.DEF"
#include "RIPPER2.DEF"
#include "ROOMS.DEF"
#include "ROTATOR.DEF"
#include "SAVE.DEF"
#include "SCRIP2.DEF"
#include "SECTOR.DEF"
#include "SERP.DEF"
#include "SETUP.DEF"
#include "SKEL.DEF"
#include "SKULL.DEF"
#include "SLIDOR.DEF"
#include "SOUNDS.DEF"
#include "SPIKE.DEF"
#include "SPRITE.DEF"
#include "SUMO.DEF"
#include "SYNC.DEF"
#include "TEXT.DEF"
#include "TIMER.DEF"
#include "TRACK.DEF"
#include "USRHOOKS.DEF"
#include "VATOR.DEF"
#include "WALLMOVE.DEF"
#include "WARP.DEF"
#include "WEAPON.DEF"

// Build              
#include "MMULTI.DEF" 
#include "BUILD.DEF"  
#include "ENGINE.DEF" 
#include "CACHE1D.DEF"
                      
#endif                