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

// function.c

// file created by makehead.exe
// this header contains default key assignments, as well as
// default button assignments and game function names


char * gamefunctions[] =
   {
   "Move_Forward",
   "Move_Backward",
   "Turn_Left",
   "Turn_Right",
   "Strafe",
   "Fire",
   "Open",
   "Run",
   "AutoRun",
   "Jump",
   "Crouch",
   "Look_Up",
   "Look_Down",
   "Strafe_Left",
   "Strafe_Right",
   "Aim_Up",
   "Aim_Down",
   "Weapon_1",
   "Weapon_2",
   "Weapon_3",
   "Weapon_4",
   "Weapon_5",
   "Weapon_6",
   "Weapon_7",
   "Weapon_8",
   "Weapon_9",
   "Inventory",
   "Inventory_Left",
   "Inventory_Right",
   "TurnAround",
   "SendMessage",
   "Map",
   "Shrink_Screen",
   "Enlarge_Screen",
   "Holster_Weapon",
   "ScreenPeek",
   };

#define NUMKEYENTRIES 36

static char * keydefaults[] =
   {
   "Move_Forward", "Up", "Kpad8", 
   "Move_Backward", "Down", "Kpad2", 
   "Turn_Left", "Left", "Kpad4", 
   "Turn_Right", "Right", "KPad6", 
   "Strafe", "LAlt", "RAlt", 
   "Fire", "LCtrl", "RCtrl", 
   "Open", "Space", "", 
   "Run", "LShift", "RShift", 
   "AutoRun", "CapLck", "", 
   "Jump", "A", "/", 
   "Crouch", "Z", "", 
   "Look_Up", "PgUp", "", 
   "Look_Down", "PgDn", "", 
   "Strafe_Left", ",", "", 
   "Strafe_Right", ".", "", 
   "Aim_Up", "Home", "", 
   "Aim_Down", "End", "", 
   "Weapon_1", "1", "", 
   "Weapon_2", "2", "", 
   "Weapon_3", "3", "", 
   "Weapon_4", "4", "", 
   "Weapon_5", "5", "", 
   "Weapon_6", "6", "", 
   "Weapon_7", "7", "", 
   "Weapon_8", "8", "", 
   "Weapon_9", "9", "", 
   "Inventory", "Enter", "KpdEnt", 
   "Inventory_Left", "[", "", 
   "Inventory_Right", "]", "", 
   "TurnAround", "BakSpc", "", 
   "SendMessage", "T", "", 
   "Map", "Tab", "", 
   "Shrink_Screen", "-", "Kpad-", 
   "Enlarge_Screen", "=", "Kpad+", 
   "Holster_Weapon", "Delete", "", 
   "ScreenPeek", "`", "", 
   };


static char * mousedefaults[] =
   {
   "Fire",
   "Strafe",
   "Move_Forward",
   };


static char * mouseclickeddefaults[] =
   {
   "",
   "Open",
   "",
   };


static char * joystickdefaults[] =
   {
   "Fire",
   "Open",
   "Strafe",
   "Move_Forward",
   };


static char * joystickclickeddefaults[] =
   {
   "",
   "",
   "",
   "",
   };
