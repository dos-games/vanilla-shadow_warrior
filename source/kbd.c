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

#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "kbd.h"
#include "keys.h"
//#include "panel.h"
#include "game.h"

#include "types.h"
#include "control.h"
#include "function.h"

VOID initkeys(VOID)
    {
    keyfifoplc = 0;
    keyfifoend = 0;
    
    #if MACT_INPUT
    KB_Startup();
    #else
    memset(keystatus, 0, SIZ(keystatus));    
        
    oldkeyhandler = _dos_getvect(0x9);
    
    _disable();
    _dos_setvect(0x9, keyhandler);
    _enable();
    #endif
    }

VOID uninitkeys(VOID)
    {
    short *ptr;
    
    #if MACT_INPUT
    KB_Shutdown();
    #else
    _dos_setvect(0x9, oldkeyhandler);
    
    // Turn off shifts to prevent stucks with quitting
    ptr = (short *) 0x417;
    *ptr &= ~0x030f;
    #endif
    }

#if !MACT_INPUT    
void __interrupt __far keyhandler()
    {
    
    koutp(0x20, 0x20);
    oldreadch = readch;
    
    readch = kinp(0x60);
    keytemp = kinp(0x61);
    
    koutp(0x61, keytemp | 128);
    koutp(0x61, keytemp & 127);
    
    if ((readch | 1) == 0xe1)
        {
        extended = 128;
        return;
        }
        
    if (oldreadch != readch)
        {
        if ((readch & 128) == 0)
            {
            keytemp = readch + extended;
            
            if (KEY_PRESSED(keytemp) == 0)
                {
                KEY_PRESSED(keytemp) = 1;
                keyfifo[keyfifoend] = keytemp;
                keyfifo[(keyfifoend + 1) & (KEYFIFOSIZ - 1)] = 1;
                keyfifoend = ((keyfifoend + 2) & (KEYFIFOSIZ - 1));
                }
            }
        else
            {
            keytemp = (readch & 127) + extended;
            KEY_PRESSED(keytemp) = 0;
            keyfifo[keyfifoend] = keytemp;
            keyfifo[(keyfifoend + 1) & (KEYFIFOSIZ - 1)] = 0;
            keyfifoend = ((keyfifoend + 2) & (KEYFIFOSIZ - 1));
            }
        }
        
    extended = 0;
    }
#endif    

#if MACT_CONTROL
VOID gamekeys(VOID)
    {
    // KEYTIMERSTUFF
    if (!BUTTON(gamefunc_Strafe))
        {
        if (BUTTON(gamefunc_Turn_Left))
            angvel = max(angvel - 16 * synctics, -128);
        if (BUTTON(gamefunc_Turn_Right))
            angvel = min(angvel + 16 * synctics, 127);
        }
    else
        {
        if (BUTTON(gamefunc_Turn_Left))
            svel = min(svel + 8 * synctics, 127);
        if (BUTTON(gamefunc_Turn_Right))
            svel = max(svel - 8 * synctics, -128);
        }
        
    if (BUTTON(gamefunc_Move_Forward))
        vel = min(vel + 8 * synctics, 127);
        
    if (BUTTON(gamefunc_Move_Backward))
        vel = max(vel - 8 * synctics, -128);
        
    if (BUTTON(gamefunc_Strafe_Left))
        svel = min(svel + 8 * synctics, 127);
        
    if (BUTTON(gamefunc_Strafe_Right))
        svel = max(svel - 8 * synctics, -128);

    // takes you back down to 0
    
    // new momentum stuff should do this
    /*    
    if (angvel < 0) angvel = min(angvel + 12 * synctics, 0);
    if (angvel > 0) angvel = max(angvel - 12 * synctics, 0);
    if (svel < 0) svel = min(svel + 2 * synctics, 0);
    if (svel > 0) svel = max(svel - 2 * synctics, 0);
    if (vel < 0) vel = min(vel + 2 * synctics, 0);
    if (vel > 0) vel = max(vel - 2 * synctics, 0);
    */
    }
#else    
VOID gamekeys(VOID)
    {
    // KEYTIMERSTUFF
    if (!BUTTON(gamefunc_Strafe))
        {
        if (PKEY_PRESSED(KEYSC_LEFT))
            angvel = max(angvel - 16 * synctics, -128);
        if (PKEY_PRESSED(KEYSC_RIGHT))
            angvel = min(angvel + 16 * synctics, 127);
        }
    else
        {
        if (PKEY_PRESSED(KEYSC_LEFT))
            svel = min(svel + 8 * synctics, 127);
        if (PKEY_PRESSED(KEYSC_RIGHT))
            svel = max(svel - 8 * synctics, -128);
        }
        
    if (PKEY_PRESSED(KEYSC_UP))
        vel = min(vel + 8 * synctics, 127);
        
    if (PKEY_PRESSED(KEYSC_DOWN))
        vel = max(vel - 8 * synctics, -128);
        
    #if !MACT_INPUT
    if (PKEY(PK_STRAFE_LEFT) > 0)
        svel = min(svel + 8 * synctics, 127);
        
    if (PKEY(PK_STRAFE_RIGHT) > 0)
        svel = max(svel - 8 * synctics, -128);
    #endif    

    if (angvel < 0)
        angvel = min(angvel + 12 * synctics, 0);
    if (angvel > 0)
        angvel = max(angvel - 12 * synctics, 0);
        
    if (svel < 0)
        svel = min(svel + 2 * synctics, 0);
    if (svel > 0)
        svel = max(svel - 2 * synctics, 0);
        
    if (vel < 0)
        vel = min(vel + 2 * synctics, 0);
    if (vel > 0)
        vel = max(vel - 2 * synctics, 0);
    }
    
#endif
