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

#ifdef DAMAGE_TABLE
#define DAMAGE_ENTRY(id, init_func, damage_lo, damage_hi, radius, max_ammo, min_ammo, with_weapon) \
{ init_func, damage_lo, damage_hi, radius, max_ammo, min_ammo, with_weapon},
#endif

#ifdef DAMAGE_ENUM
#define DAMAGE_ENTRY(id, init_func, damage_lo, damage_hi, radius, max_ammo, min_ammo, with_weapon) \
id,
#endif

// DAMAGES ////////////////////////////////////////////////////////////////////

// weapon
DAMAGE_ENTRY(WPN_STAR,            InitWeaponStar,        5,    10,   0,  99,  3, -1 )
DAMAGE_ENTRY(WPN_UZI,             InitWeaponUzi,         7,    13,   0, 200,  1, -1 )
DAMAGE_ENTRY(WPN_SHOTGUN,         InitWeaponShotgun,     5,    11,   0,  52,  1, -1 )
DAMAGE_ENTRY(WPN_MICRO,           InitWeaponMicro,      30,    60,   0,  50,  1, -1 )
DAMAGE_ENTRY(WPN_GRENADE,         InitWeaponGrenade,  	75,   150,   0,  50,  1, -1 )
DAMAGE_ENTRY(WPN_MINE,            InitWeaponMine,        5,    10,   0,  20,  1, -1 )
DAMAGE_ENTRY(WPN_RAIL,            InitWeaponRail,       90,   110,   0,  10,  1, -1 )
DAMAGE_ENTRY(WPN_HEART,           InitWeaponHeart,      75,   100,   0,  20,  1, -1 )
DAMAGE_ENTRY(WPN_HOTHEAD,        InitWeaponHothead,     15,    25,   0,  80,  1, -1 )

DAMAGE_ENTRY(WPN_NAPALM,          InitWeaponHothead,    50,   100,   0, 100,  40, WPN_HOTHEAD )
DAMAGE_ENTRY(WPN_RING,            InitWeaponHothead,    15,    50,   0, 100,  20, WPN_HOTHEAD )
DAMAGE_ENTRY(WPN_ROCKET,          InitWeaponMicro,      30,    60,   0, 100,  1, WPN_MICRO )
DAMAGE_ENTRY(WPN_SWORD,           InitWeaponSword,    	50,    80,   0,  -1,  -1, -1 )

// extra weapons connected to other

// spell
DAMAGE_ENTRY(DMG_NAPALM,          NULL,                50,   100, 0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_MIRV_METEOR,     NULL,                35,    65, 0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_SERP_METEOR,     NULL,                30,    40, 0, -1, -1, -1     )
                                                                           
// radius damage                                                           
DAMAGE_ENTRY(DMG_ELECTRO_SHARD,   NULL,                 2,     6,      0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_SECTOR_EXP,      NULL,                50,   100,   3200, -1, -1, -1 )
DAMAGE_ENTRY(DMG_BOLT_EXP,        NULL,                80,   160,   3200, -1, -1, -1 )
DAMAGE_ENTRY(DMG_TANK_SHELL_EXP,  NULL,                80,   200,   4500, -1, -1, -1 )
DAMAGE_ENTRY(DMG_GORO_FIREBALL_EXP,  NULL,             10,    10,    800, -1, -1, -1 )
DAMAGE_ENTRY(DMG_FIREBALL_EXP,    NULL,                -1,    -1,   1000, -1, -1, -1 )
DAMAGE_ENTRY(DMG_NAPALM_EXP,      NULL,                90,   130,   2000, -1, -1, -1 )
DAMAGE_ENTRY(DMG_SKULL_EXP,       NULL,                75,   100,   4500, -1, -1, -1 )
DAMAGE_ENTRY(DMG_BASIC_EXP,       NULL,                25,    50,   2500, -1, -1, -1 )
DAMAGE_ENTRY(DMG_GRENADE_EXP,     NULL,                60,   160,   4500, -1, -1, -1 )
DAMAGE_ENTRY(DMG_MINE_EXP,        NULL,                60,   220,   4500, -1, -1, -1 )
DAMAGE_ENTRY(DMG_MINE_SHRAP,      NULL,                15,    30,      0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_MICRO_EXP,       NULL,                50,   100,   4500, -1, -1, -1 )
DAMAGE_ENTRY(DMG_NUCLEAR_EXP,     NULL,                80,   800,  16384, -1, -1, -1 )
DAMAGE_ENTRY(DMG_RADIATION_CLOUD, NULL,                 2,     6,   5000, -1, -1, -1 )
DAMAGE_ENTRY(DMG_FLASHBOMB,       NULL,               100,   150,  16384, -1, -1, -1 )

DAMAGE_ENTRY(DMG_FIREBALL_FLAMES, NULL,                 2,     6,    300, -1, -1, -1 )

// actor
DAMAGE_ENTRY(DMG_RIPPER_SLASH,    NULL,                10,    30,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_SKEL_SLASH,      NULL,                10,    20,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_COOLG_BASH,      NULL,                10,    20,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_COOLG_FIRE,      NULL,                15,    30,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_GORO_CHOP,       NULL,                20,    40,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_GORO_FIREBALL,   NULL,                5,     10,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_SERP_SLASH,      NULL,                75,    75,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_LAVA_BOULDER,    NULL,                100,  100,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_LAVA_SHARD,      NULL,                25,    25,   0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_HORNET_STING,    NULL,                2,     6,    0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_EEL_ELECTRO,     NULL,                10,   40, 3400, -1, -1, -1 )

// misc                                                                     
DAMAGE_ENTRY(DMG_SPEAR_TRAP,      NULL,                15,   20,    0, -1, -1, -1 )
DAMAGE_ENTRY(DMG_VOMIT,           NULL,                 4,    8,    0, -1, -1, -1 )
                                                                    
// inanimate objects                                                
DAMAGE_ENTRY(DMG_BLADE,           NULL,                10,    20,   0, -1, -1, -1 )
DAMAGE_ENTRY(MAX_WEAPONS,         NULL,                10,    20,   0, -1, -1, -1 )
              
#undef DAMAGE_ENTRY
